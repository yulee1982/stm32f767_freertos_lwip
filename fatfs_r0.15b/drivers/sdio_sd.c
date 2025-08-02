/**
 ******************************************************************************
 * @file    stm32f767_sdio_sd.c
 * @author  MCD Application Team
 * @version V1.0.3
 * @date    13-November-2013
 * @brief   This file provides a set of functions needed to manage the SDIO SD
 *          Card memory mounted on STM32F767ZI NUCLEO-F767ZI evaluation board.
 *
 *
 *  @verbatim
 *
 *          ===================================================================
 *                                   How to use this driver
 *          ===================================================================
 *          It implements a high level communication layer for read and write
 *          from/to this memory. The needed STM32 hardware resources (SDIO and
 *          GPIO) are defined in stm324x9i_eval.h file, and the initialization is
 *          performed in SD_LowLevel_Init() function declared in stm324x9i_eval.c
 *          file.
 *          You can easily tailor this driver to any other development board,
 *          by just adapting the defines for hardware resources and
 *          SD_LowLevel_Init() function.
 *
 *          A - SD Card Initialization and configuration
 *          ============================================
 *            - To initialize the SD Card, use the SD_Init() function.  It
 *              Initializes the SD Card and put it into StandBy State (Ready
 *              for data transfer). This function provides the following operations:
 *
 *              1 - Apply the SD Card initialization process at 400KHz and check
 *                  the SD Card type (Standard Capacity or High Capacity). You
 *                  can change or adapt this frequency by adjusting the
 *                  "SDIO_INIT_CLK_DIV" define inside the stm324x9i_eval.h file.
 *                  The SD Card frequency (SDIO_CK) is computed as follows:
 *
 *                     +---------------------------------------------+
 *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
 *                     +---------------------------------------------+
 *
 *                  In initialization mode and according to the SD Card standard,
 *                  make sure that the SDIO_CK frequency doesn't exceed 400KHz.
 *
 *              2 - Get the SD CID and CSD data. All these information are
 *                  managed by the SDCardInfo structure. This structure provides
 *                  also ready computed SD Card capacity and Block size.
 *
 *              3 - Configure the SD Card Data transfer frequency. By Default,
 *                  the card transfer frequency is set to 24MHz. You can change
 *                  or adapt this frequency by adjusting the "SDIO_TRANSFER_CLK_DIV"
 *                  define inside the stm324x9i_eval.h file.
 *                  The SD Card frequency (SDIO_CK) is computed as follows:
 *
 *                     +---------------------------------------------+
 *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
 *                     +---------------------------------------------+
 *
 *                  In transfer mode and according to the SD Card standard,
 *                  make sure that the SDIO_CK frequency doesn't exceed 25MHz
 *                  and 50MHz in High-speed mode switch.
 *                  To be able to use a frequency higher than 24MHz, you should
 *                  use the SDIO peripheral in bypass mode. Refer to the
 *                  corresponding reference manual for more details.
 *
 *              4 -  Select the corresponding SD Card according to the address
 *                   read with the step 2.
 *
 *              5 -  Configure the SD Card in wide bus mode: 4-bits data.
 *
 *          B - SD Card Read operation
 *          ==========================
 *           - You can read SD card by using two functions : SD_ReadBlock() and
 *             SD_ReadMultiBlocks() functions. These functions support only
 *             512-byte block length.
 *           - The SD_ReadBlock() function reads only one block (512-byte). This
 *             function can transfer the data using DMA controller or using
 *             polling mode. To select between DMA or polling mode refer to
 *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm324x9i_eval_sdio_sd.h
 *             file and uncomment the corresponding line. By default the SD DMA
 *             mode is selected
 *           - The SD_ReadMultiBlocks() function reads only mutli blocks (multiple
 *             of 512-byte).
 *           - Any read operation should be followed by two functions to check
 *             if the DMA Controller and SD Card status.
 *              - SD_ReadWaitOperation(): this function insure that the DMA
 *                controller has finished all data transfer.
 *              - SD_GetStatus(): to check that the SD Card has finished the
 *                data transfer and it is ready for data.
 *
 *           - The DMA transfer is finished by the SDIO Data End interrupt.
 *             User has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler()
 *             and SD_ProcessDMAIRQ() function inside the DMA2_Streamx_IRQHandler().
 *             Don't forget to enable the SDIO_IRQn and DMA2_Stream3_IRQn or
 *             DMA2_Stream6_IRQn interrupts using the NVIC controller.
 *
 *          C - SD Card Write operation
 *          ===========================
 *           - You can write SD card by using two function: SD_WriteBlock() and
 *             SD_WriteMultiBlocks() functions. These functions support only
 *             512-byte block length.
 *           - The SD_WriteBlock() function write only one block (512-byte). This
 *             function can transfer the data using DMA controller or using
 *             polling mode. To select between DMA or polling mode refer to
 *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm324x9i_eval_sdio_sd.h
 *             file and uncomment the corresponding line. By default the SD DMA
 *             mode is selected
 *           - The SD_WriteMultiBlocks() function write only mutli blocks (multiple
 *             of 512-byte).
 *           - Any write operation should be followed by two functions to check
 *             if the DMA Controller and SD Card status.
 *              - SD_ReadWaitOperation(): this function insure that the DMA
 *                controller has finished all data transfer.
 *              - SD_GetStatus(): to check that the SD Card has finished the
 *                data transfer and it is ready for data.
 *
 *           - The DMA transfer is finished by the SDIO Data End interrupt.
 *             User has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler()
 *             and SD_ProcessDMAIRQ() function inside the DMA2_Streamx_IRQHandler().
 *             Don't forget to enable the SDIO_IRQn and DMA2_Stream3_IRQn or
 *             DMA2_Stream6_IRQn interrupts using the NVIC controller.
 *
 *
 *          D - SD card status
 *          ==================
 *           - At any time, you can check the SD Card status and get the SD card
 *             state by using the SD_GetStatus() function. This function checks
 *             first if the SD card is still connected and then get the internal
 *             SD Card transfer state.
 *           - You can also get the SD card SD Status register by using the
 *             SD_SendSDStatus() function.
 *
 *          E - Programming Model (Selecting DMA for SDIO data Transfer)
 *          ============================================================
 *             Status = SD_Init(); // Initialization Step as described in section A
 *
 *             // SDIO Interrupt ENABLE
 *             NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
 *             NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 *             NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 *             NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 *             NVIC_Init(&NVIC_InitStructure);
 *             // DMA2 STREAMx Interrupt ENABLE
 *             NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
 *             NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 *             NVIC_Init(&NVIC_InitStructure);
 *
 *             // Write operation as described in Section C
 *             Status = SD_WriteBlock(buffer, address, 512);
 *             Status = SD_WaitWriteOperation();
 *             while(SD_GetStatus() != SD_TRANSFER_OK);
 *
 *             Status = SD_WriteMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
 *             Status = SD_WaitWriteOperation();
 *             while(SD_GetStatus() != SD_TRANSFER_OK);
 *
 *             // Read operation as described in Section B
 *             Status = SD_ReadBlock(buffer, address, 512);
 *             Status = SD_WaitReadOperation();
 *             while(SD_GetStatus() != SD_TRANSFER_OK);
 *
 *             Status = SD_ReadMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
 *             Status = SD_WaitReadOperation();
 *             while(SD_GetStatus() != SD_TRANSFER_OK);
 *
 *             - Add the SDIO and DMA2 StreamX (3 or 6) IRQ Handlers:
 *                 void SDIO_IRQHandler(void)
 *                 {
 *                   SD_ProcessIRQ();
 *                 }
 *                 void SD_SDIO_DMA_IRQHANDLER(void)
 *                 {
 *                   SD_ProcessDMAIRQ();
 *                 }
 *
 *          F - Programming Model (Selecting Polling for SDIO data Transfer)
 *          ================================================================
 *            //Only SD Card Single Block operation are managed.
 *            Status = SD_Init(); // Initialization Step as described in section
 *
 *            // Write operation as described in Section C
 *            Status = SD_WriteBlock(buffer, address, 512);
 *
 *            // Read operation as described in Section B
 *            Status = SD_ReadBlock(buffer, address, 512);
 *
 *          STM32 SDIO Pin assignment
 *          =========================
 *          +-----------------------------------------------------------+
 *          |                     Pin assignment                        |
 *          +-----------------------------+---------------+-------------+
 *          |  STM32 SDIO Pins            |     SD        |    Pin      |
 *          +-----------------------------+---------------+-------------+
 *          |      SDIO D2                |   D2          |    1        |
 *          |      SDIO D3                |   D3          |    2        |
 *          |      SDIO CMD               |   CMD         |    3        |
 *          |                             |   VCC         |    4 (3.3 V)|
 *          |      SDIO CLK               |   CLK         |    5        |
 *          |                             |   GND         |    6 (0 V)  |
 *          |      SDIO D0                |   D0          |    7        |
 *          |      SDIO D1                |   D1          |    8        |
 *          +-----------------------------+---------------+-------------+
 *
 *  @endverbatim
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "sdio_sd.h"
#include "main.h"
#include <string.h>
#include <stdint.h>
#include "tm_stm32f7_fatfs.h"
/*
#include "tm_stm32f4_usart.h"
#define logf(x)	TM_USART_Puts(USART1, x); TM_USART_Puts(USART1, "\n");
*/
//#define logf(x)

#if SDIO_USE_HAL_LIB

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;
HAL_SD_CardInfoTypedef sdcardinfo;
static volatile DSTATUS TM_FATFS_SD_SDIO_Stat = STA_NOINIT;	/* Physical drive status */

static void MX_SDMMC1_DMA(void);
static void SD_LowLevel_Init(void);

DSTATUS TM_FATFS_SD_SDIO_disk_initialize(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Detect pin */
#if FATFS_USE_DETECT_PIN > 0
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  /*!< Configure SD_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
#endif

  /* Write protect pin */
#if FATFS_USE_WRITEPROTECT_PIN > 0
  M_GPIO_Init(FATFS_USE_WRITEPROTECT_PIN_PORT, FATFS_USE_WRITEPROTECT_PIN_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
#endif

  //Check disk initialized
  if(SD_Init() == SD_OK){
    TM_FATFS_SD_SDIO_Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
  }else{
    TM_FATFS_SD_SDIO_Stat |= STA_NOINIT;
  }
  //Check write protected
  //if (!TM_FATFS_SDIO_WriteEnabled()) {
  //  TM_FATFS_SD_SDIO_Stat |= STA_PROTECT;
  //} else {
  //  TM_FATFS_SD_SDIO_Stat &= ~STA_PROTECT;
  //}

  return TM_FATFS_SD_SDIO_Stat;
}

DSTATUS TM_FATFS_SD_SDIO_disk_status(void)
{
  if (TM_FATFS_CheckCardDetectPin() != SD_PRESENT) {
    return STA_NOINIT;
  }

  //if (!TM_FATFS_SDIO_WriteEnabled()) {
  //  TM_FATFS_SD_SDIO_Stat |= STA_PROTECT;
  //} else {
  //  TM_FATFS_SD_SDIO_Stat &= ~STA_PROTECT;
  //}

  if(HAL_SD_GetStatus(&hsd) == SD_TRANSFER_OK)
  {
    TM_FATFS_SD_SDIO_Stat &= ~STA_NOINIT;
  }

  return TM_FATFS_SD_SDIO_Stat;
}

DRESULT TM_FATFS_SD_SDIO_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;

  if((TM_FATFS_SD_SDIO_Stat & STA_NOINIT)){
    return RES_NOTRDY;
  }

  /* Read block(s) in DMA transfer mode */
  if(HAL_SD_ReadBlocks_DMA(&hsd, buff, (uint64_t)((uint64_t)sector*512), 512, count) != SD_OK)
  {
    res = RES_ERROR;
  }
  /* Wait until transfer is complete */
  if(res == RES_OK)
  {
    if(HAL_SD_CheckReadOperation(&hsd, (uint32_t)SDMMC_DATATIMEOUT) != SD_OK)
      res = RES_ERROR;
    else
      res = RES_OK;
  }

  return res;
}

DRESULT TM_FATFS_SD_SDIO_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_OK;

  /* Write block(s) in DMA transfer mode */
  if(HAL_SD_WriteBlocks_DMA(&hsd, buff, (uint64_t)((uint64_t)sector*512), 512, count) != SD_OK)
  {
    res = RES_ERROR;
  }

  /* Wait until transfer is complete */
  if(res == RES_OK)
  {
    if(HAL_SD_CheckWriteOperation(&hsd, (uint32_t)SDMMC_DATATIMEOUT) != SD_OK)
      res = RES_ERROR;
    else
      res = RES_OK;
  }

  return res;
}

DRESULT TM_FATFS_SD_SDIO_disk_ioctl(BYTE cmd, void *buff)
{
  switch(cmd)
  {
    case GET_SECTOR_SIZE :     // Get R/W sector size (WORD)
      *(WORD *) buff = 512;
      break;
    case GET_BLOCK_SIZE :      // Get erase block size in unit of sector (DWORD)
      *(DWORD *) buff = 32;
      break;
    case CTRL_SYNC :
    case CTRL_TRIM :  //CTRL_ERASE_SECTOR :
      break;
  }

  return RES_OK;
}
__weak DWORD get_fattime(void)
{
#if 0
  RTC_Update();    //更新RTC时钟
  return ((DWORD)(xRTC.Year - 1980) << 25) /* 年   */
         | ((DWORD)xRTC.Month << 21)            /* 月   */
         | ((DWORD)xRTC.Day << 16)            /* 日   */
         | ((DWORD)xRTC.Hour << 11)            /* 时   */
         | ((DWORD)xRTC.Minute << 5)         /* 分   */
         | ((DWORD)xRTC.Second >> 1);        /* 秒   */
#endif
  /* 返回当前时间戳 */
  return ((DWORD)(2025 - 1980) << 25)  /* Year 2024 */
         | ((DWORD)8 << 21)         /* Month 5 */
         | ((DWORD)1 << 16)        /* Mday 30 */
         | ((DWORD)17 << 11)        /* Hour 0 */
         | ((DWORD)20 << 5)         /* Min 0 */
         | ((DWORD)0 >> 1);         /* Sec 0 */
}

