/* This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
 *
 * Copyright ©2016 Dmitry Filimonchuk <dmitrystu[at]gmail[dot]com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32_compat.h"
#include "usb.h"
#include "usb_dwc2_param.h"
//#include "dwc2_type.h"

#if defined(USBD_STM32F767FS)

#define MAX_EP          6
#define MAX_RX_PACKET   128  /*in 32-bit chunks; 128*4=512 byte */
#define MAX_CONTROL_EP  1
#define MAX_FIFO_SZ     320  /*in 32-bit chunks; 320*4=1280 byte = 1.25K byte */

#define RX_FIFO_SZ      ((5 * MAX_CONTROL_EP + 8) + ((MAX_RX_PACKET / 4) + 1) + (MAX_EP * 2) + 1)  //59

#define STATUS_VAL(x)   (USBD_HW_ADDRFST | (x))

#define USB_OTG_FS_GLOBAL ((USB_OTG_GlobalTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_GLOBAL_BASE))
#define USB_OTG_FS_DEVICE ((USB_OTG_DeviceTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_FS_PCGCCTL *((__IO uint32_t *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE))

#define IN_ENDPOINT(i)  ((USB_OTG_INEndpointTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))
#define OUT_ENDPOINT(i)  ((USB_OTG_OUTEndpointTypeDef *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_OUT_ENDPOINT_BASE + ((i) * USB_OTG_EP_REG_SIZE)))
#define EPFIFO(i)  *(__IO uint32_t *)(USB_OTG_FS_PERIPH_BASE + USB_OTG_FIFO_BASE + ((i) * USB_OTG_FIFO_SIZE))

#define CONFIG_CPU_CLOCK_HZ  (SystemCoreClock)


typedef union {
  uint32_t value;
  struct TU_ATTR_PACKED {
    uint32_t ep_ch_num     : 4; // 0..3 Endpoint/Channel Number
    uint32_t byte_count    :11; // 4..14 Byte Count
    uint32_t dpid          : 2; // 15..16 Data PID
    uint32_t packet_status : 4; // 17..20 Packet Status
    uint32_t frame_number  : 4; // 21..24 Frame Number
    uint32_t rsv25_31      : 7; // 25..31 Reserved
  };
} dwc2_grxstsp_t;

#ifndef CONFIG_USB_DWC2_CUSTOM_PARAM
const struct dwc2_user_params param_pa11_pa12 = {
  .phy_type = DWC2_PHY_TYPE_PARAM_FS,
  .device_dma_enable = false,
  .device_dma_desc_enable = false,
  .device_rx_fifo_size = (320 - 16 - 16 - 16 - 16 - 16 - 16),
  .device_tx_fifo_size = {
        [0] = 16, // 64 byte
        [1] = 16, // 64 byte
        [2] = 16, // 64 byte
        [3] = 16, // 64 byte
        [4] = 16,
        [5] = 16,
        [6] = 0,
        [7] = 0,
        [8] = 0,
        [9] = 0,
        [10] = 0,
        [11] = 0,
        [12] = 0,
        [13] = 0,
        [14] = 0,
        [15] = 0 },

  .device_gccfg = (1 << 16), // fs: USB_OTG_GCCFG_PWRDWN
  .host_gccfg = (1 << 16),   // fs: USB_OTG_GCCFG_PWRDWN

  .b_session_valid_override = true,
  .total_fifo_size = 320 // 1280 byte
};
/*
const struct dwc2_user_params param_pb14_pb15 = {
  .phy_type = DWC2_PHY_TYPE_PARAM_FS,
  .device_dma_enable = false,
  .device_dma_desc_enable = false,
  .device_rx_fifo_size = (1006 - 16 - 256 - 128 - 128 - 128 - 128),
  .device_tx_fifo_size = {
        [0] = 16,  // 64 byte
        [1] = 256, // 1024 byte
        [2] = 128, // 512 byte
        [3] = 128, // 512 byte
        [4] = 128, // 512 byte
        [5] = 128, // 512 byte
        [6] = 0,
        [7] = 0,
        [8] = 0,
        [9] = 0,
        [10] = 0,
        [11] = 0,
        [12] = 0,
        [13] = 0,
        [14] = 0,
        [15] = 0 },

  .host_dma_desc_enable = false,
  .host_rx_fifo_size = 622,
  .host_nperio_tx_fifo_size = 128, // 512 byte
  .host_perio_tx_fifo_size = 256,  // 1024 byte
  .device_gccfg = (1 << 16), // fs: USB_OTG_GCCFG_PWRDWN hs:0
  .host_gccfg = (1 << 16),   // fs: USB_OTG_GCCFG_PWRDWN hs:0
  .b_session_valid_override = true
};
*/
#endif // CONFIG_USB_DWC2_CUSTOM_PARAM


