/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SDIO_SD_H
#define SDIO_SD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stm32f767xx.h>
#include <stm32f7xx.h>
#include <stdint.h>
#include "stm32f7xx_ll_sdmmc.h"
#include "stm32f7xx_hal_sd.h"

#if 0
typedef enum
{
/**
  * @brief  SDIO specific error defines
  */
  SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
  SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
  SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
  SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
  SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
  SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
  SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
  SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
  SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
  SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
  SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
  SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
  SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
  SD_CC_ERROR                        = (18), /*!< Internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
  SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
  SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
  SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
  SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
  SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
  SD_INVALID_VOLTRANGE               = (27),
  SD_ADDR_OUT_OF_RANGE               = (28),
  SD_SWITCH_ERROR                    = (29),
  SD_SDIO_DISABLED                   = (30),
  SD_SDIO_FUNCTION_BUSY              = (31),
  SD_SDIO_FUNCTION_FAILED            = (32),
  SD_SDIO_UNKNOWN_FUNCTION           = (33),

/**
  * @brief  Standard error defines
  */
  SD_INTERNAL_ERROR,
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING,
  SD_REQUEST_NOT_APPLICABLE,
  SD_INVALID_PARAMETER,
  SD_UNSUPPORTED_FEATURE,
  SD_UNSUPPORTED_HW,
  SD_ERROR,
  SD_OK = 0
} SD_Error;

/**
  * @brief  SDIO Transfer state
  */
typedef enum
{
  SD_TRANSFER_OK  = 0,
  SD_TRANSFER_BUSY = 1,
  SD_TRANSFER_ERROR
} SDTransferState;

/**
  * @brief  SD Card States
  */
typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;
#endif

/**
  * @brief  Card Specific Data: CSD Register
  */
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/**
  * @brief SD Card Status
  */
typedef struct
{
  __IO uint8_t DAT_BUS_WIDTH;
  __IO uint8_t SECURED_MODE;
  __IO uint16_t SD_CARD_TYPE;
  __IO uint32_t SIZE_OF_PROTECTED_AREA;
  __IO uint8_t SPEED_CLASS;
  __IO uint8_t PERFORMANCE_MOVE;
  __IO uint8_t AU_SIZE;
  __IO uint16_t ERASE_SIZE;
  __IO uint8_t ERASE_TIMEOUT;
  __IO uint8_t ERASE_OFFSET;
} SD_CardStatus;


/**
  * @brief SD Card information
  */
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint64_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
} SD_CardInfo;

/**
  * @}
  */


//Exported Variables
extern SD_CardInfo SDCardInfo;

/** @defgroup STM32F4_DISCOVERY_SDIO_SD_Exported_Constants
  * @{
  */

/* Uncomment the following line to select the SDIO Data transfer mode */
#if !defined (SD_DMA_MODE) && !defined (SD_POLLING_MODE)
#define SD_DMA_MODE                                ((uint32_t)0x00000000)
/*#define SD_POLLING_MODE                            ((uint32_t)0x00000002)*/
#endif

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)

/**
  * @brief Supported SD Memory Cards
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

/**
  * @}
  */
/**
  * @brief  SD FLASH SDIO Interface
  */
#define SD_DETECT_PIN                    GPIO_Pin_2                 /* PG.02 */
#define SD_DETECT_GPIO_PORT              GPIOG                      /* GPIOG */
#define SD_DETECT_GPIO_CLK               RCC_AHB1Periph_GPIOG

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)
/**
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
/**
  * @brief  SDIO Data Transfer Frequency (25MHz max)
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x2)

#define SD_SDIO_DMA                  	 DMA2
#define SD_SDIO_DMA_CLK              	RCC_AHB1Periph_DMA2

#define SD_SDIO_DMA_STREAM3               3
//#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
 #define SD_SDIO_DMA_STREAM           	DMA2_Stream3_BASE //DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL          	LL_DMA_CHANNEL_4 //DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         	DMA_LISR_FEIF3 //DMA_FLAG_FEIF3 数据流FIFO错误中断标志
 #define SD_SDIO_DMA_FLAG_DMEIF        	DMA_LISR_DMEIF3 //DMA_FLAG_DMEIF3 数据流直接模式错误标志
 #define SD_SDIO_DMA_FLAG_TEIF         	DMA_LISR_TEIF3 //DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         	DMA_LISR_HTIF3 //DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         	DMA_LISR_TCIF3 //DMA_FLAG_TCIF3
 #define SD_SDIO_DMA_IRQn              	DMA2_Stream3_IRQn
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM            	DMA2_Stream6_BASE //DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           	LL_DMA_CHANNEL_4 //DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         	DMA_HISR_FEIF6 //DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        	DMA_HISR_DMEIF6 //DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         	DMA_HISR_TEIF6 //DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         	DMA_HISR_HTIF6 //DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         	DMA_HISR_TCIF6 //DMA_FLAG_TCIF6
 #define SD_SDIO_DMA_IRQn              	DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        	DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */
/** @defgroup STM32F4_DISCOVERY_SDIO_SD_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_SDIO_SD_Exported_Functions
  * @{
  */
void SD_DeInit(void);
HAL_SD_ErrorTypedef SD_Init(void);

HAL_SD_TransferStateTypedef SD_GetStatus(void); //SDTransferState
HAL_SD_CardStateTypedef SD_GetState(void); //SDCardState

uint8_t SD_Detect(void);
HAL_SD_ErrorTypedef SD_PowerON(void);
HAL_SD_ErrorTypedef SD_PowerOFF(void);
HAL_SD_ErrorTypedef SD_InitializeCards(void);
HAL_SD_ErrorTypedef SD_GetCardInfo(SD_CardInfo *cardinfo);
HAL_SD_ErrorTypedef SD_GetCardStatus(SD_CardStatus *cardstatus);
HAL_SD_ErrorTypedef SD_EnableWideBusOperation(uint32_t WideMode);
HAL_SD_ErrorTypedef SD_SelectDeselect(uint32_t addr);
HAL_SD_ErrorTypedef SD_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize);
HAL_SD_ErrorTypedef SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
HAL_SD_ErrorTypedef SD_ReadMultiBlocksFIXED(uint8_t *readbuff, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumberOfBlocks);
HAL_SD_ErrorTypedef SD_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);
HAL_SD_ErrorTypedef SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
HAL_SD_ErrorTypedef SD_WriteMultiBlocksFIXED(uint8_t *writebuff, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumberOfBlocks);
HAL_SD_TransferStateTypedef SD_GetTransferState(void);
HAL_SD_ErrorTypedef SD_SetDeviceMode(uint32_t Mode);
HAL_SD_ErrorTypedef SD_StopTransfer(void);
HAL_SD_ErrorTypedef SD_Erase(uint32_t startaddr, uint32_t endaddr);
HAL_SD_ErrorTypedef SD_SendStatus(uint32_t *pcardstatus);
HAL_SD_ErrorTypedef SD_SendSDStatus(uint32_t *psdstatus);
HAL_SD_ErrorTypedef SD_ProcessIRQSrc(void);
void SD_ProcessDMAIRQ(void);
HAL_SD_ErrorTypedef SD_WaitReadOperation(void);
HAL_SD_ErrorTypedef SD_WaitWriteOperation(void);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
void SDMMC1_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
