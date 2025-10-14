

#ifndef __USART_H__
#define __USART_H__

#include "main.h"

#define USART_OK       0
#define USART_ERROR    1
#define USART_BUSY     2
#define USART_TIMEOUT  3

/** @defgroup UART_State_Definition UART State Code Definition
  * @{
  */
#define  USART_STATE_RESET       0x00000000U    /*!< Peripheral is not initialized. Value is allowed for TxState and RxState */
#define  USART_STATE_READY       0x00000020U    /*!< Peripheral Initialized and ready for use. Value is allowed for TxState and RxState */
#define  USART_STATE_BUSY        0x00000024U    /*!< an internal process is ongoing. Value is allowed for TxState only */
#define  USART_STATE_BUSY_TX     0x00000021U    /*!< Data Transmission process is ongoing. Value is allowed for TxState only */
#define  USART_STATE_BUSY_RX     0x00000022U    /*!< Data Reception process is ongoing. Value is allowed for RxState only */
#define  USART_STATE_BUSY_TX_RX  0x00000023U    /*!< Data Transmission and Reception process is ongoing
                                                          Not to be used for neither TxState nor RxState.Value is result
                                                          of combination (Or) between TxState and RxState values */
#define  USART_STATE_TIMEOUT     0x000000A0U    /*!< Timeout state. Value is allowed for TxState only */
#define  USART_STATE_ERROR       0x000000E0U    /*!< Error. Value is allowed for TxState only */

void MX_USART2_UART_Init(void);
void MX_USART2_DMA(void);
uint8_t Usart_Transmit(USART_TypeDef *pUSARTx, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t Usart_Transmit_DMA(USART_TypeDef *pUSARTx, const uint8_t *pData, uint16_t Size);
uint8_t DMA_Usart2_Start_Transfers_IT(uint32_t SrcAddress, uint32_t DataLength);

uint8_t Usart_Receive(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t Usart_Receive_IT(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size);
uint8_t Usart_Receive_IT_Get_Flag(void);
uint8_t Usart_Start_Receive_IT(USART_TypeDef *pUSARTx, uint8_t *pData, uint16_t Size);

void USART_IRQHandler(USART_TypeDef *pUSARTx);

typedef void (*USART_RxCpltCallback)(int);              //声明回调函数指针 定义回调函数类型
//USART_RxCpltCallback USART_RxCallback = NULL;         //定义回调函数指针
void USART_RxFinishCallback(int value, USART_RxCpltCallback callback);  //需要回调的函数
void USART_RxFinish(int value);	                        //实际处理函数(回调函数)
#endif
