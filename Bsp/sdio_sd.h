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

extern SD_HandleTypeDef hsd;

void SD_Init(void);
uint8_t Find_SD_IsDetected(void);

#ifdef __cplusplus
}
#endif

#endif