HAL_SD_ErrorTypedef SD_Init(void)
{
  HAL_SD_ErrorTypedef errorstate = SD_OK;

  /* Check if the SD card is plugged in the slot */
  if (TM_FATFS_CheckCardDetectPin() != SD_PRESENT)
  {
    printf("SD card is not plugged in the slot !\r\n");
    return SD_ERROR;
  }

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDMMC1;
  hsd.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = SDMMC_TRANSFER_CLK_DIV;
  //hsd.Init.ClockDiv = SDMMC_INIT_CLK_DIV;

  /* USER CODE BEGIN SDIO_Init 2 */
  errorstate = HAL_SD_Init(&hsd, &sdcardinfo);
  if(errorstate != SD_OK)	//For initialization need SDIO_BUS_WIDE_1B
  {
    Error_Handler();
  }

  /* Configure SD Bus width */
  if(errorstate == SD_OK)
  {
    /* Enable wide operation */
    errorstate = HAL_SD_WideBusOperation_Config(&hsd, SDMMC_BUS_WIDE_1B); //SDMMC_BUS_WIDE_4B
  }

  if(errorstate != SD_OK)	//For initialization need SDIO_BUS_WIDE_1B
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */
  return errorstate;
}

void HAL_SD_MspDeInit(SD_HandleTypeDef *sdHandle)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(sdHandle->Instance != SDMMC1)
    return;

  /*!< DeInitializes the SDIO peripheral */
  //LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_SDMMC1);
  //LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_SDMMC1);

  /* Disable the SDIO APB2 Clock */
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SDMMC1);
  //__HAL_RCC_SDIO_CLK_DISABLE();//RCC->APB2ENR &= ~(RCC_APB2ENR_SDMMC1EN);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* SDIO DMA DeInit */
  HAL_DMA_DeInit(sdHandle->hdmarx);
  HAL_DMA_DeInit(sdHandle->hdmatx);
}

void HAL_SD_MspInit(SD_HandleTypeDef *sdHandle)
{
  if(sdHandle->Instance != SDMMC1)
    return;

  SD_LowLevel_Init();
  MX_SDMMC1_DMA();
}
static void SD_LowLevel_Init(void)
{
  //uint32_t tempreg;
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**SDIO GPIO Configuration
  PC8    ------> SDMMC_D0
  PC9    ------> SDMMC_D1/I2S_B_CKIN
  PC10   ------> SDMMC_D2
  PC11   ------> SDMMC_D3
  PC12   ------> SDMMC_CK
  PD2    ------> SDMMC_CMD
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Enable the SDIO APB2 Clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SDMMC1);

  /* Peripheral interrupt init */
  //Configure SDIO Global Interrupt
  NVIC_SetPriority(SDMMC1_IRQn, 5);
  NVIC_EnableIRQ(SDMMC1_IRQn);
}
static void MX_SDMMC1_DMA(void)
{
  /* DMA2 used for SDMMC1 Transmission and Reception */
  /* (1) Enable the clock of DMA2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* Configure DMA Rx parameters */
  hdma_sdio_rx.Instance = DMA2_Stream3;
  hdma_sdio_rx.Init.Channel             = DMA_CHANNEL_4;
  hdma_sdio_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_sdio_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_sdio_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_sdio_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_sdio_rx.Init.Mode                = DMA_PFCTRL;
  hdma_sdio_rx.Init.Priority            = DMA_PRIORITY_LOW; //DMA_PRIORITY_HIGH
  hdma_sdio_rx.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_sdio_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_sdio_rx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_sdio_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

  /* Associate the DMA handle */
  __HAL_LINKDMA(&hsd, hdmarx, hdma_sdio_rx);

  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&hdma_sdio_rx);

  /* Configure the DMA stream */
  HAL_DMA_Init(&hdma_sdio_rx);

  /* Configure DMA Tx parameters */
  hdma_sdio_tx.Instance = DMA2_Stream6;
  hdma_sdio_tx.Init.Channel             = DMA_CHANNEL_4;
  hdma_sdio_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_sdio_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_sdio_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_sdio_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_sdio_tx.Init.Mode                = DMA_PFCTRL;
  hdma_sdio_tx.Init.Priority            = DMA_PRIORITY_LOW; //DMA_PRIORITY_HIGH
  hdma_sdio_tx.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_sdio_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_sdio_tx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_sdio_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

  /* Associate the DMA handle */
  __HAL_LINKDMA(&hsd, hdmatx, hdma_sdio_tx);

  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&hdma_sdio_tx);

  /* Configure the DMA stream */
  HAL_DMA_Init(&hdma_sdio_tx);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  //NVIC_SetPriority(DMA2_Stream3_IRQn, 6); //rx
  //NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  //NVIC_SetPriority(DMA2_Stream6_IRQn, 6); //tx
  //NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#if 0
  /* (3) Configure the DMA functional DMA Rx parameters for transmission */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_3,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |  /*设备 to memory bit:6  */
						LL_DMA_PRIORITY_HIGH              |  /*优先级   */
						LL_DMA_MODE_PFCTRL                |  /*外设流量控制模式   */
                        LL_DMA_PERIPH_NOINCREMENT         |  /*外设地址指针固定，不增量 bit:9  */
						LL_DMA_MEMORY_INCREMENT           |  /*memory地址指针增量   */
						LL_DMA_PDATAALIGN_WORD            |  /*外设数据大小32位，bit:12-11   */
						LL_DMA_PDATAALIGN_WORD);             /*memory数据大小32位，bit:14-13   */
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_3,
                         (uint32_t) &(SDMMC1->FIFO),
						 (uint32_t)pRxBuffPtr,
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_3));
  //LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_3, ubNbDataToTransmit);

  /* (4) Configure the DMA functional parameters for reception */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_6,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
						LL_DMA_PRIORITY_HIGH              |
						LL_DMA_MODE_PFCTRL                |
                        LL_DMA_PERIPH_NOINCREMENT         |
						LL_DMA_MEMORY_INCREMENT           |
						LL_DMA_PDATAALIGN_WORD            |
						LL_DMA_PDATAALIGN_WORD);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_6,
		                 (uint32_t)pTxBuffPtr,
						 (uint32_t) &(SDMMC1->FIFO),
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_6));
  //LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_6, ubNbDataToReceive);

  /* (5) Enable DMA transfer complete/error interrupts  */
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_6);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_6);
#endif
}


/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @param  None
 * @retval Returns if SD is detected or not
 */
uint8_t TM_FATFS_CheckCardDetectPin(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  __IO uint8_t status = SD_PRESENT;
  uint32_t tempreg;

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  /*!< Configure SD_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  //GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  HAL_Delay(1);
  tempreg = GPIOG->IDR;
  if(tempreg & 0x0004)
	  status = SD_NOT_PRESENT;
  return status;
}

#else
static uint32_t CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;
static uint8_t SDSTATUS_Tab[16];
__IO uint32_t StopCondition = 0;
__IO SD_Error TransferError = SDMMC_OK;
__IO uint32_t TransferEnd = 0, DMAEndOfTransfer = 0;
SD_CardInfo SDCardInfo;

//SDMMC_InitTypeDef SDIO_InitStructure;
//SDMMC_CmdInitTypeDef SDIO_CmdInitStructure;
//SDMMC_DataInitTypeDef SDIO_DataInitStructure;

/** @defgroup STM32F4_DISCOVERY_SDIO_SD_Private_Function_Prototypes
  * @{
  */
static SD_Error CmdError(void);
static SD_Error CmdResp1Error(uint8_t cmd);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp3Error(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca);
static SD_Error SDEnWideBus(FunctionalState NewState);
static SD_Error IsCardProgramming(uint8_t *pstatus);
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr);
uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes);

static uint32_t SDMMC1_Command(uint32_t cmd, uint32_t resp, uint32_t arg);

static volatile DSTATUS TM_FATFS_SD_SDIO_Stat = STA_NOINIT;	/* Physical drive status */

#define BLOCK_SIZE            512

uint8_t TM_FATFS_SDIO_WriteEnabled(void)
{
#if FATFS_USE_WRITEPROTECT_PIN > 0
  return !TM_GPIO_GetInputPinValue(FATFS_USE_WRITEPROTECT_PIN_PORT, FATFS_USE_WRITEPROTECT_PIN_PIN);
#else
  return 1;
#endif
}
uint8_t TM_FATFS_CheckCardDetectPin(void)
{
  uint8_t status = 1;
  uint32_t tempreg;

#if FATFS_USE_DETECT_PIN > 0
  //if (TM_GPIO_GetInputPinValue(FATFS_USE_DETECT_PIN_PORT, FATFS_USE_DETECT_PIN_PIN) != 0) {
  //  status = 0;
  //}
  tempreg = GPIOG->IDR;
  if ((tempreg & 0x0004) != 0)
    status = 0;
#endif

	/* Return status */
	return status;
}
DSTATUS TM_FATFS_SD_SDIO_disk_initialize(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Detect pin */
#if FATFS_USE_DETECT_PIN > 0
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  /*!< Configure SD_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
#endif

  /* Write protect pin */
#if FATFS_USE_WRITEPROTECT_PIN > 0
  M_GPIO_Init(FATFS_USE_WRITEPROTECT_PIN_PORT, FATFS_USE_WRITEPROTECT_PIN_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
#endif

  SD_LowLevel_DeInit();
  SD_LowLevel_Init();

  //Check disk initialized
  if (SD_Init() == SDMMC_OK) {
    TM_FATFS_SD_SDIO_Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
  }else{
    TM_FATFS_SD_SDIO_Stat |= STA_NOINIT;
  }
  //Check write protected
  if (!TM_FATFS_SDIO_WriteEnabled()) {
    TM_FATFS_SD_SDIO_Stat |= STA_PROTECT;
  } else {
    TM_FATFS_SD_SDIO_Stat &= ~STA_PROTECT;
  }

  return TM_FATFS_SD_SDIO_Stat;
}

DSTATUS TM_FATFS_SD_SDIO_disk_status(void)
{
  if (SD_Detect() != SD_PRESENT) {
    return STA_NOINIT;
  }

  if (!TM_FATFS_SDIO_WriteEnabled()) {
    TM_FATFS_SD_SDIO_Stat |= STA_PROTECT;
  } else {
    TM_FATFS_SD_SDIO_Stat &= ~STA_PROTECT;
  }

  return TM_FATFS_SD_SDIO_Stat;
}

DRESULT TM_FATFS_SD_SDIO_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  SD_Error Status = SDMMC_OK;

  if((TM_FATFS_SD_SDIO_Stat & STA_NOINIT)){
    return RES_NOTRDY;
  }

  if((DWORD)buff & 3) {
    DRESULT res = RES_OK;
    DWORD scratch[BLOCK_SIZE / 4];

    while(count--)
    {
      res = TM_FATFS_SD_SDIO_disk_read((void *)scratch, sector++, 1);
      if(res != RES_OK)
        break;

      memcpy(buff, scratch, BLOCK_SIZE);
      buff += BLOCK_SIZE;
    }

    return res;
  }

  Status = SD_ReadMultiBlocks(buff, sector << 9, BLOCK_SIZE, count);

  if(Status == SDMMC_OK)
  {
    SDTransferState State;

    Status = SD_WaitReadOperation();

    while((State = SD_GetStatus()) == SDMMC_TRANSFER_BUSY);

    if((State == SDMMC_TRANSFER_ERROR) || (Status != SDMMC_OK)) {
      return RES_ERROR;
    }else{
      return RES_OK;
    }
  }else{
    return RES_ERROR;
  }
}

DRESULT TM_FATFS_SD_SDIO_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  SD_Error Status = SDMMC_OK;

  if(!TM_FATFS_SDIO_WriteEnabled()){
    return RES_WRPRT;
  }

  if(SD_Detect() != SD_PRESENT)
  {
    return RES_NOTRDY;
  }

  if((DWORD)buff & 3)
  {
    DRESULT res = RES_OK;
    DWORD scratch[BLOCK_SIZE / 4];

    while(count--)
    {
      memcpy(scratch, buff, BLOCK_SIZE);
      res = TM_FATFS_SD_SDIO_disk_write((void *)scratch, sector++, 1);

      if(res != RES_OK){
        break;
      }

      buff += BLOCK_SIZE;
    }

    return(res);
  }

  Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9, BLOCK_SIZE, count); // 4GB Compliant

  if(Status == SDMMC_OK)
  {
    SDTransferState State;

    Status = SD_WaitWriteOperation(); // Check if the Transfer is finished

    while((State = SD_GetStatus()) == SDMMC_TRANSFER_BUSY); // BUSY, OK (DONE), ERROR (FAIL)

    if((State == SDMMC_TRANSFER_ERROR) || (Status != SDMMC_OK))
      return RES_ERROR;
    else
      return RES_OK;
  }else{
    return RES_ERROR;
  }
}

DRESULT TM_FATFS_SD_SDIO_disk_ioctl(BYTE cmd, void *buff)
{
  switch(cmd)
  {
    case GET_SECTOR_SIZE :     // Get R/W sector size (WORD)
      *(WORD *) buff = 512;
      break;
    case GET_BLOCK_SIZE :      // Get erase block size in unit of sector (DWORD)
      *(DWORD *) buff = 32;
      break;
    case CTRL_SYNC :
    case CTRL_TRIM :  //CTRL_ERASE_SECTOR :
      break;
  }

  return RES_OK;
}
__weak DWORD get_fattime(void)
{
#if 0
  RTC_Update();    //更新RTC时钟
  return ((DWORD)(xRTC.Year - 1980) << 25) /* 年   */
         | ((DWORD)xRTC.Month << 21)            /* 月   */
         | ((DWORD)xRTC.Day << 16)            /* 日   */
         | ((DWORD)xRTC.Hour << 11)            /* 时   */
         | ((DWORD)xRTC.Minute << 5)         /* 分   */
         | ((DWORD)xRTC.Second >> 1);        /* 秒   */
#endif
  /* 返回当前时间戳 */
  return ((DWORD)(2025 - 1980) << 25)  /* Year 2024 */
         | ((DWORD)8 << 21)         /* Month 5 */
         | ((DWORD)1 << 16)        /* Mday 30 */
         | ((DWORD)17 << 11)        /* Hour 0 */
         | ((DWORD)20 << 5)         /* Min 0 */
         | ((DWORD)0 >> 1);         /* Sec 0 */
}

