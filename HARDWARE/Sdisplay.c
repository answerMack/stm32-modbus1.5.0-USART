#include "Sdisplay.h"
#include "GPIO.h"
#include "delay.h"



/*

STM32F103RCT6  封装形式位LQFP64  引脚位64个  不足够使用驱动12位（引脚）的数码管

*/
void Sdisplay_init(void){
	
	PORTD_INIT();
	
}


void Sdisplay_show(void){
	
//	PD12=0;
//	delay_ms(10);
//	PD12=1;
//	PD9=0;
//	delay_ms(10);
//	PD9=1;
//	PD8=0;
//	delay_ms(10);
//	PD8=1;
//	PD6=0;
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	delay_ms(1000);
//	
}




