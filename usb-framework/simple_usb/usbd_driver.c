/*
 * usbd_driver.c
 *
 *  Created on: Mar 6, 2023
 *      Author: IsaiahOdunze
 */
#include "usbd_driver.h"
#include "usb_device.h"

#define MAX_EP          6
#define MAX_RX_PACKET   128
#define MAX_CONTROL_EP  1
#define MAX_FIFO_SZ     320  /*in 32-bit chunks */

#define RX_FIFO_SZ      ((5 * MAX_CONTROL_EP + 8) + ((MAX_RX_PACKET / 4) + 1) + (MAX_EP * 2) + 1)
//#define RX_FIFO_SZ      (59)
//#define STATUS_VAL(x)   (USBD_HW_ADDRFST | (x))

static void initialize_gpio_pins(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure USB FS GPIOs */
  //Enables the clock for GPIOA GPIOG
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); //SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);//SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);

  /**USBOTG GPIO Configuration
  PA9    ------> OTG_FS_VBUS ?
  PA10   ------> USB_ID      OTG_FS_ID
  PA11   ------> USB_DM D-   OTG_FS_DM
  PA12   ------> USB_DP D+   OTG_FS_DP
  PG6    ------> USB_PowerSwitchOn
  PG7    ------> USB_OverCurrent
  */
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
}

static void initialize_core(void)
{
  //Enables the clock for USB core
  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

  // Wait for AHB idle.
  while(!(USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL))
    __NOP();

  // core soft reset
  USB_OTG_FS_GLOBAL->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;
  while (USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_CSRST)
    __NOP();

  //Configures the USB core to run in device mode, and to use the embedded full-speed PHY.
  USB_OTG_FS_GLOBAL->GUSBCFG = USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | _VAL2FLD(USB_OTG_GUSBCFG_TRDT, 0x06);

  //Enable VBUS sensing device and powerup PHY
#if defined(USBD_VBUS_DETECT)
  USB_OTG_FS_GLOBAL->GCCFG |= USB_OTG_GCCFG_VBDEN | USB_OTG_GCCFG_PWRDWN;
#else
  USB_OTG_FS_GLOBAL->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN | USB_OTG_GOTGCTL_BVALOVAL;
  USB_OTG_FS_GLOBAL->GCCFG = USB_OTG_GCCFG_PWRDWN;
#endif

#if defined(USBD_DMA_MODE)
  USB_OTG_FS_GLOBAL->GAHBCFG |= USB_OTG_GAHBCFG_HBSTLEN_2;
  USB_OTG_FS_GLOBAL->GAHBCFG |= USB_OTG_GAHBCFG_DMAEN;
#endif

  /* restart PHY*/
  USB_OTG_FS_PCGCCTL = 0;
  /* soft disconnect device */
  USB_OTG_FS_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;
  /* Setup USB FS speed and frame interval */
  USB_OTG_FS_DEVICE->DCFG |= (_VAL2FLD(USB_OTG_DCFG_PERSCHIVL, 0) | _VAL2FLD(USB_OTG_DCFG_DSPD, 0x03));
  /* unmask EP interrupts */
  USB_OTG_FS_DEVICE->DIEPMSK = USB_OTG_DIEPMSK_XFRCM;
  //USB_OTG_FS_DEVICE->DOEPMSK = USB_OTG_DOEPMSK_XFRCM;  //device mode not use ??

  /*
  //Unmasks the main USB core interrupts
  SET_BIT(USB_OTG_HS->GINTMSK,
    USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
    USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
    USB_OTG_GINTSTS_OEPINT | USB_OTG_GINTMSK_RXFLVLM
  );
  */
  /* unmask core interrupts */
  USB_OTG_FS_GLOBAL->GINTMSK  = USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM |
#if !defined(USBD_SOF_DISABLED)
                  USB_OTG_GINTMSK_SOFM |
#endif
                  USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM |
                  USB_OTG_GINTMSK_IEPINT | USB_OTG_GINTMSK_RXFLVLM;
  //Clears all pending core interrupts.
  USB_OTG_FS_GLOBAL->GINTSTS = 0xFFFFFFFF;
  //Unmasks USB global interrupts.
  USB_OTG_FS_GLOBAL->GAHBCFG = USB_OTG_GAHBCFG_GINT;
  /* setting max RX FIFO size */
  USB_OTG_FS_GLOBAL->GRXFSIZ = RX_FIFO_SZ;
  /* setting up EP0 TX FIFO SZ as 64 byte */
  USB_OTG_FS_GLOBAL->DIEPTXF0_HNPTXFSIZ = RX_FIFO_SZ | (0x10 << 16);

}

