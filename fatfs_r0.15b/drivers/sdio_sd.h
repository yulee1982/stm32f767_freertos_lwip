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
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_sdmmc.h"
#include "stm32f7xx_hal_sd.h"
#include "main.h"

#include "diskio.h"
#include "ff.h"

#ifndef FATFS_USE_DETECT_PIN
#define FATFS_USE_DETECT_PIN				1
#endif

#ifndef FATFS_USE_WRITEPROTECT_PIN
#define FATFS_USE_WRITEPROTECT_PIN			0
#endif

#ifndef FATFS_SDIO_4BIT
#define FATFS_SDIO_4BIT						1
#endif

#if FATFS_USE_DETECT_PIN > 0
#ifndef FATFS_USE_DETECT_PIN_PIN
#define FATFS_USE_DETECT_PIN_PORT			GPIOG
#define FATFS_USE_DETECT_PIN_PIN			GPIO_PIN_2
#endif
#endif

#if FATFS_USE_WRITEPROTECT_PIN > 0
#ifndef FATFS_USE_WRITEPROTECT_PIN_PIN
#define FATFS_USE_WRITEPROTECT_PIN_PORT		GPIOB
#define FATFS_USE_WRITEPROTECT_PIN_PIN		GPIO_PIN_7
#endif
#endif

#define SDIO_USE_HAL_LIB                   0

#if SDIO_USE_HAL_LIB

extern SD_HandleTypeDef hsd;
HAL_SD_ErrorTypedef SD_Init(void);

