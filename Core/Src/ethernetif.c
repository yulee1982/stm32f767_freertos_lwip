/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c
  * @author  MCD Application Team
  * @brief   This file implements Ethernet network interface drivers for lwIP
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"
#include "lan8742.h"
#include <string.h>
#include "lwip/netifapi.h"
#include "lwip/pbuf.h"

#include "FreeRTOS.h"
#include "task.h"
//#include "timers.h"
#include "queue.h"
#include "semphr.h"
//#include "event_groups.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                 (0xFFFFFFFF)  /// ( osWaitForever ) //< wait forever timeout value
/* Time to block waiting for transmissions to finish */
#define ETHIF_TX_TIMEOUT                       (2000U)
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE            ( 512 )

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define ETH_DMA_TRANSMIT_TIMEOUT                (20U)

#define ETH_RX_BUFFER_CNT           10     /* This app buffers receive packets of its primary service
                                               * protocol for processing later. */
#define ETH_TX_BUFFER_MAX             ((ETH_TX_DESC_CNT) * 2) /* HAL_ETH_Transmit(_IT) may attach two
                                               * buffers per descriptor. */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*
@Note: This interface is implemented to operate in zero-copy mode only:
        - Rx Buffers will be allocated from LwIP stack Rx memory pool,
          then passed to ETH HAL driver.
        - Tx Buffers will be allocated from LwIP stack memory heap,
          then passed to ETH HAL driver.

@Notes:
  1.a. ETH DMA Rx descriptors must be contiguous, the default count is 4,
       to customize it please redefine ETH_RX_DESC_CNT in ETH GUI (Rx Descriptor Length)
       so that updated value will be generated in stm32xxxx_hal_conf.h
  1.b. ETH DMA Tx descriptors must be contiguous, the default count is 4,
       to customize it please redefine ETH_TX_DESC_CNT in ETH GUI (Tx Descriptor Length)
       so that updated value will be generated in stm32xxxx_hal_conf.h

  2.a. Rx Buffers number: ETH_RX_BUFFER_CNT must be greater than ETH_RX_DESC_CNT.
  2.b. Rx Buffers must have the same size: ETH_RX_BUF_SIZE, this value must
       passed to ETH DMA in the init field (heth.Init.RxBuffLen)
*/
typedef enum
{
  RX_ALLOC_OK       = 0x00,
  RX_ALLOC_ERROR    = 0x01
} RxAllocStatusTypeDef;

typedef struct
{
  struct pbuf_custom pbuf_custom;
  uint8_t buff[(ETH_RX_BUF_SIZE + 31) & ~31];
} RxBuff_t;

// #elif defined ( __GNUC__ ) /* GNU Compiler */
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDescripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDescripSection")));   /* Ethernet Tx DMA Descriptors */
//uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
//uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

LWIP_MEMPOOL_DECLARE(RX_POOL, ETH_RX_BUFFER_CNT, sizeof(RxBuff_t), "Zero-copy RX PBUF pool");

static uint8_t RxAllocStatus;

QueueHandle_t RxPktSemaphore = NULL; /* Semaphore to signal incoming packets  --osSemaphoreId ++QueueHandle_t*/

TaskHandle_t EthIfThread;       /* Handle of the interface thread */
QueueHandle_t TxPktSemaphore = NULL;   /* Semaphore to signal transmit packet complete  --osSemaphoreId ++QueueHandle_t*/

/* Global Ethernet handle */
ETH_HandleTypeDef EthHandle;
ETH_TxPacketConfig TxConfig;
lan8742_Object_t LAN8742;

/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'


struct ethernetif {
	struct eth_addr *ethaddr;
/* Add whatever per-interface state that is needed here. */
};
/* Global Ethernet handle */
ETH_HandleTypeDef heth;
//static void arp_timer(void *arg);

const uint8_t PHYLinkStateStr[11][32] =
{
  "read err !!!\r\n",
  "write err !!!\r\n",
  "address err !!!\r\n",
  "err !!!\r\n",
  "OK !\r\n",
  "LINK down !\r\n",
  "100MBits full duplex !\r\n",
  "100MBits half duplex !\r\n",
  "10MBits full duplex !\r\n",
  "10MBits half duplex !\r\n",
  "autonego notdone !\r\n",
};
/* Private function prototypes -----------------------------------------------*/
static void ethernetif_input( void * argument );
static void RMII_Thread( void * argument );
int32_t ETH_PHY_IO_Init(void);
int32_t ETH_PHY_IO_DeInit (void);
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
int32_t ETH_PHY_IO_GetTick(void);
void pbuf_free_custom(struct pbuf *p);

