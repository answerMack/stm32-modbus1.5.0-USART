#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY0    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)//��ȡ����0
#define KEY1    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define WKUP_PRES   3	//KEY2����(��WK_UP)

void KEY_Init(void);//IO��ʼ��
int KEY_Scan(u8);  	//����ɨ�躯��		


#endif



