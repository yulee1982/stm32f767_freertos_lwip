//#include "common.h"
#include "audio_common.h"
#include "audio_flac.h"
#include "audio_mp3.h"
#include "audio_wav.h"
//#include "system_i2s.h"
//#include "bsp_wm8978.h"
#include "ff.h"
#include "pcm5102.h"
#include <string.h>
#include <stdio.h>
#include "system_mem.h"

AudioPlay_Info AudioPlayInfo;

/*
将单声道填充为双声道
从中间一个元素开始 将其复制到数组最后面两个
然后被复制的元素向前移动1 复制目标向前移动2
可以防止前面未被复制的数据的损坏
    (i==0)                   (i = size-1)
┌←─┬→─┐        ┌──→→→→──┬───┐
↓	   ↑    ↓        ↑                ↓      ↓
*[0]  ┘  *[1]      *[2]       [3]     [4]     [5]
            ↓        ↑        ↑
            ┕→→──┴────┘
                    (i--)
*/
void MonoChannelFill(uint16_t* buf,uint16_t size)
{
	uint16_t i;
	
	i = size-1;
	
	do
	{
		buf[2*i] = buf[i];//目标元素的第一个数
		buf[2*i + 1] = buf[i];//目标元素的第二个数
		i --;
	}
	while(i);
}

void* AudioPlay_GetNextBuff(void)
{
	if(PCM5102_I2S_DMA_STREAM->CR & 1<<19)//如果现在是缓冲区1
	{
		return I2Sbuf1;
	}
	else
	{
		return I2Sbuf2;
	}
}

void* AudioPlay_GetPlayingBuff(void)
{
	if(PCM5102_I2S_DMA_STREAM->CR & 1<<19)//如果现在是缓冲区1
	{
		return I2Sbuf2;
	}
	else
	{
		return I2Sbuf1;
	}
}

void AudioPlay_BuffSwitch(void)
{
	if(PCM5102_I2S_DMA_STREAM->CR & 1<<19)//如果现在是缓冲区1
	{
		PCM5102_I2S_DMA_STREAM->CR &= ~(1<<19);
	}
	else
	{
		PCM5102_I2S_DMA_STREAM->CR |= (1<<19);
	}
}

void AudioPlay_PendSem(void)
{
	while(!DataRequestFlag)//等待传输完成
	{
		__asm ("wfi");
	}
	DataRequestFlag = 0;//传输完成后清除传输完成标志位
}

void AudioPlay_ClearSem(void)
{
	DataRequestFlag = 0;
}

void AudioPlay_ClearBuf(void)
{
	uint32_t i;
	for(i = 0;i < AudioPlayInfo.BufferSize;i ++)
	{
		I2Sbuf1[i] = 0;
		I2Sbuf2[i] = 0;
	}
}

/** @defgroup SPI_I2S_Mode
  * @{
  */

#define I2S_Mode_SlaveTx                ((uint16_t)0x0000)
#define I2S_Mode_SlaveRx                ((uint16_t)0x0100)
#define I2S_Mode_MasterTx               ((uint16_t)0x0200)
#define I2S_Mode_MasterRx               ((uint16_t)0x0300)
#define IS_I2S_MODE(MODE) (((MODE) == I2S_Mode_SlaveTx) || \
                           ((MODE) == I2S_Mode_SlaveRx) || \
                           ((MODE) == I2S_Mode_MasterTx)|| \
                           ((MODE) == I2S_Mode_MasterRx))
uint8_t AudioPlay_I2SConfig(uint8_t Bits,uint32_t SampleRate,uint16_t BufSize)
{
  static uint8_t bits_last = 0xFF;
  uint32_t DataFormat;
  uint8_t ret = 0;

  if(bits_last != Bits)
  {
    if(Bits == 16)//16位数据
    {
      //WM8978_I2SConfig(2,0);//飞利浦标准,16位数据长度
      DataFormat = LL_I2S_DATAFORMAT_16B;
    }
    else if(Bits == 24)//24位数据
    {
      //WM8978_I2SConfig(2,2);//飞利浦标准,24位数据长度
      DataFormat = LL_I2S_DATAFORMAT_24B;
    }
    else if(Bits == 32)
    {
      //WM8978_I2SConfig(2,3);//飞利浦标准,32位数据长度
      DataFormat = LL_I2S_DATAFORMAT_32B;
    }
    else
    {
      return 1;
    }
    ret = I2S_Config(AUDIO_PLAY_I2S, I2S_Mode_MasterTx, SampleRate, DataFormat);
  }
  bits_last = Bits;
	
  I2S_TX_DMA_Init(I2Sbuf1,I2Sbuf2,BufSize,Bits);//配置DMA
	
  if(ret)
  {
    return 2;
  }
	
  return 0;
}

AudioFileType Audio_CheckFileExtname(char* path)
{
	char* temp;
	
	temp = strrchr(path,'.');
	
	temp ++;
	
	if(!strcasecmp(temp,"MP3"))
		return AudioFileType_MP3;
	else if(!strcasecmp(temp,"FLAC") || !strcasecmp(temp,"FLA"))
		return AudioFileType_FLAC;
	else if(!strcasecmp(temp,"WAV"))
		return AudioFileType_WAV;
	
	return AudioFileType_ERROR;
}

void AudioPlayFile(char* path,char* fname)
{
	char music_path[64];
	
	sprintf(music_path,"%s/%s",path,fname);
	memset(&AudioPlayInfo,0,sizeof(AudioPlay_Info));
	
	switch(Audio_CheckFileExtname(fname))
	{
		case AudioFileType_MP3:
			AudioPlayInfo.PlayRes = MP3_Play(music_path);
			break;
		case AudioFileType_WAV:
			AudioPlayInfo.PlayRes = WAV_Play(music_path);
			break;
		case AudioFileType_FLAC:
			AudioPlayInfo.PlayRes = FLAC_Play(music_path);
			break;
		default:
			break;
	}
}

void Play_Start(void)
{
  //WM8978_I2S_DMA_CMD(ENABLE);//启动传输
  //Start_DAC_DMA (SPI2, DMA1, 4, AudioPlayInfo.Bitrate, uint32_t DataFormat, uint32_t *outbuf, uint32_t bufsize);
  PCM5102_I2S_DMA_STREAM->CR |= (uint32_t)DMA_SxCR_EN;
}

void Play_Stop(void)
{
  //WM8978_I2S_DMA_CMD(DISABLE);//关闭传输
  PCM5102_I2S_DMA_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
  AudioPlay_BuffSwitch();//切换缓冲区 防止恢复播放的时候出现破音
}
