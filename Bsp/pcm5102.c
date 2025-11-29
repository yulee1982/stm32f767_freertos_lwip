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

#include "pcm5102.h"

static void I2S2_LowLevel_DMA_TxConfig (uint32_t *BufferSRE, uint32_t BufferSize);

// I2S2
/*
 * 开发板 PCM5102模块
   GND  ---- SCK(可以短接SCK丝印附近预留的焊盘来代替此连接；若没有短接，则此连接必不可少)
   PB13 ---- BCK ---- CK
   PB15 ---- DIN ---- SD
   PB12 ---- LCK ---- WS
   GND  ---- GND
   3.3V ---- Vin(3.3V或5V供电均可，推荐使用5V)
   PCM5102模块的耳机座直接插耳机或音箱即可听到声音

   1  FLT     to L     Low:正常延迟 / High:低延迟
   2  DEMP    to L     44.1 kHz采样率的去重控制(1)：关闭(低)/开启(高)
   3  XSMT    to H     软静音(低)/软取消静音(高)
   4  FMT     to L     Low:I2S模式/ High:左对齐

 */
void pcm5102_init(void)
{
  uint32_t tempreg;
  /*  low_level_init, GPIO Configuration
   *  PB12 ------> I2S2_WS  (AF05) MCU_73PIN
   *  PB13 ------> I2S2_CK  (AF05) MCU_74PIN
   *  PB15 ------> I2S2_SD  (AF05) MCU_76PIN
   *  PC6  ------> I2S2_MCK (AF05) MCU_96PIN  here not use
   */

  //Enables the clock for GPIOA
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

  //Configures I2S2 pins to work in alternate function mode. transmit master
  GPIOB->MODER &= ~((0x3 << 30) | (0x3 << 26) | (0x3 << 24));
  GPIOB->MODER |= ((0x2 << 30) | (0x2 << 26) | (0x2 << 24)); //复用功能模式
  GPIOB->AFR[1] &= 0x0f00ffff;
  GPIOB->AFR[1] |= 0x50550000;  //AFR15[3:0] = AFR13[3:0] = AFR12[3:0] = af5(0x5)
  GPIOB->OTYPER &= ~((0x1 << 15) | (0x1 << 13) | (0x1 << 12)); // 复位状态 推挽输出
  GPIOB->OSPEEDR |= ((0x3 << 30) | (0x3 << 26) | (0x3 << 24)); // IO速度 高速
  GPIOB->PUPDR &= ~((0x3 << 30) | (0x3 << 26) | (0x3 << 24));  // 无上下拉

  //PLLI2S enable
  RCC->CR |= RCC_CR_PLLI2SON;
  //PLLI2S clock ready flag
  while (!(RCC->CR & RCC_CR_PLLI2SRDY)) { ; }
  //时钟选择 I2S 选择 PLLI2S
  RCC->CFGR &= ~RCC_CFGR_I2SSRC;
  RCC->PLLI2SCFGR = ((0x2 << 28) | (216 << 6)); // 216M/2
  /* Enable spi2/i2s Clocks */
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

  //Enable DMA1 controller clock
  SET_BIT(RCC->AHB1ENR, LL_AHB1_GRP1_PERIPH_DMA1);
  NVIC_SetPriority(DMA1_Stream4_IRQn, 8);
  NVIC_EnableIRQ(DMA1_Stream4_IRQn);

  // I2S3/SPI2 config
  SPI2->I2SCFGR &= ~(0x1<<10); //~(1 << SPI_I2SCFGR_I2SE);
  tempreg = 0;
  tempreg |= (0x0<<12) & SPI_I2SCFGR_ASTRTEN;
  tempreg |= (0x1<<11) & SPI_I2SCFGR_I2SMOD;  //I2S mode
  //tempreg |= (0x1<<10) & SPI_I2SCFGR_I2SE;    //I2S en
  tempreg |= (0x2<<8) & SPI_I2SCFGR_I2SCFG;   //I2S master tx mode
  tempreg |= (0x0<<4) & SPI_I2SCFGR_I2SSTD;   //I2S Philips
  tempreg |= (0x0<<3) & SPI_I2SCFGR_CKPOL;    //Inactive state clock polarity is low
  tempreg |= (0x0<<1) & SPI_I2SCFGR_DATLEN;   //Data length to be transferred is 16bit
  tempreg |= (0x0<<0) & SPI_I2SCFGR_CHLEN;    //Channel length (number of bits per audio channel) is 16bit
  SPI2->I2SCFGR = tempreg;

}

