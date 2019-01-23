 #ifndef __GPIO_H
 #define __GPIO_H
 #include "sys.h"

#define PD12 PDout(12)
#define PD6 PDout(6)
#define PD8 PDout(8)
#define PD9 PDout(9)



 /*��ʼ����������*/
 void  GPIO_ALL_init(void);
 
/*��ʼ��led����*/
 void  LED_IO_init(void); 
 
/*��ʼ��spi����*/
void SPI1_IO_Init(void);			 //��ʼ��SPI1��
void SPI2_IO_Init(void);			 //��ʼ��SPI2��

/*��ʼ��max6675����*/
void max6675_IO_init(void);			 //��ʼ��max6675ģ�� 

void erjiguan(void);  //PROTB6 �Ӷ���������

void PORTD_INIT(void);//��������ų�ʼ��
 
#endif
