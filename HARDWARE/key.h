#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY0    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)//读取按键0
#define KEY1    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define WKUP_PRES   3	//KEY2按下(即WK_UP)

void KEY_Init(void);//IO初始化
int KEY_Scan(u8);  	//按键扫描函数		


#endif