/**
 * @brief  Configures the DMA1 Channel0 for I2S Tx request.
 * @param  BufferDST: pointer to the source buffer
 * @param  BufferSize: buffer size
 * @retval None
 */
static void I2S2_LowLevel_DMA_TxConfig (uint32_t *BufferSRE, uint32_t BufferSize)
{
  uint32_t tempreg;

  /////PART I::::Adjust the DMA
  //Reset the control register (0x00 is the default value. this also disables the dma. When EN=0, it stops any ongoing DMA transfer)
  DMA1_Stream4->CR &= (~DMA_SxCR_EN);
  DMA1_Stream4->CR = 0;

  //Clear all the flags
  DMA1->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;

  //Set the DMA Addresses
  DMA1_Stream4->PAR = SPI2->DR;
  DMA1_Stream4->M0AR = (uint32_t)BufferSRE;    //Memory address

  //Set the number of data to transfer
  DMA1_Stream4->NDTR = BufferSize;   //0 ?? Peripheral controls, therefore we don't need to indicate a size

  /* Configure the DMA functional DMA Tx parameters for transmission */
  //Set the DMA CR
  tempreg = 0;
  tempreg |= (0x00<<25) & DMA_SxCR_CHSEL;  //Select Channel 0.
  tempreg |= (0x00<<23) & DMA_SxCR_MBURST;
  tempreg |= (0x00<<21) & DMA_SxCR_PBURST;
  tempreg |= (0x00<<18) & DMA_SxCR_DBM;   //Disable double buffer mode (when this is set, circluar mode is also automatically set. (the actual value is don't care)
  tempreg |= (0x00<<16) & DMA_SxCR_PL;     //Priority is very_high
  tempreg |= (0x00<<15) & DMA_SxCR_PINCOS;  //Peripheral increment offset (if this is 1 and Pinc=1, then Peripheral will be incremented by 4 regardless of Psize)
  tempreg |= (0x01<<13) & DMA_SxCR_MSIZE;  //Memory data size is 16bit (half word)
  tempreg |= (0x01<<11) & DMA_SxCR_PSIZE;  //Peripheral data size is 16bit (half word)
  tempreg |= (0x01<<10) & DMA_SxCR_MINC;  //Enable Memory Increment
  tempreg |= (0x00<<9) & DMA_SxCR_MINC;  //Disable Peripheral Increment
  tempreg |= (0x01<<8) & DMA_SxCR_CIRC;   //EN Circular mode
  tempreg |= (0x01<<6) & DMA_SxCR_DIR;  //Direction 0:P2M, 1:M2P
  tempreg |= (0x00<<5) & DMA_SxCR_PFCTRL; //Peripheral controls the flow control.
  //Bit [4..1] is for interupt mask. I don't use interrupts here
  //Bit 0 is EN. I will set it after I set the FIFO CR. (FIFO CR cannot be modified when EN=1)
  DMA1_Stream4->CR = tempreg;

  //Set the FIFO CR
  tempreg = 0x21; //Reset value
  tempreg |= (0<<7); //FEIE is disabled
  tempreg |= (0<<2); //Fifo is enabled (Direct mode is en);
  tempreg |= (0<<0); //Full fifo (at direct mode, this is not use)
  DMA1_Stream4->FCR = tempreg;

  /* Enable DMA transfer complete/error interrupts  */
  DMA1_Stream4->CR |= (DMA_SxCR_TCIE | DMA_SxCR_TEIE);
  //LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_3);
  //LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_3);

  //Enable the DMA (When it is enabled, it starts to respond dma requests)
  DMA1_Stream4->CR |= DMA_SxCR_EN;
  //END of PART I
}

