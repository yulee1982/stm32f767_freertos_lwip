
#include "usart.h"
#include <string.h>


const uint8_t aTxBuffer[] = "STM32F7xx USART LL API Example : TX/RX in DMA mode\r\nConfiguration UART 115200 bps, 8 data bit/1 stop bit/No parity/No HW flow control\r\n";

uint32_t TxState = USART_STATE_RESET;    /*!< UART state information related to Tx operations. */
uint32_t RxState = USART_STATE_RESET;   /*!< UART state information related to Rx operations. */
uint32_t ErrorCode;
static uint8_t *pRxData = NULL;
static uint16_t RxSize;
static uint16_t RxMask;
static volatile uint32_t usart_rx_buff_tail = 0;
uint8_t rx_it_flag = 0;

static const uint8_t *pTxData = NULL;
static uint16_t TxSize;

//__IO uint8_t ubTransmissionComplete = 0;
//__IO uint8_t ubReceptionComplete = 0;
/*
uint8_t usart_rx_buff[1024];
volatile uint32_t usart_rx_buff_head = 0;
volatile uint32_t usart_rx_buff_tail = 0;
volatile uint32_t TXmsgQty = 0;
uint8_t usart_tx_buff[1024];
volatile uint32_t usart_tx_buff_head = 0;
volatile uint32_t usart_tx_buff_tail = 0;
*/

//#define dataendmagic
static uint8_t TxISR(USART_TypeDef *pUSARTx);
static uint8_t RxISR(USART_TypeDef *pUSARTx);
//static uint8_t rx_data_fifo(uint8_t rw);
//static uint8_t tx_data_fifo(uint8_t rw);

int _write(int fd, char *ptr, int len)
{
  Usart_Transmit(USART2, (const uint8_t *)ptr, (uint16_t)len, 0xFFFF);
  return len;
}

/**
  * @brief  Function called for achieving TX buffer sending
  * @param  None
  * @retval None
  */
uint8_t Usart_Transmit(USART_TypeDef *pUSARTx, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  const uint8_t  *pdata8bits;
  const uint16_t *pdata16bits;
  uint16_t ubSend = 0;
  uint16_t ubNbDataToTransmit = 0;
  uint32_t tmpTimeout;

  /* Check that a Tx process is not already ongoing */
  if (TxState == USART_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return USART_ERROR;
    }

    TxState = USART_STATE_BUSY_TX;
    ubNbDataToTransmit = Size;
    /* In case of 9bits/No Parity transfer, pData needs to be handled as a uint16_t pointer */
    if((pUSARTx->CR1 & USART_CR1_M0)&&(!(pUSARTx->CR1 & USART_CR1_PCE))) //UART_WORDLENGTH_9B and Parity control enable (奇偶校验使能)
    {
      pdata8bits  = NULL;
      pdata16bits = (const uint16_t *) pData;
    }else{
      pdata8bits  = pData;
      pdata16bits = NULL;
    }

    /* Send characters one per one, until last char to be sent */
    while(ubSend < ubNbDataToTransmit) // while (ubSend < sizeof(aTxBuffer_init))
    {
      tmpTimeout = Timeout; //mS
      /* Wait for TXE flag to be raised */
      while(!(pUSARTx->ISR & USART_ISR_TXE))  //while(!LL_USART_IsActiveFlag_TXE(USART2))
      {
        /* Check Systick counter flag to decrement the time-out value */
        if(LL_SYSTICK_IsActiveCounterFlag()) /* USE_TIMEOUT */
        {
          if(tmpTimeout-- == 0)
          {
            TxState = USART_STATE_READY;
            /* Time-out occurred. Set LED to blinking mode */
            return USART_TIMEOUT;
          }
        }
      }

      /* If last char to be sent, clear TC flag */
      if(ubSend == (ubNbDataToTransmit - 1)) //if (ubSend == (sizeof(aTxBuffer_init) - 1))
      {
        LL_USART_ClearFlag_TC(pUSARTx); //pUSARTx->ICR = USART_ICR_TCCF;
      }

      /* Write character in Transmit Data register. TXE flag is cleared by writing data in TDR register */
      //LL_USART_TransmitData8(USART2, *pData++);
      if(pdata8bits == NULL)
      {
        pUSARTx->TDR = (uint16_t)(*pdata16bits & 0x01FFU);
        pdata16bits++;
      }else{
        pUSARTx->TDR = (uint8_t)(*pdata8bits & 0xFFU);
        pdata8bits++;
      }
      ubSend++;
    }

    tmpTimeout = Timeout;
    /* Wait for TC flag to be raised for last char */
    while(!(pUSARTx->ISR & USART_ISR_TC))  //while(!LL_USART_IsActiveFlag_TC(USART2))
    {
      /* Check Systick counter flag to decrement the time-out value */
      if(LL_SYSTICK_IsActiveCounterFlag())
      {
        if(tmpTimeout-- == 0)
        {
          TxState = USART_STATE_READY;
          /* Time-out occurred. Set LED to blinking mode */
          return USART_TIMEOUT;
        }
      }
    }
    /* At end of Tx process, restore huart->gState to Ready */
    TxState = USART_STATE_READY;
    return USART_OK;
  }else{
    return USART_BUSY;
  }
}

