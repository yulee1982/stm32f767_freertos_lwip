/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */

#include "tm_stm32f7_fatfs.h"
#include "sdio_sd.h"

/* Private functions */
static FRESULT scan_files(char* path, uint16_t tmp_buffer_size, TM_FATFS_Search_t* FindStructure);

FRESULT TM_FATFS_GetDriveSize(char* str, TM_FATFS_Size_t* SizeStruct) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree(str, &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    SizeStruct->TotalSize = (fs->n_fatent - 2) * fs->csize * 0.5;
    SizeStruct->FreeSize = fre_clust * fs->csize * 0.5;
	
	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_USBDriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("1:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_TruncateBeginning(FIL* fil, uint32_t index) {
	uint8_t Buffer[FATFS_TRUNCATE_BUFFER_SIZE];				/* Buffer for temporary data */

	uint32_t FileSize = f_size(fil);						/* Size of file */
	uint32_t ReadIndex = index;								/* Starting read index */
	uint32_t WriteIndex = 0;								/* We have to write at beginning */
	uint32_t TotalSize = FileSize - ReadIndex;				/* New file size after truncate */
	uint32_t NewSize = TotalSize;							/* Save new file size */
	uint32_t BlockSize;										/* Block size for read operation */
	uint32_t Read;											/* Read bytes */
	uint32_t Written;										/* Written bytes */
	FRESULT fr;												/* Result typedef */
	
	/* Index is 0 or file is empty, nothing to do */
	if (index == 0 || FileSize == 0) {
		return FR_OK;
	}
	
	/* Check if index is more than file size, truncate all */
	if (index > FileSize) {
		fr = f_lseek(fil, 0);								/* Go to beginning */
		if (fr) return fr;									/* Check for success */
		return f_truncate(fil);								/* Truncate file from new end to actual end */
	}
	
	/* Until we have available data in file after user specific index */
	while (TotalSize > 0) {
		/* Calculate new block size for new read operation */
		BlockSize = (TotalSize > FATFS_TRUNCATE_BUFFER_SIZE) ? (FATFS_TRUNCATE_BUFFER_SIZE) : (TotalSize);
	
		fr = f_lseek(fil, ReadIndex);						/* Go to the read index */
		if (fr) return fr;									/* Check for success */
		fr = f_read(fil, &Buffer, BlockSize, &Read);		/* Read data */
		if (fr) return fr;									/* Check for success */

		fr = f_lseek(fil, WriteIndex);						/* Go back to the write index */
		if (fr) return fr;									/* Check for success */
		fr = f_write(fil, &Buffer, BlockSize, &Written);    /* Write data */
		if (fr) return fr;									/* Check for success */

		TotalSize -= BlockSize;								/* Calculate new total size we have more to move everything */
		ReadIndex += Read;									/* Calculate new read pointer */
		WriteIndex += Written;								/* Calculate new write pointer */
	}
	
	fr = f_lseek(fil, NewSize);								/* Move pointer to the "end" of new file */
	if (fr) return fr;										/* Check for success */
	fr =  f_truncate(fil);									/* Truncate file from new end to actual end */
	return f_lseek(fil, 0);									/* Move pointer to the beginning */
}



FRESULT TM_FATFS_Search(char* Folder, char* tmp_buffer, uint16_t tmp_buffer_size, TM_FATFS_Search_t* FindStructure) {
	uint8_t malloc_used = 0;
	FRESULT res;
	
	/* Reset values first */
	FindStructure->FilesCount = 0;
	FindStructure->FoldersCount = 0;
	
	/* Check for buffer */
	if (tmp_buffer == NULL) {
		/* Try to allocate memory */
		tmp_buffer = (char *) LIB_ALLOC_FUNC(tmp_buffer_size);
		
		/* Check for success */
		if (tmp_buffer == NULL) {
			return FR_NOT_ENOUGH_CORE;
		}
	}
	
	/* Check if there is a lot of memory allocated */
	if (strlen(Folder) < tmp_buffer_size) {
		/* Reset TMP buffer */
		tmp_buffer[0] = 0;
		
		/* Format path first */
		strcpy(tmp_buffer, Folder);
		
		/* Call search function */
		res = scan_files(tmp_buffer, tmp_buffer_size, FindStructure);
	} else {
		/* Not enough memory */
		res = FR_NOT_ENOUGH_CORE;
	}
	
	/* Check for malloc */
	if (malloc_used) {
		LIB_FREE_FUNC(tmp_buffer);
	}
	
	/* Return result */
	return res;
}

/*******************************************************************/
/*                      FATFS SEARCH CALLBACK                      */
/*******************************************************************/
__weak uint8_t TM_FATFS_SearchCallback(char* path, uint8_t is_file, TM_FATFS_Search_t* FindStructure) {
	/* NOTE: This function Should not be modified, when the callback is needed,
             the TM_FATFS_SearchCallback could be implemented in the user file
	*/
	
	/* Allow next search */
	return 1;
}

/*******************************************************************/
/*                    FATFS PRIVATE FUNCTIONS                      */
/*******************************************************************/
static FRESULT scan_files(char* path, uint16_t tmp_buffer_size, TM_FATFS_Search_t* FindStructure) {
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	uint8_t gonext;
	char* fn;
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
	fno.lfsize = sizeof lfn;
#endif

	/* Try to open file */
	if ((res = f_opendir(&dir, path)) == FR_OK) {
		/* Get length of current path */
		i = strlen(path);

		/* Read item from card */
		while ((res = f_readdir(&dir, &fno)) == FR_OK && fno.fname[0] != 0) {

			/* Ignore files */
			if (fno.fname[0] == '.') {
				continue;
			}

			/* Format name */
	#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
	#else
			fn = fno.fname;
	#endif

			/* Check if available memory for tmp buffer */
			/* + 1 is for "/" used for path formatting */
			if ((i + strlen(fn) + 1) >= tmp_buffer_size) { 
				/* Not enough memory */
				res = FR_NOT_ENOUGH_CORE;

				break;
			}

			/* Format temporary path */
			sprintf(&path[i], "/%s", fn);

			/* Check for folder or file */
			if (fno.fattrib & AM_DIR) {
				/* We are folder */

				/* Increase number of folders */
				FindStructure->FoldersCount++;

				/* Call user function */
				gonext = TM_FATFS_SearchCallback(path, 0, FindStructure);

				/* Stop execution if user wants that */
				if (gonext) {
					/* Call recursive */
					res = scan_files(path, tmp_buffer_size, FindStructure);

					/* Check recursive scan result */
					if (res != FR_OK) {
						break;
					}
				}
			} else {
				/* We are file */

				/* Increase number of files */
				FindStructure->FilesCount++;

				/* Call user function */
				gonext = TM_FATFS_SearchCallback(path, 1, FindStructure);
			}
			
			/* Set path back */
			path[i] = 0;

			/* Stop execution if user wants that */
			if (!gonext || res != FR_OK) {
				break;
			}
		}

		/* Close directory */
		f_closedir(&dir);
	}

	/* Return result */
	return res;
}


//---------------for test
#define TEST_RW_SD

#ifdef TEST_RW_SD

static FATFS fs;                         /* FatFs文件系统对象 */
static FIL fnew;                         /* 文件对象 */
static FRESULT res_sd;                   /* 文件操作结果 */
static UINT fnum;                        /* 文件成功读写数量 */
static BYTE ReadBuffer[256]= {0};        /* 读缓冲区 */
static BYTE WriteBuffer[] =              /* 写缓冲区*/
        "Welcome to STM32 FatFs, This is a test file!";


void sd_card_fatfs_test(void)
{
  printf("%s\t%d\r\n", __FUNCTION__ , __LINE__);
  res_sd = f_mount(&fs, "1:", 1);
  printf("%s\t%d\r\n", __FUNCTION__ , __LINE__);
  /*----------------------- 格式化测试 -----------------------*/
  /* 如果没有文件系统就格式化创建创建文件系统 */
  if(res_sd == FR_NO_FILESYSTEM)
  {
    printf(">> SD Card don't have FatFs Type File System!\r\nCreate SD Card FAT32 File System...\r\nPlease Wait some time!\r\n");
    /* 格式化 */
    //res_sd=f_mkfs("0:", 0, NULL, 0);
    MKFS_PARM opt = {0};
    opt.fmt = FM_FAT32; // 选择FAT32格式
    opt.n_fat = 2; // 设定FAT副本数量，通常为2
    opt.align = 1; // 数据区域对齐，默认为1，表示按扇区对齐
    // opt.n_root     不需要设置，因为我们使用的是FAT32，它没有固定大小的根目录
    opt.au_size = 512; // 每簇大小，以字节为单位，这里是假设每簇为1个扇区，即512字节

    //res_sd = f_mkfs("0:", &opt, NULL, 0);
    #define FORMAT_WORK_BUF_SZ 4096         // 根据需要调整
    BYTE formatWorkBuf[FORMAT_WORK_BUF_SZ];
    res_sd = f_mkfs("1:", &opt, formatWorkBuf, FORMAT_WORK_BUF_SZ);
    //res_sd = f_mkfs("0:", &opt, NULL, 4096); //根据实际需求调整
    if(res_sd == FR_OK)
    {
      printf(">> SD Card Create File System success! \r\n");
      /* 格式化后，先取消挂载 */
      res_sd = f_mount(NULL,"1:",0);
      /* 重新挂载 */
      res_sd = f_mount(&fs,"1:",1);
    }else{
      printf(">> Create file system failed! %d \r\n", res_sd);
      //while(1);
      return;
    }
  }
  else if(res_sd != FR_OK) {
    printf(">> SD Card Mount File System failed! %d \r\n",res_sd);
    //while(1);
    return;
  }
  else{
    printf(">> Mount file system success! \r\n");
  }

  /*----------------------- 文件系统测试：写测试 -----------------------*/
  printf("\r\n---------------FatFs Write Test---------------\r\n");
  /* 打开文件，如果文件不存在则创建它 */
  res_sd = f_open(&fnew, "1:FatFs_test.txt",FA_CREATE_ALWAYS | FA_WRITE );
  if(res_sd == FR_OK)
  {
    printf(">> Open Or Create file success, write data...\r\n");
    /* 将指定存储区内容写入到文件内 */
    res_sd = f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_sd==FR_OK)
    {
      printf(">> Write data Success：%d\r\n",fnum);
      printf(">> Write data:\r\n%s\r\n",WriteBuffer);
    }else{
      printf("Write data failed! (%d)\r\n",res_sd);
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);
  }else{
    printf("Open Or Create file failed!\r\n");
  }
  /*----------------------- 文件系统测试：读测试 -----------------------*/
  printf("--------------- Read file test ---------------\r\n");
  res_sd = f_open(&fnew, "1:FatFs_test.txt", FA_OPEN_EXISTING | FA_READ);
  if(res_sd == FR_OK)
  {
    printf(">> Open success...\r\n");
    res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
    if(res_sd == FR_OK)
    {
      printf(">>Read file data success%d\r\n",fnum);
      printf(">>Read data:\r\n%s \r\n", ReadBuffer);
    }else{
      printf("Read file failed! (%d)\r\n",res_sd);
    }
  }else{
    printf("Open file failed!\r\n");
  }

  /* 不再读写，关闭文件 */
  f_close(&fnew);

  /* 不再使用文件系统，取消挂载文件系统 */
  f_mount(NULL,"1:",1);

}
#endif