//SDIO IRQHandler
void SDMMC1_IRQHandler(void)
{
  SD_ProcessIRQSrc();
}

//SDIO_DMA IRQHandler
#ifdef SD_SDIO_DMA_STREAM3
void DMA2_Stream3_IRQHandler(void)
{
  SD_ProcessDMAIRQ();
}
#endif

#ifdef SD_SDIO_DMA_STREAM6
void DMA2_Stream6_IRQHandler(void)
{
  SD_ProcessDMAIRQ();
}
#endif
/**
  * @brief  DeInitializes the SDIO interface.
  * @param  None
  * @retval None
  */
void SD_DeInit(void)
{
  SD_LowLevel_DeInit();
}

/**
 * @brief  Gets the cuurent sd card data transfer status.
 * @param  None
 * @retval SDTransferState: Data Transfer state.
 *   This value can be:
 *        - SD_TRANSFER_OK: No data transfer is acting
 *        - SD_TRANSFER_BUSY: Data transfer is acting
 */
SDTransferState SD_GetStatus (void)
{
  SDCardState cardstate = SDMMC_CARD_TRANSFER;

  cardstate = SD_GetState();

  if (cardstate == SDMMC_CARD_TRANSFER)
    return (SDMMC_TRANSFER_OK);
  else if (cardstate == SDMMC_CARD_ERROR)
    return (SDMMC_TRANSFER_ERROR);
  else
    return (SDMMC_TRANSFER_BUSY);
}

/**
 * @brief  Returns the current card's state.
 * @param  None
 * @retval SDCardState: SD Card Error or SD Card Current State.
 */
SDCardState SD_GetState(void)
{
  uint32_t resp1 = 0;

  if(SD_Detect() == SD_PRESENT ) {
    if(SD_SendStatus(&resp1) != SDMMC_OK)
      return SDMMC_CARD_ERROR;
    else
      return (SDCardState) ((resp1 >> 9) & 0x0F);
  }

  return SDMMC_CARD_ERROR;
}

/**
 * @brief  Returns the current SD card's status.
 * @param  psdstatus: pointer to the buffer that will contain the SD card status
 *         (SD Status register).
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_SendSDStatus (uint32_t *psdstatus)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t count = 0;
  uint32_t tmpreg;

  if(SDMMC_GetResponse(SDMMC1, SDMMC_RESP1) & SDMMC_CARD_LOCKED )
  {
    errorstatus = SDMMC_LOCK_UNLOCK_FAILED;
    return (errorstatus);
  }

  /*!< Set block size for card if it is not equal to current block size for card. */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, 64);
  errorstatus = CmdResp1Error (SDMMC_CMD_SET_BLOCKLEN);
  if (errorstatus != SDMMC_OK)
  {
    return (errorstatus);
  }

  /*!< CMD55 */
  SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t) RCA << 16));
  errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
  if (errorstatus != SDMMC_OK)
  {
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SDMMC_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = 64;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = 64;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (SDMMC_DATABLOCK_SIZE_64B) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //0: sdio to sdcard 1: sdcard to sdio Data transfer direction selection. from sdcard to mcu
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  /*!< Send ACMD13 SD_APP_STAUS  with argument as card's RCA.*/
  SDMMC1_Command(SDMMC_CMD_SD_APP_STATUS, SDMMC_RESPONSE_SHORT, 0);
  errorstatus = CmdResp1Error(SDMMC_CMD_SD_APP_STATUS);
  if (errorstatus != SDMMC_OK)
  {
	return (errorstatus);
  }

  tmpreg = SDMMC1->STA;
  while (!(tmpreg & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND))) // | SDMMC_STA_STBITERR
  {
    if(tmpreg & SDMMC_STA_RXFIFOHF)//(SDIO_GetFlagStatus (SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for(count = 0; count < 8; count++)
      {
        *(psdstatus + count) = SDMMC1->FIFO;
      }
      psdstatus += 8;
    }
    tmpreg = SDMMC1->STA;
  }

  tmpreg = SDMMC1->STA;
  if(tmpreg & SDMMC_STA_DTIMEOUT){
    SDMMC1->ICR = (SDMMC_ICR_DTIMEOUTC);
    errorstatus = SDMMC_DATA_TIMEOUT;
    return (errorstatus);
  }
  else if(tmpreg & SDMMC_STA_DCRCFAIL){
    SDMMC1->ICR = (SDMMC_ICR_DCRCFAILC);
    errorstatus = SDMMC_DATA_CRC_FAIL;
    return (errorstatus);
  }
  else if(tmpreg & SDMMC_STA_RXOVERR){
    SDMMC1->ICR = (SDMMC_ICR_RXOVERRC);
    errorstatus = SDMMC_RX_OVERRUN;
    return (errorstatus);
  }
  //else if(SDIO_GetFlagStatus (SDIO_FLAG_STBITERR) != RESET) {
  //  SDIO->ICR =  (SDIO_FLAG_STBITERR);
  //  errorstatus = SD_START_BIT_ERR;
  //  return (errorstatus);
  //}

  count = SDMMC_DATATIMEOUT;
  tmpreg = SDMMC1->STA;
  while((tmpreg & SDMMC_STA_RXDAVL) && (count > 0))
  {
    *psdstatus = SDMMC1->FIFO;
    psdstatus++;
    count--;
    tmpreg = SDMMC1->STA;
  }

  /*!< Clear all the static status flags*/
  SDMMC1->ICR = (SDMMC_STATIC_FLAGS);

  return (errorstatus);
}

/**
 * @brief  Detect if SD card is correctly plugged in the memory slot.
 * @param  None
 * @retval Return if SD is detected or not
 */
uint8_t SD_Detect(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* Check status */
  if (!TM_FATFS_CheckCardDetectPin())
    status = SD_NOT_PRESENT;

  /* Return status */
  return status;
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for data
  *         transfer).
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_Init(void)
{
  __IO SD_Error errorstatus = SDMMC_OK;
  uint32_t tempreg;

  /* SDIO Peripheral Low Level DeInit */
  SD_DeInit();
  /* SDIO Peripheral Low Level Init */
  SD_LowLevel_Init();

  errorstatus = SD_PowerON();

  if (errorstatus != SDMMC_OK)
  {
    /*!< CMD Response TimeOut (wait for CMDSENT flag) */
	//printf("SD_PowerON failed\r\n");
    return(errorstatus);
  }

  //printf("SD_PowerON OK\r\n");

  errorstatus = SD_InitializeCards();

  if (errorstatus != SDMMC_OK)
  {
    /*!< CMD Response TimeOut (wait for CMDSENT flag) */
    //printf("SD_InitializeCards failed\r\n");
    return(errorstatus);
  }

  //printf("SD_InitializeCards OK\r\n");

  /*----------------- Read CSD/CID MSD registers ------------------*/
  errorstatus = SD_GetCardInfo(&SDCardInfo);
  if (errorstatus == SDMMC_OK)
  {
    /*-------------- Select Card ----------A dead loop occurs in this step validation. */
    //printf("SD_GetCardInfo OK\r\n");
    errorstatus = SD_SelectDeselect((uint32_t) (((uint32_t)SDCardInfo.RCA) << 16));
  }else{
    printf("SD_SelectDeselect failed\r\n");
  }

  /*!< Configure the SDIO peripheral */
  /*!< SDIO_CK = SDIOCLK / (SDIO_TRANSFER_CLK_DIV + 2) */
  /*!< on STM32F4xx devices, SDIOCLK is fixed to 48MHz */
  tempreg = 0;
  tempreg |= (0x0UL<<14) & SDMMC_CLKCR_HWFC_EN; //SDMMC_HardwareFlowControl_Disable
  tempreg |= (0x0UL<<13) & SDMMC_CLKCR_NEGEDGE; //在 SDMMC_CK 上升沿后的 SDMMCCLK 下降沿更改命令和数据
  tempreg |= (0x0UL<<11) & SDMMC_CLKCR_WIDBUS;  //BusWide_4b err
  tempreg |= (0x0UL<<10) & SDMMC_CLKCR_BYPASS;  //SDIO_ClockBypass_Disable
  tempreg |= (0x0UL<<9) & SDMMC_CLKCR_PWRSAV;   //SDIO_ClockPowerSave_Disable 非节能模式始终使能 SDMMC_CK 时钟
  tempreg |= (0x1UL<<8) & SDMMC_CLKCR_CLKEN_Msk;  //SDMMC_CK
  tempreg |= (SDMMC_TRANSFER_CLK_DIV<<0) & SDMMC_CLKCR_CLKDIV;
  SDMMC1->CLKCR = tempreg;

  //The above code runs OK, and the following steps are not verified.
  if (errorstatus == SDMMC_OK)
  {
    //SD_HandleTypeDef hsd;
    //hsd.Instance = SDMMC1;
    //hsd.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
    //hsd.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
    //hsd.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    //hsd.Init.BusWide = SDMMC_BUS_WIDE_1B;
    //hsd.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    //hsd.Init.ClockDiv = SDMMC_TRANSFER_CLK_DIV;
    //hsd.CardType = CardType;
    //errorstatus = HAL_SD_WideBusOperation_Config(&hsd, SDMMC_BUS_WIDE_4B);
    //printf("SD_SelectDeselect OK\r\n");
#if FATFS_SDIO_4BIT == 1
    errorstatus = SD_EnableWideBusOperation(SDMMC_BUS_WIDE_4B); //4 bit data width
#else
    errorstatus = SD_EnableWideBusOperation(SDMMC_BUS_WIDE_1B);
#endif

  }else{
    //printf("SD_EnableWideBusOperation failed\r\n");
  }

  if (errorstatus == SDMMC_OK)
  {
    //printf("SD_EnableWideBusOperation OK\r\n");
  }

  return(errorstatus);
}



void SD_LowLevel_DeInit(void)
{
  uint32_t tempreg;
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*!< Disable SDIO Clock */
  tempreg = SDMMC1->CLKCR;
  tempreg &= ~SDMMC_CLKCR_CLKEN; //(uint32_t)1<<8;  //Clock is disable
  SDMMC1->CLKCR = tempreg;

  /*!< Set Power State to OFF */
  SDMMC1->POWER = 0;

  /*!< DeInitializes the SDIO peripheral */
  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_SDMMC1);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_SDMMC1);

  /* Disable the SDIO APB2 Clock */
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SDMMC1);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SD_LowLevel_Init(void)
{
  //uint32_t tempreg;
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**SDIO GPIO Configuration
  PC8    ------> SDMMC_D0
  PC9    ------> SDMMC_D1/I2S_B_CKIN
  PC10   ------> SDMMC_D2
  PC11   ------> SDMMC_D3
  PC12   ------> SDMMC_CK
  PD2    ------> SDMMC_CMD
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Enable the SDIO APB2 Clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SDMMC1);

  /* Peripheral interrupt init */
  //Configure SDIO Global Interrupt
  NVIC_SetPriority(SDMMC1_IRQn, 5);
  NVIC_EnableIRQ(SDMMC1_IRQn);

  /* (1) Enable the clock of DMA2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
#ifdef SD_SDIO_DMA_STREAM3
  NVIC_SetPriority(DMA2_Stream3_IRQn, 6); //0
  NVIC_EnableIRQ(DMA2_Stream3_IRQn);
#elif defined SD_SDIO_DMA_STREAM6
  NVIC_SetPriority(DMA2_Stream6_IRQn, 6); //0
  NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
#ifdef SD_SDIO_DMA_STREAM3
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);
#elif defined SD_SDIO_DMA_STREAM6
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_6);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_6);
#endif
}

/**
  * @brief  Enquires cards about their operating voltage and configures
  *   clock controls.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_PowerON(void)
{
  __IO SD_Error errorstatus = SDMMC_OK;
  uint32_t response = 0, count = 0, validvoltage = 0;
  uint32_t SDType = SDMMC_STD_CAPACITY;
  uint32_t tempreg;

  /*!< Power ON Sequence -----------------------------------------------------*/
  /*!< Configure the SDIO peripheral */
  /*!< SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) */
  /*!< on STM32F4xx devices, SDIOCLK is fixed to 48MHz */
  /*!< SDIO_CK for initialization should not exceed 400 KHz */
  //Set the DMA CR
  tempreg = 0;
  tempreg |= (0x0UL<<14) & SDMMC_CLKCR_HWFC_EN; //SDMMC_HardwareFlowControl_Disable
  tempreg |= (0x0UL<<13) & SDMMC_CLKCR_NEGEDGE; //在 SDMMC_CK 上升沿后的 SDMMCCLK 下降沿更改命令和数据
  tempreg |= (0x0UL<<11) & SDMMC_CLKCR_WIDBUS;  //BusWide_1b
  tempreg |= (0x0UL<<10) & SDMMC_CLKCR_BYPASS;  //SDIO_ClockBypass_Disable
  tempreg |= (0x0UL<<9) & SDMMC_CLKCR_PWRSAV;   //SDIO_ClockPowerSave_Disable 非节能模式始终使能 SDMMC_CK 时钟
  tempreg |= (0x0UL<<8) & SDMMC_CLKCR_CLKEN_Msk;  //SDMMC_CK
  tempreg |= (SDIO_INIT_CLK_DIV<<0) & SDMMC_CLKCR_CLKDIV;  //Clock Divider. Clock=48000/(118+2)=400Khz
  //Keep the rest at 0 => HW_Flow Disabled, Rising Clock Edge, Disable CLK ByPass, Bus Width=0, Power save Disable
  SDMMC1->CLKCR = tempreg;

  /*!< Set Power State to ON */
  SDMMC1->POWER = 0x3UL;

  /* 1ms: required power up waiting time before starting the SD initialization sequence */
  HAL_Delay(1);

  /*!< Enable SDIO Clock */
  SDMMC1->CLKCR |= (0x1UL<<8) & SDMMC_CLKCR_CLKEN_Msk;  //SDMMC_CK en

  /*!< CMD0: GO_IDLE_STATE ---------------------------------------------------*/
  /*!< No CMD response required */
  SDMMC1_Command(SDMMC_CMD_GO_IDLE_STATE, SDMMC_RESPONSE_NO, 0);
  errorstatus = CmdError();
  if (errorstatus != SDMMC_OK)
  {
    /*!< CMD Response TimeOut (wait for CMDSENT flag) */
    return(errorstatus);
  }

  /*!< CMD8: SEND_IF_COND ----------------------------------------------------*/
  /*!< Send CMD8 to verify SD card interface operating condition */
  /*!< Argument: - [31:12]: Reserved (shall be set to '0')
                 - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
                 - [7:0]: Check Pattern (recommended 0xAA) */
  /*!< CMD Response: R7 */
  SDMMC1_Command(SDMMC_CMD_HS_SEND_EXT_CSD, SDMMC_RESPONSE_SHORT, SDMMC_CHECK_PATTERN);
  errorstatus = CmdResp7Error();
  if (errorstatus == SDMMC_OK)
  {
    /* SD Card 2.0 */
    CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;
    SDType = SDMMC_HIGH_CAPACITY;
  }

  /*!< CMD55 */
  SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, 0x00);
  errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);

  /*!< If errorstatus is Command TimeOut, it is a MMC card */
  /*!< If errorstatus is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch)
     or SD card 1.x */
  if (errorstatus == SDMMC_OK)
  {
    /*!< SD CARD */
    /*!< Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
    while ((!validvoltage) && (count < SDMMC_MAX_VOLT_TRIAL))
    {
      /*!< SEND CMD55 APP_CMD with RCA as 0 */
      SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, 0x00);
      errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      //ACMD41
      SDMMC1_Command(SDMMC_CMD_SD_APP_OP_COND, SDMMC_RESPONSE_SHORT, (SDMMC_VOLTAGE_WINDOW_SD | SDType));
      errorstatus = CmdResp3Error();
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      response = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);
      validvoltage = (((response >> 31) == 1) ? 1 : 0);
      count++;
    }

    if (count >= SDMMC_MAX_VOLT_TRIAL)
    {
      errorstatus = SDMMC_INVALID_VOLTRANGE;
      return(errorstatus);
    }

    if((response & SDMMC_HIGH_CAPACITY) == SDMMC_HIGH_CAPACITY) //if (response &= SD_HIGH_CAPACITY)
    {
      CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }

  }/*!< else MMC Card */

  return(errorstatus);
}


