#ifndef __IWDG_H
#define __IWDG_H
#include "sys.h"


/*prer Ԥ��Ƶϵ���� RLR Ϊ�Զ���װ��ֵ*/
void iwdg_init(u8 prer,u16 RLR);

/*ι������*/

 void iwdg_feed(void);
 
 
 /*���ڿ��Ź�*/
void WWDG_Init(u8 tr,u8 wr,u32 fprer);//��ʼ��WWDG
void WWDG_Set_Counter(u8 cnt);       //����WWDG�ļ�����
void WWDG_NVIC_Init(void);
 
#endif


