//#include "common.h"
#include "proc_player.h"
#include "proc_files.h"
//#include "proc_key_led.h"
#include "audio_common.h"
#include "diskio.h"
#include <stdlib.h>
#include <string.h>

uint8_t Volume = 15, PlayMode = 0;//0Ϊ˳�򲥷� 1Ϊ����ѭ�� 2Ϊ�������
uint16_t NumberOfMusics, CurrentMusic = 0;
uint16_t NumberOfFolders, CurrentFolder = 0;

#define RANDOM_HIS_MAX_CNT 10

AudioPlayRes LastValidPlayRes;
uint8_t RandomPlay_HistoryTabCnt = 0;
uint16_t RandomPlay_HistoryTab[RANDOM_HIS_MAX_CNT];


void RandomPlay_Reset(void)
{
	RandomPlay_HistoryTabCnt = 0;
	
	memset(RandomPlay_HistoryTab,0,sizeof(RandomPlay_HistoryTab));
}

uint8_t RandomPlay_CheckRepetation(uint16_t tracknum)
{
	uint8_t i;
	
	for(i = 0;i < RandomPlay_HistoryTabCnt;i ++)
	{
		if(tracknum == RandomPlay_HistoryTab[i])
			return 1;
	}
	
	return 0;
}

void RandomPlay_InsertTrackNumber(uint16_t tracknum)
{
	uint8_t num = NumberOfMusics-1<RANDOM_HIS_MAX_CNT ? NumberOfMusics-1:RANDOM_HIS_MAX_CNT;//��ȡ��ǰӦ�������������С Ҫ�ȵ�ǰ�ļ����ڸ�����һ��
	
	if(RandomPlay_HistoryTabCnt < num)//��ǰ��û�б�����
	{
		RandomPlay_HistoryTab[RandomPlay_HistoryTabCnt] = tracknum;//ֱ�Ӳ���
		RandomPlay_HistoryTabCnt ++;
	}
	else
	{
		memmove(RandomPlay_HistoryTab,&RandomPlay_HistoryTab[1],(num-1)*sizeof(RandomPlay_HistoryTab[0]));
		RandomPlay_HistoryTab[num-1] = tracknum;
	}
}

uint16_t RandomPlay_GetNextTrack(void)
{
	uint16_t new_track;
	
	RandomPlay_InsertTrackNumber(CurrentMusic);
	while(1)
	{
		new_track = rand() % NumberOfMusics;//��ȡ����Ŀ
		if(RandomPlay_CheckRepetation(new_track) == 0)//���û���ظ�
			break;
	}
	
	return new_track;
}

uint16_t RandomPlay_GetPrevTrack(void)
{
	if(RandomPlay_HistoryTabCnt)
	{
		RandomPlay_HistoryTabCnt --;
	}
	else//�Ѿ��ǵ�һ����
	{
		
		//LED_SetBlinkPattern(LED_BLINK_RED_2TIMES);
	}
		
	return RandomPlay_HistoryTab[RandomPlay_HistoryTabCnt];
}

AudioPlayRes MusicPlayingProcess(void)
{
	static uint8_t vol_last = 0xFF,sd_check_counter = 0;
	
	if(AudioPlayInfo.Flags & AUDIO_FLAG_PAUSED)
	{
		Play_Stop();
		
		while(AudioPlayInfo.Flags & AUDIO_FLAG_PAUSED)
		{
			__asm ("wfi");
			
			sd_check_counter ++;
			if(sd_check_counter > 10)
			{
				sd_check_counter = 0;
				if(disk_status(0) == RES_ERROR)//SD�����γ�
				{
					return AudioPlay_ReadFileError;
				}
			}
			
			if(AudioPlayInfo.Flags & AUDIO_FLAG_ABORT)
			{
				return AudioPlay_Exit;
			}
			
			if(AudioPlayInfo.PlayRes)
			{
				return AudioPlayInfo.PlayRes;
			}
		}
		
		Play_Start();
	}
	
	if(AudioPlayInfo.PlayRes)
	{
		return AudioPlayInfo.PlayRes;
	}
	
	return AudioPlay_OK;
}

