#include "GPIO.h"
#include "stm32f10x.h"


/*��ʼ����������*/
 void  GPIO_ALL_init(void){
 
	GPIO_InitTypeDef GPIO_INITJIEGOU;//����GPIOָ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA ʱ��ʹ��
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_INITJIEGOU);//��ʼ��GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITJIEGOU);//��ʼ��GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_INITJIEGOU);//��ʼ��GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_INITJIEGOU.GPIO_Mode= GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_INITJIEGOU.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITJIEGOU.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITJIEGOU);//��ʼ��GPIOD
 }


/*��ʼ��led����*/
void  LED_IO_init(void){

	GPIO_InitTypeDef GPIO_INITSTRUCTURE;//����GPIO��һ��ָ��
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��GPIOAʱ��

	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_8;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_INITSTRUCTURE);//ȡ��ַ
	
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_12;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_INITSTRUCTURE);//ȡ��ַ
	
	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_2;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITSTRUCTURE);//ȡ��ַ
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
}

 
/*��ʼ��spi����*/
void SPI1_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� SPI1ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/6/7����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB12
	GPIO_SetBits(GPIOA,GPIO_Pin_4);  //PA4����
	
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7����

}  			 //��ʼ��SPI1��



void SPI2_IO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB12������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB12
	GPIO_SetBits(GPIOB,GPIO_Pin_12);  //PB12����

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

}			 //��ʼ��SPI2��



/*��ʼ��max6675����*/
void max6675_IO_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE );//PORTA/Bʱ��ʹ�� SPI1��AFIOʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB8�����������  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOb8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;//GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;//GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;//GPIO_Mode_IPD   //PB7�������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB11�����������  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB11	
	
}	 //��ʼ��max6675ģ�� 

void erjiguan(void){
    
	GPIO_InitTypeDef GPIO_INITSTRUCTURE;//����GPIO��һ��ָ��
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOAʱ��

	GPIO_INITSTRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_INITSTRUCTURE.GPIO_Pin=GPIO_Pin_6;
	GPIO_INITSTRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITSTRUCTURE);//ȡ��ַ
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}



void PORTD_INIT(void){

    GPIO_InitTypeDef GPIO_INITZHIZHEN;//����GPIOָ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//GPIOA ʱ��ʹ��
	GPIO_INITZHIZHEN.GPIO_Mode= GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_INITZHIZHEN.GPIO_Pin=GPIO_Pin_All;
	GPIO_INITZHIZHEN.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_INITZHIZHEN);//��ʼ��GPIOD
   // GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_11);  
    GPIO_SetBits(GPIOD,GPIO_Pin_All);
}