static const uint8_t STREAM_OFFSET_TAB1[] =
{
  (uint8_t)(DMA1_Stream0_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream1_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream2_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream3_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream4_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream5_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream6_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Stream7_BASE - DMA1_BASE)
};
static void I2S_StartTransmitDMA(SPI_TypeDef *spi, DMA_TypeDef *dma_x, uint32_t stream, uint32_t *buff, uint32_t buff_size)
{
  I2S2_LowLevel_DMA_TxConfig (buff, buff_size);

  //select i2s and en i2s
  spi->I2SCFGR |= (SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SE);

  DMA_Stream_TypeDef *dma_TX = ((DMA_Stream_TypeDef *)((uint32_t)((uint32_t)dma_x + STREAM_OFFSET_TAB1[stream])));
  uint8_t shift[8] = {0, 6, 16, 22, 0, 6, 16, 22};  //IFCR标志寄存器（L和H）中的位偏移
  volatile uint32_t *ifcr_tx = (stream > 3) ? &(dma_x->HIFCR) : &(dma_x->LIFCR);

  //关闭DMA传输tx通道
  dma_TX->CR &= ~DMA_SxCR_EN;
  while (dma_TX->CR & DMA_SxCR_EN) {__NOP();} //等待DMA通道断开
  //重置tx中断标志
  *ifcr_tx = 0x3F<<shift[stream];
  //允许SPI接受来自DMA的请求
  spi->CR2 |= SPI_CR2_TXDMAEN;
  //禁止对传输通道tx的某些事件中断以及双缓冲模式
  dma_TX->CR &= ~(DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_DBM);
  dma_TX->FCR &= ~DMA_SxFCR_FEIE;
  //允许在传输完成和传输到缓冲区一半时产生中断
  dma_TX->CR |= (DMA_SxCR_TCIE | DMA_SxCR_HTIE);
  //配置地址和长度
  dma_TX->PAR = (uint32_t)(&spi->DR);  //接收器外设 - SPI的DR寄存器地址
  dma_TX->M0AR = (uint32_t)buff;       //源内存 - 发出数据缓冲区地址
  dma_TX->NDTR = (uint32_t)buff_size;  //传输数据的大小
  dma_TX->CR |= (DMA_SxCR_EN);         //启用传输通道
}


