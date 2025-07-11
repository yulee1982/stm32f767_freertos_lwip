#ifndef __BUTTON_H__
#define __BUTTON_H__


#define BUTTON_TASK_PRIO         0      //任务优先级
#define BUTTON_STK_SIZE          128    //任务堆栈大小

void prvButtonTask( void * pvParameters );  //任务函数
void prvLedBreathTask( void * pvParameters );  //任务函数
#endif