lan8742_IOCtx_t  LAN8742_IOCtx = {ETH_PHY_IO_Init,
                               ETH_PHY_IO_DeInit,
                               ETH_PHY_IO_WriteReg,
                               ETH_PHY_IO_ReadReg,
                               ETH_PHY_IO_GetTick};

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{
  uint32_t duplex, speed = 0;
  int32_t PHYLinkState = 0;
  ETH_MACConfigTypeDef MACConf = {0};
  uint8_t macaddress[6]= {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
  int32_t PHYInit_result = 0;

  EthHandle.Instance = ETH;
  EthHandle.Init.MACAddr = macaddress;
  EthHandle.Init.MediaInterface = HAL_ETH_RMII_MODE;
  EthHandle.Init.RxDesc = DMARxDscrTab;
  EthHandle.Init.TxDesc = DMATxDscrTab;
  EthHandle.Init.RxBuffLen = ETH_RX_BUF_SIZE;

  /* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
  HAL_ETH_Init(&EthHandle);

  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;

  /* maximum transfer unit */
  netif->mtu = ETH_MAX_PAYLOAD;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  /* Initialize the RX POOL */
  LWIP_MEMPOOL_INIT(RX_POOL);

  /* Set Tx packet config common parameters */
  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

  /* create a binary semaphore used for informing ethernetif of frame reception */
  RxPktSemaphore = xSemaphoreCreateBinary();

  /* create a binary semaphore used for informing ethernetif of frame transmission */
  TxPktSemaphore = xSemaphoreCreateBinary();

  /* create the task that handles the ETH_MAC */
  xTaskCreate(ethernetif_input,		/* Function that implements the task. */
              "EthIf", 			    /* Task name, for debugging only. */
			  INTERFACE_THREAD_STACK_SIZE,   /* Size of stack (in words) to allocate for this task. */
			  netif, 				/* Task parameter, not used in this case. */
			  3, 		            /* Task priority. */
			  NULL );		        /* Task handle, used to unblock task from interrupt. */

  /* Set PHY IO functions */
  LAN8742_RegisterBusIO(&LAN8742, &LAN8742_IOCtx);

  /* Initialize the LAN8742 ETH PHY */
  PHYInit_result = LAN8742_Init(&LAN8742);
  //if(LAN8742_Init(&LAN8742) != LAN8742_STATUS_OK)
  if(PHYInit_result != LAN8742_STATUS_OK)
  {
    netif_set_link_down(netif);
    netif_set_down(netif);
    //printf("STM32F7xx LAN8742 init status != OK !!! status = 0x%08X \r\n", PHYInit_result);
    printf("STM32F7xx LAN8742 init status != OK ! ");
    printf(PHYLinkStateStr[(uint16_t)(PHYInit_result + 5)]);
    return;
  }

  PHYLinkState = LAN8742_GetLinkState(&LAN8742);

  /* Get link state */
  if(PHYLinkState <= LAN8742_STATUS_LINK_DOWN)
  {
    netif_set_link_down(netif);
    netif_set_down(netif);
  }
  else
  {
    switch (PHYLinkState)
    {
    case LAN8742_STATUS_100MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      break;
    case LAN8742_STATUS_100MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      break;
    case LAN8742_STATUS_10MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      break;
    case LAN8742_STATUS_10MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      break;
    default:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      break;
    }

    /* Get MAC Config MAC */
    HAL_ETH_GetMACConfig(&EthHandle, &MACConf);
    MACConf.DuplexMode = duplex;
    MACConf.Speed = speed;
    HAL_ETH_SetMACConfig(&EthHandle, &MACConf);
    HAL_ETH_Start_IT(&EthHandle);
    netif_set_up(netif);
    netif_set_link_up(netif);
  }

  if(HAL_GetREVID() == 0x1000)
  { 
    /* 
      This thread will keep resetting the RMII interface until good frames are received
    */
    //osThreadDef(RMII_Watchdog, RMII_Thread, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE);
    //osThreadCreate (osThread(RMII_Watchdog), NULL);
    xTaskCreate(RMII_Thread, 		/* Function that implements the task. */
                "RMII_Watchdog", 	/* Task name, for debugging only. */
  			    128,                /* Size of stack (in words) to allocate for this task. */
                NULL, 				/* Task parameter, not used in this case. */
  			    3, 		            /* Task priority. */
  			    NULL );		        /* Task handle, used to unblock task from interrupt. */
  }

}


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet was sent, or ERR_IF if the packet was unable to be sent
 *
 * @note ERR_OK means the packet was sent (but not necessarily transmit complete),
 * and ERR_IF means the packet has more chained buffers than what the interface supports.
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  uint32_t i = 0U;
  struct pbuf *q = NULL;
  err_t errval = ERR_OK;
  ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];

  memset(Txbuffer, 0 , ETH_TX_DESC_CNT*sizeof(ETH_BufferTypeDef));

  for(q = p; q != NULL; q = q->next)
  {
    if(i >= ETH_TX_DESC_CNT)
      return ERR_IF;

    Txbuffer[i].buffer = q->payload;
    Txbuffer[i].len = q->len;

    if(i>0)
    {
      Txbuffer[i-1].next = &Txbuffer[i];
    }

    if(q->next == NULL)
    {
      Txbuffer[i].next = NULL;
    }

    i++;
  }

  TxConfig.Length = p->tot_len;
  TxConfig.TxBuffer = Txbuffer;
  TxConfig.pData = p;

  pbuf_ref(p);
  HAL_ETH_Transmit_IT(&EthHandle, &TxConfig);
  while (xSemaphoreTake(TxPktSemaphore, portMAX_DELAY) != pdTRUE)
  {
  }
  HAL_ETH_ReleaseTxPacket(&EthHandle);

  return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p = NULL;

  if(RxAllocStatus == RX_ALLOC_OK)
  {
    HAL_ETH_ReadData(&EthHandle, (void **)&p);
  }

  return p;
}

