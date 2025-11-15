#include "proc_files.h"
#include "ff.h"
#include <stddef.h>
char FileList[MAX_FILE_NUMBER_PER_DIR][13];
char FolderList[MAX_DIR_NUMBER][13];

const char* MUSIC_EXTNAME_FILTER[] = {"MP3","WAV","FLA",""};

uint8_t CheckExtName(const char* path,const char** extname)
{
	uint8_t i = 0;
	char* ch = strrchr(path,'.');
	
	if(ch == NULL)
		return 0;
	
	if(*extname[0] == '\0')
		return 1;//δָ����չ�� ��������
	
	while(*extname[i] != '\0')
	{
		if(!strncasecmp(ch+1,extname[i],strlen(extname[i])))
			return 1;
		i ++;
	}
	
	return 0;
}

uint16_t ReadRootDir(void)
{
	uint16_t cnt = 1;
	DIR dir;
	FILINFO fileinfo;
	FRESULT res = FR_OK;
	
	strcpy(FolderList[0],"");//��һ��Ŀ¼�Ǹ�Ŀ¼
	f_opendir(&dir,FolderList[0]);//�򿪸�Ŀ¼

	while(1)
	{
		res = f_readdir(&dir,&fileinfo);
		
		if(fileinfo.fname[0] == '\0' || res != FR_OK)
			break;
		
		if((fileinfo.fattrib & AM_DIR) == 0)
			continue;
		
		strcpy(FolderList[cnt],fileinfo.fname);
		
		cnt ++;
		if (cnt >= MAX_DIR_NUMBER)
			break;
	}
	
	f_closedir(&dir);
	
	return cnt;
}

uint16_t ReadFileList(char* dir_str)
{
	uint16_t cnt = 0;
	DIR dir;
	FILINFO fileinfo;
	FRESULT res = FR_OK;
	
	f_opendir(&dir,dir_str);

	while(1)
	{
		res = f_readdir(&dir,&fileinfo);
		
		if(fileinfo.fname[0] == '\0' || res != FR_OK)
			break;
		
		if(fileinfo.fattrib & AM_DIR)
			continue;
		
		if(!CheckExtName(fileinfo.fname,MUSIC_EXTNAME_FILTER))
			continue;
		
		strcpy(FileList[cnt],fileinfo.fname);
		
		cnt ++;
		if (cnt >= MAX_FILE_NUMBER_PER_DIR)
			break;
	}
	
	f_closedir(&dir);
	
	return cnt;
}