DSTATUS TM_FATFS_SD_SDIO_disk_initialize(void);
uint8_t TM_FATFS_CheckCardDetectPin(void);
DSTATUS TM_FATFS_SD_SDIO_disk_status(void);
DRESULT TM_FATFS_SD_SDIO_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_SDIO_disk_write(const BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_SDIO_disk_ioctl(BYTE cmd, void *buff);

#else

typedef enum
{
  /**
  * @brief  SDIO specific error defines
  */
  SDMMC_CMD_CRC_FAIL = (1), /*!< Command response received (but CRC check failed) */
  SDMMC_DATA_CRC_FAIL = (2), /*!< Data bock sent/received (CRC check Failed) */
  SDMMC_CMD_RSP_TIMEOUT = (3), /*!< Command response timeout */
  SDMMC_DATA_TIMEOUT = (4), /*!< Data time out */
  SDMMC_TX_UNDERRUN = (5), /*!< Transmit FIFO under-run */
  SDMMC_RX_OVERRUN = (6), /*!< Receive FIFO over-run */
  SDMMC_START_BIT_ERR = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  SDMMC_CMD_OUT_OF_RANGE = (8), /*!< CMD's argument was out of range.*/
  SDMMC_ADDR_MISALIGNED = (9), /*!< Misaligned address */
  SDMMC_BLOCK_LEN_ERR = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SDMMC_ERASE_SEQ_ERR = (11), /*!< An error in the sequence of erase command occurs.*/
  SDMMC_BAD_ERASE_PARAM = (12), /*!< An Invalid selection for erase groups */
  SDMMC_WRITE_PROT_VIOLATION = (13), /*!< Attempt to program a write protect block */
  SDMMC_LOCK_UNLOCK_FAILED = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SDMMC_COM_CRC_FAILED = (15), /*!< CRC check of the previous command failed */
  SDMMC_ILLEGAL_CMD = (16), /*!< Command is not legal for the card state */
  SDMMC_CARD_ECC_FAILED = (17), /*!< Card internal ECC was applied but failed to correct the data */
  SDMMC_CC_ERROR = (18), /*!< Internal card controller error */
  SDMMC_GENERAL_UNKNOWN_ERROR = (19), /*!< General or Unknown error */
  SDMMC_STREAM_READ_UNDERRUN = (20), /*!< The card could not sustain data transfer in stream read operation. */
  SDMMC_STREAM_WRITE_OVERRUN = (21), /*!< The card could not sustain data programming in stream mode */
  SDMMC_CID_CSD_OVERWRITE = (22), /*!< CID/CSD overwrite error */
  SDMMC_WP_ERASE_SKIP = (23), /*!< only partial address space was erased */
  SDMMC_CARD_ECC_DISABLED = (24), /*!< Command has been executed without using internal ECC */
  SDMMC_ERASE_RESET = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SDMMC_AKE_SEQ_ERROR = (26), /*!< Error in sequence of authentication. */
  SDMMC_INVALID_VOLTRANGE = (27),
  SDMMC_ADDR_OUT_OF_RANGE = (28),
  SDMMC_SWITCH_ERROR = (29),
  SDMMC_SDIO_DISABLED = (30),
  SDMMC_SDIO_FUNCTION_BUSY = (31),
  SDMMC_SDIO_FUNCTION_FAILED = (32),
  SDMMC_SDIO_UNKNOWN_FUNCTION = (33),

  /**
  * @brief  Standard error defines
  */
  SDMMC_INTERNAL_ERROR,
  SDMMC_NOT_CONFIGURED,
  SDMMC_REQUEST_PENDING,
  SDMMC_REQUEST_NOT_APPLICABLE,
  SDMMC_INVALID_PARAMETER,
  SDMMC_UNSUPPORTED_FEATURE,
  SDMMC_UNSUPPORTED_HW,
  SDMMC_ERROR,
  SDMMC_OK = 0
} SD_Error;


/**
  * @brief  SDIO Transfer state
  */
typedef enum
{
  SDMMC_TRANSFER_OK  = 0,
  SDMMC_TRANSFER_BUSY = 1,
  SDMMC_TRANSFER_ERROR
} SDTransferState;

/**
  * @brief  SD Card States
  */
typedef enum
{
  SDMMC_CARD_READY                  = ((uint32_t)0x00000001),
  SDMMC_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SDMMC_CARD_STANDBY                = ((uint32_t)0x00000003),
  SDMMC_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SDMMC_CARD_SENDING                = ((uint32_t)0x00000005),
  SDMMC_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SDMMC_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SDMMC_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SDMMC_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;


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
#define SD_DMA_MODE                         ((uint32_t)0x00000000)
/*#define SD_POLLING_MODE                   ((uint32_t)0x00000002)*/
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

extern void SD_LowLevel_DeInit (void);
extern void SD_LowLevel_Init (void);
extern void SD_LowLevel_DMA_TxConfig (uint32_t *BufferSRC, uint32_t BufferSize);
extern void SD_LowLevel_DMA_RxConfig (uint32_t *BufferDST, uint32_t BufferSize);
extern uint8_t IOE16_MonitorIOPin (uint16_t IO_Pin);
extern void SD_DeInit(void);
extern SD_Error SD_Init(void);
extern SDTransferState SD_GetStatus (void);
extern SDCardState SD_GetState (void);
extern uint8_t SD_Detect(void);
extern SD_Error SD_PowerON(void);
extern SD_Error SD_PowerOFF(void);
extern SD_Error SD_InitializeCards(void);
extern SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
extern SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus);
extern SD_Error SD_EnableWideBusOperation(uint32_t WideMode);
extern SD_Error SD_SelectDeselect(uint64_t addr);
extern SD_Error SD_ReadBlock (uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize);
extern SD_Error SD_ReadMultiBlocks (uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern SD_Error SD_ReadMultiBlocksFIXED (uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern SD_Error SD_WriteBlock (uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize);
extern SD_Error SD_WriteMultiBlocks (uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern SD_Error SD_WriteMultiBlocksFIXED(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern SDTransferState SD_GetTransferState (void);
extern SD_Error SD_StopTransfer(void);
extern SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr);
extern SD_Error SD_SendStatus(uint32_t *pcardstatus);
extern SD_Error SD_SendSDStatus(uint32_t *psdstatus);
extern SD_Error SD_ProcessIRQSrc(void);
extern void SD_ProcessDMAIRQ(void);
extern SD_Error SD_WaitReadOperation(void);
extern SD_Error SD_WaitWriteOperation(void);

extern SD_Error SD_HighSpeed(void);
extern void SD_LowLevel_DeInit(void);
extern void SD_LowLevel_Init(void);
extern void SD_LowLevel_DMA_TxConfig (uint32_t *BufferSRC, uint32_t BufferSize);
extern void SD_LowLevel_DMA_RxConfig (uint32_t *BufferDST, uint32_t BufferSize);
extern void SDMMC1_IRQHandler(void);

/**
 * @brief  Checks card detect pin (if activated) if card is inserted
 * @note   Pin must be set low in order to get card inserted, otherwise card is not inserted
 * @note   Card detect pin must be activated in order to get this functionality to work
 * @param  None
 * @retval Card detected status:
 *            - 0: Card is not inserted
 *            - > 0: Card is inserted
 */

DSTATUS TM_FATFS_SD_SDIO_disk_initialize(void);
uint8_t TM_FATFS_CheckCardDetectPin(void);
DSTATUS TM_FATFS_SD_SDIO_disk_status(void);
DRESULT TM_FATFS_SD_SDIO_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_SDIO_disk_write(const BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SD_SDIO_disk_ioctl(BYTE cmd, void *buff);

#endif

#ifdef __cplusplus
}
#endif

#endif