static void portinit(void)
{
  /**USBOTG GPIO Configuration
  PA9    ------> OTG_FS_VBUS ?
  PA10   ------> USB_ID      OTG_FS_ID
  PA11   ------> USB_DM D-   OTG_FS_DM
  PA12   ------> USB_DP D+   OTG_FS_DP
  PG6    ------> USB_PowerSwitchOn
  PG7    ------> USB_OverCurrent
  */
  //Enables the clock for GPIOA
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);

  //Configures USB pins (in GPIOA) to work in alternate function mode.
  //Sets alternate function 10 for : PA11 (-) otg_fs_dm , and PA12 (+) otg_fs_dp.
  GPIOA->MODER &= ~((0x3 << 24) | (0x3 << 22));
  GPIOA->MODER |= ((0x2 << 24) | (0x2 << 22)); //复用功能模式
  GPIOA->AFR[1] &= 0xfff00fff;
  GPIOA->AFR[1] |= 0x000aa000;  //AFR12[3:0] = AFR11[3:0] = af10(0xa)
  GPIOA->OTYPER &= ~((0x1 << 12) | (0x1 << 11)); // 复位状态 推挽输出
  GPIOA->OSPEEDR |= ((0x3 << 24) | (0x3 << 22)); // IO速度 高速
  GPIOA->PUPDR &= ~((0x3 << 24) | (0x3 << 22)); // 无上下拉

  /* This for ID line debug  PA10 (id) otg_fs_id*/
  //GPIOA->AFR[1] &= 0xfffff0ff;
  //GPIOA->AFR[1] |= 0x00000a00;  //AFR10[3:0] = af10(0xa)
  //GPIOA->OTYPER |= (0x1 << 10); // 开漏输出类型
  //GPIOA->OSPEEDR |= (0x3 << 20); // IO速度 高速
  //GPIOA->PUPDR &= ~(0x3 << 20);
  //GPIOA->PUPDR |= ~(0x1 << 20); //上拉

  /* Configure Power Switch Vbus Pin  PA6 */
  GPIOA->MODER &= ~(0x3 << 12);
  GPIOA->MODER |= (0x1 << 12); //输出模式

  /* Configure PA7 */
  GPIOA->MODER &= ~(0x3 << 14); //输入模式

  /* Enable USB FS Clocks */
  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /* Set USBFS Interrupt to the lowest priority */
  /* Enable USBFS Interrupt */
  NVIC_SetPriority(OTG_FS_IRQn, 7);
  NVIC_EnableIRQ(OTG_FS_IRQn);

#if 0
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure USB FS GPIOs */
  //Enables the clock for GPIOA GPIOG
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); //SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);//SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);

  /* Configure DM DP Pins */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* This for ID line debug */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    // 开漏输出类型
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure Power Switch Vbus Pin */
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT); //无上下拉，推挽输出
  LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_6);

  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOG, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);

  /* Enable USB FS Clocks */
  //RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
  //RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /* Set USBFS Interrupt to the lowest priority */
  /* Enable USBFS Interrupt */
  //NVIC_SetPriority(OTG_FS_IRQn, 3);
  //NVIC_EnableIRQ(OTG_FS_IRQn);

#endif
}