void MusicSwitchProc(void)
{
	if(AudioPlayInfo.PlayRes == AudioPlay_PlayEnd)//�����������
	{
		if(PlayMode == 0)//˳�򲥷�
		{
			if(CurrentMusic >= NumberOfMusics-1)//���һ����
			{
				CurrentMusic = 0;
			}
			else
			{
				CurrentMusic ++;//��һ��
			}
		}
		else if(PlayMode == 1)//����ѭ��
		{
			//��������ͬһ��
		}
		else if(PlayMode == 2)//�������
		{
			CurrentMusic = RandomPlay_GetNextTrack();
		}
	}
	else if(AudioPlayInfo.PlayRes == AudioPlay_Next)
	{
		if(PlayMode == 0 || PlayMode == 1)//˳�򲥷Ż��ߵ���ѭ��
		{
			if(CurrentMusic >= NumberOfMusics-1)//���һ����
			{
				CurrentMusic = 0;
			}
			else
			{
				CurrentMusic ++;//��һ��
			}
		}
		else if(PlayMode == 2)//�������
		{
			CurrentMusic = RandomPlay_GetNextTrack();
		}
	}
	else if(AudioPlayInfo.PlayRes == AudioPlay_Prev)
	{
		if(PlayMode == 0 || PlayMode == 1)//˳�򲥷Ż��ߵ���ѭ��
		{
			if(CurrentMusic == 0)//��һ��
			{
				CurrentMusic = NumberOfMusics-1;
			}
			else
			{
				CurrentMusic --;//��һ��
			}
		}
		else if(PlayMode == 2)//�������
		{
			CurrentMusic = RandomPlay_GetPrevTrack();
		}
	}
}

void FolderSwitchProc(void)
{
	if(AudioPlayInfo.PlayRes == AudioPlay_NextFolder)//��һ���ļ���
	{
		if(CurrentFolder >= NumberOfFolders-1)
		{
			CurrentFolder = 0;
		}
		else
		{
			CurrentFolder ++;
		}
		CurrentMusic = 0;
	}	
	else if(AudioPlayInfo.PlayRes == AudioPlay_PrevFolder)//��һ���ļ���
	{
		if(CurrentFolder)
		{
			CurrentFolder --;
		}
		else
		{
			CurrentFolder = NumberOfFolders-1;
		}
		CurrentMusic = 0;
	}
}

AudioPlayRes MP3_Player(void)
{	
	static uint8_t file_error_cnt;
	
	NumberOfFolders = ReadRootDir();
	
	CurrentMusic = 0;
	CurrentFolder = 0;
	
	while(1)//�ļ����л�
	{
		FolderSwitchProc();//�л��ļ���
		
		NumberOfMusics = ReadFileList(FolderList[CurrentFolder]);//�л��ļ���
		
		if(NumberOfMusics == 0)//��ǰ�ǿ��ļ���
		{
			continue;//����ִ����һ�ε��ļ��в���
		}
		
		file_error_cnt = 0;//��λ�������
		RandomPlay_Reset();//��λ�������
		LastValidPlayRes = AudioPlay_Next;//Ĭ����һ��
		while(1)//��Ŀ�л�
		{
			MusicSwitchProc();//�л�����
			
			AudioPlayFile(FolderList[CurrentFolder],FileList[CurrentMusic]);//����
			
			if(AudioPlayInfo.PlayRes == AudioPlay_NextFolder || AudioPlayInfo.PlayRes == AudioPlay_PrevFolder)//���л��ļ��� ���˳�ѭ��
			{
				break;
			}
			else if(AudioPlayInfo.PlayRes == AudioPlay_Exit || AudioPlayInfo.PlayRes == AudioPlay_OpenFileError || AudioPlayInfo.PlayRes == AudioPlay_ReadFileError)//���Ž���
			{
				return AudioPlayInfo.PlayRes;
			}
			else if(AudioPlayInfo.PlayRes == AudioPlay_Next || AudioPlayInfo.PlayRes == AudioPlay_Prev)
			{
				LastValidPlayRes = AudioPlayInfo.PlayRes;//������һ�ε���������
			}
			else if(AudioPlayInfo.PlayRes != AudioPlay_OK && AudioPlayInfo.PlayRes != AudioPlay_PlayEnd)//���ų���
			{
				AudioPlayInfo.PlayRes = LastValidPlayRes;//������һ�β������²��� �ﵽ������Ч��
				file_error_cnt ++;
				
				if(file_error_cnt > 5)//��������5�δﵽ��ֵ
				{
					//���������
				}
			}
		}
	}
}