static uint32_t SDMMC1_Command(uint32_t cmd, uint32_t resp, uint32_t arg)
{
  //Response must be:
  //0,2:No response (expect cmdsent) ->NORESP
  //1:Short Response  (expect cmdrend and ccrcfail) ->SHRESP
  //3:Long Response   (expect cmdrend and ccrcfail) ->LNRESP

  //uint32_t status;
  //SD_Error errorstatus = SDMMC_OK;

  //Clear the Command Flags
  //SDMMC1->ICR = (SDMMC_ICR_CCRCFAILC | SDMMC_ICR_CTIMEOUTC | SDMMC_ICR_CMDRENDC | SDMMC_ICR_CMDSENTC);

  SDMMC1->ARG = arg;  //First adjust the argument (because I will immediately enable CPSM next)
  SDMMC1->CMD = (uint32_t)(cmd & SDMMC_CMD_CMDINDEX) | (SDMMC_WAIT_NO & SDMMC_CMD_WAITRESP) | (resp & SDMMC_CMD_WAITRESP) | (0x0400);  //The last argument is to enable CSPM SDMMC_CPSM_ENABLE

  return 0;
}
/**
  * @brief  Checks for error conditions for CMD0.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t timeout;
  uint32_t status;

  timeout = ((uint32_t)0x00010000); //SDIO_CMD0TIMEOUT; /*!< 10000 */

  status = SDMMC1->STA;
  while ((timeout > 0) && ((status & SDMMC_STA_CMDSENT) == RESET))
  {
    status = SDMMC1->STA;
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = SDIO_STATIC_FLAGS;

  return(errorstatus);
}
/**
  * @brief  Checks for error conditions for R7 response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t status;
  uint32_t timeout = ((uint32_t)0x00010000); //SDIO_CMD0TIMEOUT;

  status = SDMMC1->STA;
  while (!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT)) && (timeout > 0))
  {
    status = SDMMC1->STA;
    timeout--;
  }

  status = SDMMC1->STA;
  if ((timeout == 0) || (status & SDMMC_STA_CTIMEOUT))
  {
    /*!< Card is not V2.0 complient or card does not support the set voltage range */
    errorstatus = SDMMC_ERROR_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = SDMMC_ICR_CTIMEOUTC;
    return(errorstatus);
  }

  if (status & SDMMC_STA_CMDREND)
  {
    /*!< Card is SD V2.0 compliant */
    errorstatus = SDMMC_OK;
    SDMMC1->ICR = SDMMC_ICR_CMDRENDC;
    return(errorstatus);
  }
  return(errorstatus);
}
/**
  * @brief  Checks for error conditions for R1 response.
  * @param  cmd: The sent command index.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdResp1Error(uint8_t cmd)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t status;
  uint32_t response_r1;

  status = SDMMC1->STA;

  while (!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT)))
  {
    status = SDMMC1->STA;
  }

  if (status & SDMMC_STA_CTIMEOUT)
  {
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = SDMMC_ICR_CTIMEOUTC;
    return(errorstatus);
  }
  else if (status & SDMMC_STA_CCRCFAIL)
  {
    errorstatus = SDMMC_CMD_CRC_FAIL;
    SDMMC1->ICR = SDMMC_ICR_CCRCFAILC;
    return(errorstatus);
  }

  /*!< Check response received is of desired command */
  if ((SDMMC1->RESPCMD) != cmd)
  {
    errorstatus = SDMMC_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = SDIO_STATIC_FLAGS;

  /*!< We have received response, retrieve it for analysis  */
  response_r1 = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);  //stm32f7xx_ll_sdmmc.h 446 SDMMC_RESP1 = 0x00000000U

  if ((response_r1 & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO)
  {
    return(errorstatus);
  }

  if (response_r1 & SDMMC_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SDMMC_ADDR_OUT_OF_RANGE);
  }

  if (response_r1 & SDMMC_OCR_ADDR_MISALIGNED)
  {
    return(SDMMC_ADDR_MISALIGNED);
  }

  if (response_r1 & SDMMC_OCR_BLOCK_LEN_ERR)
  {
    return(SDMMC_BLOCK_LEN_ERR);
  }

  if (response_r1 & SDMMC_OCR_ERASE_SEQ_ERR)
  {
    return(SDMMC_ERASE_SEQ_ERR);
  }

  if (response_r1 & SDMMC_OCR_BAD_ERASE_PARAM)
  {
    return(SDMMC_BAD_ERASE_PARAM);
  }

  if (response_r1 & SDMMC_OCR_WRITE_PROT_VIOLATION)
  {
    return(SDMMC_WRITE_PROT_VIOLATION);
  }

  if (response_r1 & SDMMC_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SDMMC_LOCK_UNLOCK_FAILED);
  }

  if (response_r1 & SDMMC_OCR_COM_CRC_FAILED)
  {
    return(SDMMC_COM_CRC_FAILED);
  }

  if (response_r1 & SDMMC_OCR_ILLEGAL_CMD)
  {
    return(SDMMC_ILLEGAL_CMD);
  }

  if (response_r1 & SDMMC_OCR_CARD_ECC_FAILED)
  {
    return(SDMMC_CARD_ECC_FAILED);
  }

  if (response_r1 & SDMMC_OCR_CC_ERROR)
  {
    return(SDMMC_CC_ERROR);
  }

  if (response_r1 & SDMMC_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SDMMC_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SDMMC_OCR_STREAM_READ_UNDERRUN)
  {
    return(SDMMC_STREAM_READ_UNDERRUN);
  }

  if (response_r1 & SDMMC_OCR_STREAM_WRITE_OVERRUN) //SD_OCR_STREAM_WRITE_OVERRUN
  {
    return(SDMMC_STREAM_WRITE_OVERRUN);
  }

  if (response_r1 & SDMMC_OCR_CID_CSD_OVERWRITE)//SD_OCR_CID_CSD_OVERWRIETE
  {
    return(SDMMC_CID_CSD_OVERWRITE);
  }

  if (response_r1 & SDMMC_OCR_WP_ERASE_SKIP)
  {
    return(SDMMC_WP_ERASE_SKIP);
  }

  if (response_r1 & SDMMC_OCR_CARD_ECC_DISABLED)
  {
    return(SDMMC_CARD_ECC_DISABLED);
  }

  if (response_r1 & SDMMC_OCR_ERASE_RESET)
  {
    return(SDMMC_ERASE_RESET);
  }

  if (response_r1 & SDMMC_OCR_AKE_SEQ_ERROR)
  {
    return(SDMMC_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}
/**
  * @brief  Checks for error conditions for R3 (OCR) response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdResp3Error(void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t status;

  status = SDMMC1->STA;

  while (!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT)))
  {
    status = SDMMC1->STA;
  }

  if (status & SDMMC_STA_CTIMEOUT)
  {
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = SDMMC_ICR_CTIMEOUTC;
    return(errorstatus);
  }
  /*!< Clear all the static flags */
  SDMMC1->ICR = SDIO_STATIC_FLAGS;
  return(errorstatus);
}
/**
  * @brief  Checks for error conditions for R2 (CID or CSD) response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t status;

  status = SDMMC1->STA;

  while (!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CTIMEOUT | SDMMC_STA_CMDREND)))
  {
    status = SDMMC1->STA;
  }

  if (status & SDMMC_STA_CTIMEOUT)
  {
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = SDMMC_ICR_CTIMEOUTC;
    return(errorstatus);
  }
  else if (status & SDMMC_STA_CCRCFAIL)
  {
    errorstatus = SDMMC_CMD_CRC_FAIL;
    SDMMC1->ICR = SDMMC_ICR_CCRCFAILC;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = SDIO_STATIC_FLAGS;

  return(errorstatus);
}
/**
  * @brief  Checks for error conditions for R6 (RCA) response.
  * @param  cmd: The sent command index.
  * @param  prca: pointer to the variable that will contain the SD card relative
  *         address RCA.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t status;
  uint32_t response_r1;

  status = SDMMC1->STA;

  while (!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CTIMEOUT | SDMMC_STA_CMDREND)))
  {
    status = SDMMC1->STA;
  }

  if (status & SDMMC_STA_CTIMEOUT)
  {
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = SDMMC_ICR_CTIMEOUTC;
    return(errorstatus);
  }
  else if (status & SDMMC_STA_CCRCFAIL)
  {
    errorstatus = SDMMC_CMD_CRC_FAIL;
    SDMMC1->ICR = SDMMC_ICR_CCRCFAILC;
    return(errorstatus);
  }
  else
  {
    /* No error flag set */
  }

  /*!< Check response received is of desired command */
  if (SDMMC_GetCommandResponse(SDMMC1) != cmd)
  {
    errorstatus = SDMMC_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = SDMMC_STATIC_FLAGS;

  /*!< We have received response, retrieve it.  */
  response_r1 = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);

  if (SDMMC_ALLZERO == (response_r1 & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_FAILED)))
  {
    *prca = (uint16_t) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SDMMC_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SDMMC_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SDMMC_R6_ILLEGAL_CMD)
  {
    return(SDMMC_ILLEGAL_CMD);
  }

  if (response_r1 & SDMMC_R6_COM_CRC_FAILED)
  {
    return(SDMMC_COM_CRC_FAILED);
  }

  return(errorstatus);
}

/**
 * @brief  Turns the SDIO output signals off.
 * @param  None
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_PowerOFF(void)
{
  SD_Error errorstatus = SDMMC_OK;

  /* Set power state to OFF */
  SDMMC1->POWER = (uint32_t)0x00000000;

  return (errorstatus);
}

/**
  * @brief  Intialises all cards or single card as the case may be Card(s) come
  *         into standby state.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_InitializeCards(void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint16_t rca = 0x01;

  if (SDMMC_GetPowerState(SDMMC1) == 0) //SDIO_PowerState_OFF
  {
    errorstatus = SDMMC_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    /*!< Send CMD2 ALL_SEND_CID */
    SDMMC1_Command(SDMMC_CMD_ALL_SEND_CID, SDMMC_RESPONSE_LONG, 0x00);
    errorstatus = CmdResp2Error();
    if (SDMMC_OK != errorstatus)
    {
      return(errorstatus);
    }

    /* Get Card identification number data */
    CID_Tab[0] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);
    CID_Tab[1] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP2);
    CID_Tab[2] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP3);
    CID_Tab[3] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP4);
  }

  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType)
      || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    /*!< Send CMD3 SET_REL_ADDR with argument 0 */
    /*!< SD Card publishes its RCA. */
    SDMMC1_Command(SDMMC_CMD_SET_REL_ADDR, SDMMC_RESPONSE_SHORT, 0x00);
    errorstatus = CmdResp6Error(SDMMC_CMD_SET_REL_ADDR, &rca);
    if (SDMMC_OK != errorstatus)
    {
      return(errorstatus);
    }
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    RCA = rca;

    /*!< Send CMD9 SEND_CSD with argument as card's RCA */
    SDMMC1_Command(SDMMC_CMD_SEND_CSD, SDMMC_RESPONSE_LONG, (uint32_t)(rca << 16));
    errorstatus = CmdResp2Error();
    if (SDMMC_OK != errorstatus)
    {
      return(errorstatus);
    }

    /* Get Card Specific Data */
    CSD_Tab[0] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);
    CSD_Tab[1] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP2);
    CSD_Tab[2] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP3);
    CSD_Tab[3] = SDMMC_GetResponse(SDMMC1, SDMMC_RESP4);
  }

  errorstatus = SD_OK; /*!< All cards get intialized */

  return(errorstatus);
}

