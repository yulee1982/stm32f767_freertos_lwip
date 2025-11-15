#ifndef __AUDIOPLAY_H__
#define __AUDIOPLAY_H__

//#include "common.h"
//#include "bsp_wm8978.h"
#include "pcm5102.h"

#define AUDIO_PLAY_I2S PCM5102_I2S

#define AUDIO_FLAG_INFO_READY (1<<1)//������Ϣ�ṹ��������
#define AUDIO_FLAG_PAUSED (1<<2)//���ű���ͣ
#define AUDIO_FLAG_ABORT (1<<3)//���������߳�

#define BigEndianTab2LittleEndianInt(tab) (((unsigned int)(tab)[0]<<24) + ((unsigned int)(tab)[1]<<16) +((unsigned int)(tab)[2]<<8) + ((tab)[3]))

typedef enum
{
	AudioPlay_OK = 0,
	AudioPlay_OpenFileError,//�޷����ļ�
	AudioPlay_ReadFileError,//�޷���ȡ�ļ�
	AudioPlay_UnsupportedFormat,//������Ƶ�ļ��ļ�
	AudioPlay_UnsupportedParameter,//�ļ���������ȷ
	AudioPlay_DamagedFile,//�ļ���
	
	AudioPlay_MP3_UnsupportedID3v2Version,
	AudioPlay_MP3_NoID3v1Tag,
	AudioPlay_MP3_NoID3v2Tag,
	
	AudioPlay_Next,
	AudioPlay_Prev,
	
	AudioPlay_NextFolder,
	AudioPlay_PrevFolder,
	
	AudioPlay_Exit,
	AudioPlay_PlayEnd,
}AudioPlayRes;

typedef enum
{
	AudioFileType_ERROR,
	AudioFileType_MP3,
	AudioFileType_WAV,
	AudioFileType_FLAC,
}AudioFileType;

typedef struct
{
	//char FileName[_MAX_LFN+1];
	
	AudioFileType FileType;
	volatile AudioPlayRes PlayRes;
	
	uint16_t TotalSec;
	uint16_t CurrentSec;
	
	uint32_t Bitrate;
	uint32_t Samplerate;
	uint32_t BufferSize;
	uint8_t Channels;
	volatile uint8_t Flags;
}AudioPlay_Info;

extern AudioPlay_Info AudioPlayInfo;

void MonoChannelFill(uint16_t* buf,uint16_t size);

AudioPlayRes MusicPlayingProcess(void);

void AudioPlay_MemAlloc(uint16_t bufsize);
void AudioPlay_MemFree(void);

void AudioPlay_ClearSem(void);
void AudioPlay_CreateSem(void);
void AudioPlay_PendSem(void);

void* AudioPlay_GetNextBuff(void);
void* AudioPlay_GetPlayingBuff(void);
void AudioPlay_ClearBuf(void);
uint8_t AudioPlay_I2SConfig(uint8_t Bits,uint32_t SampleRate,uint16_t BufSize);

void Audio_StopPlay(void);
void Audio_MemAlloc(void);
void Audio_MemFree(void);

void AudioPlayFile(char* path,char* fname);

void Play_Start(void);
void Play_Stop(void);

#endif