/**
 * This task should be signaled when a receive packet is ready to be read
 * from the interface.
 *
 * @param argument the lwip network interface structure for this ethernetif
 */
static void ethernetif_input( void * argument )
{
  struct pbuf *p = NULL;
  struct netif *netif = (struct netif *) argument;

  for( ;; )
  {
    //if (osSemaphoreWait( RxPktSemaphore, TIME_WAITING_FOR_INPUT)==osOK)
    if(xSemaphoreTake( RxPktSemaphore, TIME_WAITING_FOR_INPUT ) == pdTRUE)
    {
      do
      {
        p = low_level_input( netif );
        if (p != NULL)
        {
          if (netif->input( p, netif) != ERR_OK )
          {
            pbuf_free(p);
          }
        }

      }while(p!=NULL);
    }
  }
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);// --LINK_SPEED_OF_YOUR_NETIF_IN_BPS ++ETH_SPEED_100M

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
  * @brief  Custom Rx pbuf free callback
  * @param  pbuf: pbuf to be freed
  * @retval None
  */
void pbuf_free_custom(struct pbuf *p)
{
  struct pbuf_custom* custom_pbuf = (struct pbuf_custom*)p;
  LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);

  if (RxAllocStatus == RX_ALLOC_ERROR)
  {
    RxAllocStatus = RX_ALLOC_OK;
    //osSemaphoreRelease(RxPktSemaphore);
    xSemaphoreGive(RxPktSemaphore);
  }
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Current Time value
  */
//u32_t sys_now(void)
//{
//  return HAL_GetTick();
//}

/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: ETH handle
  * @retval None