inline static uint8_t flush_rxfifo(void)
{
#if 0
  _BST(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH);
  _WBC(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH);
#else
  volatile uint32_t count = 0U;

  /* Wait for AHB master IDLE state. */
  do
  {
    if(++count > 200000U)
    {
      return -1;
    }
  }while((USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);

  count = 0;
  USB_OTG_FS_GLOBAL->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

  do
  {
    if(++count > 200000U)
    {
      return -1;
    }
  }while((USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);

  return 0;
#endif
}

inline static uint8_t flush_txfifo(uint8_t ep)
{
#if 0
  _BMD(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_TXFNUM, _VAL2FLD(USB_OTG_GRSTCTL_TXFNUM, ep) | USB_OTG_GRSTCTL_TXFFLSH);
  _WBC(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_TXFFLSH);
#else
  __IO uint32_t count = 0U;

  /* Wait for AHB master IDLE state. */
  do
  {
    if(count++ > 0xF000000)
    {
      return 1;
    }
  }while((USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);

  /* Flush TX Fifo */
  count = 0U;
  //USB_OTG_FS_GLOBAL->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (ep << 6)); //USB_OTG_GRSTCTL_TXFNUM_Pos
  _BMD(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_TXFNUM,
			 _VAL2FLD(USB_OTG_GRSTCTL_TXFNUM, ep) | USB_OTG_GRSTCTL_TXFFLSH);
  do
  {
    if (count++ > 0xF000000)
    {
      return 1;
    }
  } while((USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

  return 0;
#endif
}


static uint32_t getinfo(void) {
    if (!(RCC->AHB2ENR & RCC_AHB2ENR_OTGFSEN)) return STATUS_VAL(0);
    if (!(USB_OTG_FS_DEVICE->DCTL & USB_OTG_DCTL_SDIS)) return STATUS_VAL(USBD_HW_ENABLED | USBD_HW_SPEED_FS);
    return STATUS_VAL(USBD_HW_ENABLED);
}

static void ep_setstall(uint8_t ep, bool stall)
{
  if(ep & 0x80)
  {
    ep &= 0x7F;
    uint32_t _t = IN_ENDPOINT(ep)->DIEPCTL;

    if(_t & USB_OTG_DIEPCTL_USBAEP)
    {
      if(stall)
        _BST(_t, USB_OTG_DIEPCTL_STALL);
      else
        _BMD(_t, USB_OTG_DIEPCTL_STALL, USB_OTG_DIEPCTL_SD0PID_SEVNFRM | USB_OTG_DOEPCTL_SNAK);

      IN_ENDPOINT(ep)->DIEPCTL = _t;
    }
  }else{
    uint32_t _t = OUT_ENDPOINT(ep)->DOEPCTL;
    if(_t & USB_OTG_DOEPCTL_USBAEP)
    {
      if(stall)
        _BST(_t, USB_OTG_DOEPCTL_STALL);
      else
        _BMD(_t, USB_OTG_DOEPCTL_STALL, USB_OTG_DOEPCTL_SD0PID_SEVNFRM | USB_OTG_DOEPCTL_CNAK);

      OUT_ENDPOINT(ep)->DOEPCTL = _t;
    }
  }
}

static bool ep_isstalled(uint8_t ep)
{
  if(ep & 0x80)
  {
    ep &= 0x7F;
    return (IN_ENDPOINT(ep)->DIEPCTL & USB_OTG_DIEPCTL_STALL) ? true : false;
  }else{
    return (OUT_ENDPOINT(ep)->DOEPCTL & USB_OTG_DOEPCTL_STALL) ? true : false;
  }
}

static void dwc2_set_txfifo(uint8_t fifo, uint16_t size)
{
  uint8_t i;
  uint32_t tx_offset;

  /*  TXn min size = 16 words. (n  : Transmit FIFO index)
      When a TxFIFO is not used, the Configuration should be as follows:
          case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
         --> Txm can use the space allocated for Txn.
         case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
         --> Txn should be configured with the minimum space of 16 words
      The FIFO is used optimally when used TxFIFOs are allocated in the top
         of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
      When DMA is used 3n * FIFO locations should be reserved for internal DMA registers */

  tx_offset = USB_OTG_FS_GLOBAL->GRXFSIZ;

  if(fifo == 0U)
  {
    USB_OTG_FS_GLOBAL->DIEPTXF0_HNPTXFSIZ = ((uint32_t)size << 16) | tx_offset;
  }else{
    tx_offset += (USB_OTG_FS_GLOBAL->DIEPTXF0_HNPTXFSIZ) >> 16;
    for(i = 0U; i < (fifo - 1U); i++)
    {
      tx_offset += (USB_OTG_FS_GLOBAL->DIEPTXF[i] >> 16);
    }

    /* Multiply Tx_Size by 2 to get higher performance */
    USB_OTG_FS_GLOBAL->DIEPTXF[fifo - 1U] = ((uint32_t)size << 16) | tx_offset;
  }

}
static void enable(bool enable)
{
  portinit();
  if(enable)
  {
    /* enabling USB_OTG in RCC */
    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN; //_BST(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    _WBS(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_AHBIDL); //while((OTG->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0)

    /* configure OTG as device */
    USB_OTG_FS_GLOBAL->GUSBCFG = USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | _VAL2FLD(USB_OTG_GUSBCFG_TRDT, 0x06);

    /* configuring Vbus sense and powerup PHY */
    USB_OTG_FS_GLOBAL->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL; //BVALID信号是AXI协议中用于指示写响应有效的信号
    USB_OTG_FS_GLOBAL->GCCFG = USB_OTG_GCCFG_PWRDWN; // USB_OTG_FS_GLOBAL->GCCFG = param_pa11_pa12.device_gccfg;

    /* Restart the Phy Clock */
    USB_OTG_FS_PCGCCTL = 0;

    /* soft disconnect device */
    USB_OTG_FS_DEVICE->DCTL |= USB_OTG_DCTL_SDIS; //_BST(OTGD->DCTL, USB_OTG_DCTL_SDIS);

    /* Setup USB FS speed and frame interval */
    _BMD(USB_OTG_FS_DEVICE->DCFG, USB_OTG_DCFG_PERSCHIVL | USB_OTG_DCFG_DSPD,
             _VAL2FLD(USB_OTG_DCFG_PERSCHIVL, 0) | _VAL2FLD(USB_OTG_DCFG_DSPD, 0x03));

    /* unmask EP interrupts */
    USB_OTG_FS_DEVICE->DIEPMSK = USB_OTG_DIEPMSK_XFRCM;

    /* unmask core interrupts */
    USB_OTG_FS_GLOBAL->GINTMSK = USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM |
#if !defined(USBD_SOF_DISABLED)
        USB_OTG_GINTMSK_SOFM |
#endif
		USB_OTG_GINTMSK_IEPINT | USB_OTG_GINTMSK_OEPINT |
        USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM |
        USB_OTG_GINTMSK_RXFLVLM;

    /* clear pending interrupts */
    USB_OTG_FS_GLOBAL->GINTSTS = 0xFFFFFFFF;
    /* unmask global interrupt */
    USB_OTG_FS_GLOBAL->GAHBCFG = USB_OTG_GAHBCFG_GINT;

    /* setting max RX FIFO size */
    //USB_OTG_FS_GLOBAL->GRXFSIZ = RX_FIFO_SZ;
    USB_OTG_FS_GLOBAL->GRXFSIZ = param_pa11_pa12.device_rx_fifo_size;

    /* setting up EP0 TX FIFO SZ as 64 byte */
    USB_OTG_FS_GLOBAL->DIEPTXF0_HNPTXFSIZ = RX_FIFO_SZ | (0x10 << 16);
    for(uint8_t i = 0; i < MAX_EP; i++)
    {
      dwc2_set_txfifo(i, param_pa11_pa12.device_tx_fifo_size[i]);
    }

  }else{
    if(RCC->AHB2ENR & RCC_AHB2ENR_OTGFSEN)
    {
      _BST(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST);
      _BCL(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST);
      _BCL(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    }
  }
}

static uint8_t connect(bool connect)
{
  if(connect)
    _BCL(USB_OTG_FS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);
  else
    _BST(USB_OTG_FS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);

  return usbd_lane_unk;
}

static void setaddr(uint8_t addr)
{
  _BMD(USB_OTG_FS_DEVICE->DCFG, USB_OTG_DCFG_DAD, addr << 4);
}

/**\brief Helper. Set up TX fifo
 * \param ep endpoint index
 * \param epsize required max packet size in bytes
 * \return true if TX fifo is successfully set
 */
static bool set_tx_fifo(uint8_t ep, uint16_t epsize)
{
  uint32_t _fsa = USB_OTG_FS_GLOBAL->DIEPTXF0_HNPTXFSIZ;
  /* calculating initial TX FIFO address. next from EP0 TX fifo */
  _fsa = 0xFFFF & (_fsa + (_fsa >> 16));
  /* looking for next free TX fifo address */
  for(int i = 0; i < (MAX_EP - 1); i++)
  {
    uint32_t _t = USB_OTG_FS_GLOBAL->DIEPTXF[i];
    if((_t & 0xFFFF) < 0x200)
    {
      _t = 0xFFFF & (_t + (_t >> 16));
      if(_t > _fsa)
        _fsa = _t;
    }
  }
  /* calculating requited TX fifo size */
  /* getting in 32 bit terms */
  epsize = (epsize + 0x03) >> 2;
  /* it must be 16 32-bit words minimum */
  if(epsize < 0x10)
    epsize = 0x10;
  /* checking for the available fifo */
  if((_fsa + epsize) > MAX_FIFO_SZ)
    return false;
  /* programming fifo register */
  _fsa |= (epsize << 16);
  USB_OTG_FS_GLOBAL->DIEPTXF[ep - 1] = _fsa;
  return true;
}

static bool ep_config(uint8_t ep, uint8_t eptype, uint16_t epsize)
{
  if(ep == 0)
  {
    /* configureing control endpoint EP0 */
    uint32_t mpsize;
    if(epsize <= 0x08){
      epsize = 0x08;
      mpsize = 0x03;
    }
    else if(epsize <= 0x10){
      epsize = 0x10;
      mpsize = 0x02;
    }
    else if(epsize <= 0x20){
      epsize = 0x20;
      mpsize = 0x01;
    }else{
      epsize = 0x40;
      mpsize = 0x00;
    }
    /* EP0 TX FIFO size is setted on init level */
    /* enabling RX and TX interrupts from EP0 */
    USB_OTG_FS_DEVICE->DAINTMSK |= 0x00010001;
    /* setting up EP0 TX and RX registers */
    /*EPIN(ep)->DIEPTSIZ  = epsize;*/
    IN_ENDPOINT(ep)->DIEPCTL = mpsize | USB_OTG_DIEPCTL_SNAK;
    /* 1 setup packet, 1 packets total */
    OUT_ENDPOINT(ep)->DOEPTSIZ = epsize | (1 << 29) | (1 << 19);
    OUT_ENDPOINT(ep)->DOEPCTL = mpsize | USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK;
    //OUT_ENDPOINT(ep)->DOEPCTL = mpsize | USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_SNAK;
    return true;
  }
  if(ep & 0x80)
  {
    ep &= 0x7F;
    /* configuring TX endpoint */
    /* setting up TX fifo and size register */
    if((eptype == USB_EPTYPE_ISOCHRONUS) || (eptype == (USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF)))
    {
      if(!set_tx_fifo(ep, epsize << 1))
        return false;
    }else{
      if(!set_tx_fifo(ep, epsize))
    	return false;
    }
    /* enabling EP TX interrupt */
    USB_OTG_FS_DEVICE->DAINTMSK |= (0x0001UL << ep);
    /* setting up TX control register*/
    switch(eptype)
    {
      case USB_EPTYPE_ISOCHRONUS:
        IN_ENDPOINT(ep)->DIEPCTL = USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK |
                                   (0x01 << 18) | USB_OTG_DIEPCTL_USBAEP |
                                   USB_OTG_DIEPCTL_SD0PID_SEVNFRM |
                                   (ep << 22) | epsize;
        break;
      case USB_EPTYPE_BULK:
      case USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF:
        IN_ENDPOINT(ep)->DIEPCTL = USB_OTG_DIEPCTL_SNAK | USB_OTG_DIEPCTL_USBAEP |
                                   (0x02 << 18) | USB_OTG_DIEPCTL_SD0PID_SEVNFRM |
                                   (ep << 22) | epsize;
        break;

      default:
        IN_ENDPOINT(ep)->DIEPCTL = USB_OTG_DIEPCTL_SNAK | USB_OTG_DIEPCTL_USBAEP |
                                   (0x03 << 18) | USB_OTG_DIEPCTL_SD0PID_SEVNFRM |
                                   (ep << 22) | epsize;
        break;
    }
  }else{
    /* configuring RX endpoint */
    /* setting up RX control register */
    switch(eptype)
    {
      case USB_EPTYPE_ISOCHRONUS:
        OUT_ENDPOINT(ep)->DOEPCTL = USB_OTG_DOEPCTL_SD0PID_SEVNFRM | USB_OTG_DOEPCTL_CNAK |
                                    USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_USBAEP |
                                    (0x01 << 18) | epsize;
        break;
      case USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF:
      case USB_EPTYPE_BULK:
        OUT_ENDPOINT(ep)->DOEPCTL = USB_OTG_DOEPCTL_SD0PID_SEVNFRM | USB_OTG_DOEPCTL_CNAK |
                                    USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_USBAEP |
                                    (0x02 << 18) | epsize;
        break;
      default:
        OUT_ENDPOINT(ep)->DOEPCTL = USB_OTG_DOEPCTL_SD0PID_SEVNFRM | USB_OTG_DOEPCTL_CNAK |
                                    USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_USBAEP |
                                    (0x03 << 18) | epsize;
        break;
    }
  }
  return true;
}

static void ep_deconfig(uint8_t ep)
{
  ep &= 0x7F;

  /* deconfiguring TX part */
  /* disable interrupt, Masks all interrupts of the targeted IN and OUT endpoints */
  //USB_OTG_FS_DEVICE->DAINTMSK &= ~((1 << ep) | (1 << 16 << ep));
  USB_OTG_FS_DEVICE->DAINTMSK &= ~(0x10001 << ep);

  /* decativating endpoint */
  IN_ENDPOINT(ep)->DIEPCTL &= ~(USB_OTG_DIEPCTL_USBAEP); //_BCL(in_endpoint->DIEPCTL, USB_OTG_DIEPCTL_USBAEP);

  /* flushing FIFO */
  flush_txfifo(ep);

  /* disabling endpoint */
  if((IN_ENDPOINT(ep)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) && (ep != 0))
  {
    IN_ENDPOINT(ep)->DIEPCTL = USB_OTG_DIEPCTL_EPDIS; //Disables endpoint transmission
  }
  /* clean EP interrupts */
  IN_ENDPOINT(ep)->DIEPINT = 0xFF;
  /* deconfiguring TX FIFO */
  if(ep > 0)
    USB_OTG_FS_GLOBAL->DIEPTXF[ep-1] = 0x02000200 + 0x200 * ep;

  /* deconfigureing RX part */
    OUT_ENDPOINT(ep)->DOEPCTL &= ~USB_OTG_DOEPCTL_USBAEP;
  if((OUT_ENDPOINT(ep)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) && (ep != 0))
    OUT_ENDPOINT(ep)->DOEPCTL = USB_OTG_DOEPCTL_EPDIS;
  OUT_ENDPOINT(ep)->DOEPINT = 0xFF;
}

static int32_t ep_read(uint8_t ep, void* buf, uint16_t blen)
{
  uint32_t len, tmp = 0;

  ep &= 0x7F;
  //volatile uint32_t *fifo = EPFIFO(0);

  /* no data in RX FIFO */
  if(!(USB_OTG_FS_GLOBAL->GINTSTS & USB_OTG_GINTSTS_RXFLVL))
    return -1;
  if((USB_OTG_FS_GLOBAL->GRXSTSR & USB_OTG_GRXSTSP_EPNUM) != ep)
    return -1;
  /* pop data from fifo */
  //len = _FLD2VAL(USB_OTG_GRXSTSP_BCNT, USB_OTG_FS_GLOBAL->GRXSTSP);
  len = ((USB_OTG_FS_GLOBAL->GRXSTSP & USB_OTG_GRXSTSP_BCNT) >> USB_OTG_GRXSTSP_BCNT_Pos);

  for(int idx = 0; idx < len; idx++)
  {
    if((idx & 0x03) == 0x00)
    {
      tmp = EPFIFO(0); //*fifo
    }
    if(idx < blen)
    {
      ((uint8_t*)buf)[idx] = tmp & 0xFF;
      tmp >>= 8;
    }
  }

  _BST(OUT_ENDPOINT(ep)->DOEPCTL, USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);

  return (len < blen) ? len : blen;
}

static int32_t ep_write(uint8_t ep, const void *buf, uint16_t blen)
{
  uint32_t len, tmp;
  ep &= 0x7F;

  /* transfer data size in 32-bit words */
  len = (blen + 3) >> 2;
  /* no enough space in TX fifo */
  if(len > IN_ENDPOINT(ep)->DTXFSTS)
    return -1;
  if(ep != 0 && IN_ENDPOINT(ep)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
    return -1;

  IN_ENDPOINT(ep)->DIEPTSIZ = 0;
  IN_ENDPOINT(ep)->DIEPTSIZ = (1 << 19) + blen;
  _BMD(IN_ENDPOINT(ep)->DIEPCTL, USB_OTG_DIEPCTL_STALL, USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK);
  /* push data to FIFO */
  tmp = 0;
  for(int idx = 0; idx < blen; idx++)
  {
    tmp |= (uint32_t)((const uint8_t*)buf)[idx] << ((idx & 0x03) << 3);
    if((idx & 0x03) == 0x03 || (idx + 1) == blen)
    {
      EPFIFO(ep) = tmp;
      tmp = 0;
    }
  }
  return blen;
}

static uint16_t get_frame (void)
{
  return _FLD2VAL(USB_OTG_DSTS_FNSOF, USB_OTG_FS_DEVICE->DSTS);
}

void ep0_outstart_read_setup(void)
{
  OUT_ENDPOINT(0U)->DOEPTSIZ = 0U;
  OUT_ENDPOINT(0U)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19));
  OUT_ENDPOINT(0U)->DOEPTSIZ |= (3U * 8U);
  OUT_ENDPOINT(0U)->DOEPTSIZ |=  USB_OTG_DOEPTSIZ_STUPCNT;
}

void set_turnaroundTime(uint32_t speed)
{
  uint32_t UsbTrd;

  UNUSED(speed);
  /* The USBTRD is configured according to the tables below, depending on AHB frequency
  used by application. In the low AHB frequency range it is used to stretch enough the USB response
  time to IN tokens, the USB turnaround time, so to compensate for the longer AHB read access
  latency to the Data FIFO */
  //if(speed == USBD_FS_SPEED)
  {
    if((CONFIG_CPU_CLOCK_HZ >= 14200000U) && (CONFIG_CPU_CLOCK_HZ < 15000000U))
    {
      /* hclk Clock Range between 14.2-15 MHz */
      UsbTrd = 0xFU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 15000000U) && (CONFIG_CPU_CLOCK_HZ < 16000000U))
    {
      /* hclk Clock Range between 15-16 MHz */
      UsbTrd = 0xEU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 16000000U) && (CONFIG_CPU_CLOCK_HZ < 17200000U))
    {
      /* hclk Clock Range between 16-17.2 MHz */
      UsbTrd = 0xDU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 17200000U) && (CONFIG_CPU_CLOCK_HZ < 18500000U))
    {
      /* hclk Clock Range between 17.2-18.5 MHz */
      UsbTrd = 0xCU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 18500000U) && (CONFIG_CPU_CLOCK_HZ < 20000000U))
    {
      /* hclk Clock Range between 18.5-20 MHz */
      UsbTrd = 0xBU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 20000000U) && (CONFIG_CPU_CLOCK_HZ < 21800000U))
    {
      /* hclk Clock Range between 20-21.8 MHz */
      UsbTrd = 0xAU;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 21800000U) && (CONFIG_CPU_CLOCK_HZ < 24000000U))
    {
      /* hclk Clock Range between 21.8-24 MHz */
      UsbTrd = 0x9U;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 24000000U) && (CONFIG_CPU_CLOCK_HZ < 27700000U))
    {
      /* hclk Clock Range between 24-27.7 MHz */
      UsbTrd = 0x8U;
    }
    else if((CONFIG_CPU_CLOCK_HZ >= 27700000U) && (CONFIG_CPU_CLOCK_HZ < 32000000U))
    {
      /* hclk Clock Range between 27.7-32 MHz */
      UsbTrd = 0x7U;
    }
    else /* if(hclk >= 32000000) */
    {
      /* hclk Clock Range between 32-200 MHz */
      UsbTrd = 0x6U;
    }
  }
  /*
  else if(speed == USBD_HS_SPEED) //USBD_HS_SPEED
  {
    UsbTrd = 0x9U; //USBD_HS_TRDT_VALUE
  }
  else
  {
    UsbTrd = 0x9U; //USBD_DEFAULT_TRDT_VALUE
  }
  */

  USB_OTG_FS_GLOBAL->GUSBCFG &= ~USB_OTG_GUSBCFG_TRDT;
  USB_OTG_FS_GLOBAL->GUSBCFG |= (uint32_t)((UsbTrd << 10) & USB_OTG_GUSBCFG_TRDT);
}

