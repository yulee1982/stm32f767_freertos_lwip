
/* Includes ------------------------------------------------------------------*/
#include "sdio_sd.h"
#include "main.h"


#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;
HAL_SD_CardInfoTypedef sdcardinfo;

#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            1   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((512 * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
uint32_t aTxBuffer_sd[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer_sd[BUFFER_WORDS_SIZE];

static void SD_LowLevel_Init(void);
static void MX_SDMMC1_DMA(void);

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for data
  *         transfer).
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
void SD_Init(void)
{
  HAL_SD_ErrorTypedef errorstate = SD_OK;

  /* Check if the SD card is plugged in the slot */
  if (Find_SD_IsDetected() != SD_PRESENT)
  {
    printf("SD card is not plugged in the slot !\r\n");
    return;
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
    //errorstate = HAL_SD_WideBusOperation_Config(&hsd, SDMMC_BUS_WIDE_4B);
  }

  HAL_SD_ReadBlocks_DMA(&hsd, aRxBuffer_sd, BLOCK_START_ADDR, 512, NUM_OF_BLOCKS);
  /* wait until the read operation is finished */
  while(HAL_SD_CheckReadOperation(&hsd, 500)!= SD_OK)
  {
  }

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
  hdma_sdio_rx.Init.Priority            = DMA_PRIORITY_HIGH;
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
  hdma_sdio_tx.Init.Priority            = DMA_PRIORITY_HIGH;
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
uint8_t Find_SD_IsDetected(void)
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


/////////////////////////////////////////////////////////////////////



#if 0


static uint32_t SD_Response(uint32_t *response, uint32_t type)
{
  //I mainly use this to block the execution in case an unexpected response is received.
  //Actually I don't need this at all. However, just for the sake of extra information I keep this.
  //All I reall need is for this function to return SDIO->RESP1
  //In the main code, I don't use the retun values at all. Perhaps I ought to have used void.
  uint32_t tempreg = 0;

  //R1 Responses
  if ((type == RESP_R1) || (type == RESP_R1b)) {
    //tempreg = SDMMC1->RESPCMD;
    //if(tempreg != 55)
    //  return tempreg;
    *response = SDMMC1->RESP1;
    tempreg = SDMMC1->RESP1;
    //if (SDMMC1->RESP1 & (uint32_t)0xFDFFE008) {   //All error bits must be zero
    if (tempreg & (uint32_t)0xFDFFE008) {
      SD_Panic(SDMMC1->RESPCMD, "SDIO:Response Error\n");
    }
    return (*response & 0x1F00)>>8; //Return the card status
  }
  else if (type == RESP_R2) { //CSD or CSI register. 128 bit
    *response++ = SDMMC1->RESP1;
    *response++ = SDMMC1->RESP2;
    *response++ = SDMMC1->RESP3;
    *response = SDMMC1->RESP4;
    return 0;
  }
  else if (type == RESP_R3) { //OCR
    if (SDMMC1->RESPCMD != 0x3F) {SD_Panic(SDMMC1->RESPCMD,"SDIO:Unexpected command index\n");} //CMD index for R3 must be 0x3F
    *response = SDMMC1->RESP1;  //Equals to OCR
    return 0;
  }
  else if (type==RESP_R6) { //RCA Response
    if (SDMMC1->RESPCMD != 0x03) {SD_Panic(SDMMC1->RESPCMD,"SDIO:Unexpected command index\n");} //Only cmd3 generates R6 response
    *response=SDMMC1->RESP1;  //Equals to OCR
    return (*response)>>16; //Return is equal to RCA. (The first 16 bit is equal to status)
  }
  else {  //RESP_R7:Card Interface condition. Obtained after CMD8
    if (SDMMC1->RESPCMD != 0x08) {SD_Panic(SDMMC1->RESPCMD,"SDIO:Unexpected command index\n");} //Only cmd8 generates R7 response
    *response = SDMMC1->RESP1;
    if ((*response & 0xFF) != 0xAA) {SD_Panic(CMD8, "SDIO:Pattern did not match\n");} //Only cmd8 generates R7 response
    return ((*response) & 0xFF00)>>8; //Echo back value
  }
}

static void SD_StartBlockTransfer(uint8_t *buf, uint32_t cnt, uint32_t dir)
{
  //cnt must be integer multiple of 512!!! I will enforce this inside this function
  //Starts the actual data tranfer using the DMA.
  //Prior to calling this command. The SDCard must have been adjusted using commands
  uint32_t tempreg;

  //Make cnt an integer multiple of 512
  //Then mask it with the maximum value allowed (2^24)
  cnt = 0x01FFFFFF & ((cnt>>8) << 8);

  /////PART I::::Adjust the DMA
  //Reset the control register (0x00 is the default value. this also disables the dma. When EN=0, it stops any ongoing DMA transfer)
  DMA2_Stream3->CR = 0;

  //Clear all the flags
  DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3 | DMA_LIFCR_CHTIF3;

  //Set the DMA Addresses
  DMA2_Stream3->PAR=((uint32_t) 0x40012C80);  //SDIO FIFO Address (=SDIO Base+0x80)
  DMA2_Stream3->M0AR=(uint32_t) buf;    //Memory address

  //Set the number of data to transfer
  DMA2_Stream3->NDTR=0;   //Peripheral controls, therefore we don't need to indicate a size

  //Set the DMA CR
  tempreg = 0;
  tempreg |= (0x04<<25) & DMA_SxCR_CHSEL;  //Select Channel 4
  tempreg |= (0x01<<23) & DMA_SxCR_MBURST;  //4 beat memory burst (memory is 32word. Therefore, each time dma access memory, it reads 4*32 bits) (FIFO size must be integer multiple of memory burst)(FIFO is 4byte. Therefore we can only use 4 beat in this case)
  //Note: Ref manual (p173 (the node at the end of 8.3.11) says that burst mode is not allowed when Pinc=0. However, it appears that this is not true at all. Furthermore. when I set pBurst=0, the SDIO's dma control does not work at all.)
  tempreg |= (0x01<<21) & DMA_SxCR_PBURST;  //4 beat memory burst Mode ([Burst Size*Psize] must be equal to [FIFO size] to prevent FIFO underrun and overrun errors) (burst also does not work in direct mode).
  tempreg |= (0x00<<18) & DMA_SxCR_DBM;   //Disable double buffer mode (when this is set, circluar mode is also automatically set. (the actual value is don't care)
  tempreg |= (0x03<<16) & DMA_SxCR_PL;     //Priority is very_high
  tempreg |= (0x00<<15) & DMA_SxCR_PINCOS;  //Peripheral increment offset (if this is 1 and Pinc=1, then Peripheral will be incremented by 4 regardless of Psize)
  tempreg |= (0x02<<13) & DMA_SxCR_MSIZE;  //Memory data size is 32bit (word)
  tempreg |= (0x02<<11) & DMA_SxCR_PSIZE;  //Peripheral data size is 32bit (word)
  tempreg |= (0x01<<10) & DMA_SxCR_MINC;  //Enable Memory Increment
  tempreg |= (0x00<<9) & DMA_SxCR_MINC;  //Disable Peripheral Increment
  tempreg |= (0x00<<8) & DMA_SxCR_CIRC;   //Disable Circular mode
  //tempreg |= (0x00<<6) & DMA_SxCR_DIR;  //Direction 0:P2M, 1:M2P
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

  //Set the Direction of transfer
  if (dir == UM2SD) {
    DMA2_Stream3->CR |= (0x01<<6) & DMA_SxCR_DIR;
  } else if (dir == SD2UM) {
    DMA2_Stream3->CR |= (0x00<<6) & DMA_SxCR_DIR;
  }

  //Enable the DMA (When it is enabled, it starts to respond dma requests)
  DMA2_Stream3->CR |= DMA_SxCR_EN;
  //END of PART I

  ////PART II::::Adjust and enable SDIO Peripheral
  //Clear the Data status flags
  SDMMC1->ICR = (SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_TXUNDERR | SDMMC_STA_RXOVERR | SDMMC_STA_DATAEND | SDMMC_STA_DBCKEND); // SDMMC_STA_STBITERR ??

  //First adjust the Dtimer and Data length
  SDMMC1->DTIMER = (uint32_t) DATATIMEOUT;
  SDMMC1->DLEN = cnt;

  //Now adjust DCTRL (and enable it at the same time)
  tempreg = 0;  //Reset value
  tempreg |= (uint32_t) 9 << 4;  //Block size is 512 Compute log2(BlockSize) and shift 4bit
  tempreg |= 1<<3; //Enable the DMA
  tempreg |= 0<<2; //DTMode=Block Transfer (Actualy this is the reset value. Just a remainder)
  tempreg |= (dir & SDMMC_DCTRL_DTDIR);  //Direction. 0=Controller to card, 1=Card to Controller
  tempreg |= 1; //DPSM is enabled
  //Keep the rest at 0 => OTher SDIO functions is disabled(we don't need them)
  SDMMC1->DCTRL = tempreg;
  //End of PART II

  //Warn everyone that there may be a transfer in progress
  SDIOTxRx = 1;
}


void SD_WaitTransmissionEnd(void)
{
  //This function first checks if there is an ogoing tranmission and block till it ends.
  //It then checks the data flags to see if there is an error. In case of an error it blocks
  //Before the start of data transmission the data flags are all cleared. Therefore, calling this fucntion after a real transmission works as expected.

  ////Check if there is an ongoing transmission
  //Check if the DMA is disabled (SDIO disables the DMA after it is done with it)
  while (DMA2_Stream3->CR & DMA_SxCR_EN) {};
  //Wait for the DMA Interrupt flags if there exist a previous SDIO transfer.
  if (SDIOTxRx) {
    if (DMA2->LISR & (DMA_LISR_TCIF3 | DMA_LISR_TEIF3 | DMA_LISR_DMEIF3 | DMA_LISR_FEIF3)) {
      if (!(DMA2->LISR & DMA_LISR_TCIF3)) {//A DMA error has occured. Panic!
        SD_Panic(DMA2->LISR, "SDIO:DMA Error");
      }
    }
  }

  //Wait till SDIO is not active
  while (SDMMC1->STA & (SDMMC_STA_RXACT | SDMMC_STA_TXACT)) {};

  //if there exist a previous transmission, check if the transmission has been completed without error
  if (SDIOTxRx) {
    //I will block here till I get a data response
    while (!(SDMMC1->STA & (SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND))) {}; // | SDIO_IT_STBITERR ??
    if (!(SDMMC1->STA & SDMMC_STA_DBCKEND)) {  //An Error has occured.
      SD_Panic(SDMMC1->STA, "SDIO:Data Transmission Error\n");
    }
  }

  //If we are here, we can be sure that there is no ongoing transmission any more
  SDIOTxRx=0;
}
static void SD_Panic(uint32_t code, uint8_t *message)
{
  uint32_t i=0;

  printf((char *) message);

  //Block the execution with blinky leds
  while (1) {
    i = 216000000/4;
    while(i--){}
    LL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    i = 216000000/4;
    while(i--){}
  }
}
#endif
