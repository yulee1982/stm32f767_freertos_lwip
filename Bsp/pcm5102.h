/*
 *  Author: VadRov
 *  Copyright (C) 2019, VadRov, all right reserved.
 *
 *	Драйвер для работы с аудио ЦАП (Audio Stereo DAC) PCM5102
 *
 *  Допускается свободное распространение без целей коммерческого использования.
 *  При коммерческом использовании необходимо согласование с автором.
 *  Распространятся по типу "как есть", то есть использование осуществляете на свой страх и риск.
 *  Автор не предоставляет никаких гарантий.
 *
 *  https://www.youtube.com/@VadRov
 *  https://dzen.ru/vadrov
 *  https://vk.com/vadrov
 *  https://t.me/vadrov_channel
 *
 */

#ifndef PCM5102_H_
#define PCM5102_H_

#include "main.h"

#define PCM5102_I2S_DMA_CMD(cmd) DMA_Cmd(DMA1_Stream4,cmd)
#define PCM5102_I2S_DMA_STREAM DMA1_Stream4
#define PCM5102_I2S_DMA_IRQ DMA1_Stream4_IRQn
//#define PCM5102_I2S_DMA_IT_FLAG DMA_IT_TCIF5
#define PCM5102_I2S_DMA_CH DMA_Channel_0

#define PCM5102_I2S SPI2

#define LL_I2S_DATAFORMAT_16B              0x00000000U                                   /*!< Data length 16 bits, Channel length 16bit */
#define LL_I2S_DATAFORMAT_16B_EXTENDED     (SPI_I2SCFGR_CHLEN)                           /*!< Data length 16 bits, Channel length 32bit */
#define LL_I2S_DATAFORMAT_24B              (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN_0)    /*!< Data length 24 bits, Channel length 32bit */
#define LL_I2S_DATAFORMAT_32B              (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN_1)    /*!< Data length 16 bits, Channel length 32bit */


#define I2S_BUF_DEPTH   8
#define I2S_START_LEVEL     (I2S_BUF_DEPTH / 4)
#define I2S_TARGET_LEVEL    (I2S_BUF_DEPTH / 2)
#define I2S_DATA_LEN    ((384 + 1) * 2 * 2)



//старт DAC DMA
int Start_DAC_DMA (SPI_TypeDef *spi, DMA_TypeDef *dma, uint32_t dma_stream, uint32_t SampleRate, uint32_t DataFormat, uint32_t *outbuf, uint32_t bufsize);
void pcm5102_init(void);
uint8_t I2S_Config(SPI_TypeDef* SPIx, uint16_t I2S_Mode, uint32_t SampleRate, uint16_t I2S_DataFormat);
void I2S_TX_DMA_Init(void* buf0, void* buf1, uint16_t datnum, uint8_t datsize);
//Start_DAC_DMA (SPI2, DMA1, 4, AudioPlayInfo.Bitrate, uint32_t DataFormat, uint32_t *outbuf, uint32_t bufsize);

void I2S_TxHalfCpltCallback(uint32_t value);
void I2S_TxCpltCallback(uint32_t value);

#endif /* PCM5102_H_ */