uint8_t Usart_Transmit_DMA(USART_TypeDef *pUSARTx, const uint8_t *pData, uint16_t Size)
{
  /* Check that a Tx process is not already ongoing */
  //if(TxState == USART_STATE_READY)
  //{
    if ((pData == NULL) || (Size == 0U))
    {
      return USART_ERROR;
    }

    pTxData = pData;
    TxSize = Size;

    TxState = USART_STATE_BUSY_TX;

    /* Enable the UART transmit DMA channel */
    DMA_Usart2_Start_Transfers_IT((uint32_t)pTxData, TxSize);

    /* Clear the TC flag in the ICR register. Transmission complete clear flag */
    USART2->ICR = USART_ICR_TCCF;

    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the UART CR3 register */
    //ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);
    LL_USART_EnableDMAReq_TX(USART2);

    return USART_OK;
  //}
  //else
  //{
  //  return USART_BUSY;
  //}
}

void MX_USART2_UART_Init(void)
{
  /* (1) Enable GPIO clock and configures the USART pins **********************/

   /* Enable the peripheral clock of GPIO Port */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOD, LL_GPIO_PIN_5, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_5, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOD, LL_GPIO_PIN_6, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);

  /* (2) Enable USART2 peripheral clock and clock source ****************/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  /* Set clock source */
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

  /* (3) Configure USART3 functional parameters ********************************/

  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USART2);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
  // LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(USART2, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 54000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
        Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance

	    In this example, Peripheral Clock is expected to be equal to 54000000 Hz => equal to SystemCoreClock/4
  */
  LL_USART_SetBaudRate(USART2, SystemCoreClock/4, LL_USART_OVERSAMPLING_16, 115200);

  /* (4) Enable USART3 **********************************************************/
  LL_USART_Enable(USART2);

  /* Peripheral interrupt init */
  NVIC_SetPriority(USART2_IRQn, 8);
  NVIC_EnableIRQ(USART2_IRQn);

  TxState = USART_STATE_READY;
  RxState = USART_STATE_READY;
  /* transfer Tx buffer to PC application */
  Usart_Transmit(USART2, (const uint8_t *)("STM32F7xx USART initial ...\r\n"), 29, 0x1ff);
  Usart_Transmit(USART2, (const uint8_t *)aTxBuffer, sizeof(aTxBuffer), 0x1ff);
}