/**
 * @brief  Switch mode High-Speed
 * @note   This function must be used after "Transfer State"
 * @note   This operation should be followed by the configuration
 *         of PLL to have SDIOCK clock between 67 and 75 MHz
 * @param  None
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_HighSpeed (void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t scr[2] = {0, 0};
  uint32_t SD_SPEC = 0;
  uint8_t hs[64] = { 0 };
  uint32_t count = 0, *tempbuff = (uint32_t *) hs;
  uint32_t tmpreg;

  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDMMC1->DCTRL = 0x0;

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);

  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /* Test the Version supported by the card*/
  SD_SPEC = (scr[1] & 0x01000000) || (scr[1] & 0x02000000);

  if(SD_SPEC != SDMMC_ALLZERO)
  {
    /* Set Block Size for Card */
    SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) 64));
    errorstatus = CmdResp1Error (SDMMC_CMD_SET_BLOCKLEN);
    if(errorstatus != SDMMC_OK){
      return (errorstatus);
    }

    //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    //SDIO_DataInitStructure.SDIO_DataLength = 64;
    //SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
    //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    //SDIO_DataConfig (&SDIO_DataInitStructure);
    //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);//SDIO_DataConfig (&SDIO_DataInitStructure);
    /* Set the SDMMC Data TimeOut value */
    SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
    /* Set the SDMMC DataLength value */
    SDMMC1->DLEN = 64;
    /* Get the SDIO DCTRL value */
    tmpreg = SDMMC1->DCTRL;
    /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
    tmpreg &= ((uint32_t)0xFFFFFF08);
    /* Set the SDMMC data configuration parameters */
    /* Set DEN bit according to SDIO_DPSM value */
    /* Set DTMODE bit according to SDIO_TransferMode value */
    /* Set DTDIR bit according to SDIO_TransferDir value */
    /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
    tmpreg |= (SDMMC_DATABLOCK_SIZE_64B) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
    tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
    tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //0:sdio to sdcard 1:sdcard to sdio Data transfer direction selection.
    tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
    /* Write to SDMMC DCTRL */
    SDMMC1->DCTRL = tmpreg;

    /*!< Send CMD6 switch mode */
    SDMMC1_Command(SDMMC_CMD_HS_SWITCH, SDMMC_RESPONSE_SHORT, 0x80FFFF01);
    errorstatus = CmdResp1Error(SDMMC_CMD_HS_SWITCH);
    if(errorstatus != SDMMC_OK){
      return (errorstatus);
    }

    tmpreg = SDMMC1->STA;
    while(!(tmpreg & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND))) // | SDMMC_STA_STBITERR
    {
      if(tmpreg & SDMMC_STA_RXFIFOHF)
      {
        for(count = 0; count < 8; count++)
          *(tempbuff + count) = SDMMC1->FIFO;

        tempbuff += 8;
      }
      tmpreg = SDMMC1->STA;
    }

    tmpreg = SDMMC1->STA;
    if(tmpreg & SDMMC_STA_DTIMEOUT){
      SDMMC1->ICR = (SDMMC_ICR_DTIMEOUTC);
      errorstatus = SDMMC_DATA_TIMEOUT;
      return (errorstatus);
    }
    else if(tmpreg & SDMMC_STA_DCRCFAIL){
      SDMMC1->ICR = (SDMMC_FLAG_DCRCFAIL);
      errorstatus = SDMMC_DATA_CRC_FAIL;
      return (errorstatus);
    }
    else if(tmpreg & SDMMC_STA_RXOVERR){
      SDMMC1->ICR = (SDMMC_ICR_RXOVERRC);
      errorstatus = SDMMC_RX_OVERRUN;
      return (errorstatus);
    }
    //else if(tmpreg & SDMMC_STA_STBITERR){
    //  SDMMC1->ICR = (SDMMC_ICR_STBITERRC);
    //  errorstatus = SDMMC_START_BIT_ERR;
    //  return (errorstatus);
    //}

    count = SDMMC_DATATIMEOUT;
    tmpreg = SDMMC1->STA;
    while ((tmpreg & SDMMC_STA_RXDAVL) && (count > 0))
    {
      *tempbuff = SDMMC1->FIFO;
      tempbuff++;
      count--;
      tmpreg = SDMMC1->STA;
    }

    /*!< Clear all the static flags */
    SDMMC1->ICR = (SDMMC_STATIC_FLAGS);

    /* Test if the switch mode HS is ok */
    if((hs[13] & 0x2) == 0x2)
      errorstatus = SDMMC_OK;
    else
      errorstatus = SDMMC_UNSUPPORTED_FEATURE;
  }
  return (errorstatus);
}
/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
  *         information.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SDMMC_OK;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);

    cardinfo->CardCapacity = (uint64_t)(cardinfo->SD_csd.DeviceSize + 1) * (uint64_t)(512 * 1024);
    cardinfo->CardBlockSize = 512;
  }

  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}

/**
  * @brief  Selects od Deselects the corresponding card.
  * @param  addr: Address of the Card to be selected.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_SelectDeselect(uint64_t addr)
{
  SD_Error errorstatus = SDMMC_OK;

  /*!< Send CMD7 SDIO_SEL_DESEL_CARD */
  SDMMC1_Command(SDMMC_CMD_SEL_DESEL_CARD, SDMMC_RESPONSE_SHORT, addr);
  errorstatus = CmdResp1Error(SDMMC_CMD_SEL_DESEL_CARD);

  return(errorstatus);
}

/**
 * @brief  Enables wide bus opeartion for the requeseted card if supported by
 *         card.
 * @param  WideMode: Specifies the SD card wide bus mode.
 *   This parameter can be one of the following values:
 *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
 *     @arg SDIO_BusWide_4b: 4-bit data transfer
 *     @arg SDIO_BusWide_1b: 1-bit data transfer
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_GetCardStatus (SD_CardStatus *cardstatus)
{
  SD_Error errorstatus = SDMMC_OK;
  uint8_t tmp = 0;

  errorstatus = SD_SendSDStatus((uint32_t *) SDSTATUS_Tab);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /*!< Byte 0 */
  tmp = (uint8_t) ((SDSTATUS_Tab[0] & 0xC0) >> 6);
  cardstatus->DAT_BUS_WIDTH = tmp;

  /*!< Byte 0 */
  tmp = (uint8_t) ((SDSTATUS_Tab[0] & 0x20) >> 5);
  cardstatus->SECURED_MODE = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t) ((SDSTATUS_Tab[2] & 0xFF));
  cardstatus->SD_CARD_TYPE = tmp << 8;

  /*!< Byte 3 */
  tmp = (uint8_t) ((SDSTATUS_Tab[3] & 0xFF));
  cardstatus->SD_CARD_TYPE |= tmp;

  /*!< Byte 4 */
  tmp = (uint8_t) (SDSTATUS_Tab[4] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;

  /*!< Byte 5 */
  tmp = (uint8_t) (SDSTATUS_Tab[5] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;

  /*!< Byte 6 */
  tmp = (uint8_t) (SDSTATUS_Tab[6] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;

  /*!< Byte 7 */
  tmp = (uint8_t) (SDSTATUS_Tab[7] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;

  /*!< Byte 8 */
  tmp = (uint8_t) ((SDSTATUS_Tab[8] & 0xFF));
  cardstatus->SPEED_CLASS = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t) ((SDSTATUS_Tab[9] & 0xFF));
  cardstatus->PERFORMANCE_MOVE = tmp;

  /*!< Byte 10 */
  tmp = (uint8_t) ((SDSTATUS_Tab[10] & 0xF0) >> 4);
  cardstatus->AU_SIZE = tmp;

  /*!< Byte 11 */
  tmp = (uint8_t) (SDSTATUS_Tab[11] & 0xFF);
  cardstatus->ERASE_SIZE = tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t) (SDSTATUS_Tab[12] & 0xFF);
  cardstatus->ERASE_SIZE |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t) ((SDSTATUS_Tab[13] & 0xFC) >> 2);
  cardstatus->ERASE_TIMEOUT = tmp;

  /*!< Byte 13 */
  tmp = (uint8_t) ((SDSTATUS_Tab[13] & 0x3));
  cardstatus->ERASE_OFFSET = tmp;

  return (errorstatus);
}

/**
  * @brief  Enables wide bus opeartion for the requeseted card if supported by
  *         card.
  * @param  WideMode: Specifies the SD card wide bus mode.
  *   This parameter can be one of the following values:
  *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
  *     @arg SDIO_BusWide_4b: 4-bit data transfer
  *     @arg SDIO_BusWide_1b: 1-bit data transfer
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_EnableWideBusOperation(uint32_t WideMode)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tempreg;

  /*!< MMC Card doesn't support this feature */
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SDMMC_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
  else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    if (SDMMC_BUS_WIDE_8B == WideMode) //SDMMC_BUS_WIDE_8B  SDIO_BusWide_8b
    {
      errorstatus = SDMMC_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
    else if (SDMMC_BUS_WIDE_4B == WideMode)
    {
      errorstatus = SDEnWideBus(ENABLE);
    }
    else if (WideMode == SDMMC_BUS_WIDE_1B)
    {
      errorstatus = SDEnWideBus(DISABLE);//SD_WideBus_Disable(hsd);
    }
    else
    {
      errorstatus = SDMMC_INVALID_PARAMETER;
    }

    if (errorstatus == SDMMC_OK)
    {
      /* Configure the SDMMC peripheral */
      tempreg = 0;
      tempreg |= (0x0UL<<14) & SDMMC_CLKCR_HWFC_EN; //SDMMC_HardwareFlowControl_Disable
      tempreg |= (0x0UL<<13) & SDMMC_CLKCR_NEGEDGE; //在 SDMMC_CK 上升沿后的 SDMMCCLK 下降沿更改命令和数据
      tempreg |= (0x0UL<<11) & SDMMC_CLKCR_WIDBUS;  //SDMMC_BUS_WIDE_1B
      tempreg |= (0x0UL<<10) & SDMMC_CLKCR_BYPASS;  //SDIO_ClockBypass_Disable
      tempreg |= (0x0UL<<9) & SDMMC_CLKCR_PWRSAV;   //SDIO_ClockPowerSave_Disable 非节能模式始终使能 SDMMC_CK 时钟
      tempreg |= (0x1UL<<8) & SDMMC_CLKCR_CLKEN_Msk;  //SDMMC_CK
      tempreg |= (SDMMC_TRANSFER_CLK_DIV<<0) & SDMMC_CLKCR_CLKDIV;  // 48MHz / (SDMMC_TRANSFER_CLK_DIV + 2) < 25MHz  define in stm32f7xx_ll_sdmmc.h 656
      SDMMC1->CLKCR = tempreg;
    }

  }

  return(errorstatus);
}
/**
  * @brief  Enables or disables the SDIO wide bus mode.
  * @param  NewState: new state of the SDIO wide bus mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error SDEnWideBus(FunctionalState NewState)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t scr[2] = {0, 0};

  if((SDMMC_GetResponse(SDMMC1, SDMMC_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
  {
    errorstatus = SDMMC_LOCK_UNLOCK_FAILED;
    return errorstatus;
  }

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);
  if (errorstatus != SDMMC_OK)
  {
    return(errorstatus);
  }

  /*!< If wide bus operation to be enabled */
  if (NewState == ENABLE)
  {
    /*!< If requested card supports wide bus operation */
    if ((scr[1] & SDMMC_WIDE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t) RCA << 16));
      errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
      SDMMC1_Command(SDMMC_CMD_APP_SD_SET_BUSWIDTH, SDMMC_RESPONSE_SHORT, 0x2);
      errorstatus = CmdResp1Error(SDMMC_CMD_APP_SD_SET_BUSWIDTH);
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      return(errorstatus);
    }
    else
    {
      errorstatus = SDMMC_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }   /*!< If wide bus operation to be disabled */
  else
  {
    /*!< If requested card supports 1 bit mode operation */
    if ((scr[1] & SDMMC_SINGLE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t) RCA << 16));
      errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
      SDMMC1_Command(SDMMC_CMD_APP_SD_SET_BUSWIDTH, SDMMC_RESPONSE_SHORT, 0x0);
      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
      if (errorstatus != SDMMC_OK)
      {
        return(errorstatus);
      }

      return(errorstatus);
    }
    else
    {
      errorstatus = SDMMC_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}

/**
 * @brief  Checks if the SD card is in programming state.
 * @param  pstatus: pointer to the variable that will contain the SD card state.
 * @retval SD_Error: SD Card Error code.
 */
