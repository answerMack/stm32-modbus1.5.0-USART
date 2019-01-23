#include "iwdg.h"
#include "sys.h"




/*
���Ź�����Ϊ��ֹ������ܷɣ���������ѭ����������������б����
���ڶԵ�Ƭ������״̬����ʵʱ���Ŀ��ǣ�������ר�����ڼ�ⵥƬ����������״̬��ģ�����оƬ���׳ơ����Ź���(watchdog) ��
��OXFFFF������0x0000�����һ��RESET��Ϊ��ֹ����RESET����Ҫι��������KRд��0xAAAA�ؼ�����ֵ
prer Ԥ��Ƶϵ���� RLR Ϊ�Զ���װ��ֵ
void iwdg_init(4,625)  ԼΪ1s RESET
//���ʱ�����
Tout=����4x2^prer��xRLR��/40  ��λ��S��

*/
void iwdg_init(u8 prer,u16 RLR){

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);
	IWDG_SetReload(RLR);
	IWDG_ReloadCounter();	
	IWDG_Enable();

}

/*ι������*/

void iwdg_feed(void){

IWDG_ReloadCounter();

}

//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). Ƶ��
/*
���ڿ��Ź� ʹ��ǰ��Ҫ�����жϷ���
���磺
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	   
*/
void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDGʱ��ʹ��

	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.   
	WWDG_SetPrescaler(fprer);////����IWDGԤ��Ƶֵ

	WWDG_SetWindowValue(wr);//���ô���ֵ

	WWDG_Enable(WWDG_CNT);	 //ʹ�ܿ��Ź� ,	���� counter .                  

	WWDG_ClearFlag();//�����ǰ�����жϱ�־λ 

	WWDG_NVIC_Init();//��ʼ�����ڿ��Ź� NVIC

	WWDG_EnableIT(); //�������ڿ��Ź��ж�
} 
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//ʹ�ܿ��Ź� ,	���� counter .	 
}
//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

void WWDG_IRQHandler(void)
	{

	WWDG_SetCounter(WWDG_CNT);	  //�������˾��,���ڿ��Ź���������λ

	WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ

	
	}