static void set_device_address(uint8_t address)
{
  //#define _VAL2FLD(field, value)    (((uint32_t)(value) << field ## _Pos) & field ## _Msk)
  MODIFY_REG(USB_OTG_FS_DEVICE->DCFG, USB_OTG_DCFG_DAD, _VAL2FLD(USB_OTG_DCFG_DAD,address));
}

//Connects the USB device to the bus. (brief)
static void connect(uint8_t connect)
{
  // Normal operation  1:device disconnect event to the USB host
  if(connect != 0)
    USB_OTG_FS_DEVICE->DCTL &= ~USB_OTG_DCTL_SDIS; //CLEAR_BIT(USB_OTG_FS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);
  else
    USB_OTG_FS_DEVICE->DCTL |= USB_OTG_DCTL_SDIS; //SET_BIT(USB_OTG_FS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);
}

// Flushes RxFIFO buffer for all OUT endpoints
static void flush_rxfifo(void)
{
  SET_BIT(USB_OTG_FS_GLOBAL->GRSTCTL, USB_OTG_GRSTCTL_RXFFLSH);
}

// Updates the start addresses of all FIFOs according to their size
static void refresh_fifo_start_addresses(void)
{
  uint16_t start_address = _FLD2VAL(USB_OTG_GRXFSIZ_RXFD, USB_OTG_FS->GRXFSIZ) * 4;
  //#define _FLD2VAL(field, value)    (((uint32_t)(value) & field ## _Msk) >> field ## _Pos)
  MODIFY_REG(USB_OTG_HS->DIEPTXF0_HNPTXFSIZ, USB_OTG_TX0FSA, _VAL2FLD(USB_OTG_TX0FSA, start_address));

  start_address += _FLD2VAL(USB_OTG_TX0FD, USB_OTG_FS->DIEPTXF0_HNPTXFSIZ) * 4;

  for(uint8_t txfifo_number = 0 ; txfifo_number < ENDPOINT_COUNT - 1; txfifo_number++)
  {
    MODIFY_REG(USB_OTG_HS->DIEPTXF[txfifo_number], USB_OTG_NPTXFSA, _VAL2FLD(USB_OTG_NPTXFSA, start_address));
    start_address += _FLD2VAL(USB_OTG_NPTXFD, USB_OTG_HS->DIEPTXF[txfifo_number]) * 4;
  }
}

/** \brief configures the RxFIFIO of all OUT endpoints
	\param size The size of the largest OUT endpoint in bytes
	\note RxFIFO is shared between all OUT endpoints
*/
static void configure_rxfifo_size(uint16_t size)
{
  /** for stm32f767 note Information in section 41.11.3 of Reference manual
  Device RxFIFO = (5 * number of control endpoints + 8)
	              + ((largest USB packet used / 4) + 1 for status information)
	              + (2 * number of OUT endpoints) + 1 for Global NAK
  */
  size = (5*1 + 8) + ((size/4) + 1) + 2*6 + 1;
  //configures depth of FIFO
  MODIFY_REG(USB_OTG_HS->GRXFSIZ, USB_OTG_GRXFSIZ_RXFD, _VAL2FLD(USB_OTG_GRXFSIZ_RXFD, size));
  refresh_fifo_start_addresses();
}

/** \brief Configures the TxFIFO of and IN endpoint
 *  \param endpoint_number The number of the IN endpoint we want to confugre its TxFIFO
 *  \param size The size of the IN endpoint in bytes.
 *  \note Any change of any FIFO will update the registers of all TxFIFOs to adapt the start offsets
 */
