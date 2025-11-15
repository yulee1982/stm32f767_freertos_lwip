#ifndef __SYSTEM_MEM_H__
#define __SYSTEM_MEM_H__

#include "ff.h"
//#include "common.h"

#define WAV_OUTPUT_BUFFER_SIZE 2048
#define MP3_OUTPUT_BUFFER_SIZE 2304
#define FLAC_OUTPUT_BUFFER_SIZE 4608

extern uint32_t I2Sbuf1[];
extern uint32_t I2Sbuf2[];

extern uint32_t DecodeBuf[];
extern uint8_t FileBuf[];

extern FIL AudioFile;

extern uint8_t DataRequestFlag;

#endif
