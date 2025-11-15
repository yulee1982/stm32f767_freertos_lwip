//#include "common.h"
#include "audio_wav.h"
#include "pcm5102.h"
#include "system_mem.h"
#include "ff.h"

AudioPlayRes WAV_GetInfo(FIL* file,Wav_Info* info)
{
	uint8_t buf[64];
	uint32_t br;
	uint32_t fptr = 0;
	
	ChunkFMT *fmt;
	ChunkHDR *header;
	
	f_lseek(file,0);
	f_read(file,buf,sizeof(buf),&br);//��ȡ64�ֽ�
	
	fptr = 12;//�ʼ��RIFF�� 12bytes
	
	if(((ChunkRIFF*)buf)->ChunkID == 0x46464952 && ((ChunkRIFF*)buf)->Format == 0x45564157)//�����IDΪRIFF ��ʽ��WAVE
	{
		fmt = (ChunkFMT*)(buf + 12);//��ȡFMT��
		if(fmt->ChunkID == 0x20746D66)//�����FMT��
		{
			info->AudioFormat=fmt->AudioFormat;//��Ƶ��ʽ
			info->nChannels=fmt->NumOfChannels;//ͨ����
			info->Samplerate=fmt->SampleRate;//������
			info->Bitrate=fmt->ByteRate*8;//�õ�λ��
			info->BlockAlign=fmt->BlockAlign;//�����
			info->Bps=fmt->BitsPerSample;//λ�� 16/24/32λ
		}
		else
		{
			return AudioPlay_UnsupportedFormat;//�ļ���ʽ����WAVE
		}
		
		fptr += fmt->ChunkSize + 8;//�����ļ�ͷ
		
		while(1)
		{
			f_lseek(file,fptr);
			f_read(file,buf,sizeof(buf),&br);//��ȡ32�ֽ�
			header = (ChunkHDR*) buf;
			
			if(header->ChunkID == 0x61746164)
			{
				info->DataStartOffset = fptr + 8;//����ͷ
				break;//�ҵ�������
			}
			
			fptr += header->ChunkSize + 8;//����Ѱ����һ�� �������ļ�ͷ�Ĵ�С
			
			if(fptr > 4096)//����4k��û���ҵ�
				return AudioPlay_UnsupportedFormat;//�ļ���ʽ����WAVE
		}
	}
	else
	{
		return AudioPlay_UnsupportedFormat;//����RIFF�ļ�
	}
	
	return AudioPlay_OK;
}

AudioPlayRes WAV_Play(char* path)
{
	AudioPlayRes res = AudioPlay_OK;
	Wav_Info WavInfo;
	FIL* Wav_File = NULL;
	unsigned int br = 0xFFFF;
	
	Wav_File = &AudioFile;
	
	if(f_open(Wav_File,path,FA_READ))//���ļ�
	{
		res =  AudioPlay_OpenFileError;//���ļ�����
	}
	else//�򿪳ɹ�
	{
		res = WAV_GetInfo(Wav_File,&WavInfo);//��ȡ�ļ���Ϣ
		
		if(res == AudioPlay_OK)
		{			
			AudioPlayInfo.FileType = AudioFileType_WAV;
			AudioPlayInfo.Channels = WavInfo.nChannels;
			
			AudioPlayInfo.Samplerate = WavInfo.Samplerate;
			AudioPlayInfo.Bitrate = WavInfo.Bitrate/1000;
			AudioPlayInfo.TotalSec = (f_size(Wav_File) - WavInfo.DataStartOffset) / (WavInfo.Bitrate / WavInfo.Bps * WavInfo.nChannels);
			AudioPlayInfo.BufferSize = WAV_OUTPUT_BUFFER_SIZE * 2;

			//AudioPlayInfo.Flags |= AUDIO_FLAG_INFO_READY;
			
			AudioPlay_ClearBuf();
			
			if(AudioPlay_I2SConfig(WavInfo.Bps,WavInfo.Samplerate,WAV_OUTPUT_BUFFER_SIZE))
			{
				res = AudioPlay_UnsupportedParameter;//���ݸ�ʽ��֧��
			}
		}
	}
	
	if(res == AudioPlay_OK)
	{
		f_lseek(Wav_File,WavInfo.DataStartOffset);
		Play_Start();
	}
	
	while(res == AudioPlay_OK)
	{
		if(br < WAV_OUTPUT_BUFFER_SIZE*2)//�����ļ���
		{
			res = AudioPlay_PlayEnd;
			break;
		}
		
		AudioPlayInfo.CurrentSec = (Wav_File->fptr - WavInfo.DataStartOffset) / (WavInfo.Bitrate / WavInfo.Bps * WavInfo.nChannels);//���㲥��ʱ��
		
		/*�û������� ��ʼ*/
		res = MusicPlayingProcess();
		if(res)
			break;
		/*�û������� ����*/
		
		AudioPlay_PendSem();
		
		f_read(Wav_File,(unsigned char*)AudioPlay_GetNextBuff(),WAV_OUTPUT_BUFFER_SIZE*2,&br);//��仺����
	}
	
	Play_Stop();
	f_close(Wav_File);//�ر��ļ�
	
	return res;
}