void MX_USART2_DMA(void)
{
  /* DMA1 used for USART3 Transmission and Reception
   */
  /* (1) Enable the clock of DMA1 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Stream6_IRQn, 9);
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  //NVIC_SetPriority(DMA1_Stream5_IRQn, 0);
  //NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  /* (3) Configure the DMA functional parameters for transmission */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_6,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |  /*memory to interface bit:6  */
                        LL_DMA_PRIORITY_HIGH              |  /*优先级   */
                        LL_DMA_MODE_NORMAL                |  /*   */
                        LL_DMA_PERIPH_NOINCREMENT         |  /*外设地址指针固定，不增量 bit:9  */
                        LL_DMA_MEMORY_INCREMENT           |  /*memory地址指针增量   */
                        LL_DMA_PDATAALIGN_BYTE            |  /*外设数据大小8位，bit:12-11   */
                        LL_DMA_MDATAALIGN_BYTE);             /*memory数据大小8位，bit:14-13   */
#if 0
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_6,
                         (uint32_t)aTxBuffer,
                         LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_6));
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, ubNbDataToTransmit);

  /* (4) Configure the DMA functional parameters for reception , here not use*/
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_5, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_5,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_PRIORITY_HIGH              |
                        LL_DMA_MODE_NORMAL                |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_BYTE            |
                        LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_5,
                         LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_RECEIVE),
                         (uint32_t)aRxBuffer,
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_5));
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, ubNbDataToReceive);

  /* (5) Enable DMA transfer complete/error interrupts  */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_6);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_6);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_5);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_5);
#endif
}

#if 0
void StartTransfers(void)
{
  /* Enable DMA RX Interrupt */
  LL_USART_EnableDMAReq_RX(USART2);
  /* Enable DMA TX Interrupt */
  LL_USART_EnableDMAReq_TX(USART2);
  /* Enable DMA Channel Rx */
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
  /* Enable DMA Channel Tx */
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
}
#endif
uint8_t DMA_Usart2_Start_Transfers_IT(uint32_t SrcAddress, uint32_t DataLength)
{
  /* Clear DBM bit */
  DMA1_Stream6->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Stream source, destination address and the data length */
  //LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, ubNbDataToTransmit);
  DMA1_Stream6->NDTR = DataLength;

  /* Memory to Peripheral */
  /* Configure DMA Stream destination address */
  //DMA1_Stream6->PAR = LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT);
  /* Configure DMA Stream source address */
  //DMA1_Stream6->M0AR = SrcAddress;
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_6,
                         (uint32_t)SrcAddress,
                         LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_6));

  /* Clear all interrupt flags at correct offset within the register */
  //regs->IFCR = 0x3FU << hdma->StreamIndex;
  //DMA1->HIFCR = 0x003D0000U;  ???

  /* Enable Common interrupts*/
  DMA1_Stream6->CR  |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME; //
  DMA1_Stream6->FCR |= DMA_IT_FE;

  /* Enable DMA RX Interrupt */
  //LL_USART_EnableDMAReq_RX(USART2);
  /* Enable DMA TX Interrupt */
  //LL_USART_EnableDMAReq_TX(USART2);

  /* Enable DMA Channel Rx */
  //LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);

  /* Enable DMA Channel Tx */
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
  //DMA1_Stream6->CR |= DMA_SxCR_EN;

  return 0;
}