*/
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  /* Enable GPIOs clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

/* Ethernet pins configuration ************************************************/
  /*
  RMII_REF_CLK ----------------------> PA1
  RMII_MDIO -------------------------> PA2
  RMII_MDC --------------------------> PC1
  RMII_MII_CRS_DV -------------------> PA7
  RMII_MII_RXD0 ---------------------> PC4
  RMII_MII_RXD1 ---------------------> PC5
  RMII_MII_TX_EN --------------------> PG11
  RMII_MII_TXD0 ---------------------> PG13
  RMII_MII_TXD1 ---------------------> PB13
  */

  /* Configure PA1, PA2 and PA7 */
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PB13 */
  GPIO_InitStructure.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PC1, PC4 and PC5 */
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PG11 and PG13 */
  GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable the Ethernet global Interrupt */
  HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
  HAL_NVIC_EnableIRQ(ETH_IRQn);

  /* Enable ETHERNET clock  */
  __HAL_RCC_ETH_CLK_ENABLE();
}
#if 0
void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)   //HAL库 HAL_ETH_Init() 回调函数
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ETHMAC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ETHMACTX);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ETHMACRX);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /**ETH GPIO Configuration
  PC1     ------> ETH_MDC      RMII_MDC_Pin
  PA1     ------> ETH_REF_CLK  RMII_REF_CLK_Pin
  PA2     ------> ETH_MDIO     RMII_MDIO_Pin
  PA7     ------> ETH_CRS_DV   RMII_CRS_DV_Pin
  PC4     ------> ETH_RXD0     RMII_RXD0_Pin
  PC5     ------> ETH_RXD1     RMII_RXD1_Pin
  PG11    ------> ETH_TX_EN    RMII_TX_EN_Pin  --PB11 ++PG11
  PG13    ------> ETH_TXD0     RMII_TXD0_Pin
  PB13    ------> ETH_TXD1     RMII_TXD1_Pin   --PG14 ++PB13
  */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;  // 备用功能模式
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // 推挽输出模式
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;    // 无上下拉
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ETH_MspInit 1 */
  /* Enable the Ethernet global Interrupt */
  HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
  HAL_NVIC_EnableIRQ(ETH_IRQn);

 }
#endif
/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
  //osSemaphoreRelease(RxPktSemaphore);
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(RxPktSemaphore, &xHigherPriorityTaskWoken);
  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/**
  * @brief  Ethernet Tx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
  //osSemaphoreRelease(TxPktSemaphore);
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(TxPktSemaphore, &xHigherPriorityTaskWoken);
  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/**
  * @brief  Ethernet DMA transfer error callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
  if ((HAL_ETH_GetDMAError(heth) & ETH_DMASR_RBUS) == ETH_DMASR_RBUS)
  {
    //osSemaphoreRelease(RxPktSemaphore);  // 释放信号量
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(RxPktSemaphore, &xHigherPriorityTaskWoken); // 释放一个信号量，带中断保护
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);  // 使能PendSV中断
  }
}

/*******************************************************************************
                       PHI IO Functions
*******************************************************************************/
/**
  * @brief  Initializes the MDIO interface GPIO and clocks.
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_Init(void)
{
  /* We assume that MDIO GPIO configuration is already done
     in the ETH_MspInit() else it should be done here
  */

  /* Configure the MDIO Clock */
  HAL_ETH_SetMDIOClockRange(&EthHandle);

  return 0;
}

/**
  * @brief  De-Initializes the MDIO interface .
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_DeInit (void)
{
  return 0;
}

/**
  * @brief  Read a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  pRegVal: pointer to hold the register value
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
  if(HAL_ETH_ReadPHYRegister(&EthHandle, DevAddr, RegAddr, pRegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Write a value to a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  RegVal: Value to be written
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
  if(HAL_ETH_WritePHYRegister(&EthHandle, DevAddr, RegAddr, RegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Get the time in millisecons used for internal PHY driver process.
  * @retval Time value
  */
int32_t ETH_PHY_IO_GetTick(void)
{
  return HAL_GetTick();
}

/**
  * @brief  Check the ETH link state and update netif accordingly.
  * @param  argument: netif
  * @retval None
  */