static uint32_t get_outep_interrupt(uint32_t epnum)
{
  uint32_t tmpreg;

  tmpreg = OUT_ENDPOINT((uint32_t)epnum)->DOEPINT;
  OUT_ENDPOINT((uint32_t)epnum)->DOEPINT = tmpreg;
  tmpreg = tmpreg & USB_OTG_FS_DEVICE->DOEPMSK;

  return tmpreg;
}
static uint32_t get_inep_interrupt(uint32_t epnum)
{
  uint32_t tmpreg, msk, emp;

  msk = USB_OTG_FS_DEVICE->DIEPMSK;
  emp = USB_OTG_FS_DEVICE->DIEPEMPMSK;
  msk |= ((emp >> (epnum & 0x0F)) & 0x1U) << 7;

  tmpreg = IN_ENDPOINT((uint32_t)epnum)->DIEPINT;
  IN_ENDPOINT((uint32_t)epnum)->DIEPINT = tmpreg;
  tmpreg = tmpreg & msk;

  return tmpreg;
}

static void dwc2_ep0_start_read_setup(void)
{
  OUT_ENDPOINT(0U)->DOEPTSIZ = 0U;
  OUT_ENDPOINT(0U)->DOEPTSIZ |= (USB_OTG_DOEPTSIZ_PKTCNT & (1U << 19));
  OUT_ENDPOINT(0U)->DOEPTSIZ |= (3U * 8U);
  OUT_ENDPOINT(0U)->DOEPTSIZ |= USB_OTG_DOEPTSIZ_STUPCNT;
}
//--------------------------------------------------------------------
// Interrupt Handler
//--------------------------------------------------------------------

