 #ifndef __GPIO_H
 #define __GPIO_H
 #include "sys.h"

#define PD12 PDout(12)
#define PD6 PDout(6)
#define PD8 PDout(8)
#define PD9 PDout(9)



 /*初始化所有引脚*/
 void  GPIO_ALL_init(void);
 
/*初始化led引脚*/
 void  LED_IO_init(void); 
 
/*初始化spi引脚*/
void SPI1_IO_Init(void);			 //初始化SPI1口
void SPI2_IO_Init(void);			 //初始化SPI2口

/*初始化max6675引脚*/
void max6675_IO_init(void);			 //初始化max6675模块 

void erjiguan(void);  //PROTB6 接二极管正脚

void PORTD_INIT(void);//数码管引脚初始化
 
#endif