static void configure_txfifo_size(uint8_t endpoint_number, uint16_t size)
{
  size = (size + 3)/4;

  if(endpoint_number == 0)
  {
    MODIFY_REG(USB_OTG_HS->DIEPTXF0_HNPTXFSIZ, USB_OTG_TX0FD, _VAL2FLD(USB_OTG_TX0FD, size));
  }else{
    MODIFY_REG(USB_OTG_HS->DIEPTXF[endpoint_number - 1], USB_OTG_NPTXFD, _VAL2FLD(USB_OTG_NPTXFD, size));
  }
  refresh_fifo_start_addresses();
}
static uint8_t set_tx_fifo(uint8_t ep, uint16_t epsize)
{
  uint32_t _fsa = USB_OTG_FS->DIEPTXF0_HNPTXFSIZ;
  /* calculating initial TX FIFO address. next from EP0 TX fifo */
  _fsa = 0xFFFF & (_fsa + (_fsa >> 16));
  /* looking for next free TX fifo address */
  for(int i = 0; i < (ENDPOINT_COUNT - 1); i++)
  {
    uint32_t _t = USB_OTG_FS->DIEPTXF[i];
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
	return 1;
  /* programming fifo register */
  _fsa |= (epsize << 16);
  USB_OTG_FS->DIEPTXF[ep - 1] = _fsa;
  return 0;
}

static void read_packet(void const *buffer, uint16_t size)
{
  //remember there is only one RxFIFO
  uint32_t *fifo = FIFO(0);

  for(; size >= 4; size -=4, buffer += 4)
  {
    //pop a 32-bit word of data ( untill less than one word remaining)
    uint32_t data = *fifo;

    //stores data in a buffer
    *((uint32_t*)buffer) = data;
  }

  if(size > 0)
  {
    //pops the last remaining bytes (which are less than one word).
    uint32_t data = *fifo;
    for(; size > 0; size--, buffer++, data >>= 8)
    {
      //Stores the data in the buffer with the correct alignment.
      *((uint8_t*)buffer) = 0xFF & data;
    }
  }
}

/** \brief pushes a packet int the TxFIFO of an IN endpoint
 * param endpoint_number The number of the endpoint where data will be written
 * param buffer Pointer to the buffer contains the data to be written to the endpoint
 * param size The size of data to be written in bytes.
 */
static void write_packet(uint8_t endpoint_number, void const *buffer, uint16_t size)
{
  uint32_t *fifo = FIFO(endpoint_number);
  USB_OTG_INEndpointTypeDef *in_endpoint = IN_ENDPOINT(endpoint_number);

  //Configures the transmission (1 packet that has size bytes).
  MODIFY_REG(in_endpoint->DIEPTSIZ, USB_OTG_DIEPTSIZ_PKTCNT | USB_OTG_DIEPTSIZ_XFRSIZ,
			   _VAL2FLD(USB_OTG_DIEPTSIZ_PKTCNT, 1) | _VAL2FLD(USB_OTG_DIEPTSIZ_XFRSIZ, size));

  // Enables the transmission after clearing both STALL and NAK of the endpoint
  MODIFY_REG(in_endpoint->DIEPCTL, USB_OTG_DIEPCTL_STALL, USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);

  // Get the size in terms of 32 it words
  size = (size + 3) / 4;

  for(; size > 0 ; size--, buffer += 4)
  {
    //push data to TxFIFO
    *fifo = *((uint32_t*)buffer);
  }
}

//Flushes the TxFIFO buffer of an IN endpoint
// param endpoint_number The number of the IN endpoint to flush its TxFIFO buffer
static uint8_t flush_txfifo(uint8_t endpoint_number)
{
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
  //USB_OTG_FS_GLOBAL->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (endpoint_number << 6)); //USB_OTG_GRSTCTL_TXFNUM_Pos
  MODIFY_REG(USB_OTG_FS->GRSTCTL, USB_OTG_GRSTCTL_TXFNUM,
			   _VAL2FLD(USB_OTG_GRSTCTL_TXFNUM, endpoint_number) | USB_OTG_GRSTCTL_TXFFLSH);
  do
  {
    if (count++ > 0xF000000)
    {
      return 1;
    }
  } while((USB_OTG_FS_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);

  return 0;
}

static void configure_endpoint0(uint8_t endpoint_size)
{
  //Unmask all interrupts of IN and OUT endpoint0
  SET_BIT(USB_OTG_FS_DEVICE->DAINTMSK, 1 << 0 | 1 << 16);

  //Configures the maximum packet size, activates the endpoint, and NAK the endpoint (cannot send data)
  MODIFY_REG(IN_ENDPOINT(0)->DIEPCTL, USB_OTG_DIEPCTL_MPSIZ,
			   USB_OTG_DIEPCTL_USBAEP | _VAL2FLD(USB_OTG_DIEPCTL_MPSIZ, endpoint_size) | USB_OTG_DIEPCTL_SNAK);

  // Clears NAK, and enables endpoint data transmission
  SET_BIT(OUT_ENDPOINT(0)->DOEPCTL, USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK);

  configure_rxfifo_size(128);
  configure_txfifo_size(0, endpoint_size);
}

static void configure_in_endpoint(uint8_t endpoint_number, UsbEndpointType endpoint_type, uint16_t endpoint_size)
{
  if(endpoint_number == 0)
  {
    /* configureing control endpoint EP0 */
    uint32_t mpsize;
    if(endpoint_size <= 0x08){
      endpoint_size = 0x08;
      mpsize = 0x03;
    }
    else if(endpoint_size <= 0x10){
      endpoint_size = 0x10;
      mpsize = 0x02;
    }
    else if (endpoint_size <= 0x20){
      endpoint_size = 0x20;
      mpsize = 0x01;
    }else{
      endpoint_size = 0x40;
      mpsize = 0x00;
    }
    /* EP0 TX FIFO size is setted on init level */
    //Unmask all interrupts of IN and OUT endpoint0; enabling RX and TX interrupts from EP0
    SET_BIT(USB_OTG_FS_DEVICE->DAINTMSK, 1 << 0 | 1 << 16);
    /* setting up EP0 TX and RX registers */
    //Configures the maximum packet size, activates the endpoint, and NAK the endpoint (cannot send data)
    IN_ENDPOINT(endpoint_number)->DIEPCTL = mpsize | USB_OTG_DIEPCTL_SNAK;
    /* 1 setup packet, 1 packets total */
    OUT_ENDPOINT(endpoint_number)->DOEPTSIZ = endpoint_size | (1 << 29) | (1 << 19);
    // Clears NAK, and enables endpoint data transmission; mpsize is in which only read bit
    OUT_ENDPOINT(endpoint_number)->DOEPCTL = mpsize | USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_CNAK;

    //configure_rxfifo_size(128);
    //configure_txfifo_size(0, endpoint_size);
    return;
  }

  if(endpoint_number & 0x80)
  {
    endpoint_number &= 0x7F;
    /* configuring TX endpoint */
    /* setting up TX fifo and size register */
    if((endpoint_type == USB_EPTYPE_ISOCHRONUS) ||
        (endpoint_type == (USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF))) {
        if (!set_tx_fifo(ep, epsize << 1)) return;//false
    } else {
        if (!set_tx_fifo(ep, epsize)) return; //false
    }

  //Unmasks all interrupts of the targeted IN endpoint
  SET_BIT(USB_OTG_HS_DEVICE->DAINTMSK, 1 << endpoint_number);

  //Activates the endpoint, sets endpoint handshake to NAK (not ready to send data), sets DATA0 packet
  //configures its type , its maximum packet size, and assigns it a TxFIFO.
  MODIFY_REG(IN_ENDPOINT(endpoint_number)->DIEPCTL,
      USB_OTG_DIEPCTL_MPSIZ | USB_OTG_DIEPCTL_EPTYP | USB_OTG_DIEPCTL_TXFNUM,
      USB_OTG_DIEPCTL_USBAEP | _VAL2FLD(USB_OTG_DIEPCTL_MPSIZ, endpoint_size) | USB_OTG_DIEPCTL_SNAK |
      _VAL2FLD(USB_OTG_DIEPCTL_EPTYP, endpoint_type)| _VAL2FLD(USB_OTG_DIEPCTL_TXFNUM, endpoint_number) | USB_OTG_DIEPCTL_SD0PID_SEVNFRM
  );

  configure_txfifo_size(endpoint_number, endpoint_size);
  }else{
    /* configuring RX endpoint */

  }
}

static void endpoint_deconfigure(uint8_t endpoint_number)
{
  USB_OTG_INEndpointTypeDef *in_endpoint = IN_ENDPOINT(endpoint_number);
  USB_OTG_OUTEndpointTypeDef *out_endpoint = OUT_ENDPOINT(endpoint_number);

  //Masks all interrupts of the targeted IN and OUT endpoints
  //CLEAR_BIT(USB_OTG_HS_DEVICE->DAINTMSK, (1 << endpoint_number) | (1 << 16 << endpoint_number));
  //USB_OTG_FS_DEVICE->DAINTMSK &= ~((1 << endpoint_number) | (1 << 16 << endpoint_number));
  USB_OTG_FS_DEVICE->DAINTMSK &= ~(0x10001 << endpoint_number);

  /* decativating endpoint */
  in_endpoint->DIEPCTL &= ~(USB_OTG_DIEPCTL_USBAEP);

  flush_txfifo(endpoint_number);
  flush_rxfifo();

  //disables the endpoints if possible
  if((in_endpoint->DIEPCTL & USB_OTG_DIEPCTL_EPENA) && (endpoint_number != 0))
  {
    //Disables endpoint transmission
    SET_BIT(in_endpoint->DIEPCTL, USB_OTG_DIEPCTL_EPDIS); //in_endpoint->DIEPCTL = USB_OTG_DIEPCTL_EPDIS; ?
  }

  //clears all interrupts of the endpoint
  SET_BIT(in_endpoint->DIEPINT, 0xFF);  // hs:0x29FF
  //SET_BIT(out_endpoint->DOEPINT, 0x715F);
  /* deconfiguring TX FIFO */
  if(endpoint_number > 0)
    USB_OTG_FS_GLOBAL->DIEPTXF[endpoint_number-1] = 0x02000200 + 0x200 * endpoint_number;
  /* deconfigureing RX part */
  out_endpoint->DOEPCTL &= ~(USB_OTG_DOEPCTL_USBAEP);
  if((out_endpoint->DOEPCTL & USB_OTG_DOEPCTL_EPENA) && (endpoint_number != 0))
  {
    SET_BIT(out_endpoint->DOEPCTL, USB_OTG_DOEPCTL_EPDIS); //out_endpoint->DOEPCTL = USB_OTG_DOEPCTL_EPDIS;
  }
  out_endpoint->DOEPINT = 0xFF;
}

static void usbrst_handler()
{
  //log_info("USB reset signal was detected.");
  for(uint8_t i = 0; i <= ENDPOINT_COUNT; i++)
  {
    endpoint_deconfigure(i);
  }

  usb_events.on_usb_reset_received();
}

//Enumeration done interrupt handler
//note Can be refactored to get packet size from framework layer.
static void enumdne_handler()
{
  //log_info("USB device speed enumeration done.");
  configure_endpoint0(8);

  //(void)USB_ActivateSetup(hpcd->Instance);
  //hpcd->Init.speed = USB_GetDevSpeed(hpcd->Instance);

  /* Set USB Turnaround time */
  //(void)USB_SetTurnaroundTime(hpcd->Instance, HAL_RCC_GetHCLKFreq(), (uint8_t)hpcd->Init.speed);
}

static void rxflvl_handler()
{
	//Pop the status information word from the RxFIFO.
	uint32_t receive_status = USB_OTG_HS_GLOBAL->GRXSTSP;

	//endpoint that received the data
	uint8_t endpoint_number = _FLD2VAL(USB_OTG_GRXSTSP_EPNUM, receive_status);
	// Count of bytes in the received packet
	uint16_t bcnt = _FLD2VAL(USB_OTG_GRXSTSP_BCNT, receive_status);
	// Status of received packet
	uint16_t pktsts = _FLD2VAL(USB_OTG_GRXSTSP_PKTSTS, receive_status);

	switch(pktsts)
	{
	case 0x06: //setup packet (followed by data)
		usb_events.on_setup_data_received(endpoint_number, bcnt);
		break;
	case 0x02:// OUT packet (includes data)
		//TODO
		break;
	case 0x04: // SETUP stage is complete
		//re-enable transmission on endpoint
		SET_BIT(OUT_ENDPOINT(endpoint_number)->DOEPCTL,
				USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
		break;
	case 0x03: // OUT transfer is complete
		//re-enable transmission on endpoint
		SET_BIT(OUT_ENDPOINT(endpoint_number)->DOEPCTL,
			    USB_OTG_DOEPCTL_CNAK | USB_OTG_DOEPCTL_EPENA);
		break;
	}
}



//handles USB core interrupts
static void usbd_gintsts_handler()
{

	volatile uint32_t gintsts = USB_OTG_HS_GLOBAL->GINTSTS;

	if(gintsts & USB_OTG_GINTSTS_USBRST)
	{
		usbrst_handler();
		//Clears the interrupt
		SET_BIT(USB_OTG_HS_GLOBAL->GINTSTS, USB_OTG_GINTSTS_USBRST);
	}
	else if(gintsts & USB_OTG_GINTSTS_ENUMDNE)
	{
		enumdne_handler();
		SET_BIT(USB_OTG_HS_GLOBAL->GINTSTS, USB_OTG_GINTSTS_ENUMDNE);
	}
	else if(gintsts & USB_OTG_GINTSTS_RXFLVL)
	{
		rxflvl_handler();
		SET_BIT(USB_OTG_HS_GLOBAL->GINTSTS, USB_OTG_GINTSTS_RXFLVL);
	}
	else if(gintsts & USB_OTG_GINTSTS_IEPINT)
	{

	}
	else if(gintsts & USB_OTG_GINTSTS_OEPINT)
	{

	}
}
void usbd_gintsts_handler(UsbDevice *usb_device)
{
  uint32_t i;
  uint32_t ep_intr;
  uint32_t epint;
  uint32_t epnum;
  uint32_t fifoemptymsk;
  uint32_t RegVal;

  uint32_t FrameNumber;
  volatile uint32_t gintsts = USB_OTG_FS_GLOBAL->GINTSTS;

  /* ensure that we are in device mode */
  if (usb_device->device_mode == USB_OTG_MODE_DEVICE)
  {
    /* store current frame number  接收 SOF 的帧编号 */
    FrameNumber = (USB_OTG_FS_DEVICE->DSTS & USB_OTG_DSTS_FNSOF_Msk) >> USB_OTG_DSTS_FNSOF_Pos;

    if(gintsts & USB_OTG_GINTSTS_MMIS)
    {
      /* incorrect mode, acknowledge the interrupt */
      USB_OTG_FS_GLOBAL->GINTSTS |= USB_OTG_GINTSTS_MMIS;
    }
    /* Handle Reset Interrupt */
	if(gintsts & USB_OTG_GINTSTS_USBRST)
	{
      usbrst_handler();
      //Clears the interrupt
      USB_OTG_FS_GLOBAL->GINTSTS |= USB_OTG_GINTSTS_USBRST;
	}
    /* Handle Enumeration done Interrupt */
    if(gintsts & USB_OTG_GINTSTS_ENUMDNE)
    {
      enumdne_handler();
      SET_BIT(USB_OTG_FS_GLOBAL->GINTSTS, USB_OTG_GINTSTS_ENUMDNE);

      (void)USB_ActivateSetup(hpcd->Instance);
      hpcd->Init.speed = USB_GetDevSpeed(hpcd->Instance);

      /* Set USB Turnaround time */
      (void)USB_SetTurnaroundTime(hpcd->Instance,
                                  HAL_RCC_GetHCLKFreq(),
                                  (uint8_t)hpcd->Init.speed);

    }


    /* Handle RxQLevel Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_RXFLVL))
    {
      USB_MASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);

      RegVal = USBx->GRXSTSP;

      ep = &hpcd->OUT_ep[RegVal & USB_OTG_GRXSTSP_EPNUM];

      if (((RegVal & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_DATA_UPDT)
      {
        if ((RegVal & USB_OTG_GRXSTSP_BCNT) != 0U)
        {
          (void)USB_ReadPacket(USBx, ep->xfer_buff,
                               (uint16_t)((RegVal & USB_OTG_GRXSTSP_BCNT) >> 4));

          ep->xfer_buff += (RegVal & USB_OTG_GRXSTSP_BCNT) >> 4;
          ep->xfer_count += (RegVal & USB_OTG_GRXSTSP_BCNT) >> 4;
        }
      }
      else if (((RegVal & USB_OTG_GRXSTSP_PKTSTS) >> 17) == STS_SETUP_UPDT)
      {
        (void)USB_ReadPacket(USBx, (uint8_t *)hpcd->Setup, 8U);
        ep->xfer_count += (RegVal & USB_OTG_GRXSTSP_BCNT) >> 4;
      }
      else
      {
        /* ... */
      }

      USB_UNMASK_INTERRUPT(hpcd->Instance, USB_OTG_GINTSTS_RXFLVL);
    }

    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OEPINT))
    {
      epnum = 0U;

      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllOutEpInterrupt(hpcd->Instance);

      while (ep_intr != 0U)
      {
        if ((ep_intr & 0x1U) != 0U)
        {
          epint = USB_ReadDevOutEPInterrupt(hpcd->Instance, (uint8_t)epnum);

          if ((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_XFRC);
            (void)PCD_EP_OutXfrComplete_int(hpcd, epnum);
          }

          if ((epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_STUP);
            /* Class B setup phase done for previous decoded setup */
            (void)PCD_EP_OutSetupPacket_int(hpcd, epnum);
          }

          if ((epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPDIS);
          }

          /* Clear OUT Endpoint disable interrupt */
          if ((epint & USB_OTG_DOEPINT_EPDISD) == USB_OTG_DOEPINT_EPDISD)
          {
            if ((USBx->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF) == USB_OTG_GINTSTS_BOUTNAKEFF)
            {
              USBx_DEVICE->DCTL |= USB_OTG_DCTL_CGONAK;
            }

            ep = &hpcd->OUT_ep[epnum];

            if (ep->is_iso_incomplete == 1U)
            {
              ep->is_iso_incomplete = 0U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
              hpcd->ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);
#else
              HAL_PCD_ISOOUTIncompleteCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
            }

            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_EPDISD);
          }

          /* Clear Status Phase Received interrupt */
          if ((epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);
          }

          /* Clear OUT NAK interrupt */
          if ((epint & USB_OTG_DOEPINT_NAK) == USB_OTG_DOEPINT_NAK)
          {
            CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_NAK);
          }
        }
        epnum++;
        ep_intr >>= 1U;
      }
    }

    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IEPINT))
    {
      /* Read in the device interrupt bits */
      ep_intr = USB_ReadDevAllInEpInterrupt(hpcd->Instance);

      epnum = 0U;

      while (ep_intr != 0U)
      {
        if ((ep_intr & 0x1U) != 0U) /* In ITR */
        {
          epint = USB_ReadDevInEPInterrupt(hpcd->Instance, (uint8_t)epnum);

          if ((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC)
          {
            fifoemptymsk = (uint32_t)(0x1UL << (epnum & EP_ADDR_MSK));
            USBx_DEVICE->DIEPEMPMSK &= ~fifoemptymsk;

            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_XFRC);

            if (hpcd->Init.dma_enable == 1U)
            {
              hpcd->IN_ep[epnum].xfer_buff += hpcd->IN_ep[epnum].maxpacket;

              /* this is ZLP, so prepare EP0 for next setup */
              if ((epnum == 0U) && (hpcd->IN_ep[epnum].xfer_len == 0U))
              {
                /* prepare to rx more setup packets */
                (void)USB_EP0_OutStart(hpcd->Instance, 1U, (uint8_t *)hpcd->Setup);
              }
            }

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
            hpcd->DataInStageCallback(hpcd, (uint8_t)epnum);
#else
            HAL_PCD_DataInStageCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
          }
          if ((epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_TOC);
          }
          if ((epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_ITTXFE);
          }
          if ((epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE)
          {
            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_INEPNE);
          }
          if ((epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD)
          {
            (void)USB_FlushTxFifo(USBx, epnum);

            ep = &hpcd->IN_ep[epnum];

            if (ep->is_iso_incomplete == 1U)
            {
              ep->is_iso_incomplete = 0U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
              hpcd->ISOINIncompleteCallback(hpcd, (uint8_t)epnum);
#else
              HAL_PCD_ISOINIncompleteCallback(hpcd, (uint8_t)epnum);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
            }

            CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_EPDISD);
          }
          if ((epint & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE)
          {
            (void)PCD_WriteEmptyTxFifo(hpcd, epnum);
          }
        }
        epnum++;
        ep_intr >>= 1U;
      }
    }

    /* Handle Resume Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT))
    {
      /* Clear the Remote Wake-up Signaling */
      USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

      if (hpcd->LPM_State == LPM_L1)
      {
        hpcd->LPM_State = LPM_L0;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->LPMCallback(hpcd, PCD_LPM_L0_ACTIVE);
#else
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L0_ACTIVE);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
      else
      {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->ResumeCallback(hpcd);
#else
        HAL_PCD_ResumeCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT);
    }

    /* Handle Suspend Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP))
    {
      if ((USBx_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS)
      {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->SuspendCallback(hpcd);
#else
        HAL_PCD_SuspendCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP);
    }

    /* Handle LPM Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT))
    {
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT);

      if (hpcd->LPM_State == LPM_L0)
      {
        hpcd->LPM_State = LPM_L1;
        hpcd->BESL = (hpcd->Instance->GLPMCFG & USB_OTG_GLPMCFG_BESL) >> 2U;

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->LPMCallback(hpcd, PCD_LPM_L1_ACTIVE);
#else
        HAL_PCDEx_LPM_Callback(hpcd, PCD_LPM_L1_ACTIVE);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
      else
      {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->SuspendCallback(hpcd);
#else
        HAL_PCD_SuspendCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
    }



    /* Handle SOF Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SOF))
    {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
      hpcd->SOFCallback(hpcd);
#else
      HAL_PCD_SOFCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SOF);
    }

    /* Handle Global OUT NAK effective Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_BOUTNAKEFF))
    {
      USBx->GINTMSK &= ~USB_OTG_GINTMSK_GONAKEFFM;

      for (epnum = 1U; epnum < hpcd->Init.dev_endpoints; epnum++)
      {
        if (hpcd->OUT_ep[epnum].is_iso_incomplete == 1U)
        {
          /* Abort current transaction and disable the EP */
          (void)HAL_PCD_EP_Abort(hpcd, (uint8_t)epnum);
        }
      }
    }

    /* Handle Incomplete ISO IN Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR))
    {
      for (epnum = 1U; epnum < hpcd->Init.dev_endpoints; epnum++)
      {
        RegVal = USBx_INEP(epnum)->DIEPCTL;

        if ((hpcd->IN_ep[epnum].type == EP_TYPE_ISOC) &&
            ((RegVal & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA))
        {
          hpcd->IN_ep[epnum].is_iso_incomplete = 1U;

          /* Abort current transaction and disable the EP */
          (void)HAL_PCD_EP_Abort(hpcd, (uint8_t)(epnum | 0x80U));
        }
      }

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR);
    }

    /* Handle Incomplete ISO OUT Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
    {
      for (epnum = 1U; epnum < hpcd->Init.dev_endpoints; epnum++)
      {
        RegVal = USBx_OUTEP(epnum)->DOEPCTL;

        if ((hpcd->OUT_ep[epnum].type == EP_TYPE_ISOC) &&
            ((RegVal & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA) &&
            ((RegVal & (0x1U << 16)) == (hpcd->FrameNumber & 0x1U)))
        {
          hpcd->OUT_ep[epnum].is_iso_incomplete = 1U;

          USBx->GINTMSK |= USB_OTG_GINTMSK_GONAKEFFM;

          if ((USBx->GINTSTS & USB_OTG_GINTSTS_BOUTNAKEFF) == 0U)
          {
            USBx_DEVICE->DCTL |= USB_OTG_DCTL_SGONAK;
            break;
          }
        }
      }

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
    }

    /* Handle Connection event Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT))
    {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
      hpcd->ConnectCallback(hpcd);
#else
      HAL_PCD_ConnectCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */

      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT);
    }

    /* Handle Disconnection event Interrupt */
    if (__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OTGINT))
    {
      RegVal = hpcd->Instance->GOTGINT;

      if ((RegVal & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET)
      {
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        hpcd->DisconnectCallback(hpcd);
#else
        HAL_PCD_DisconnectCallback(hpcd);
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
      }
      hpcd->Instance->GOTGINT |= RegVal;
    }
  }
}

const UsbDriver usb_driver = {
  .initialize_core = &initialize_core,
  .initialize_gpio_pins = &initialize_gpio_pins,
  .set_device_address = &set_device_address,
  .connect = &connect,
  .flush_rxfifo = &flush_rxfifo,
  .flush_txfifo = &flush_txfifo,
  .configure_in_endpoint = &configure_in_endpoint,
  .read_packet = &read_packet,
  .write_packet = &write_packet,
  .poll = &usbd_gintsts_handler
};