uint8_t Usart_Receive(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint8_t  *pdata8bits;
  uint16_t *pdata16bits;
  uint16_t uhMask;
  uint16_t ubReceive = 0;
  uint16_t ubNbDataToReceive = 0;
  uint32_t tmpTimeout;

  /* Check that a Rx process is not already ongoing */
  if(RxState == USART_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  USART_ERROR;
    }

    RxState = USART_STATE_BUSY_RX;
    ubNbDataToReceive = Size;
    /* Computation of UART mask to apply to RDR register */
    //UART_MASK_COMPUTATION(huart);
    if((pUSARTx->CR1 & USART_CR1_M0)&&(!(pUSARTx->CR1 & USART_CR1_M1))) //UART_WORDLENGTH_9B
    {
      if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
        uhMask = 0x01FFU;
      else
        uhMask = 0x00FFU;
    }
    else if((!(pUSARTx->CR1 & USART_CR1_M0))&&(!(pUSARTx->CR1 & USART_CR1_M1))) //UART_WORDLENGTH_8B
    {
      if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
        uhMask = 0x00FFU;
      else
        uhMask = 0x007FU;
    }
    else if((!(pUSARTx->CR1 & USART_CR1_M0))&&(pUSARTx->CR1 & USART_CR1_M1)) //UART_WORDLENGTH_7B
    {
      if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
        uhMask = 0x007FU;
      else
        uhMask = 0x003FU;
    }
    else
    {
      uhMask = 0x0000U;
      RxState = USART_OK;
      return  USART_ERROR;
    }

    /* In case of 9bits/No Parity transfer, pRxData needs to be handled as a uint16_t pointer */
    if((pUSARTx->CR1 & USART_CR1_M0)&&(!(pUSARTx->CR1 & USART_CR1_PCE))) //UART_WORDLENGTH_9B and Parity control enable (奇偶校验使能)
    {
      pdata8bits  = NULL;
      pdata16bits = (uint16_t *) pData;
    }
    else
    {
      pdata8bits  = pData;
      pdata16bits = NULL;
    }

    /* as long as data have to be received */
    while(ubReceive < ubNbDataToReceive) // while (ubSend < sizeof(aTxBuffer_init))
    {
      tmpTimeout = Timeout; //mS
      /* Wait for TXE flag to be raised */
      while(!(pUSARTx->ISR & USART_ISR_RXNE))
      {
        /* Check Systick counter flag to decrement the time-out value */
        if(LL_SYSTICK_IsActiveCounterFlag()) /* USE_TIMEOUT */
        {
          if(tmpTimeout-- == 0)
          {
            RxState = USART_STATE_READY;
            /* Time-out occurred. Set LED to blinking mode */
            return USART_TIMEOUT;
          }
        }
      }

      if(pdata8bits == NULL)
      {
        *pdata16bits = (uint16_t)(pUSARTx->RDR & uhMask);
        pdata16bits++;
      }
      else
      {
        *pdata8bits = (uint8_t)(pUSARTx->RDR & (uint8_t)uhMask);
        pdata8bits++;
      }
      ubReceive++;
    }

    /* At end of Rx process, restore huart->RxState to Ready */
    RxState = USART_STATE_READY;
    return USART_OK;
  }
  else
  {
    return USART_BUSY;
  }
}

/**
  * @brief Receive an amount of data in interrupt mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         address of user data buffer for storing data to be received, should be aligned on a half word frontier
  *         (16 bits) (as received data will be handled using u16 pointer cast). Depending on compilation chain,
  *         use of specific alignment compilation directives or pragmas might be required
  *         to ensure proper alignment for pData.
  * @param huart UART handle.
  * @param pData Pointer to data buffer (u8 or u16 data elements).
  * @param Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
uint8_t Usart_Receive_IT(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size)
{
  /* Check that a Rx process is not already ongoing */
  if (RxState == USART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return USART_ERROR;
    }

    /* Check that USART RTOEN bit is set */
    if((pUSARTx->CR2 & USART_CR2_RTOEN) != 0U)
    {
      /* Enable the UART Receiver Timeout Interrupt */
      pUSARTx->CR1 |= USART_CR1_RTOIE;
    }

    return (Usart_Start_Receive_IT(pUSARTx, pData, Size));
  }else{
    return USART_BUSY;
  }
}

