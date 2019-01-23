/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "stm32f10x_tim.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );
/*
��ʼ��ʱ��xMBPortTimersInit()
��Ӵ򿪺͹ر�ʱ�ӵĺ���void vMBPortTimersEnable(  )
�Լ�voidvMBPortTimersDisable(  )��
���г�ʱ�жϺ���voidTIM2_IRQHandler(void)��
*/
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	TIM_DeInit(TIM2); //���½�Timer3����Ϊȱʡֵ
  uint16_t PrescalerValue = 0;
  
  //ʹ�ܶ�ʱ��2ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
  
  //��ʱ��ʱ�������˵��
  //ϵͳʱ��Ϊ72MHz��APB1����2��ƵΪ36MHz
  //TIM2��ʱ�ӱ�Ƶ��Ϊ72MHz��Ӳ���Զ���Ƶ,�ﵽ���
  //TIM2�ķ�Ƶϵ��Ϊ3599��ʱ���Ƶ��Ϊ72 / (1 + Prescaler) = 20KHz,��׼Ϊ50us
  //TIM������ֵΪusTim1Timerout50us
  PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
  //��ʱ��1��ʼ��
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  //Ԥװ��ʹ��
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //��ʱ��2�ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //�������жϱ�־λ
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  //��ʱ��2����жϹر�
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  //��ʱ��2����
  TIM_Cmd(TIM2,  DISABLE);
  return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
	
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //�趨��ʱ��2�ĳ�ʼֵ
  TIM_SetCounter(TIM2,0x0000); 
  //��ʱ��2����
  TIM_Cmd(TIM2, ENABLE);
	
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
}

inline void
vMBPortTimersDisable(  )
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM2,0x0000); 
  //�رն�ʱ��2
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
    //�����ʱ��T4����жϱ�־λ
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    prvvTIMERExpiredISR( );
  }
}