static SD_Error IsCardProgramming (uint8_t *pstatus)
{
  SD_Error errorstatus = SDMMC_OK;
  __IO uint32_t respR1 = 0, status = 0;

  SDMMC1_Command(SDMMC_CMD_SEND_STATUS, SDMMC_RESPONSE_SHORT, ((uint32_t) RCA << 16));
  status = SDMMC1->STA;
  while(!(status & (SDMMC_STA_CCRCFAIL | SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT)))
  {
    status = SDMMC1->STA;
  }

  if(status & SDMMC_STA_CTIMEOUT){
    errorstatus = SDMMC_CMD_RSP_TIMEOUT;
    SDMMC1->ICR = (SDMMC_ICR_CTIMEOUTC);
    return (errorstatus);
  }else if(status & SDMMC_STA_CCRCFAIL){
    errorstatus = SDMMC_CMD_CRC_FAIL;
    SDMMC1->ICR = (SDMMC_ICR_CCRCFAILC);
    return (errorstatus);
  }

  status = (uint32_t) SDMMC_GetCommandResponse(SDMMC1);

  /*!< Check response received is of desired command */
  if(status != SDMMC_CMD_SEND_STATUS){
    errorstatus = SD_ILLEGAL_CMD;
    return (errorstatus);
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = (SDMMC_STATIC_FLAGS);

  /*!< We have received response, retrieve it for analysis  */
  respR1 = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);

  /*!< Find out card status */
  *pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

  if((respR1 & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO){
    return (errorstatus);
  }

  if(respR1 & SDMMC_OCR_ADDR_OUT_OF_RANGE){
    return (SDMMC_ADDR_OUT_OF_RANGE);
  }

  if(respR1 & SDMMC_OCR_ADDR_MISALIGNED){
    return (SDMMC_ADDR_MISALIGNED);
  }

  if(respR1 & SDMMC_OCR_BLOCK_LEN_ERR){
    return (SDMMC_BLOCK_LEN_ERR);
  }

  if(respR1 & SDMMC_OCR_ERASE_SEQ_ERR){
    return (SDMMC_ERASE_SEQ_ERR);
  }

  if(respR1 & SDMMC_OCR_BAD_ERASE_PARAM){
    return (SDMMC_BAD_ERASE_PARAM);
  }

  if(respR1 & SDMMC_OCR_WRITE_PROT_VIOLATION){
    return (SDMMC_WRITE_PROT_VIOLATION);
  }

  if(respR1 & SDMMC_OCR_LOCK_UNLOCK_FAILED){
    return (SDMMC_LOCK_UNLOCK_FAILED);
  }

  if(respR1 & SDMMC_OCR_COM_CRC_FAILED){
    return (SDMMC_COM_CRC_FAILED);
  }

  if(respR1 & SDMMC_OCR_ILLEGAL_CMD){
    return (SDMMC_ILLEGAL_CMD);
  }

  if(respR1 & SDMMC_OCR_CARD_ECC_FAILED){
    return (SDMMC_CARD_ECC_FAILED);
  }

  if(respR1 & SDMMC_OCR_CC_ERROR){
    return (SDMMC_CC_ERROR);
  }

  if(respR1 & SDMMC_OCR_GENERAL_UNKNOWN_ERROR){
    return (SDMMC_GENERAL_UNKNOWN_ERROR);
  }

  if(respR1 & SDMMC_OCR_STREAM_READ_UNDERRUN){
    return (SDMMC_STREAM_READ_UNDERRUN);
  }

  if(respR1 & SDMMC_OCR_STREAM_WRITE_OVERRUN){
    return (SDMMC_STREAM_WRITE_OVERRUN);
  }

  if(respR1 & SDMMC_OCR_CID_CSD_OVERWRITE){
    return (SDMMC_CID_CSD_OVERWRITE);
  }

  if(respR1 & SDMMC_OCR_WP_ERASE_SKIP){
    return (SDMMC_WP_ERASE_SKIP);
  }

  if(respR1 & SDMMC_OCR_CARD_ECC_DISABLED){
    return (SDMMC_CARD_ECC_DISABLED);
  }

  if(respR1 & SDMMC_OCR_ERASE_RESET){
    return (SDMMC_ERASE_RESET);
  }

  if(respR1 & SDMMC_OCR_AKE_SEQ_ERROR){
    return (SDMMC_AKE_SEQ_ERROR);
  }

  return (errorstatus);
}
/**
  * @brief  Find the SD card SCR register value.
  * @param  rca: selected card address.
  * @param  pscr: pointer to the buffer that will contain the SCR value.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr)
{
  uint32_t tmpreg, index = 0;
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tempscr[2] = {0, 0};

  /*!< Set Block Size To 8 Bytes */
  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, (uint32_t)8);
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCKLEN);
  if (errorstatus != SDMMC_OK)
  {
    return(errorstatus);
  }

  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t)(((uint32_t) rca) << 16)));
  errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
  if (errorstatus != SDMMC_OK)
  {
    return(errorstatus);
  }
  //SDIO_DataInitStructure.DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.DataLength = 8;
  //SDIO_DataInitStructure.DataBlockSize = SDIO_DataBlockSize_8b;
  //SDIO_DataInitStructure.TransferDir = SDIO_TransferDir_ToSDIO;
  //SDIO_DataInitStructure.TransferMode = SDIO_TransferMode_Block;SDIO_TRANSFER_MODE_BLOCK=0
  //SDIO_DataInitStructure.DPSM = SDIO_DPSM_Enable;SDIO_DPSM_ENABLE=SDIO_DCTRL_DTEN=1
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = 8;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (SDMMC_DATABLOCK_SIZE_8B) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //Data transfer direction selection. from sdcard to mcu
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  /*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
  SDMMC1_Command(SDMMC_CMD_SD_APP_SEND_SCR, SDMMC_RESPONSE_SHORT, 0x0);
  errorstatus = CmdResp1Error(SDMMC_CMD_SD_APP_SEND_SCR);
  if (errorstatus != SDMMC_OK)
  {
    return(errorstatus);
  }

  tmpreg = SDMMC1->STA;
  while (!(tmpreg & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND))) //| SDMMC_STA_STBITERR
  {
    tmpreg = SDMMC1->STA;
    if ((tmpreg & SDMMC_STA_RXDAVL) != RESET)
    {
      *(tempscr + index) = SDMMC1->FIFO;
      index++;
    }
  }

  tmpreg = SDMMC1->STA;
  if ((tmpreg & SDMMC_STA_DTIMEOUT) != RESET)
  {
    SDMMC1->ICR = SDMMC_ICR_DTIMEOUTC;
    errorstatus = SDMMC_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if ((tmpreg & SDMMC_STA_DCRCFAIL) != RESET)
  {
    SDMMC1->ICR = SDMMC_ICR_DCRCFAILC;
    errorstatus = SDMMC_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if ((tmpreg & SDMMC_STA_RXOVERR) != RESET)
  {
    SDMMC1->ICR = SDMMC_ICR_RXOVERRC;
    errorstatus = SDMMC_RX_OVERRUN;
    return(errorstatus);
  }
  //else if ((tempreg & SDMMC_STA_STBITERR) != RESET)
  //{
  //  SDIO_ClearFlag(SDIO_FLAG_STBITERR);
  //  errorstatus = SD_START_BIT_ERR;
  //  return(errorstatus);
  //}

  /*!< Clear all the static flags */
  SDMMC1->ICR = SDMMC_STATIC_FLAGS;

  *(pscr + 1) = ((tempscr[0] & SDMMC_0TO7BITS) << 24) | ((tempscr[0] & SDMMC_8TO15BITS) << 8) | ((tempscr[0] & SDMMC_16TO23BITS) >> 8) | ((tempscr[0] & SDMMC_24TO31BITS) >> 24);

  *(pscr) = ((tempscr[1] & SDMMC_0TO7BITS) << 24) | ((tempscr[1] & SDMMC_8TO15BITS) << 8) | ((tempscr[1] & SDMMC_16TO23BITS) >> 8) | ((tempscr[1] & SDMMC_24TO31BITS) >> 24);

  return(errorstatus);
}


/**
 * @brief  Returns the current card's status.
 * @param  pcardstatus: pointer to the buffer that will contain the SD card
 *         status (Card Status register).
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_SendStatus (uint32_t *pcardstatus)
{
  SD_Error errorstatus = SDMMC_OK;

  if (pcardstatus == 0) {
    errorstatus = SDMMC_INVALID_PARAMETER;
    return (errorstatus);
  }

  /*!< Send ACMD13 SD_APP_SEND_SCR with argument as RCA<<16 */
  SDMMC1_Command(SDMMC_CMD_SEND_STATUS, SDMMC_RESPONSE_SHORT, ((uint32_t) RCA<<16));
  errorstatus = CmdResp1Error(SDMMC_CMD_SEND_STATUS);
  if (errorstatus != SDMMC_OK)
  {
    return(errorstatus);
  }

  *pcardstatus = SDMMC_GetResponse(SDMMC1, SDMMC_RESP1);

  return (errorstatus);
}

/**
 * @brief  Allows to read one block from a specified address in a card. The Data
 *         transfer can be managed by DMA mode or Polling mode.
 * @note   This operation should be followed by two functions to check if the
 *         DMA Controller and SD Card status.
 *          - SD_ReadWaitOperation(): this function insure that the DMA
 *            controller has finished all data transfer.
 *          - SD_GetStatus(): to check that the SD Card has finished the
 *            data transfer and it is ready for data.
 * @param  readbuff: pointer to the buffer that will contain the received data
 * @param  ReadAddr: Address from where data are to be read.
 * @param  BlockSize: the SD card Data block size. The Block size should be 512.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;
#if defined (SD_POLLING_MODE)
  uint32_t count = 0, *tempbuff = (uint32_t *)readbuff;
#endif

  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDMMC1->DCTRL = 0x0;

#if defined (SD_DMA_MODE)
  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_RXOVERR); // | SDMMC_IT_STBITERR
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN; //SDIO_DMACmd (ENABLE);
  SD_LowLevel_DMA_RxConfig ((uint32_t *) readbuff, BlockSize);
#endif

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /* Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error (SDMMC_CMD_SET_BLOCKLEN );
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) SDIO_DATABLOCKSIZE;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDIO_DataConfig (&SDIO_DataInitStructure);
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);//SDIO_DataConfig (&SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //Data transfer direction selection. from sdcard to mcu
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  /*!< Send CMD17 READ_SINGLE_BLOCK */
  SDMMC1_Command(SDMMC_CMD_READ_SINGLE_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t) ReadAddr));
  errorstatus = CmdResp1Error(SDMMC_CMD_READ_SINGLE_BLOCK);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

#if defined (SD_POLLING_MODE)
	/*!< In case of single block transfer, no need of stop transfer at all.*/
	/*!< Polling mode */
	while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
		if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET) {
			for (count = 0; count < 8; count++) {
				*(tempbuff + count) = SDIO_ReadData();
			}
			tempbuff += 8;
		}
	}

	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO->ICR = (SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO->ICR = (SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
		SDIO->ICR = (SDIO_FLAG_RXOVERR);
		errorstatus = SD_RX_OVERRUN;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO->ICR = (SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return(errorstatus);
	}
	count = SD_DATATIMEOUT;
	while ((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) && (count > 0)) {
		*tempbuff = SDIO_ReadData();
		tempbuff++;
		count--;
	}

	/*!< Clear all the static flags */
	SDIO->ICR = (SDIO_STATIC_FLAGS);

#endif

  return (errorstatus);
}

/**
 * @brief  Allows to read blocks from a specified address  in a card.  The Data
 *         transfer can be managed by DMA mode or Polling mode.
 * @note   This operation should be followed by two functions to check if the
 *         DMA Controller and SD Card status.
 *          - SD_ReadWaitOperation(): this function insure that the DMA
 *            controller has finished all data transfer.
 *          - SD_GetStatus(): to check that the SD Card has finished the
 *            data transfer and it is ready for data.
 * @param  readbuff: pointer to the buffer that will contain the received data.
 * @param  ReadAddr: Address from where data are to be read.
 * @param  BlockSize: the SD card Data block size. The Block size should be 512.
 * @param  NumberOfBlocks: number of blocks to be read.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_ReadMultiBlocks (uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;

  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDMMC1->DCTRL = 0x0;

#if defined (SD_DMA_MODE)
  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_RXOVERR);// | SDMMC_IT_STBITERR
  SD_LowLevel_DMA_RxConfig ((uint32_t *) readbuff, (NumberOfBlocks * BlockSize));
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN; //SDIO_DMACmd (ENABLE);
#endif

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /*!< Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCKLEN);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);//SDIO_DataConfig (&SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = NumberOfBlocks * BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //Data transfer direction selection. from sdcard to mcu
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
  SDMMC1_Command(SDMMC_CMD_READ_MULT_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t) ReadAddr));
  errorstatus = CmdResp1Error (SDMMC_CMD_READ_MULT_BLOCK);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  return (errorstatus);
}

SD_Error SD_ReadMultiBlocksFIXED (uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;

  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDMMC1->DCTRL = 0x0;

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    BlockSize = 512;
  else
    ReadAddr *= BlockSize; // Convert to Bytes for NON SDHC

  /*!< Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error (SDMMC_CMD_SET_BLOCKLEN);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) SDIO_DATABLOCKSIZE;//(uint32_t) 9 << 4;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDIO_DataConfig(&SDIO_DataInitStructure);
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = NumberOfBlocks * BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x1UL<<1) & SDMMC_DCTRL_DTDIR;  //Data transfer direction selection. from sdcard to mcu
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
  SDMMC1_Command(SDMMC_CMD_READ_MULT_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t)ReadAddr));
  errorstatus = CmdResp1Error(SDMMC_CMD_READ_MULT_BLOCK);
  if(errorstatus != SDMMC_OK){
    return(errorstatus);
  }

  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_RXOVERR); // | SDMMC_IT_STBITERR
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN;//SDIO_DMACmd(ENABLE);
  SD_LowLevel_DMA_RxConfig((uint32_t *)readbuff, (NumberOfBlocks * BlockSize));

  return(errorstatus);
}

/**
 * @brief  This function waits until the SDIO DMA data transfer is finished.
 *         This function should be called after SDIO_ReadMultiBlocks() function
 *         to insure that all data sent by the card are already transferred by
 *         the DMA controller.
 * @param  None.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_WaitReadOperation (void)
{
  SD_Error errorstatus = SDMMC_OK;
  volatile uint32_t timeout;
  uint32_t tempreg;

  timeout = SDMMC_DATATIMEOUT;

  while((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SDMMC_OK) && (timeout > 0))
  {
    timeout--;
  }

  DMAEndOfTransfer = 0x00;

  timeout = SDMMC_DATATIMEOUT;

  tempreg = SDMMC1->STA;
  while((tempreg & SDMMC_STA_RXACT) && (timeout > 0))
  {
    tempreg = SDMMC1->STA;
    timeout--;
  }

  if(StopCondition == 1)
  {
    errorstatus = SD_StopTransfer();
    StopCondition = 0;
  }

  if((timeout == 0) && (errorstatus == SDMMC_OK))
  {
    errorstatus = SDMMC_DATA_TIMEOUT;
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = (SDMMC_STATIC_FLAGS);

  if(TransferError != SDMMC_OK)
  {
    return (TransferError);
  }

  return (errorstatus);
}

/**
 * @brief  Allows to write one block starting from a specified address in a card.
 *         The Data transfer can be managed by DMA mode or Polling mode.
 * @note   This operation should be followed by two functions to check if the
 *         DMA Controller and SD Card status.
 *          - SD_ReadWaitOperation(): this function insure that the DMA
 *            controller has finished all data transfer.
 *          - SD_GetStatus(): to check that the SD Card has finished the
 *            data transfer and it is ready for data.
 * @param  writebuff: pointer to the buffer that contain the data to be transferred.
 * @param  WriteAddr: Address from where data are to be read.
 * @param  BlockSize: the SD card Data block size. The Block size should be 512.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_WriteBlock (uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;

#if defined (SD_POLLING_MODE)
	uint32_t bytestransferred = 0, count = 0, restwords = 0;
	uint32_t *tempbuff = (uint32_t *)writebuff;
#endif

  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDMMC1->DCTRL = 0x0;

#if defined (SD_DMA_MODE)
  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_RXOVERR); // | SDMMC_IT_STBITERR
  SD_LowLevel_DMA_TxConfig ((uint32_t *) writebuff, BlockSize);
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN; //SDIO_DMACmd (ENABLE);
#endif

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /* Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCKLEN);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /*!< Send CMD24 WRITE_SINGLE_BLOCK */
  SDMMC1_Command(SDMMC_CMD_WRITE_SINGLE_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t) WriteAddr));
  errorstatus = CmdResp1Error (SDMMC_CMD_WRITE_SINGLE_BLOCK);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) SDIO_DATABLOCKSIZE;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDIO_DataConfig (&SDIO_DataInitStructure);
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x0UL<<1) & SDMMC_DCTRL_DTDIR;  //0:SDIO to sdcard 1:sdcard to SDIO Data transfer direction selection.
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

	/*!< In case of single data block transfer no need of stop command at all */
