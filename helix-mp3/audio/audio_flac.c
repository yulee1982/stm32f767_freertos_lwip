//TRAXMOD Digital Audio Player
// 
//Copyright (c) 2009, K9spud LLC.
// http://www.k9spud.com/traxmod/
//
// Largely based on main.c from the Rockbox FLAC test decoder program, which is
// Copyright (c) 2005, Dave Chapman
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
/*******************************************************************************/

#include "flacdecoder.h"
#include "audio_flac.h"
#include "audio_common.h"
#include "ff.h"
#include <string.h>
#include "system_mem.h"
//#include "system_i2c.h"
//#include "common.h"


AudioPlayRes FLAC_GetInfo(FIL* file, FLACContext* fc)
{
	AudioPlayRes res;
	uint8_t found_streaminfo = 0;
	uint8_t endofmetadata = 0;
	uint32_t blocklength;
	uint8_t* buff;
	UINT br;
	
	buff = FileBuf;//�����ļ�������
	
	f_read(file,buff,4,&br);
	
	if(br<4)
	{
		res = AudioPlay_ReadFileError;
		goto END;
	}

	if (memcmp(buff, "fLaC", 4) != 0)
	{
		res = AudioPlay_UnsupportedFormat;
		goto END;
	}
	
	fc->metadatalength = 4;

	while (!endofmetadata) 
	{
		f_read(file, buff, 4,&br);
		if (br < 4)
		{
			res = AudioPlay_ReadFileError;
			goto END;
		}

		endofmetadata = (buff[0]&0x80);
		blocklength = (buff[1] << 16) | (buff[2] << 8) | buff[3];
		fc->metadatalength+=blocklength+4;

		if ((buff[0] & 0x7f) == 0)       /* 0 is the STREAMINFO block */
		{
			/* FIXME: Don't trust the value of blocklength */
			f_read(file, buff, blocklength,&br); 
			if(br == 0)
			{
				res = AudioPlay_ReadFileError;
				goto END;
			}
		
			fc->filesize = f_size(file);
			fc->min_blocksize = (buff[0] << 8) | buff[1];
			fc->max_blocksize = (buff[2] << 8) | buff[3];
			fc->min_framesize = (buff[4] << 16) | (buff[5] << 8) | buff[6];
			fc->max_framesize = (buff[7] << 16) | (buff[8] << 8) | buff[9];
			fc->samplerate = (buff[10] << 12) | (buff[11] << 4) 
											 | ((buff[12] & 0xf0) >> 4);
			fc->channels = ((buff[12]&0x0e)>>1) + 1;
			fc->bps = (((buff[12]&0x01) << 4) | ((buff[13]&0xf0)>>4) ) + 1;

			/* totalsamples is a 36-bit field, but we assume <= 32 bits are 
				 used */
			fc->totalsamples = (buff[14] << 24) | (buff[15] << 16) 
												 | (buff[16] << 8) | buff[17];

			found_streaminfo = 1;
		}
		
		else if((buff[0] & 0x7f) == 3)
		{
			/* 3 is the SEEKTABLE block */
			while (blocklength >= 18) 
			{
				f_read(file, buff, 18,&br);
				if(br < 18) 
				{
					res = AudioPlay_OK;
					goto END;
				}
				blocklength -= br;
			}
			
			f_lseek(file,file->fptr + blocklength);
		}
		else 
		{
			// Skip to next metadata block
			f_lseek(file,file->fptr +blocklength);
			if(file->fptr >= f_size(file))
			{
				res = AudioPlay_DamagedFile;
				goto END;
			}
		}
	}

	if (found_streaminfo) 
	{
		fc->bitrate = ((fc->filesize - fc->metadatalength) * 8) / ((fc->totalsamples / fc->samplerate) * 1000);
		res = AudioPlay_OK;
		goto END;
	} 
	else 
	{
		res = AudioPlay_DamagedFile;
		goto END;
	}
	
	END:
	return res;
}

