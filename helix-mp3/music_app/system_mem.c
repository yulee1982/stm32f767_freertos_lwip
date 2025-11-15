#include "system_mem.h"

uint32_t I2Sbuf1[FLAC_OUTPUT_BUFFER_SIZE];
uint32_t I2Sbuf2[FLAC_OUTPUT_BUFFER_SIZE];

uint32_t DecodeBuf[FLAC_OUTPUT_BUFFER_SIZE*2];
uint8_t FileBuf[40960];

FIL AudioFile;
