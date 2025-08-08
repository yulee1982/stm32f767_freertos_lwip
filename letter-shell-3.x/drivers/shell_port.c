/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
//#include "portmacro.h"

#include "shell.h"
#include "shell_port.h"

#include "stm32f767xx.h"
#include "usart.h"
#include <stdio.h>

//#include "serial.h"
//#include "stm32f4xx_hal.h"
//#include "cevent.h"
//#include "log.h"


Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

SemaphoreHandle_t Usart2_BinarySem_Handle;
uint8_t recv_buf[4];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
  //serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
  Usart_Transmit(USART2, (const uint8_t *)data, (uint16_t)len, 0x1FF);
  //if(Usart_Transmit(USART2, (const uint8_t *)data, (uint16_t)len, 0x1FF) != USART_OK)
  //  return 0;
  //else
  //  return 1;
  return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
  //return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
  //Usart_Receive(USART2, (uint8_t *)data, (uint16_t)len, 0x1FF);
  //return len;
  if(Usart_Receive(USART2, (uint8_t *)data, (uint16_t)len, 0xFFFF) != USART_OK)
    return 0;
  else
    return 1;
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
  //xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY); //递归互斥量获取函数
  xSemaphoreTake(shellMutex, portMAX_DELAY);
  return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
  //xSemaphoreGiveRecursive(shellMutex); //递归互斥量
  xSemaphoreGive(shellMutex);
  return 0;
}


void shellTask1(void *param)
{
  Shell *shell = (Shell *)param;
  while(1)
  {
    xSemaphoreTake(Usart2_BinarySem_Handle, 0xffff);
    if(Usart_Receive_IT_Get_Flag() != 0)
    {
      shellHandler(shell, recv_buf[0]);
    }
    Usart_Receive_IT(USART2, (uint8_t*)recv_buf, 1);
  }
}
void USART_RxCpltCallback(USART_TypeDef *pUSARTx)
{
  BaseType_t pxHigherPriorityTaskWoken;
  xSemaphoreGiveFromISR(Usart2_BinarySem_Handle,&pxHigherPriorityTaskWoken);
}
/**
 * @brief 用户shell初始化
 * 
 */
SemaphoreHandle_t Usart2_BinarySem_Handle;
void userShellInit(void)
{
  shellMutex = xSemaphoreCreateMutex();

  shell.write = userShellWrite;
  shell.read = userShellRead;
  shell.lock = userShellLock;
  shell.unlock = userShellUnlock;
  shellInit(&shell, shellBuffer, 512);
  Usart2_BinarySem_Handle = xSemaphoreCreateBinary();
  //xSemaphoreGive(Usart2_BinarySem_Handle);
  Usart_Receive_IT(USART2, (uint8_t*)recv_buf, 1);
  if(xTaskCreate(shellTask1, "shell", 256, &shell, 5, NULL) != pdPASS)
  {
    printf("shell task creat failed");
  }
}
CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