uint8_t Usart_Start_Receive_IT(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size)
{
  uint16_t uhMask;

  pRxData = pData;
  RxSize = Size;
  usart_rx_buff_tail = 0;
  RxState = USART_STATE_BUSY_RX;
  rx_it_flag = 0;

  /* Computation of UART mask to apply to RDR register */
  //UART_MASK_COMPUTATION(huart);
  if((pUSARTx->CR1 & USART_CR1_M0)&&(!(pUSARTx->CR1 & USART_CR1_M1))) //UART_WORDLENGTH_9B
  {
    if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
      uhMask = 0x01FFU;
    else
      uhMask = 0x00FFU;
  }
  else if((!(pUSARTx->CR1 & USART_CR1_M0))&&(!(pUSARTx->CR1 & USART_CR1_M1))) //UART_WORDLENGTH_8B
  {
    if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
      uhMask = 0x00FFU;
    else
      uhMask = 0x007FU;
  }
  else if((!(pUSARTx->CR1 & USART_CR1_M0))&&(pUSARTx->CR1 & USART_CR1_M1)) //UART_WORDLENGTH_7B
  {
    if(!(pUSARTx->CR1 & USART_CR1_PCE))//Parity control enable (奇偶校验未使能)
      uhMask = 0x007FU;
    else
      uhMask = 0x003FU;
  }
  else
  {
    uhMask = 0x0000U;
  }

  RxMask = uhMask;

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  pUSARTx->CR3 |= USART_CR3_EIE;

  /* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
  if(pUSARTx->CR1 & USART_CR1_PCE)
  {
    pUSARTx->CR1 |= USART_CR1_PEIE | USART_CR1_RXNEIE;
  }
  else
  {
    pUSARTx->CR1 |= USART_CR1_RXNEIE;
  }

  return USART_OK;
}
static uint8_t TxISR(USART_TypeDef *pUSARTx)
{

  return 0;
}


static uint8_t RxISR(USART_TypeDef *pUSARTx)
{
  uint8_t  *pdata8bits;
  uint16_t *pdata16bits;
  uint16_t ubReceive = 0;

  /* Set the Rx ISR function pointer according to the data word length */
  if((pUSARTx->CR1 & USART_CR1_M0)&&(!(pUSARTx->CR1 & USART_CR1_PCE)))
  {
    pdata8bits  = NULL;
    pdata16bits = (uint16_t *) pRxData;
    *(pdata16bits + usart_rx_buff_tail) = (uint16_t)(pUSARTx->RDR & RxMask);
    usart_rx_buff_tail += 2U;
    ubReceive = usart_rx_buff_tail/2;
  }
  else
  {
    pdata8bits  = pRxData;
    pdata16bits = NULL;
    *(pdata8bits + usart_rx_buff_tail) = (uint8_t)(pUSARTx->RDR & (uint8_t)RxMask);
    usart_rx_buff_tail++;
    ubReceive = usart_rx_buff_tail;
  }

  /* as long as data have to be received */
  if(ubReceive == RxSize)
  {
    /* Disable the UART Parity Error Interrupt and RXNE interrupt*/
    pUSARTx->CR1 &= ~(USART_CR1_RXNEIE | USART_CR1_PEIE);

    /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    pUSARTx->CR3 &= ~USART_CR3_EIE;

    /* Rx process is completed, restore huart->RxState to Ready */
    RxState = USART_STATE_READY;

    /* Check that USART RTOEN bit is set */
    if((pUSARTx->CR2 & USART_CR2_RTOEN) != 0U)
    {
      /* Enable the UART Receiver Timeout Interrupt */
      pUSARTx->CR1 &= ~USART_CR1_RTOIE;
    }

    /* Check current reception Mode :
       If Reception till IDLE event has been selected : */
    /* Set reception type to Standard */
    /* Disable IDLE interrupt */
    pUSARTx->CR1 &= ~USART_CR1_IDLEIE;
    if(pUSARTx->ISR & USART_ISR_IDLE)
    {
      /* Clear IDLE Flag */
      pUSARTx->ICR = USART_ICR_IDLECF;
    }

    /*Call legacy weak Rx Event callback*/
    rx_it_flag = 1;
    USART_RxFinishCallback(1, USART_RxFinish);
  }

  return 0;
}

void USART_RxFinishCallback(int value, USART_RxCpltCallback callback)  //需要回调的函数
{
  //UNUSED(value);
  callback(value);
}
__weak void USART_RxFinish(int value)
{
  UNUSED(value);
}

