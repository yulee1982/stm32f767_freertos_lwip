/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 William D. Jones (thor0505@comcast.net),
 * Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de),
 * Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/* metadata:
   manufacturer: STMicroelectronics
*/

#include "stm32f7xx_hal.h"
#include "bsp/board_api.h"
#include "usart.h"
#include "board.h"

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+


//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+
void OTG_FS_IRQHandler(void) {
  tusb_int_handler(0, true);
}

// Despite being call USB2_OTG
// OTG_HS is marked as RHPort1 by TinyUSB to be consistent across stm32 port
void OTG_HS_IRQHandler(void) {
  tusb_int_handler(1, true);
}

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+

void board_init(void)
{
#if CFG_TUSB_OS == OPT_OS_NONE
  // 1ms tick timer
  SysTick_Config(SystemCoreClock / 1000);

#elif CFG_TUSB_OS == OPT_OS_FREERTOS
  // Explicitly disable systick to prevent its ISR runs before scheduler start
  SysTick->CTRL &= ~1U;

  // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
  NVIC_SetPriority(OTG_FS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
  //NVIC_SetPriority(OTG_HS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
#endif

#ifdef UART_DEV
  HAL_UART_Init(&UartHandle);
#endif
  MX_USART2_UART_Init();

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

  //------------- rhport0: OTG_FS -------------//
  //Configures USB pins (in GPIOA) to work in alternate function mode.
  //Sets alternate function 10 for : PA11 (-) otg_fs_dm , and PA12 (+) otg_fs_dp.
  GPIOA->MODER &= ~((0x3 << 24) | (0x3 << 22));
  GPIOA->MODER |= ((0x2 << 24) | (0x2 << 22)); //复用功能模式
  GPIOA->AFR[1] &= 0xfff00fff;
  GPIOA->AFR[1] |= 0x000aa000;  //AFR12[3:0] = AFR11[3:0] = af10(0xa)
  GPIOA->OTYPER &= ~((0x1 << 12) | (0x1 << 11)); // 复位状态 推挽输出
  GPIOA->OSPEEDR |= ((0x3 << 24) | (0x3 << 22)); // IO速度 高速
  GPIOA->PUPDR &= ~((0x3 << 24) | (0x3 << 22)); // 无上下拉

  /* Configure OTG-FS ID pin */
  //GPIO_InitStruct.Pin = GPIO_PIN_10;
  //GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  //GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* This for ID line debug  PA10 (id) otg_fs_id*/
  GPIOA->MODER &= 0xffcfffff;
  GPIOA->MODER |= 0x00200000;  //复用功能
  GPIOA->AFR[1] &= 0xfffff0ff;
  GPIOA->AFR[1] |= 0x00000a00;  //AFR10[3:0] = af10(0xa)
  GPIOA->OTYPER |= (0x1 << 10); // 开漏输出类型
  GPIOA->OSPEEDR |= (0x3 << 20); // IO速度 高速
  GPIOA->PUPDR &= ~(0x3 << 20);
  GPIOA->PUPDR |= ~(0x1 << 20); //上拉

// Suppress warning caused by mcu driver
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif

  /* Enable USB FS Clocks */
  //__HAL_RCC_USB_OTG_FS_CLK_ENABLE();
  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#if OTG_FS_VBUS_SENSE
  /* Configure VBUS Pin */
  //GPIO_InitStruct.Pin = GPIO_PIN_9;
  //GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIOA->MODER &= 0xfff3ffff;  //输入模式

  // Enable VBUS sense (B device) via pin PA9
  USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBDEN;
#else
  // Disable VBUS sense (B device) via pin PA9
  USB_OTG_FS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

  // B-peripheral session valid override enable
  USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
  USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;
#endif // vbus sense


}

//--------------------------------------------------------------------+
// Board porting API
//--------------------------------------------------------------------+

void board_led_write(bool state)
{
#ifdef PINID_LED
  if(state != 0)
  {
    GPIOB->BSRR = (0x1UL << 14);
  }else{
    GPIOB->BSRR = ((0x1UL << 14) << 16);
  }
#else
  (void) state;
#endif
}

uint32_t board_button_read(void)
{
#ifdef PINID_BUTTON
  uint32_t ret;
  ret = GPIOC->IDR & (0x1UL << 13U);
  ret = (ret >> 13);
  return ret;
#else
  return 0;
#endif
}

size_t board_get_unique_id(uint8_t id[], size_t max_len)
{
  (void) max_len;
  volatile uint32_t * stm32_uuid = (volatile uint32_t *) UID_BASE;
  uint32_t* id32 = (uint32_t*) (uintptr_t) id;
  uint8_t const len = 12;

  id32[0] = stm32_uuid[0];
  id32[1] = stm32_uuid[1];
  id32[2] = stm32_uuid[2];

  return len;
}

int board_uart_read(uint8_t *buf, int len)
{
  (void) buf;
  (void) len;
  return 0;
}

int board_uart_write(void const *buf, int len) {
#ifdef UART_DEV
  HAL_UART_Transmit(&UartHandle, (uint8_t *) (uintptr_t) buf, len, 0xffff);
  return len;
#else
  //(void) buf; (void) len;
  //return -1;
  Usart_Transmit(USART2, (const uint8_t *) (uintptr_t) buf, len, 0xffff);
  return -1;
  return len;
#endif
}

#if CFG_TUSB_OS == OPT_OS_NONE
volatile uint32_t system_ticks = 0;

void SysTick_Handler(void) {
  HAL_IncTick();
  system_ticks++;
}

uint32_t board_millis(void) {
  return system_ticks;
}

#endif

/*
void HardFault_Handler(void) {
  __asm("BKPT #0\n");
}
*/
// Required by __libc_init_array in startup code if we are compiling using
// -nostdlib/-nostartfiles.
//void _init(void)
//{
//}