#if defined (SD_POLLING_MODE)
  while(!(SDMMC1->STA & (SDMMC_STA_DBCKEND | SDMMC_STA_TXUNDERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT))) // | SDMMC_STA_STBITERR
  {
		if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET) {
			if ((512 - bytestransferred) < 32) {
				restwords = ((512 - bytestransferred) % 4 == 0) ? ((512 - bytestransferred) / 4) : (( 512 - bytestransferred) / 4 + 1);
				for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4) {
					SDIO->FIFO = (*tempbuff);
				}
			} else {
				for (count = 0; count < 8; count++) {
					SDIO->FIFO = (*(tempbuff + count));
				}
				tempbuff += 8;
				bytestransferred += 32;
			}
		}
	}
	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
		SDIO->ICR = (SDIO_FLAG_DTIMEOUT);
		errorstatus = SD_DATA_TIMEOUT;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
		SDIO->ICR = (SDIO_FLAG_DCRCFAIL);
		errorstatus = SD_DATA_CRC_FAIL;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET) {
		SDIO->ICR = (SDIO_FLAG_TXUNDERR);
		errorstatus = SD_TX_UNDERRUN;
		return(errorstatus);
	} else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
		SDIO->ICR = (SDIO_FLAG_STBITERR);
		errorstatus = SD_START_BIT_ERR;
		return(errorstatus);
	}

#endif

  return (errorstatus);
}

/**
 * @brief  Allows to write blocks starting from a specified address in a card.
 *         The Data transfer can be managed by DMA mode only.
 * @note   This operation should be followed by two functions to check if the
 *         DMA Controller and SD Card status.
 *          - SD_ReadWaitOperation(): this function insure that the DMA
 *            controller has finished all data transfer.
 *          - SD_GetStatus(): to check that the SD Card has finished the
 *            data transfer and it is ready for data.
 * @param  WriteAddr: Address from where data are to be read.
 * @param  writebuff: pointer to the buffer that contain the data to be transferred.
 * @param  BlockSize: the SD card Data block size. The Block size should be 512.
 * @param  NumberOfBlocks: number of blocks to be written.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_WriteMultiBlocks (uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;
  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDMMC1->DCTRL = 0x0;

#if defined (SD_DMA_MODE)
  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_TXUNDERR);  // | SDMMC_IT_STBITERR
  SD_LowLevel_DMA_TxConfig ((uint32_t *) writebuff, (NumberOfBlocks * BlockSize));
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN; //SDIO_DMACmd (ENABLE);
#endif

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /* Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCKLEN);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /*!< To improve performance */
  SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t) (RCA << 16)));
  errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /*!< To improve performance */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCK_COUNT, SDMMC_RESPONSE_SHORT, ((uint32_t) NumberOfBlocks));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCK_COUNT);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  /*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
  SDMMC1_Command(SDMMC_CMD_WRITE_MULT_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t) WriteAddr));
  errorstatus = CmdResp1Error(SDMMC_CMD_WRITE_MULT_BLOCK);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) SDIO_DATABLOCKSIZE;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDIO_DataConfig (&SDIO_DataInitStructure);
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = NumberOfBlocks * BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x0UL<<1) & SDMMC_DCTRL_DTDIR;  //0:SDIO to sdcard 1:sdcard to SDIO Data transfer direction selection.
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  return (errorstatus);
}

/**
 * @brief  Allows to write blocks starting from a specified address in a card.
 *         The Data transfer can be managed by DMA mode only.
 * @note   This operation should be followed by two functions to check if the
 *         DMA Controller and SD Card status.
 *          - SD_ReadWaitOperation(): this function insure that the DMA
 *            controller has finished all data transfer.
 *          - SD_GetStatus(): to check that the SD Card has finished the
 *            data transfer and it is ready for data.
 * @param  WriteAddr: Address from where data are to be read.
 * @param  writebuff: pointer to the buffer that contain the data to be transferred.
 * @param  BlockSize: the SD card Data block size. The Block size should be 512.
 * @param  NumberOfBlocks: number of blocks to be written.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_WriteMultiBlocksFIXED (uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t tmpreg;
  TransferError = SDMMC_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDMMC1->DCTRL = 0x0;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    BlockSize = 512;
  else
    WriteAddr *= BlockSize; // Convert to Bytes for NON SDHC

  /* Set Block Size for Card */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCKLEN, SDMMC_RESPONSE_SHORT, ((uint32_t) BlockSize));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCKLEN);
  if(errorstatus != SDMMC_OK){
    return(errorstatus);
  }

  /*!< To improve performance */
  SDMMC1_Command(SDMMC_CMD_APP_CMD, SDMMC_RESPONSE_SHORT, ((uint32_t) (RCA << 16)));
  errorstatus = CmdResp1Error(SDMMC_CMD_APP_CMD);
  if(errorstatus != SDMMC_OK){
    return(errorstatus);
  }

  /*!< To improve performance */
  SDMMC1_Command(SDMMC_CMD_SET_BLOCK_COUNT, SDMMC_RESPONSE_SHORT, ((uint32_t) NumberOfBlocks));
  errorstatus = CmdResp1Error(SDMMC_CMD_SET_BLOCK_COUNT);
  if(errorstatus != SDMMC_OK){
    return(errorstatus);
  }

  /*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
  SDMMC1_Command(SDMMC_CMD_WRITE_MULT_BLOCK, SDMMC_RESPONSE_SHORT, ((uint32_t) WriteAddr));
  errorstatus = CmdResp1Error(SDMMC_CMD_WRITE_MULT_BLOCK);
  if(errorstatus != SDMMC_OK){
    return(errorstatus);
  }

  //SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  //SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  //SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) SDIO_DATABLOCKSIZE;
  //SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  //SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  //SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  //SDIO_DataConfig(&SDIO_DataInitStructure);
  //SDMMC_ConfigData(SDMMC1, &SDIO_DataInitStructure);
  /* Set the SDMMC Data TimeOut value */
  SDMMC1->DTIMER = SDMMC_DATATIMEOUT;
  /* Set the SDMMC DataLength value */
  SDMMC1->DLEN = NumberOfBlocks * BlockSize;
  /* Get the SDIO DCTRL value */
  tmpreg = SDMMC1->DCTRL;
  /* Clear DEN, DTMODE, DTDIR and DBCKSIZE bits */
  tmpreg &= ((uint32_t)0xFFFFFF08);
  /* Set the SDMMC data configuration parameters */
  /* Set DEN bit according to SDIO_DPSM value */
  /* Set DTMODE bit according to SDIO_TransferMode value */
  /* Set DTDIR bit according to SDIO_TransferDir value */
  /* Set DBCKSIZE bits according to SDIO_DataBlockSize value */
  tmpreg |= (0x9UL<<4) & SDMMC_DCTRL_DBLOCKSIZE;  //data block length when the block data transfer mode is selected 512
  tmpreg |= (0x0UL<<2) & SDMMC_DCTRL_DTMODE; //0: Block data transfer 1: Stream or SDIO multibyte data transfer
  tmpreg |= (0x0UL<<1) & SDMMC_DCTRL_DTDIR;  //0:SDIO to sdcard 1:sdcard to SDIO Data transfer direction selection.
  tmpreg |= (0x1UL<<0) & SDMMC_DCTRL_DTEN;   //Data transfer enabled bit
  /* Write to SDMMC DCTRL */
  SDMMC1->DCTRL = tmpreg;

  SDMMC1->MASK |= (SDMMC_IT_DCRCFAIL | SDMMC_IT_DTIMEOUT | SDMMC_IT_DATAEND | SDMMC_IT_RXOVERR);// | SDMMC_IT_STBITERR
  SDMMC1->DCTRL |= SDMMC_DCTRL_DMAEN; //SDIO_DMACmd(ENABLE);
  SD_LowLevel_DMA_TxConfig((uint32_t *)writebuff, (NumberOfBlocks * BlockSize));

  return (errorstatus);
}

/**
 * @brief  This function waits until the SDIO DMA data transfer is finished.
 *         This function should be called after SDIO_WriteBlock() and
 *         SDIO_WriteMultiBlocks() function to insure that all data sent by the
 *         card are already transferred by the DMA controller.
 * @param  None.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_WaitWriteOperation (void)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t timeout;
  uint32_t tmpreg;

  timeout = SDMMC_DATATIMEOUT;

  while((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SDMMC_OK) && (timeout > 0))
  {
    timeout--;
  }

  DMAEndOfTransfer = 0x00;

  timeout = SDMMC_DATATIMEOUT;

  tmpreg = SDMMC1->STA;
  while (((tmpreg & SDMMC_STA_TXACT)) && (timeout > 0))
  {
    timeout--;
    tmpreg = SDMMC1->STA;
  }

  if(StopCondition == 1){
    errorstatus = SD_StopTransfer();
    StopCondition = 0;
  }

  if((timeout == 0) && (errorstatus == SDMMC_OK)) {
    errorstatus = SDMMC_DATA_TIMEOUT;
  }

  /*!< Clear all the static flags */
  SDMMC1->ICR = (SDMMC_STATIC_FLAGS);

  if(TransferError != SDMMC_OK)
    return (TransferError);
  else
    return (errorstatus);
}

/**
 * @brief  Gets the cuurent data transfer state.
 * @param  None
 * @retval SDTransferState: Data Transfer state.
 *   This value can be:
 *        - SD_TRANSFER_OK: No data transfer is acting
 *        - SD_TRANSFER_BUSY: Data transfer is acting
 */
SDTransferState SD_GetTransferState (void)
{
  uint32_t tmpreg;

  tmpreg = SDMMC1->STA;
  if (tmpreg & (SDMMC_STA_TXACT | SDMMC_STA_RXACT))
    return (SDMMC_TRANSFER_BUSY);
  else
    return (SDMMC_TRANSFER_OK);
}

/**
 * @brief  Aborts an ongoing data transfer.
 * @param  None
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_StopTransfer(void)
{
  SD_Error errorstatus = SDMMC_OK;

  /*!< Send CMD12 STOP_TRANSMISSION  */
  SDMMC1_Command(SDMMC_CMD_STOP_TRANSMISSION, SDMMC_RESPONSE_SHORT, 0x0);
  errorstatus = CmdResp1Error(SDMMC_CMD_STOP_TRANSMISSION);

  return (errorstatus);
}

/**
 * @brief  Allows to erase memory area specified for the given card.
 * @param  startaddr: the start address.
 * @param  endaddr: the end address.
 * @retval SD_Error: SD Card Error code.
 */
SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr)
{
  SD_Error errorstatus = SDMMC_OK;
  uint32_t delay = 0;
  __IO uint32_t maxdelay = 0;
  uint8_t cardstate = 0;

  /*!< Check if the card coomnd class supports erase command */
  if (((CSD_Tab[1] >> 20) & SDMMC_CCCC_ERASE )== 0)
  {
    errorstatus = SDMMC_REQUEST_NOT_APPLICABLE;
    return (errorstatus);
  }

  maxdelay = 120000 / ((SDMMC1->CLKCR & 0xFF) + 2);

  if(SDMMC_GetResponse(SDMMC1, SDMMC_RESP1) & SDMMC_CARD_LOCKED)
  {
    errorstatus = SDMMC_LOCK_UNLOCK_FAILED;
    return (errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    startaddr /= 512;
    endaddr /= 512;
  }

  /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */
    SDMMC1_Command(SDMMC_CMD_SD_ERASE_GRP_START, SDMMC_RESPONSE_SHORT, ((uint32_t) startaddr));
    errorstatus = CmdResp1Error (SDMMC_CMD_SD_ERASE_GRP_START);
    if(errorstatus != SDMMC_OK){
      return (errorstatus);
    }

    /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
    SDMMC1_Command(SDMMC_CMD_SD_ERASE_GRP_END, SDMMC_RESPONSE_SHORT, ((uint32_t) endaddr));
    errorstatus = CmdResp1Error(SDMMC_CMD_SD_ERASE_GRP_END);
    if(errorstatus != SDMMC_OK){
      return (errorstatus);
    }
  }

  /*!< Send CMD38 ERASE */
  SDMMC1_Command(SDMMC_CMD_ERASE, SDMMC_RESPONSE_SHORT, 0x0);
  errorstatus = CmdResp1Error(SDMMC_CMD_ERASE);
  if(errorstatus != SDMMC_OK){
    return (errorstatus);
  }

  for (delay = 0; delay < maxdelay; delay++);

  /*!< Wait till the card is in programming state */
  errorstatus = IsCardProgramming(&cardstate);
  delay = SDMMC_DATATIMEOUT;
  while((delay > 0) && (errorstatus == SDMMC_OK) && ((SDMMC_CARD_PROGRAMMING == cardstate) || (SDMMC_CARD_RECEIVING == cardstate)))
  {
    errorstatus = IsCardProgramming(&cardstate);
    delay--;
  }

  return (errorstatus);
}




/**
 * @brief  Configures the DMA2 Channel4 for SDIO Tx request.
 * @param  BufferSRC: pointer to the source buffer
 * @param  BufferSize: buffer size
 * @retval None
 */
