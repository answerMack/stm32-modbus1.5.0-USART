#include "max6675.h"
#include "spi.h"
#include "sys.h" 
#include "delay.h"	
#include "GPIO.h"
#include "usart.h"

/*max6675初始化程序*/
void max6675_MONI_init(void){
   
   max6675_IO_init();
	
}

u8 SPI1_tempwritebyte(u8 t){
     return SPI1_ReadWriteByte(t);
}
u16 max6675_readbits_III(void){
    u16 d1; 
	GPIO_ResetBits(GPIOA,GPIO_Pin_4) ;//开CS
	d1=SPI1_tempwritebyte(0xFF);
    d1<<=8;
	d1 |=SPI1_tempwritebyte(0xFF);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
  if (d1&0X04) 
 {
   d1 = 0; //未检测到热电偶
  printf("3-K\n");
  }
  else 
 {
	d1<<=1;
	d1>>=4;
  }
	return d1;
}
float max6675_readTemp_III(void)
{
     u16 m;
	float i3,S;
	S=1;
	m=max6675_readbits_III();
	i3=m*1023.75/4095/S;
    return i3;
}

u8 max6675_readWriteByte(u8 TxData)
{		
	/* Return the byte read from the SPI bus */
	return SPI2_ReadWriteByte(TxData);
	
}  

u16 max6675_readbits_II(void){
    u16 d; 
	GPIO_ResetBits(GPIOB,GPIO_Pin_12) ;//开CS
	d=max6675_readWriteByte(0xFF);
    d<<=8;
	d |=max6675_readWriteByte(0xFF);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
  if (d&0X04) 
 {
   d = 0; //未检测到热电偶
  printf("2-K\n");
  }
  else 
 {
	d<<=1;
	d>>=4;
	 //d>>=3;
  }
    
	return d;
 
 }

 float max6675_readTemp(void){
    u16 d;
	float i,S;
	S=1;//实验数2.142857
	//delay_ms(300);
	d=max6675_readbits_II();
	i=d*1023.75/4095/S;
    return i;
 
 }
 
u16 max6675_readRawValue(void)
{
  u16 tmp=0;
  u16 i;
  MAX6675_CS_L;//enable max6675
  for(i=0;i<16;i++)
          {  
			  /*创建一个下降沿，按下降沿读取PB7的端口电平*/
			MAX6675_CK_H;
			delay_us(1);//10
			if(MAX6675_SO)tmp|=0x0001;
		    //printf("tmp%d---:%d\n",i,tmp);/*打印每个数*/
			tmp<<=1;
		    MAX6675_CK_L;
			delay_us(1);//10
			  
                  }
	      MAX6675_CS_H;
				  
   if (tmp&0X04) 
 {
    tmp = 0; //未检测到热电偶
	printf("1-K\n");
  }
  else 
 {
	tmp<<=1;
	tmp>>=4;
  }
    
	return tmp;
}
 
float max6675_readTemperature(void)
{
	u16 d;
	float i,S;
	S=0.510416667;//实验数2.142857  2.06185567
	//delay_ms(300);
	d=max6675_readRawValue();
	i=(d*1023.75/4095*S);
    return i;
}


