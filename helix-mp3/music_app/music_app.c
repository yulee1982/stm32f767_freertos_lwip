
//USE_LIBUSB_STM32 or USE_SIMPLE_USB or USE_CHERRY_USB Choose one of the three

#include "music_app.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

#include "stm32f767xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "pcm5102.h"
#include "audio_common.h"
#include "proc_player.h"

uint32_t *mix_buffer = 0;
FATFS fatfs;
//--------------------------------------------------------------------+
// Main
//--------------------------------------------------------------------+
void prvMusicappTask(void * pvParameters)
{
  uint8_t error_cnt = 0,res_sd;

  pcm5102_init();
  //mix_buffer = (uint32_t*)calloc(3 * 1152, sizeof(uint32_t));
  //MP3_Play_ROM(0x08010000);

  while(1)
  {

    res_sd = f_mount(&fatfs, "1:", 1);//尝试挂载文件系统

	if(AudioPlayInfo.PlayRes == AudioPlay_Exit)//播放器退出
	{
      //添加播放推出功能
      ;
	}

    if(res_sd)//失败
    {
      vTaskDelay(1000);
      if(error_cnt++ > 100)
      {
        error_cnt = 0;
        //wait add 出错处理
      }
      continue;
    }
    error_cnt = 0;

    MP3_Player();
  }
}





