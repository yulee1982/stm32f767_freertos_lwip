
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* App includes. */
#include "button.h"
#include "main.h"

#define USER_BUTTON_PIN                         LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                   GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC)
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_13
#define USER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)
#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);                  \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI15_10_IRQHandler

//Key Time Define
#define KEY_TIME_DOWN               2
#define KEY_TIME_PRESS_START        20  //20
#define KEY_TIME_PRESS_STEP         6   //6
#define KEY_TIME_DOUBLEDOWN         6   //200ms

//Key Status Define
#define KEY_STATUS_NONE             0xffffffff
#define KEY_STATUS_DOWN             1
#define KEY_STATUS_PRESS_START      2
#define KEY_STATUS_PRESS            3
#define KEY_STATUS_SHORT_UP         4
#define KEY_STATUS_LONG_UP          5
#define KEY_STATUS_DOUBLEDOWN       6
//Key Val Define
#define KEY_VAL_NONE                0x00000000

#define KEY_VAL_KEY1_DOWN           ((1 << 0)|(KEY_STATUS_DOWN << 8))
#define KEY_VAL_KEY1_PRESS_START    ((1 << 0)|(KEY_STATUS_PRESS_START << 8))
#define KEY_VAL_KEY1_PRESS          ((1 << 0)|(KEY_STATUS_PRESS << 8))
#define KEY_VAL_KEY1_SHORT_UP       ((1 << 0)|(KEY_STATUS_SHORT_UP << 8))
#define KEY_VAL_KEY1_LONG_UP        ((1 << 0)|(KEY_STATUS_LONG_UP << 8))
#define KEY_VAL_KEY1_DOUBLEDOWN     ((1 << 0)|(KEY_STATUS_DOUBLEDOWN << 8))

typedef struct
{
  volatile uint8_t keypressed;
  volatile uint8_t bholdon;
  volatile uint8_t reserved;
}KEY_FLAG;

KEY_FLAG keyflag = {0,0,0};
volatile static uint32_t keyval;
volatile static uint32_t prevkeyval;
volatile static uint32_t keystatus;
volatile static uint32_t keyscancnt;
volatile static uint32_t keydoublecnt;

static void user_button_init(void);
static void keyinit(void);
//static void keyreset(void);
static uint32_t getkeyval(void);
static void keyscan(void);
static void keyproc(void);

void prvLedBreathTask( void * pvParameters )
{
  for(;;)
  {
    //LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    //vTaskDelay(400);
    LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
    vTaskDelay(100);
    LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
    vTaskDelay(100);
    LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
    vTaskDelay(100);
    LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
    vTaskDelay(1200);
  }
}
void prvButtonTask( void * pvParameters )
{

  user_button_init();
  keyinit();
  for(;;)
  {
    keyscan();
    keyproc();
    vTaskDelay(20);
  }

}

static void user_button_init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger EXTI_Line15_10 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 3);
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
}

static void keyinit(void)
{
  keyval = 0;
  prevkeyval = 0;
  keystatus = 0;
  keyscancnt = 0;
  keyflag.keypressed = 0;
  keyflag.reserved = 0;
  keyflag.bholdon = 0;
  keydoublecnt = 0;
}
#if 0
void keyreset(void)
{
  keystatus = KEY_STATUS_NONE;
  keyval = KEY_VAL_NONE;
  keyflag.keypressed = 0;
  keyflag.reserved = 0;
  keyflag.bholdon = 0;
  keydoublecnt = 0;
}
#endif
static uint32_t getkeyval(void)
{
  uint32_t tempkeyval = 0;

  if(LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN) == 1)
    tempkeyval |= (1<<0);

  return tempkeyval;
}
static void keyscan(void)
{
  uint32_t tempkeyval;

  if(keyflag.keypressed == 0){
    tempkeyval = getkeyval();
    if (tempkeyval != KEY_VAL_NONE){
      if (tempkeyval != prevkeyval){
        //--------------------------------------------------
        if(keyscancnt > KEY_TIME_PRESS_START){
          keyflag.keypressed = 1;
          keyval = (KEY_STATUS_LONG_UP << 8) | (keyval & 0x00000003);
        }else if(keyscancnt > KEY_TIME_DOWN){
          keyflag.keypressed = 1;
          keyval = (KEY_STATUS_SHORT_UP << 8) | (keyval & 0x00000003);
        }
        //--------------------------------------------------
        keyscancnt = 0;
        keystatus = KEY_STATUS_DOWN;
      }else{
        if(++keyscancnt == 0){
          keyscancnt = KEY_TIME_PRESS_START + KEY_TIME_PRESS_STEP;
        }else if(keyscancnt == KEY_TIME_DOWN){
          ;
        }
      }

      switch(keystatus){
        case KEY_STATUS_DOWN:
          if(keyscancnt == KEY_TIME_DOWN){
            keyflag.keypressed = 1;
            keyval = (KEY_STATUS_DOWN << 8) | tempkeyval;
          }else if(keyscancnt == KEY_TIME_PRESS_START){
            keystatus = KEY_STATUS_PRESS;
            keyflag.keypressed = 1;
            keyval = (KEY_STATUS_PRESS_START << 8) | tempkeyval;
          }
          break;

        case KEY_STATUS_PRESS:
          if(((keyscancnt - KEY_TIME_PRESS_START) % KEY_TIME_PRESS_STEP) == 0){
            keyflag.keypressed = 1;
            keyval = (KEY_STATUS_PRESS << 8) | tempkeyval;
          }
          break;

        default:
          //;keyscancnt = 0;
           break;
      }
    }else{
      keyflag.bholdon = 0;
      if(keyscancnt > KEY_TIME_PRESS_START){
        keyflag.keypressed = 1;
        keyval = (KEY_STATUS_LONG_UP << 8) | (keyval & 0x0003);
      }else if(keyscancnt > KEY_TIME_DOWN) {
        keyflag.keypressed = 1;
        keyval = (KEY_STATUS_SHORT_UP << 8) | (keyval & 0x0003);
      }
      if(keydoublecnt > 0)
        keydoublecnt--;

      keyscancnt = 0;
      keystatus = KEY_STATUS_NONE;
    }
    prevkeyval = tempkeyval;
  }
}

static void keyproc(void)
{
  //uint32_t tempkeyval;

  if(keyflag.keypressed == 0)
    return;
  else
    keyflag.keypressed = 0;

  if(keyflag.bholdon)
    return;

  switch (keyval)
  {
    case KEY_VAL_KEY1_DOWN:
      if(keydoublecnt > 0)
      {
        keydoublecnt = 0; //双击
        LL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        break;
      }
      //----------------------------------------
      LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
      break;

    case KEY_VAL_KEY1_SHORT_UP:
      keydoublecnt = KEY_TIME_DOUBLEDOWN;
      break;

    case KEY_VAL_KEY1_PRESS_START:
      keyflag.bholdon |= 1<<0;
	  break;

    case KEY_VAL_KEY1_LONG_UP:
      break;

    default:
      break;
  }
  //keyflag.keypressed = 0;
}