void ethernet_link_thread( void * argument )
{
  ETH_MACConfigTypeDef MACConf = {0};
  int32_t PHYLinkState = 0;
  uint32_t linkchanged = 0, speed = 0, duplex =0;
  int32_t PHYLinkStatetemp = 0;

  struct netif *netif = (struct netif *) argument;

  for(;;)
  {

    PHYLinkState = LAN8742_GetLinkState(&LAN8742);
    if(PHYLinkState != PHYLinkStatetemp)
    {
      printf("STM32F7xx PHYLink State : ");
      printf(PHYLinkStateStr[(uint16_t)(PHYLinkState + 5)]);
      PHYLinkStatetemp = PHYLinkState;
    }

    if(netif_is_link_up(netif) && (PHYLinkState <= LAN8742_STATUS_LINK_DOWN))
    {
      HAL_ETH_Stop_IT(&EthHandle);
      netifapi_netif_set_down(netif);
      netifapi_netif_set_link_down(netif);
      printf("ETH and netif down!!!\r\n");
    }
    else if(!netif_is_link_up(netif) && (PHYLinkState > LAN8742_STATUS_LINK_DOWN))
    {
      switch (PHYLinkState)
      {
      case LAN8742_STATUS_100MBITS_FULLDUPLEX:
        duplex = ETH_FULLDUPLEX_MODE;
        speed = ETH_SPEED_100M;
        linkchanged = 1;
        break;
      case LAN8742_STATUS_100MBITS_HALFDUPLEX:
        duplex = ETH_HALFDUPLEX_MODE;
        speed = ETH_SPEED_100M;
        linkchanged = 1;
        break;
      case LAN8742_STATUS_10MBITS_FULLDUPLEX:
        duplex = ETH_FULLDUPLEX_MODE;
        speed = ETH_SPEED_10M;
        linkchanged = 1;
        break;
      case LAN8742_STATUS_10MBITS_HALFDUPLEX:
        duplex = ETH_HALFDUPLEX_MODE;
        speed = ETH_SPEED_10M;
        linkchanged = 1;
        break;
      default:
        break;
      }

      if(linkchanged)
      {
        /* Get MAC Config MAC */
        HAL_ETH_GetMACConfig(&EthHandle, &MACConf);
        MACConf.DuplexMode = duplex;
        MACConf.Speed = speed;
        HAL_ETH_SetMACConfig(&EthHandle, &MACConf);
        HAL_ETH_Start_IT(&EthHandle);
        netifapi_netif_set_up(netif);
        netifapi_netif_set_link_up(netif);
      }
    }

    vTaskDelay(100);
  }
}

void HAL_ETH_RxAllocateCallback(uint8_t **buff)
{
  struct pbuf_custom *p = LWIP_MEMPOOL_ALLOC(RX_POOL);
  if (p)
  {
    /* Get the buff from the struct pbuf address. */
    *buff = (uint8_t *)p + offsetof(RxBuff_t, buff);
    p->custom_free_function = pbuf_free_custom;
    /* Initialize the struct pbuf.
    * This must be performed whenever a buffer's allocated because it may be
    * changed by lwIP or the app, e.g., pbuf_free decrements ref. */
    pbuf_alloced_custom(PBUF_RAW, 0, PBUF_REF, p, *buff, ETH_RX_BUF_SIZE);
  }
  else
  {
    RxAllocStatus = RX_ALLOC_ERROR;
    *buff = NULL;
  }
}

void HAL_ETH_RxLinkCallback(void **pStart, void **pEnd, uint8_t *buff, uint16_t Length)
{
  struct pbuf **ppStart = (struct pbuf **)pStart;
  struct pbuf **ppEnd = (struct pbuf **)pEnd;
  struct pbuf *p = NULL;

  /* Get the struct pbuf from the buff address. */
  p = (struct pbuf *)(buff - offsetof(RxBuff_t, buff));
  p->next = NULL;
  p->tot_len = 0;
  p->len = Length;

  /* Chain the buffer. */
  if (!*ppStart)
  {
    /* The first buffer of the packet. */
    *ppStart = p;
  }
  else
  {
    /* Chain the buffer to the end of the packet. */
    (*ppEnd)->next = p;
  }
  *ppEnd  = p;

  /* Update the total length of all the buffers of the chain. Each pbuf in the chain should have its tot_len
   * set to its own length, plus the length of all the following pbufs in the chain. */
  for (p = *ppStart; p != NULL; p = p->next)
  {
    p->tot_len += Length;
  }
}

void HAL_ETH_TxFreeCallback(uint32_t * buff)
{
  pbuf_free((struct pbuf *)buff);
}

/**
  * @brief  RMII interface watchdog thread (解决上电时可能会接受到错误,这是芯片BUG修复.)
  * @param  argument
  * @retval None
  */
void RMII_Thread( void * argument )
{
  (void) argument; 
  
  for(;;)
  {
    /* some unicast good packets are received */
    if(EthHandle.Instance->MMCRGUFCR > 0U)
    {
      /* RMII Init is OK: Delete the Thread */ 
      vTaskDelete(NULL);
    }    
    else if(EthHandle.Instance->MMCRFCECR > 10U) 
    {
      /* ETH received too many packets with CRC errors, resetting RMII */
      SYSCFG->PMC &= ~SYSCFG_PMC_MII_RMII_SEL;
      SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;
    
      EthHandle.Instance->MMCCR |= ETH_MMCCR_CR;
    }
    else
    {
      /* Delay 200 ms */
      vTaskDelay(200);
    }
  }
}

/*
static void arp_timer(void *arg)
{
  etharp_tmr();
  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}
*/