void SD_LowLevel_DMA_TxConfig (uint32_t *BufferSRC, uint32_t BufferSize)
{
#if 0
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SD_SDIO_DMA_STREAM, SD_SDIO_DMA_FLAG_FEIF | SD_SDIO_DMA_FLAG_DMEIF | SD_SDIO_DMA_FLAG_TEIF | SD_SDIO_DMA_FLAG_HTIF | SD_SDIO_DMA_FLAG_TCIF);

  /* DMA2 Stream3  or Stream6 disable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, DISABLE);

  /* DMA2 Stream3  or Stream6 Config */
  DMA_DeInit(SD_SDIO_DMA_STREAM );

  SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) SDIO_FIFO_ADDRESS;
  SDDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) BufferSRC;
  SDDMA_InitStructure.DMA_BufferSize = 1;

  SDDMA_InitStructure.DMA_Channel = SD_SDIO_DMA_CHANNEL;
  SDDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /* ??? DMA_MemoryBurst_INC4 */
  SDDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  SDDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  SDDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; /* DMA_MemoryDataSize_Word; */
  SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  SDDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  SDDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  SDDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  SDDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

  SDDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  SDDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /* DMA_FIFOThreshold_Full */

  DMA_Init (SD_SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig (SD_SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig (SD_SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);

  /* DMA2 Stream3  or Stream6 enable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, ENABLE);
#endif

#ifdef SD_SDIO_DMA_STREAM3
  uint32_t tempreg;

  /////PART I::::Adjust the DMA
  //Reset the control register (0x00 is the default value. this also disables the dma. When EN=0, it stops any ongoing DMA transfer)
  DMA2_Stream3->CR &= (~DMA_SxCR_EN);
  DMA2_Stream3->CR = 0;

  //Clear all the flags
  DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;

  //Set the DMA Addresses
  DMA2_Stream3->PAR = (uint32_t)SDIO_FIFO_ADDRESS; //((uint32_t) 0x40012C80) SDIO FIFO Address (=SDIO Base+0x80)
  DMA2_Stream3->M0AR = (uint32_t)BufferSRC;    //Memory address

  //Set the number of data to transfer
  DMA2_Stream3->NDTR = BufferSize;

  /* Configure the DMA functional DMA Rx parameters for transmission */
  //Set the DMA CR
  tempreg = 0;
  tempreg |= (0x04<<25) & DMA_SxCR_CHSEL;  //Select Channel 4. DMA2_Stream3->CR |= SD_SDIO_DMA_CHANNEL. LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_4);
  tempreg |= (0x01<<23) & DMA_SxCR_MBURST;  //4 beat memory burst (memory is 32word. Therefore, each time dma access memory, it reads 4*32 bits) (FIFO size must be integer multiple of memory burst)(FIFO is 4byte. Therefore we can only use 4 beat in this case)
  //Note: Ref manual (p173 (the node at the end of 8.3.11) says that burst mode is not allowed when Pinc=0. However, it appears that this is not true at all. Furthermore. when I set pBurst=0, the SDIO's dma control does not work at all.)
  tempreg |= (0x01<<21) & DMA_SxCR_PBURST;  //4 beat memory burst Mode ([Burst Size*Psize] must be equal to [FIFO size] to prevent FIFO underrun and overrun errors) (burst also does not work in direct mode).
  tempreg |= (0x00<<18) & DMA_SxCR_DBM;   //Disable double buffer mode (when this is set, circluar mode is also automatically set. (the actual value is don't care)
  tempreg |= (0x00<<16) & DMA_SxCR_PL;     //Priority is very_high
  tempreg|=(0x00<<15) & DMA_SxCR_PINCOS;  //Peripheral increment offset (if this is 1 and Pinc=1, then Peripheral will be incremented by 4 regardless of Psize)
  tempreg |= (0x02<<13) & DMA_SxCR_MSIZE;  //Memory data size is 32bit (word)
  tempreg |= (0x02<<11) & DMA_SxCR_PSIZE;  //Peripheral data size is 32bit (word)
  tempreg |= (0x01<<10) & DMA_SxCR_MINC;  //Enable Memory Increment
  tempreg |= (0x00<<9) & DMA_SxCR_MINC;  //Disable Peripheral Increment
  tempreg |= (0x00<<8) & DMA_SxCR_CIRC;   //Disable Circular mode
  tempreg |= (0x01<<6) & DMA_SxCR_DIR;  //Direction 0:P2M, 1:M2P
  tempreg |= (0x01<<5) & DMA_SxCR_PFCTRL; //Peripheral controls the flow control. (The DMA tranfer ends when the data issues end of transfer signal regardless of ndtr value)
  //Bit [4..1] is for interupt mask. I don't use interrupts here
  //Bit 0 is EN. I will set it after I set the FIFO CR. (FIFO CR cannot be modified when EN=1)
  DMA2_Stream3->CR = tempreg;

  //Set the FIFO CR
  tempreg = 0x21; //Reset value
  tempreg |= (0<<7); //FEIE is disabled
  tempreg |= (1<<2); //Fifo is enabled (Direct mode is disabled);
  tempreg |= 3;   //Full fifo (Fifo threshold selection)
  DMA2_Stream3->FCR = tempreg;

  /* Enable DMA transfer complete/error interrupts  */
  DMA2_Stream3->CR |= (DMA_SxCR_TCIE | DMA_SxCR_TEIE);
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);

  //Enable the DMA (When it is enabled, it starts to respond dma requests)
  DMA2_Stream3->CR |= DMA_SxCR_EN;
  //END of PART I
#endif
}
/**
 * @brief  Configures the DMA2 Channel4 for SDIO Rx request.
 * @param  BufferDST: pointer to the destination buffer
 * @param  BufferSize: buffer size
 * @retval None
 */
void SD_LowLevel_DMA_RxConfig (uint32_t *BufferDST, uint32_t BufferSize)
{
#if 0
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SD_SDIO_DMA_STREAM, SD_SDIO_DMA_FLAG_FEIF | SD_SDIO_DMA_FLAG_DMEIF | SD_SDIO_DMA_FLAG_TEIF | SD_SDIO_DMA_FLAG_HTIF | SD_SDIO_DMA_FLAG_TCIF);
  /* DMA2 Stream3  or Stream6 disable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, DISABLE); //DMA2_Stream3->CR &= ~(uint32_t)DMA_SxCR_EN;

  /* DMA2 Stream3 or Stream6 Config */
  DMA_DeInit(SD_SDIO_DMA_STREAM);

  SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) SDIO_FIFO_ADDRESS; //DMA2_Stream3->PAR
  SDDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) BufferDST; //DMA2_Stream3->M0AR
  SDDMA_InitStructure.DMA_BufferSize = 1; //DMA2_Stream3->NDTR Peripheral controls, therefore we don't need to indicate a size

  SDDMA_InitStructure.DMA_Channel = SD_SDIO_DMA_CHANNEL;  //DMA2_Stream3->CR bit 27:25
  SDDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;  //DMA2_Stream3->CR bit 24:23
  SDDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4; //DMA2_Stream3->CR bit 22:21
  SDDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA2_Stream3->CR bit 17:16
  SDDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; //DMA2_Stream3->CR bit 14:13
  SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // bit 12:11
  SDDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA2_Stream3->CR bit 10
  SDDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//DMA2_Stream3->CR bit 9
  SDDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA2_Stream3->CR bit 8  0:DMA_Mode_Normal 1:DMA_Mode_Circular 循环模式
  SDDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //DMA2_Stream3->CR bit 7:6

  SDDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;  //DMA2_Stream3->FCR bit 2
  SDDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //DMA2_Stream3->FCR bit 1:0
  DMA_Init (SD_SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig (SD_SDIO_DMA_STREAM, DMA_IT_TC, ENABLE); //DMA2_Stream3->CR bit 4
  DMA_FlowControllerConfig (SD_SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral); //DMA2_Stream3->CR bit 5 外设是流控制器

  /* DMA2 Stream3 or Stream6 enable */
  DMA_Cmd(SD_SDIO_DMA_STREAM, ENABLE);  //DMA2_Stream3->CR bit 0
#endif

#ifdef SD_SDIO_DMA_STREAM3
  uint32_t tempreg;

  /////PART I::::Adjust the DMA
  //Reset the control register (0x00 is the default value. this also disables the dma. When EN=0, it stops any ongoing DMA transfer)
  DMA2_Stream3->CR &= (~DMA_SxCR_EN);
  DMA2_Stream3->CR = 0;

  //Clear all the flags
  DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;

  //Set the DMA Addresses
  DMA2_Stream3->PAR = (uint32_t)SDIO_FIFO_ADDRESS; //((uint32_t) 0x40012C80) SDIO FIFO Address (=SDIO Base+0x80)
  DMA2_Stream3->M0AR = (uint32_t)BufferDST;    //Memory address

  //Set the number of data to transfer
  DMA2_Stream3->NDTR = BufferSize;   //0 ?? Peripheral controls, therefore we don't need to indicate a size

  /* Configure the DMA functional DMA Rx parameters for transmission */
  //Set the DMA CR
  tempreg = 0;
  tempreg |= (0x04<<25) & DMA_SxCR_CHSEL;  //Select Channel 4. DMA2_Stream3->CR |= SD_SDIO_DMA_CHANNEL. LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_4);
  tempreg |= (0x01<<23) & DMA_SxCR_MBURST;  //4 beat memory burst (memory is 32word. Therefore, each time dma access memory, it reads 4*32 bits) (FIFO size must be integer multiple of memory burst)(FIFO is 4byte. Therefore we can only use 4 beat in this case)
  //Note: Ref manual (p173 (the node at the end of 8.3.11) says that burst mode is not allowed when Pinc=0. However, it appears that this is not true at all. Furthermore. when I set pBurst=0, the SDIO's dma control does not work at all.)
  tempreg |= (0x01<<21) & DMA_SxCR_PBURST;  //4 beat memory burst Mode ([Burst Size*Psize] must be equal to [FIFO size] to prevent FIFO underrun and overrun errors) (burst also does not work in direct mode).
  tempreg |= (0x00<<18) & DMA_SxCR_DBM;   //Disable double buffer mode (when this is set, circluar mode is also automatically set. (the actual value is don't care)
  tempreg |= (0x00<<16) & DMA_SxCR_PL;     //Priority is very_high
  tempreg|=(0x00<<15) & DMA_SxCR_PINCOS;  //Peripheral increment offset (if this is 1 and Pinc=1, then Peripheral will be incremented by 4 regardless of Psize)
  tempreg |= (0x02<<13) & DMA_SxCR_MSIZE;  //Memory data size is 32bit (word)
  tempreg |= (0x02<<11) & DMA_SxCR_PSIZE;  //Peripheral data size is 32bit (word)
  tempreg |= (0x01<<10) & DMA_SxCR_MINC;  //Enable Memory Increment
  tempreg |= (0x00<<9) & DMA_SxCR_MINC;  //Disable Peripheral Increment
  tempreg |= (0x00<<8) & DMA_SxCR_CIRC;   //Disable Circular mode
  tempreg |= (0x00<<6) & DMA_SxCR_DIR;  //Direction 0:P2M, 1:M2P
  tempreg |= (0x01<<5) & DMA_SxCR_PFCTRL; //Peripheral controls the flow control. (The DMA tranfer ends when the data issues end of transfer signal regardless of ndtr value)
  //Bit [4..1] is for interupt mask. I don't use interrupts here
  //Bit 0 is EN. I will set it after I set the FIFO CR. (FIFO CR cannot be modified when EN=1)
  DMA2_Stream3->CR = tempreg;

  //Set the FIFO CR
  tempreg = 0x21; //Reset value
  tempreg |= (0<<7); //FEIE is disabled
  tempreg |= (1<<2); //Fifo is enabled (Direct mode is disabled);
  tempreg |= 3;   //Full fifo (Fifo threshold selection)
  DMA2_Stream3->FCR = tempreg;

  /* Enable DMA transfer complete/error interrupts  */
  DMA2_Stream3->CR |= (DMA_SxCR_TCIE | DMA_SxCR_TEIE);
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);

  //Enable the DMA (When it is enabled, it starts to respond dma requests)
  DMA2_Stream3->CR |= DMA_SxCR_EN;
  //END of PART I
#endif
}
/**
  * @brief  Allows to process all the interrupts that are high.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_ProcessIRQSrc(void)
{
  uint32_t tempreg;

  tempreg = SDMMC1->STA;
  if(tempreg & SDMMC_STA_DATAEND)
  {
    TransferError = SDMMC_OK;
    SDMMC1->ICR = SDMMC_ICR_DATAENDC;
    TransferEnd = 1;
  }
  else if(tempreg & SDMMC_STA_DCRCFAIL)
  {
    SDMMC1->ICR = SDMMC_ICR_DCRCFAILC;
    TransferError = SDMMC_DATA_CRC_FAIL;
  }
  else if(tempreg & SDMMC_STA_DTIMEOUT)
  {
    SDMMC1->ICR = SDMMC_ICR_DTIMEOUTC;
    TransferError = SDMMC_DATA_TIMEOUT;
  }
  else if(tempreg & SDMMC_STA_RXOVERR)
  {
    SDMMC1->ICR = SDMMC_ICR_RXOVERRC;
    TransferError = SDMMC_RX_OVERRUN;
  }
  else if(tempreg & SDMMC_STA_TXUNDERR)
  {
    SDMMC1->ICR = SDMMC_ICR_TXUNDERRC;
    TransferError = SDMMC_TX_UNDERRUN;
  }
  //else if (SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET)
  //{
  //  SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
  //  TransferError = SD_START_BIT_ERR;
  //}
  SDMMC1->MASK &= ~(SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_DTIMEOUTIE | SDMMC_MASK_DATAENDIE |
		            SDMMC_MASK_TXFIFOHEIE | SDMMC_MASK_RXFIFOHFIE | SDMMC_MASK_TXUNDERRIE | SDMMC_MASK_RXOVERRIE);

  return(TransferError);
}

/**
  * @brief  This function waits until the SDIO DMA data transfer is finished.
  * @param  None.
  * @retval None.
  */
void SD_ProcessDMAIRQ(void)
{
#ifdef SD_SDIO_DMA_STREAM3
  if(DMA2->LISR & SD_SDIO_DMA_FLAG_TCIF)
#else
  if(DMA2->HISR & SD_SDIO_DMA_FLAG_TCIF)
#endif
  { //数据流发生传输完成事件
    DMAEndOfTransfer = 0x01;
    DMA2->LIFCR = SD_SDIO_DMA_FLAG_TCIF|SD_SDIO_DMA_FLAG_FEIF;
    //DMA_ClearFlag(SD_SDIO_DMA_STREAM, SD_SDIO_DMA_FLAG_TCIF|SD_SDIO_DMA_FLAG_FEIF);
  }
}

#endif
/////////////////////////////////////////////////////////////////////////////




