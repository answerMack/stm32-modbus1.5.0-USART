/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "stm32f10x_tim.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );
/*
初始化时钟xMBPortTimersInit()
添加打开和关闭时钟的函数void vMBPortTimersEnable(  )
以及voidvMBPortTimersDisable(  )，
还有超时中断函数voidTIM2_IRQHandler(void)。
*/
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	TIM_DeInit(TIM2); //重新将Timer3设置为缺省值
  uint16_t PrescalerValue = 0;
  
  //使能定时器2时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
  
  //定时器时间基配置说明
  //系统时钟为72MHz，APB1经过2分频为36MHz
  //TIM2的时钟倍频后为72MHz（硬件自动倍频,达到最大）
  //TIM2的分频系数为3599，时间基频率为72 / (1 + Prescaler) = 20KHz,基准为50us
  //TIM最大计数值为usTim1Timerout50us
  PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
  //定时器1初始化
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  //预装载使能
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //定时器2中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //清除溢出中断标志位
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  //定时器2溢出中断关闭
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  //定时器2禁能
  TIM_Cmd(TIM2,  DISABLE);
  return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
	
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //设定定时器2的初始值
  TIM_SetCounter(TIM2,0x0000); 
  //定时器2启动
  TIM_Cmd(TIM2, ENABLE);
	
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM2,0x0000); 
  //关闭定时器2
  TIM_Cmd(TIM2, DISABLE);
  /* Disable any pending timers. */
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    //清除定时器T4溢出中断标志位
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    prvvTIMERExpiredISR( );
  }
}