void handle_bus_reset(usbd_device *dev)
{
  uint32_t i;

  // 1. NAK for all OUT endpoints
  for(i = 0; i < MAX_EP; i++)
  {
    OUT_ENDPOINT(i)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
    if(IN_ENDPOINT(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
      IN_ENDPOINT(i)->DIEPCTL |= (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK);
  }
  // Disable all IN endpoints
  for(i = 0; i < MAX_EP; i++)
  {
    if(IN_ENDPOINT(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
      IN_ENDPOINT(i)->DIEPCTL |= (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK);
  }

  // 2. Set up interrupt mask for EP0
  USB_OTG_FS_DEVICE->DAINTMSK = 0x10001U;
  USB_OTG_FS_DEVICE->DOEPMSK = USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM;
  USB_OTG_FS_DEVICE->DIEPMSK = USB_OTG_DIEPMSK_TOM | USB_OTG_DIEPMSK_XFRCM;

  // 4. Set up DFIFO
  flush_txfifo(0x10);  // 10000:在设备模式或主机模式下刷新所有的发送 FIFO。
  flush_rxfifo();
  dfifo_device_init();

  //evt = usbd_evt_reset;
  //callback(dev, evt, 0);

  // 5. Reset device address
  //setaddr(0);

  // Start reading setup
  // Fixed both control EP0 size to 64 bytes
  dwc2_ep0_start_read_setup();

}

static void dcd_int_handler(usbd_device *dev, usbd_evt_callback callback)
{
#if 0
  uint32_t i;
  uint32_t ep_intr;
  uint32_t epint;
  uint32_t epnum;
  uint32_t RegVal;
  uint32_t evt;
  volatile uint32_t gintsts = USB_OTG_FS_GLOBAL->GINTSTS;

  /* ensure that we are in device mode */
  if((gintsts & 0x1U) == 0) //USB_OTG_MODE_DEVICE
  {
    gintsts &= USB_OTG_FS_GLOBAL->GINTMSK;
	if(gintsts == 0 )
      return;

    /* Handle Reset Interrupt */
	if(gintsts & USB_OTG_GINTSTS_USBRST)
	{
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_USBRST;
      USB_OTG_FS_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

      flush_txfifo(0x10);  // 10000:在设备模式或主机模式下刷新所有的发送 FIFO。
      flush_rxfifo();
      for(i = 0U; i < MAX_EP; i++)
      {
        if(i == 0U)
        {
          IN_ENDPOINT(i)->DIEPCTL = USB_OTG_DIEPCTL_SNAK;
          OUT_ENDPOINT(i)->DOEPCTL = USB_OTG_DOEPCTL_SNAK;
        }else{
          if(IN_ENDPOINT(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA)
            IN_ENDPOINT(i)->DIEPCTL = (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK);
          else
            IN_ENDPOINT(i)->DIEPCTL = 0;

          if(OUT_ENDPOINT(i)->DOEPCTL & USB_OTG_DOEPCTL_EPENA)
            OUT_ENDPOINT(i)->DOEPCTL = (USB_OTG_DOEPCTL_EPDIS | USB_OTG_DOEPCTL_SNAK);
          else
            OUT_ENDPOINT(i)->DOEPCTL = 0;
        }
        IN_ENDPOINT(i)->DIEPTSIZ = 0U;
        IN_ENDPOINT(i)->DIEPINT = 0xFBFFU;
        OUT_ENDPOINT(i)->DOEPTSIZ = 0U;
        OUT_ENDPOINT(i)->DOEPINT = 0xFBFFU;
      }
      USB_OTG_FS_DEVICE->DAINTMSK |= 0x10001U;
      USB_OTG_FS_DEVICE->DOEPMSK = USB_OTG_DOEPMSK_STUPM | USB_OTG_DOEPMSK_XFRCM;
      USB_OTG_FS_DEVICE->DIEPMSK = USB_OTG_DIEPMSK_XFRCM;

      //setaddr(0);
      evt = usbd_evt_reset;
      callback(dev, evt, 0);
	}
    /* Handle Enumeration done Interrupt */
    if(gintsts & USB_OTG_GINTSTS_ENUMDNE)
    {
      //USB_OTG_FS_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;//(void)USB_ActivateSetup(hpcd->Instance);

      /* Set USB Turnaround time */
      //set_turnaroundTime(3); // speed = (USB_OTG_FS_DEVICE->DSTS & USB_OTG_DSTS_ENUMSPD) >> 1;
      //evt = usbd_evt_reset;
      //callback(dev, evt, 0);
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;
    }
    /* Handle RxQLevel Interrupt */
    if(gintsts & USB_OTG_GINTSTS_RXFLVL)
    {
      // RXFLVL bit is read-only
      USB_OTG_FS_GLOBAL->GINTMSK &= ~USB_OTG_GINTMSK_RXFLVLM; // disable RXFLVL interrupt while reading

      do{
      RegVal = USB_OTG_FS_GLOBAL->GRXSTSP;
      epnum = RegVal & USB_OTG_GRXSTSP_EPNUM;
      //const dwc2_grxstsp_t grxstsp = {.value = USB_OTG_FS_GLOBAL->GRXSTSP};

      switch((RegVal & USB_OTG_GRXSTSP_PKTSTS) >> 17)
      {
        case 0x02:  //STS_DATA_UPDT
          evt = usbd_evt_eprx;
          break;
        case 0x06:  //STS_SETUP_UPDT
          evt = usbd_evt_epsetup;
          break;
        default:
          //USB_OTG_FS_GLOBAL->GRXSTSP;
          break;
      }
      callback(dev, evt, epnum);
      }while(USB_OTG_FS_GLOBAL->GINTSTS & USB_OTG_GINTSTS_RXFLVL);

      USB_OTG_FS_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;
    }
    //OUT endpoint interrupt
    if(gintsts & USB_OTG_GINTSTS_OEPINT)
    {
      epnum = 0U;
      /* Read in the device interrupt bits */
      ep_intr = USB_OTG_FS_DEVICE->DAINT;
      ep_intr &= USB_OTG_FS_DEVICE->DAINTMSK;
      ep_intr = ((ep_intr & 0xffff0000U) >> 16);
      while(ep_intr != 0U)
      {
        if((ep_intr & 0x1U) != 0U)
        {
          epint = get_outep_interrupt(epnum);
          if((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)
          {
            if((epnum == 0U) && (dev->status.data_count == 0U)) /* this is ZLP, so prepare EP0 for next setup */
              ep_config(0, USB_EPTYPE_CONTROL, dev->status.ep0size);
            evt = usbd_evt_eprx;
            callback(dev, evt, epnum);
          }
        }
        epnum++;
        ep_intr >>= 1U;
      }
    }

    if(gintsts & USB_OTG_GINTSTS_IEPINT)
    {
      /* Read in the device interrupt bits */
      ep_intr = USB_OTG_FS_DEVICE->DAINT;
      ep_intr &= USB_OTG_FS_DEVICE->DAINTMSK;
      ep_intr &= 0xffff;

      epnum = 0U;
      while(ep_intr != 0U)
      {
        if((ep_intr & 0x1U) != 0U) /* In ITR */
        {
          epint = get_inep_interrupt(epnum); //epint = IN_ENDPOINT(epnum)->DIEPINT;
          if((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC)
          {
            //fifoemptymsk = (uint32_t)(0x1UL << (epnum & 0xFU));
            //USB_OTG_FS_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;
            evt = usbd_evt_eptx;
            callback(dev, evt, (epnum|0x80));
          }
        }
        epnum++;
        ep_intr >>= 1U;
      }
    }

    /* Handle Incomplete ISO OUT Interrupt */
    if(gintsts & USB_OTG_GINTSTS_PXFR_INCOMPISOOUT)
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_PXFR_INCOMPISOOUT;
    }
    /* Handle Incomplete ISO IN Interrupt */
    if(gintsts & USB_OTG_GINTSTS_IISOIXFR)
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_IISOIXFR;
    }
    /* Handle SOF Interrupt */
#if !defined(USBD_SOF_DISABLED)
    if(gintsts & USB_OTG_GINTSTS_SOF)
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_SOF;
      RegVal = USB_OTG_FS_GLOBAL->GRXSTSP;
      epnum = RegVal & USB_OTG_GRXSTSP_EPNUM;
      evt = usbd_evt_sof;
      callback(dev, evt, epnum);
    }
#endif
    /* Handle Suspend Interrupt */
    if(gintsts & USB_OTG_GINTSTS_USBSUSP)
    {
      if((USB_OTG_FS_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)
      {
        //HAL_PCD_SuspendCallback(hpcd);
      }
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_USBSUSP;
    }
    /* Handle Resume Interrupt */
    if(gintsts & USB_OTG_GINTSTS_WKUINT)
    {
      /* Clear the Remote Wake-up Signaling */
      USB_OTG_FS_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_WKUINT;
      RegVal = USB_OTG_FS_GLOBAL->GRXSTSP;
      epnum = RegVal & USB_OTG_GRXSTSP_EPNUM;
      evt = usbd_evt_wkup;
      callback(dev, evt, epnum);
    }
    /* Handle Disconnection event Interrupt */
    if(gintsts & USB_OTG_GINTSTS_OTGINT)
    {
      RegVal = USB_OTG_FS_GLOBAL->GOTGINT;

      if((RegVal & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET)
      {

      }else{

      }
      USB_OTG_FS_GLOBAL->GOTGINT |= RegVal;
    }
    if(gintsts & USB_OTG_GINTSTS_MMIS) //mode mismatch, device mode rw host reg or host mode rw device reg
    {
      USB_OTG_FS_GLOBAL->GINTSTS |= USB_OTG_GINTSTS_MMIS; /* incorrect mode, acknowledge the interrupt */
    }
    /* Handle LPM Interrupt, LinkPowerMangement */
    if(gintsts & USB_OTG_GINTSTS_LPMINT)
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_LPMINT;
    }
    /* Handle Connection event Interrupt */
    if(gintsts & USB_OTG_GINTSTS_SRQINT)
    {
      USB_OTG_FS_GLOBAL->GINTSTS =  USB_OTG_GINTSTS_SRQINT;
    }

  }
#else
  uint32_t evt;
  uint32_t ep = 0;
  while(1)
  {
    uint32_t _t = USB_OTG_FS_GLOBAL->GINTSTS;

    _t &= USB_OTG_FS_GLOBAL->GINTMSK;
	if(_t == 0 )
      return;

    if(_t & USB_OTG_GINTSTS_USBRST)/* bus RESET event */
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_USBRST;

      for(uint8_t i = 0; i < MAX_EP; i++ )
      {
        ep_deconfig(i);
      }
      flush_rxfifo();

      evt = usbd_evt_reset;

      //usbd_spin_lock(true);
      //handle_bus_reset();
      //usbd_spin_unlock(true);
    }
    else if(_t & USB_OTG_GINTSTS_ENUMDNE)/* 枚举完成 */
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;
      callback(dev, evt, ep);
    }
    else if(_t & USB_OTG_GINTSTS_IEPINT)
    {
      /**
       *  设备模式下IN端点挂起中断，必须读 OTG_DAINT 确定以确定发生中断的 IN 端点的准确编号
       *  然后读取相应的 OTG_DIEPINTx 寄存器,以确定引发中断的确切原因
       *  应用程序必须先将相应 OTG_DIEPINTx 寄存器的相应状态位清零,之后才能将该位清零
       */
      for(;; ep++)
      {
        if(ep >= MAX_EP)
          return;
        if(IN_ENDPOINT(ep)->DIEPINT & USB_OTG_DIEPINT_XFRC)
        {
          IN_ENDPOINT(ep)->DIEPINT = USB_OTG_DIEPINT_XFRC;
          evt = usbd_evt_eptx;
          ep |= 0x80;
          break;
        }
      }
    }
    else if(_t & USB_OTG_GINTSTS_RXFLVL) /* Rx FIFO 非空 */
    {
      _t = USB_OTG_FS_GLOBAL->GRXSTSR;
      ep = _t & USB_OTG_GRXSTSP_EPNUM;
      switch(_FLD2VAL(USB_OTG_GRXSTSP_PKTSTS, _t))
      //_t = ((_t & USB_OTG_GRXSTSP_PKTSTS_Msk) >> USB_OTG_GRXSTSP_PKTSTS);
      //switch(_t)
      {
        case 0x02:  // 接收到 OUT 数据包
          evt = usbd_evt_eprx;
          break;
        case 0x06:  // 接收到 SETUP 数据包
          evt = usbd_evt_epsetup;
          break;

        default:
          USB_OTG_FS_GLOBAL->GRXSTSP;
          continue;
      }
#if !defined(USBD_SOF_DISABLED)
    }
    else if(_t & USB_OTG_GINTSTS_SOF)  /* 帧起始 (Start of frame) SOF令牌，只有USB建立连接后有效 */
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_SOF;
      evt = usbd_evt_sof;
#endif
    }
    else if(_t & USB_OTG_GINTSTS_USBSUSP) /* 仅设备模式访问，USB挂起  */
    {
      evt = usbd_evt_susp;
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_USBSUSP;
    }
    else if(_t & USB_OTG_GINTSTS_WKUINT) /* 远程唤醒中断 */
    {
      USB_OTG_FS_GLOBAL->GINTSTS = USB_OTG_GINTSTS_WKUINT;
      evt = usbd_evt_wkup;
    }
    else
    {
      /* no more supported events */
      return;
    }
    callback(dev, evt, ep);
  }
#endif
}

static uint32_t fnv1a32_turn (uint32_t fnv, uint32_t data )
{
  for(int i = 0; i < 4 ; i++)
  {
    fnv ^= (data & 0xFF);
    fnv *= 16777619;
    data >>= 8;
  }
  return fnv;
}

static uint16_t get_serialno_desc(void *buffer) {
    struct  usb_string_descriptor *dsc = buffer;
    uint16_t *str = dsc->wString;
    uint32_t fnv = 2166136261;
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x00));
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x04));
    fnv = fnv1a32_turn(fnv, *(uint32_t*)(UID_BASE + 0x08));
    for (int i = 28; i >= 0; i -= 4 ) {
        uint16_t c = (fnv >> i) & 0x0F;
        c += (c < 10) ? '0' : ('A' - 10);
        *str++ = c;
    }
    dsc->bDescriptorType = USB_DTYPE_STRING;
    dsc->bLength = 18;
    return 18;
}

/**
 * __attribute__关键字 : 编译器指令，告诉编译器声明的特性，或者让编译器进行更多的错误检查和高级优化。
 * __attribute__((externally_visible)) : 被标记的函数或变量在当前编译单元外部仍然可见。
 */
__attribute__((externally_visible)) const struct usbd_driver usbd_otgfs = {
  getinfo,
  enable,
  connect,
  setaddr,
  ep_config,
  ep_deconfig,
  ep_read,
  ep_write,
  ep_setstall,
  ep_isstalled,
  dcd_int_handler,
  get_frame,
  get_serialno_desc,
};

#endif
