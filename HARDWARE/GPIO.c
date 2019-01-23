#include "GPIO.h"
#include "stm32f10x.h"


/*初始化所有引脚*/
 void  GPIO_ALL_init(void){
 
	GPIO_InitTypeDef GPIO_INITJIEGOU;//定义GPIO指针
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA 时钟使能
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_INITJIEGOU);//初始化GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITJIEGOU);//初始化GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_INITJIEGOU);//初始化GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITJIEGOU);//初始化GPIOD
 }


/*初始化led引脚*/
void  LED_IO_init(void){

	GPIO_InitTypeDef GPIO_INITSTRUCTURE;//定义GPIO的一个指针
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能GPIOA时钟

	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_8;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_INITSTRUCTURE);//取地址
	
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_12;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_INITSTRUCTURE);//取地址
	
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_2;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITSTRUCTURE);//取地址
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
}

 
/*初始化spi引脚*/
void SPI1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 SPI1时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/6/7复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB12
	GPIO_SetBits(GPIOA,GPIO_Pin_4);  //PA4上拉
	
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7上拉

}  			 //初始化SPI1口



void SPI2_IO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB12推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB12
	GPIO_SetBits(GPIOB,GPIO_Pin_12);  //PB12上拉

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

}			 //初始化SPI2口



/*初始化max6675引脚*/
void max6675_IO_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE );//PORTA/B时钟使能 SPI1和AFIO时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB8复用推挽输出  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOb8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;//GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;//GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//GPIO_Mode_IPD   //PB7上拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB11复用推挽输出  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB11	
	
}	 //初始化max6675模块 

void erjiguan(void){
    
	GPIO_InitTypeDef GPIO_INITSTRUCTURE;//定义GPIO的一个指针
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOA时钟

	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_6;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITSTRUCTURE);//取地址
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}



void PORTD_INIT(void){

    GPIO_InitTypeDef GPIO_INITZHIZHEN;//定义GPIO指针
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//GPIOA 时钟使能
	GPIO_INITZHIZHEN.GPIO_Mode= GPIO_Mode_Out_PP;//设置为推挽输出
	GPIO_INITZHIZHEN.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITZHIZHEN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITZHIZHEN);//初始化GPIOD
   // GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_11);  
    GPIO_SetBits(GPIOD,GPIO_Pin_All);
}

