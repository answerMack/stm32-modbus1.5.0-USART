#ifndef __MAX6675_H
#define __MAX6675_H
 
#include "stm32f10x.h"
#include "sys.h" 


#define MAX6675_CS_H   GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define MAX6675_CS_L   GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define MAX6675_CK_H   GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define MAX6675_CK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define MAX6675_SO  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
 
 
void max6675_MONI_init(void);			 //³õÊ¼»¯max6675Ä£¿é 
u16 max6675_readRawValue(void);
float max6675_readTemperature(void);

u8 max6675_readWriteByte(u8 TxData);
u16 max6675_readbits_II(void);
float max6675_readTemp(void);


u8 SPI1_tempwritebyte(u8 t);
u16 max6675_readbits_III(void);
float max6675_readTemp_III(void);
 
#endif