AudioPlayRes FLAC_Play(char * filename)
{
	FLACContext __fc;
	FLACContext* fc = &__fc;
	FIL* flacfile = &AudioFile;
	int32_t * FLAC_OUTPUT_BUFFER;
	uint8_t * FLAC_INPUT_BUFFER;
	void* pOutputBuffer = NULL;
	AudioPlayRes res = AudioPlay_OK;
	FRESULT fres;
	int decode_res;
	UINT br;
	uint16_t k;
	uint32_t consumed,bytesleft;
	
	memset(fc,0,sizeof(FLACContext));
	memset(flacfile,0,sizeof(FIL));
		
	if(f_open(flacfile, filename, FA_READ) != FR_OK)
	{
		res = AudioPlay_OpenFileError;
	}
	else
	{
		res = FLAC_GetInfo(flacfile, fc);//��ȡflac�ļ���Ϣ
	}
	
	if(res == AudioPlay_OK)
	{	
		AudioPlayInfo.FileType = AudioFileType_FLAC;
		AudioPlayInfo.Channels = fc->channels;
		
		AudioPlayInfo.TotalSec = fc->totalsamples/fc->samplerate;
		AudioPlayInfo.Samplerate = fc->samplerate;
		AudioPlayInfo.Bitrate = fc->bitrate;
		AudioPlayInfo.BufferSize = fc->max_blocksize*4;
		
		//AudioPlayInfo.Flags |= AUDIO_FLAG_INFO_READY;
		
		AudioPlay_ClearBuf();
		
		if(AudioPlay_I2SConfig(fc->bps,fc->samplerate,fc->max_blocksize*2) || fc->max_blocksize != fc->min_blocksize)
		{
			res = AudioPlay_UnsupportedParameter;
		}
	}
	
	if(res == AudioPlay_OK)
	{
		FLAC_OUTPUT_BUFFER = (int32_t *)DecodeBuf;//�������������
		FLAC_INPUT_BUFFER = (uint8_t *)FileBuf;//�������뻺����
		
		f_lseek(flacfile,fc->metadatalength);//�������ݿ�ʼ��
		fres = f_read(flacfile, FLAC_INPUT_BUFFER,fc->max_framesize,&br);
		if(fres)
		{
			res = AudioPlay_ReadFileError;
		}
		else
		{
			bytesleft = br;
			AudioPlay_ClearSem();
			Play_Start();
		}
	}
	
	while(res == AudioPlay_OK)
	{
		decode_res = flac_decode_frame(fc, FLAC_OUTPUT_BUFFER, FLAC_INPUT_BUFFER, bytesleft);//�ú���Ϊ24λȡ����� ������bufΪ32λ ʵ�������16λ��Ƶ ��Ҫ��decoded_buf������
		if(decode_res < 0)//���뷢������
		{
			res = AudioPlay_DamagedFile;
			break;
		}
		
		consumed = fc->gb.index / 8;
		memcpy(FLAC_INPUT_BUFFER, FLAC_INPUT_BUFFER + consumed, bytesleft - consumed);
		bytesleft = bytesleft - consumed;
		fres = f_read(flacfile, FLAC_INPUT_BUFFER + bytesleft, consumed, &br);
		bytesleft += br;
		if(fres != FR_OK)
		{
			res = AudioPlay_ReadFileError;
			break;
		}
		
		pOutputBuffer = AudioPlay_GetNextBuff();
		if(fc->bps == 16)//���������Ϊ32bit ������ԴΪ16bit��Ҫ���¶���
		{
			for(k = 0;k < fc-> max_blocksize * 2;k ++)
			{
				((int16_t*)pOutputBuffer)[k] = FLAC_OUTPUT_BUFFER[k];
			}
		}
		else//�������
		{
			for(k=0;k < fc->max_blocksize * 2;k ++)
			{
				((int32_t*)pOutputBuffer)[k] = FLAC_OUTPUT_BUFFER[k];
			}
		}
		
		/*�û�������*/
		res = MusicPlayingProcess();
		if(res)
			break;
		/*�û�������*/
		
		AudioPlay_PendSem();
		AudioPlayInfo.CurrentSec = AudioPlayInfo.TotalSec * fc->samplenumber/fc->totalsamples;
		
		if(br < consumed)
		{
			res = AudioPlay_PlayEnd;
			break;//��������
		}
	}
	
	Play_Stop();
	f_close(flacfile);
	
	return res;
}