int Start_DAC_DMA (SPI_TypeDef *spi, DMA_TypeDef *dma, uint32_t dma_stream, uint32_t SampleRate, uint32_t DataFormat, uint32_t *outbuf, uint32_t bufsize)
{
  uint32_t tempreg, pll_N = 0, pll_R = 0;

  typedef struct {
    uint32_t freq, pll_N16, pll_R16, pll_N32, pll_R32;
  }PLL_data;

  //PLLI2S模块设置表
  const PLL_data pll_table[] = {	{  8000, 192, LL_RCC_PLLI2SR_DIV_2, 192, LL_RCC_PLLI2SR_DIV_3},
									{ 16000, 192, LL_RCC_PLLI2SR_DIV_3, 256, LL_RCC_PLLI2SR_DIV_2},
									{ 22050, 290, LL_RCC_PLLI2SR_DIV_3, 302, LL_RCC_PLLI2SR_DIV_2},
									{ 32000, 256, LL_RCC_PLLI2SR_DIV_2, 256, LL_RCC_PLLI2SR_DIV_5},
									{ 44100, 302, LL_RCC_PLLI2SR_DIV_2, 429, LL_RCC_PLLI2SR_DIV_4},
									{ 48000, 192, LL_RCC_PLLI2SR_DIV_5, 384, LL_RCC_PLLI2SR_DIV_5},
									{ 96000, 384, LL_RCC_PLLI2SR_DIV_5, 424, LL_RCC_PLLI2SR_DIV_3},
									{192000, 424, LL_RCC_PLLI2SR_DIV_3, 258, LL_RCC_PLLI2SR_DIV_3}  };
  const PLL_data *tbl_ptr = pll_table;
  uint8_t i = 0, 	k = 1;
  //在表格中查找PLLI2S模块的设置
  while (i < 8)
  {
    if(SampleRate == tbl_ptr->freq)
    {
      if((DataFormat == LL_I2S_DATAFORMAT_16B) || (DataFormat == LL_I2S_DATAFORMAT_16B_EXTENDED))
      {
        pll_N = tbl_ptr->pll_N16;
        pll_R = tbl_ptr->pll_R16;
        k = 2;
      }else{
        pll_N = tbl_ptr->pll_N32;
        pll_R = tbl_ptr->pll_R32;
      }
      break;
    }
    tbl_ptr++;
    i++;
  }

  if(!pll_N)
	return 1;

  //关闭 I2S 以进行设置
  spi->I2SCFGR &= ~SPI_I2SCFGR_I2SE;

  //关闭PLLI2S模块
  RCC->CR &= ~RCC_CR_PLLI2SON;
  //config PLLI2S
  RCC->PLLI2SCFGR &= ~((0x7 << 28) | (0x1ff << 6));
  RCC->PLLI2SCFGR |= ((pll_R << 28) | (pll_N/2 << 6));
  //启用 PLLI2S 模块
  RCC->CR |= RCC_CR_PLLI2SON;
  //等待PLLI2S模块激活
  while (!(RCC->CR & RCC_CR_PLLI2SRDY)) { __NOP(); }

  //настройка I2S
  // I2S3/SPI2 config
  spi->I2SCFGR &= ~(0x1<<10); //~(1 << SPI_I2SCFGR_I2SE);
  tempreg = 0;
  tempreg |= (0x0<<12) & SPI_I2SCFGR_ASTRTEN;
  tempreg |= (0x1<<11) & SPI_I2SCFGR_I2SMOD;  //I2S mode
  //tempreg |= (0x1<<10) & SPI_I2SCFGR_I2SE;    //I2S en
  tempreg |= (0x2<<8) & SPI_I2SCFGR_I2SCFG;   //I2S master tx mode
  tempreg |= (0x0<<4) & SPI_I2SCFGR_I2SSTD;   //I2S Philips
  tempreg |= (0x0<<3) & SPI_I2SCFGR_CKPOL;    //Inactive state clock polarity is low
  //tempreg |= (0x0<<1) & SPI_I2SCFGR_DATLEN;   //Data length to be transferred is 16bit
  //tempreg |= (0x0<<0) & SPI_I2SCFGR_CHLEN;    //Channel length (number of bits per audio channel) is 16bit
  tempreg |= DataFormat;
  spi->I2SCFGR = tempreg;
  /*
  I2S_InitStruct.Mode = LL_I2S_MODE_MASTER_TX;
  I2S_InitStruct.Standard = LL_I2S_STANDARD_PHILIPS;
  I2S_InitStruct.MCLKOutput = LL_I2S_MCLK_OUTPUT_DISABLE;
  I2S_InitStruct.DataFormat = DataFormat;
  I2S_InitStruct.ClockPolarity = LL_I2S_POLARITY_LOW;
  I2S_InitStruct.AudioFreq = SampleRate;
  LL_I2S_Init(spi, &I2S_InitStruct);
  */
  //通过DMA将数据从内存传输到DAC
  I2S_StartTransmitDMA(spi, dma, dma_stream, outbuf, k * bufsize);
  return 0;
}