uint8_t Usart_Receive_IT_Get_Flag(void)
{
  return rx_it_flag;
}

void USART_IRQHandler(USART_TypeDef *pUSARTx)
{
  uint32_t isrflags   = pUSARTx->ISR;
  uint32_t cr1its     = pUSARTx->CR1;
  uint32_t cr3its     = pUSARTx->CR3;

  uint32_t errorflags = 0;
  uint32_t errorcode = 0;

  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));

  /* UART in mode Receiver ---------------------------------------------------*/
  if(((isrflags & USART_ISR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
  {
    RxISR(pUSARTx);
    return;
  }

  /* If some errors occur */
  if((errorflags != 0U) && (((cr3its & USART_CR3_EIE) != 0U)||((cr1its & (USART_CR1_RXNEIE|USART_CR1_PEIE|USART_CR1_RTOIE)) != 0U)))
  {
    /* UART parity error interrupt occurred -------------------------------------*/
    if(((isrflags & USART_ISR_PE) != 0U) && ((cr1its & USART_CR1_PEIE) != 0U))
    {
      pUSARTx->ICR = USART_ICR_PECF;
      errorcode |= USART_ICR_PECF;
      /* open for your use */
    }
    /* UART frame error interrupt occurred --------------------------------------*/
    if(((isrflags & USART_ISR_FE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
    {
      pUSARTx->ICR = USART_ICR_FECF;
      errorcode |= USART_ICR_FECF;
      /* open for your use */
    }
    /* UART noise error interrupt occurred --------------------------------------*/
    if(((isrflags & USART_ISR_NE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
    {
      pUSARTx->ICR = USART_ICR_NCF;
      errorcode |= USART_ICR_NCF;
      /* open for your use */
    }
    /* UART Over-Run interrupt occurred -----------------------------------------*/
    if(((isrflags & USART_ISR_ORE) != 0U) && (((cr1its & USART_CR1_RXNEIE) != 0U)||((cr3its & USART_CR3_EIE) != 0U)))
    {
      pUSARTx->ICR = USART_ICR_ORECF;
      errorcode |= USART_ICR_ORECF;
      /* open for your use */
    }
    /* UART Receiver Timeout interrupt occurred ---------------------------------*/
    if(((isrflags & USART_ISR_RTOF) != 0U) && ((cr1its & USART_CR1_RTOIE) != 0U))
    {
      pUSARTx->ICR = USART_ICR_RTOCF;
      errorcode |= USART_ICR_RTOCF;
      /* open for your use */
    }

    /* Call UART Error Call back function if need be ----------------------------*/
    if(errorcode != 0)
    {
      /* If Error is to be considered as blocking :
          - Receiver Timeout error in Reception
          - Overrun error in Reception
          - any error occurs in DMA mode reception
      */
      ErrorCode = errorcode;
      if((pUSARTx->CR3 & USART_CR3_DMAR) || ((errorcode & (USART_ISR_RTOF | USART_ICR_ORECF)) != 0U))
      {
        /* Blocking error : transfer is aborted
           Set the UART state ready to be able to start again the process,
           Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts*/
    	pUSARTx->CR1 &= ~(USART_CR1_IDLEIE | USART_CR1_RXNEIE | USART_CR1_PEIE);
    	pUSARTx->CR3 &= ~USART_CR3_EIE;
    	/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
    	//ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);

    	/* At end of Rx process, restore huart->RxState to Ready */
    	RxState = USART_STATE_READY;
        //huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

        /* Abort the UART DMA Rx channel if enabled */
        if(pUSARTx->CR3 & USART_CR3_DMAR)
        {
          /* Disable the UART DMA Rx request if enabled */
          pUSARTx->CR3 &= ~USART_CR3_DMAR;
          /* Abort the UART DMA Rx channel callback. Add for your use */

        }else{
          /* Call user error callback */
        }
      }else{
        /* Non Blocking error : transfer could go on.
           Error is notified to user through user error callback */
        /*Call legacy weak error callback*/
        //HAL_UART_ErrorCallback(huart);
        ErrorCode = 0;
      }
    }
  } /* End if some error occurs */

  /* Check current reception Mode :
     If Reception till IDLE event has been selected : */
  if(((isrflags & USART_ISR_IDLE) != 0U) && ((cr1its & USART_ISR_IDLE) != 0U))
  {
    pUSARTx->ICR = USART_ICR_IDLECF; //LL_USART_ClearFlag_IDLE(pUSARTx);
    /* Check if DMA mode is enabled in UART */
    if(pUSARTx->CR3 & USART_CR3_DMAR)
    {
      /* DMA mode enabled */
      /* Check received length : If all expected data are received, do nothing,
         (DMA cplt callback will be called).
         Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
      /* open for your use */
    }
    else
    {
      /* DMA mode not enabled */
      /* Check received length : If all expected data are received, do nothing.
         Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
      /* open for your use */
    }
  }

  /* UART wakeup from Stop mode interrupt occurred ---------------------------*/
  if(((isrflags & USART_ISR_WUF) != 0U) && ((cr3its & USART_CR3_WUFIE) != 0U))
  {
    pUSARTx->ICR = USART_ICR_WUCF;
    /* UART Rx state is not reset as a reception process might be ongoing.
       If UART handle state fields need to be reset to READY, this could be done in Wakeup callback */
    /* Call legacy weak Wakeup Callback */
    /* open for your use */
  }

  /* UART in mode Transmitter ------------------------------------------------*/
  if(((isrflags & USART_ISR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
  {
    TxISR(pUSARTx);
    return;
  }

  /* UART in mode Transmitter (transmission end) -----------------------------*/
  if (((isrflags & USART_ISR_TC) != 0U) && ((cr1its & USART_CR1_TCIE) != 0U))
  {
    /* Disable the UART Transmit Complete Interrupt */
    pUSARTx->CR1 = USART_CR1_TCIE;

    /* Tx process is ended, restore huart->gState to Ready */
    TxState = USART_STATE_READY;

    /*Call legacy weak Tx complete callback*/
    //HAL_UART_TxCpltCallback(huart);
  }

}

#if 0
int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  //uint8_t ubSend = 0;
  //uint32_t ubNbDataToTransmit = 0;

  //ubNbDataToTransmit = strlen(pBuffer);
  /* Send characters one per one, until last char to be sent */
  //while (ubSend < ubNbDataToTransmit) // while (ubSend < sizeof(aTxBuffer_init))
  {
	#if (USE_TIMEOUT == 1)
    Timeout = USART_SEND_TIMEOUT_TXE_MS;
	#endif /* USE_TIMEOUT */

    /* Wait for TXE flag to be raised */
    while (!LL_USART_IsActiveFlag_TXE(USART2))
    {
      #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          /* Time-out occurred. Set LED to blinking mode */
          LED_Blinking(LED_BLINK_SLOW);
        }
      }
      #endif /* USE_TIMEOUT */
    }

    /* If last char to be sent, clear TC flag */
    //if (ubSend == (ubNbDataToTransmit - 1)) //if (ubSend == (sizeof(aTxBuffer_init) - 1))
    {
      LL_USART_ClearFlag_TC(USART2);
    }

    /* Write character in Transmit Data register.
    TXE flag is cleared by writing data in TDR register */
    LL_USART_TransmitData8(USART2, (uint8_t)ch); //LL_USART_TransmitData8(USART2, aTxBuffer_init[ubSend++]);
    //ubSend++;
  }

  #if (USE_TIMEOUT == 1)
  Timeout = USART_SEND_TIMEOUT_TC_MS;
  #endif /* USE_TIMEOUT */

	  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(USART2))
  {
    #if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_SLOW);
      }
    }
	#endif /* USE_TIMEOUT */
  }
  return ch;
}
#endif
