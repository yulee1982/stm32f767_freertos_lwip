#ifndef __PROC_FILES_H__
#define __PROC_FILES_H__

//#include "common.h"
#include <stdint.h>

#define MAX_FILE_NUMBER_PER_DIR 512
#define MAX_DIR_NUMBER 16

extern char FileList[MAX_FILE_NUMBER_PER_DIR][13];
extern char FolderList[MAX_DIR_NUMBER][13];

uint16_t ReadRootDir(void);
uint16_t ReadFileList(char* dir_str);

#endif