#if 0
/*
采样率计算公式:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
//pllm:在Sys_Clock_Set设置的时候确定，一般是8
//PLLI2SN:一般是192~432
//PLLI2SR:2~7
//I2SDIV:2~255
//ODD:0/1
//I2S分频系数表@pllm=8,HSE=8Mhz,即vco输入频率为1Mhz
*/
const uint16_t I2S_PSC_TABLE[][5]=
{
	//采样率/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
	{800 ,256,5,12,1},//8Khz采样率
	{1102,429,4,19,0},//11.025Khz采样率
	{1600,213,2,13,0},//16Khz采样率
	{2205,429,4, 9,1},//22.05Khz采样率
	{3200,213,2, 6,1},//32Khz采样率
	{4410,271,2, 6,0},//44.1Khz采样率
	{4800,258,3, 3,1},//48Khz采样率
	{8820,316,2, 3,1},//88.2Khz采样率
	{9600,344,2, 3,1},//96Khz采样率
	{17640,361,2,2,0},//176.4Khz采样率
	{19200,393,2,2,0},//192Khz采样率
};
#endif
uint8_t I2S_Config(SPI_TypeDef* SPIx, uint16_t I2S_Mode, uint32_t SampleRate, uint16_t I2S_DataFormat)
{
  uint32_t tmpreg, pll_N = 0, pll_R = 0;
  uint32_t sourceclock, packetlength;

  typedef struct {
    uint32_t freq, pll_N16, pll_R16, pll_N32, pll_R32;
  }PLL_data;

  //PLLI2S模块设置表 PLLI2SN== 49-432
  const PLL_data pll_table[] = {
          {  8000, 192/2, LL_RCC_PLLI2SR_DIV_2, 192/2, LL_RCC_PLLI2SR_DIV_3},
          { 11025, 192/2, LL_RCC_PLLI2SR_DIV_2, 192/2, LL_RCC_PLLI2SR_DIV_3}, //
          { 16000, 192/2, LL_RCC_PLLI2SR_DIV_3, 256/2, LL_RCC_PLLI2SR_DIV_2},
          { 22050, 290/2, LL_RCC_PLLI2SR_DIV_3, 302/2, LL_RCC_PLLI2SR_DIV_2},
          { 32000, 256/2, LL_RCC_PLLI2SR_DIV_2, 256/2, LL_RCC_PLLI2SR_DIV_5},
          { 44100, 302/2, LL_RCC_PLLI2SR_DIV_2, 428/2, LL_RCC_PLLI2SR_DIV_4}, //429
          { 48000, 192/2, LL_RCC_PLLI2SR_DIV_5, 384/2, LL_RCC_PLLI2SR_DIV_5},
		  { 88200, 192/2, LL_RCC_PLLI2SR_DIV_5, 192/2, LL_RCC_PLLI2SR_DIV_3}, //
          { 96000, 384/2, LL_RCC_PLLI2SR_DIV_5, 424/2, LL_RCC_PLLI2SR_DIV_3},
		  {128000, 256/2, LL_RCC_PLLI2SR_DIV_2, 256/2, LL_RCC_PLLI2SR_DIV_4}, //
		  {176400, 424/2, LL_RCC_PLLI2SR_DIV_3, 258/2, LL_RCC_PLLI2SR_DIV_3}, //
          {192000, 424/2, LL_RCC_PLLI2SR_DIV_3, 258/2, LL_RCC_PLLI2SR_DIV_3}
  };
  const PLL_data *tbl_ptr = pll_table;
  uint8_t i = 0;
  //在表格中查找PLLI2S模块的设置
  for(i = 0; i < 12; i++)
  {
    if(SampleRate == tbl_ptr->freq)
    {
      if((I2S_DataFormat == LL_I2S_DATAFORMAT_16B) || (I2S_DataFormat == LL_I2S_DATAFORMAT_16B_EXTENDED))
      {
        pll_N = tbl_ptr->pll_N16;
        pll_R = tbl_ptr->pll_R16;
      }else{
        pll_N = tbl_ptr->pll_N32;
        pll_R = tbl_ptr->pll_R32;
      }
      break;
    }
    tbl_ptr++;

  }

  if(!pll_N)
    return 1;

  sourceclock = (2000000 * pll_N) / pll_R;

  //关闭 I2S 以进行设置
  SPIx->I2SCFGR &= ~SPI_I2SCFGR_I2SE;

  //关闭PLLI2S模块
  RCC->CR &= ~RCC_CR_PLLI2SON;
  //config PLLI2S
  RCC->PLLI2SCFGR &= ~((0x7 << 28) | (0x1ff << 6));
  RCC->PLLI2SCFGR |= ((pll_R << 28) | (pll_N/2 << 6));
  //启用 PLLI2S 模块
  RCC->CR |= RCC_CR_PLLI2SON;
  //等待PLLI2S模块激活
  while (!(RCC->CR & RCC_CR_PLLI2SRDY)) { __NOP(); }

#if 0
  /*库函数中提供了设置PLL的函数 但是计算速度较慢 这里用查表和寄存器操作替代*/
  tempreg = I2S_PSC_TABLE[i][3]<<0;	//设置I2SDIV
  tempreg |= I2S_PSC_TABLE[i][4]<<8;//设置ODD位
  SPIx->I2SPR = tempreg;//设置I2SPR寄存器

  I2S_MCLKOutput_Config(SPIx,1);
#endif


  if(SPIx == SPI2)
  {
    RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;  //复位SPI2
    RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; //结束复位

  }
  else if(SPIx == SPI3)
  {
    RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;//复位SPI3
    RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST; //结束复位
  }
  else
    return 1;

  /*----------------------- SPIx I2SCFGR & I2SPR Configuration -----------------*/
  /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
  SPIx->I2SCFGR &= ((uint16_t)0xF040);
  SPIx->I2SPR = 0x0002;

  /* Get the I2SCFGR register value */
  tmpreg = SPIx->I2SCFGR;

  /* Here the default value has to be written, reinitialize i2sdiv and i2sodd*/
  uint32_t i2sdiv = 2;
  uint32_t i2sodd = 0;
  uint32_t i2smclk = 0; //主时钟输出禁止; 使能则 i2smclk = 0x200
  /* Write to SPIx I2SPR register the computed value */
  SPIx->I2SPR = i2smclk | i2sodd | i2sdiv;

  //SPIx->I2SCFGR &= ~(0x1<<10); //~(1 << SPI_I2SCFGR_I2SE);
  tmpreg = 0;
  tmpreg |= (0x0<<12) & SPI_I2SCFGR_ASTRTEN;  //使能异步
  tmpreg |= (0x1<<11) & SPI_I2SCFGR_I2SMOD;   //I2S mode
  //tmpreg |= (0x1<<10) & SPI_I2SCFGR_I2SE;   //I2S en
  tmpreg |= (0x2<<8) & SPI_I2SCFGR_I2SCFG;   //I2S master tx mode
  tmpreg |= (0x0<<4) & SPI_I2SCFGR_I2SSTD;   //I2S Philips
  tmpreg |= (0x0<<3) & SPI_I2SCFGR_CKPOL;    //Inactive state clock polarity is low
  //tmpreg |= (0x0<<1) & SPI_I2SCFGR_DATLEN;   //Data length to be transferred is 16bit
  //tmpreg |= (0x0<<0) & SPI_I2SCFGR_CHLEN;    //Channel length (number of bits per audio channel) is 16bit
  tmpreg |= I2S_DataFormat;
  SPIx->I2SCFGR = tmpreg;

  SPIx->CR2 |= SPI_CR2_TXDMAEN; //SPI TX DMA请求使能


  /*---------------------------- SPIx I2SPR Configuration ----------------------
   * Configure SPIx I2SPR with parameters:
   * - MCLKOutput:    SPI_I2SPR_MCKOE bit
   * - AudioFreq:     SPI_I2SPR_I2SDIV[7:0] and SPI_I2SPR_ODD bits
   */

  /* If the requested audio frequency is not the default, compute the prescaler (i2sodd, i2sdiv)
   * else, default values are used:  i2sodd = 0U, i2sdiv = 2U.
   */
  packetlength = 1U;

  if (SampleRate != 2U)//LL_I2S_AUDIOFREQ_DEFAULT
  {
    /* Check the frame length (For the Prescaler computing)
     * Default value: LL_I2S_DATAFORMAT_16B (packetlength = 1U).
     */
    if (I2S_DataFormat != LL_I2S_DATAFORMAT_16B)
    {
      /* Packet length is 32 bits */
      packetlength = 2U;
    }

    /* If an external I2S clock has to be used, the specific define should be set
    in the project configuration or in the stm32f4xx_ll_rcc.h file */
    /* Get the I2S source clock value */
    //sourceclock = LL_RCC_GetI2SClockFreq(LL_RCC_I2S1_CLKSOURCE);

    /* Compute the Real divider depending on the MCLK output state with a floating point */
    /* MCLK output is disabled */
    tmpreg = (((((sourceclock / (32U * packetlength)) * 10U) / SampleRate)) + 5U);

    /* Remove the floating point */
    tmpreg = tmpreg / 10U;

    /* Check the parity of the divider */
    i2sodd = (tmpreg & (uint16_t)0x0001U);

    /* Compute the i2sdiv prescaler */
    i2sdiv = ((tmpreg - i2sodd) / 2U);

    /* Get the Mask for the Odd bit (SPI_I2SPR[8]) register */
    i2sodd = (i2sodd << 8U);
  }

  /* Test if the divider is 1 or 0 or greater than 0xFF */
  if ((i2sdiv < 2U) || (i2sdiv > 0xFFU))
  {
    /* Set the default values */
    i2sdiv = 2U;
    i2sodd = 0U;
  }

  /* Write to SPIx I2SPR register the computed value */
  WRITE_REG(SPIx->I2SPR, i2sdiv | i2sodd); //I2S_InitStruct->MCLKOutput

  SPIx->I2SCFGR |= SPI_I2SCFGR_I2SE;  //使能I2S外设
  return 0;
}

