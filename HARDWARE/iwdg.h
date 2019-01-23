#ifndef __IWDG_H
#define __IWDG_H
#include "sys.h"


/*prer 预分频系数； RLR 为自动重装载值*/
void iwdg_init(u8 prer,u16 RLR);

/*喂狗程序*/

 void iwdg_feed(void);
 
 
 /*窗口看门狗*/
void WWDG_Init(u8 tr,u8 wr,u32 fprer);//初始化WWDG
void WWDG_Set_Counter(u8 cnt);       //设置WWDG的计数器
void WWDG_NVIC_Init(void);
 
#endif