void I2S_TX_DMA_Init(void* buf0, void* buf1, uint16_t datnum, uint8_t datsize)
{
  uint32_t tmpreg;

  //Enable DMA1 controller clock
  SET_BIT(RCC->AHB1ENR, LL_AHB1_GRP1_PERIPH_DMA1);

  /////PART I::::Adjust the DMA
  //Reset the control register (0x00 is the default value. this also disables the dma. When EN=0, it stops any ongoing DMA transfer)
  PCM5102_I2S_DMA_STREAM->CR &= (~DMA_SxCR_EN);

  /* Reset DMAy Streamx control register */
  PCM5102_I2S_DMA_STREAM->CR = 0;

  /* Reset DMAy Streamx Number of Data to Transfer register */
  PCM5102_I2S_DMA_STREAM->NDTR = 0;

  /* Reset DMAy Streamx peripheral address register */
  PCM5102_I2S_DMA_STREAM->PAR  = 0;

  /* Reset DMAy Streamx memory 0 address register */
  PCM5102_I2S_DMA_STREAM->M0AR = 0;

  /* Reset DMAy Streamx memory 1 address register */
  PCM5102_I2S_DMA_STREAM->M1AR = 0;

  /* Reset DMAy Streamx FIFO control register */
  PCM5102_I2S_DMA_STREAM->FCR = (uint32_t)0x00000021;

  /* Reset interrupt pending bits for the selected stream */
  DMA1->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4;//Clear all the flags

  while((PCM5102_I2S_DMA_STREAM->CR & DMA_SxCR_EN) != 0);//等待DMA1_Stream1可配置

  /*------------------------- DMAy Streamx CR Configuration ------------------*/
  /* Get the DMAy_Streamx CR value */
  tmpreg = PCM5102_I2S_DMA_STREAM->CR;

  /* Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
  tmpreg &= ((uint32_t)~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | \
                         DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE | \
                         DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_CIRC | \
                         DMA_SxCR_DIR));
  /* Configure DMAy Streamx: */
  /* Set CHSEL bits according to DMA_CHSEL value */
  /* Set DIR bits according to DMA_DIR value */
  /* Set PINC bit according to DMA_PeripheralInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to DMA_PeripheralDataSize value */
  /* Set MSIZE bits according to DMA_MemoryDataSize value */
  /* Set CIRC bit according to DMA_Mode value */
  /* Set PL bits according to DMA_Priority value */
  /* Set MBURST bits according to DMA_MemoryBurst value */
  /* Set PBURST bits according to DMA_PeripheralBurst value */
  tmpreg |= (0x0UL << 25) & DMA_SxCR_CHSEL;  //Select Channel 0.
  tmpreg |= (0x0UL << 23) & DMA_SxCR_MBURST; //存储器突发单次传输
  tmpreg |= (0x0UL << 21) & DMA_SxCR_PBURST; //外设突发单次传输
  tmpreg |= (0x0UL << 18) & DMA_SxCR_DBM;   //Disable double buffer mode (when this is set, circluar mode is also automatically set.
  tmpreg |= (0x2UL << 16) & DMA_SxCR_PL;     //Priority is high
  tmpreg |= (0x0UL << 15) & DMA_SxCR_PINCOS;  //Peripheral increment offset (if this is 1 and Pinc=1, then Peripheral will be incremented by 4 regardless of Psize)
  if(datsize == 16)
  {
    tmpreg |= (0x1UL << 13) & DMA_SxCR_MSIZE;  //Memory data size is 16bit (half word)
    tmpreg |= (0x1UL<<11) & DMA_SxCR_PSIZE;    //Peripheral data size is 16bit (word)
  }
  else if(datsize == 24 || datsize == 32)
  {
    tmpreg |= (0x2UL << 13) & DMA_SxCR_MSIZE;  //Memory data size is 32bit (half word)
    tmpreg |= (0x2UL<<11) & DMA_SxCR_PSIZE;    //Peripheral data size is 32bit (word)
  }
  else
  {
    return;
  }
  tmpreg |= (0x1UL<<10) & DMA_SxCR_MINC;  //Enable Memory Increment
  tmpreg |= (0x0UL<<9) & DMA_SxCR_MINC;   //Disable Peripheral Increment
  tmpreg |= (0x1UL<<8) & DMA_SxCR_CIRC;   //EN Circular mode
  tmpreg |= (0x1UL<<6) & DMA_SxCR_DIR;    //Direction 0:P2M, 1:M2P
  tmpreg |= (0x0UL<<5) & DMA_SxCR_PFCTRL; //Peripheral controls the flow control.

  /* Write to DMAy Streamx CR register */
  PCM5102_I2S_DMA_STREAM->CR = tmpreg;

  /*------------------------- DMAy Streamx FCR Configuration -----------------*/
  /* Get the DMAy_Streamx FCR value */
  tmpreg = PCM5102_I2S_DMA_STREAM->FCR;

  /* Clear DMDIS and FTH bits */
  tmpreg &= (uint32_t)~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

  /* Configure DMAy Streamx FIFO:
    Set DMDIS bits according to DMA_FIFOMode value
    Set FTH bits according to DMA_FIFOThreshold value */
  tmpreg |= ((0 << 2) | (0 << 0)); //使能直接模式，FIFO阈值是FIFO的1/4
  /* Write to DMAy Streamx CR */
  PCM5102_I2S_DMA_STREAM->FCR = tmpreg;

  /*------------------------- DMAy Streamx NDTR Configuration ----------------*/
  /* Write to DMAy Streamx NDTR register */
  PCM5102_I2S_DMA_STREAM->NDTR = datnum;

  /*------------------------- DMAy Streamx PAR Configuration -----------------*/
  /* Write to DMAy Streamx PAR */
  PCM5102_I2S_DMA_STREAM->PAR = (unsigned int)&PCM5102_I2S->DR;

  /*------------------------- DMAy Streamx M0AR Configuration ----------------*/
  /* Write to DMAy Streamx M0AR */
  PCM5102_I2S_DMA_STREAM->M0AR = (unsigned int)buf0;
  PCM5102_I2S_DMA_STREAM->M1AR = (unsigned int)buf1;

  /* Enable the selected DMA FIFO interrupts */
  PCM5102_I2S_DMA_STREAM->FCR |= (uint32_t)DMA_IT_FE; //开启传输完成中断

  NVIC_SetPriority(DMA1_Stream4_IRQn, 8);
  NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

__weak void I2S_TxHalfCpltCallback(uint32_t value)
{
  UNUSED(value);
}

__weak void I2S_TxCpltCallback(uint32_t value)
{
  UNUSED(value);
}

void DMA1_Stream4_IRQHandler(void)
{

  DMA_TypeDef *dma = DMA1;
  int stream = 4;
  uint8_t shift[8] = {0, 6, 16, 22, 0, 6, 16, 22}; //寄存器中的位移
  volatile uint32_t *isr = (stream > 3) ? &(dma->HISR) : &(dma->LISR);    //中断状态寄存器
  volatile uint32_t *ifcr = (stream > 3) ? &(dma->HIFCR) : &(dma->LIFCR); //中断标志重置寄存器
  int offset = -1;
  if((*isr) & (0x10 << shift[stream]))//这是由于向DAC传输PCM缓冲区一半而产生的中断
  {
    offset = 0;
    I2S_TxHalfCpltCallback(offset);
  }
  if((*isr) & (0x20 << shift[stream]))//这是将整个PCM缓冲区传输到DAC的中断
  {
    offset = 1;
    I2S_TxCpltCallback(offset);
  }
  //(void)PlayersUpdate(offset); //正在更新播放器数据
  *ifcr = 0x3F << shift[stream]; //重置tx中断标志

  /*
  if(DMA1->HISR & (0x1U << 4))//DMA1_Stream4,传输完成标志
  {
    DMA1->HIFCR = 0x1U << 4;
    DataRequestFlag = 1;
  }
  */
}

