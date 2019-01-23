#include "stm32f10x.h"
#include "delay.h"
//#include "sys.h"
//#include "can.h"
#include "rs485.h"
#include "usart.h"	
#include "key.h"
#include "mb.h"
#include "mbutils.h"



/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
/*����Ĵ���*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
/*---------------------------------------------------------------------------*/
//���ּĴ�����ʼ��ַ
#define REG_HOLDING_START 0x0000
//���ּĴ�������
#define REG_HOLDING_NREGS 10
static USHORT   usRegHoldingStart = REG_HOLDING_START;
//���ּĴ�������
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS]
= {0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e,0x2020,0x1010};
/*---------------------------------------------------------------------------*/
/*��Ȧ�Ĵ���*/
#define REG_COILS_START 1000
#define REG_COILS_SIZE 16
static unsigned char  ucRegCoilsBuf[REG_COILS_SIZE/8];


#define REG_DISCRETE_START 1
#define REG_DISCRETE_SIZE 150
static unsigned char ucRegDiscreteBuf[REG_DISCRETE_SIZE];


/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
	
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 rs485buf[5];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RS485_Init(9600);
	KEY_Init();
    uart_init(4800);
//��ʼ�� RTUģʽ �ӻ���ַΪ10 USART2 9600 ��У��
//eMBInit(MB_RTU, 0x01, 0x01, 38400, MB_PAR_NONE);
//����FreeModbus 
//eMBEnable(); 
while (1)
{
	key=KEY_Scan(0);
	if(key==KEY0_PRES)//KEY0����,����һ������
		{
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;//��䷢�ͻ�����
 			}
			RS485_Send_Data(rs485buf,5);//����5���ֽ� 									   
		}		 
		RS485_Receive_Data(rs485buf,&key);
		if(key)//���յ�������
		{
			if(key>5)key=5;//�����5������.
 			for(i=0;i<key;i++);			//��ʾ����
			printf("jieshoudaoshuju ");
 		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			t=0;
			cnt++;	
		}	
		
//FreeMODBUS���ϲ�ѯ
 //(void)eMBPoll(); 
	
}
}

/**
* @brief ���ּĴ��������������ּĴ����ɶ����ɶ���д
* @param pucRegBuffer ������ʱ--��������ָ�룬д����ʱ--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
eMBRegisterMode eMode )
{
//����״̬
eMBErrorCode eStatus = MB_ENOERR;
//ƫ����
int16_t iRegIndex;

//�жϼĴ����ǲ����ڷ�Χ��
if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
{
//����ƫ����
iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START);

switch ( eMode )
{
//�������� 
case MB_REG_READ:
while( usNRegs > 0 )
{
*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
iRegIndex++;
usNRegs--;
}
break;

//д������ 
case MB_REG_WRITE:
while( (int)usNRegs > 0 )
{
usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
iRegIndex++;
usNRegs--;
}
break;
}
}
else
{
//���ش���״̬
eStatus = MB_ENOREG;
}

return eStatus;
}


/**
* @brief ����Ĵ���������������Ĵ����ɶ���������д��
* @param pucRegBuffer ��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int16_t iRegIndex;
//��ѯ�Ƿ��ڼĴ�����Χ��
//Ϊ�˱��⾯�棬�޸�Ϊ�з�������
if( ( (int16_t)usAddress >= REG_INPUT_START ) \
&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
{
//��ò���ƫ���������β�����ʼ��ַ-����Ĵ����ĳ�ʼ��ַ
iRegIndex = ( int16_t )( usAddress - REG_INPUT_START );
//�����ֵ
while( usNRegs > 0 )
{
//��ֵ���ֽ�
*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
//��ֵ���ֽ�
*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
//ƫ��������
iRegIndex++;
//�������Ĵ��������ݼ�
usNRegs--;
}
}
else
{
//���ش���״̬���޼Ĵ��� 
eStatus = MB_ENOREG;
}
    return eStatus;
}



/**
* @brief ��Ȧ�Ĵ�������������Ȧ�Ĵ����ɶ����ɶ���д
* @param pucRegBuffer ������---��������ָ�룬д����--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
eMBRegisterMode eMode )
{
//����״̬
eMBErrorCode eStatus = MB_ENOERR;
//�Ĵ�������
int16_t iNCoils = ( int16_t )usNCoils;
//�Ĵ���ƫ����
int16_t usBitOffset;

//���Ĵ����Ƿ���ָ����Χ��
if( ( (int16_t)usAddress >= REG_COILS_START ) &&
( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
{
//����Ĵ���ƫ����
usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
switch ( eMode )
{
//������
case MB_REG_READ:
while( iNCoils > 0 )
{
*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
iNCoils -= 8;
usBitOffset += 8;
}
break;

//д����
case MB_REG_WRITE:
while( iNCoils > 0 )
{
xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
*pucRegBuffer++ );
iNCoils -= 8;

}
break;
}

}
else
{
eStatus = MB_ENOREG;
}
return eStatus;
}

/**
* @brief ��������Ĵ�������������������Ĵ������ɶ�
* @param pucRegBuffer ������---��������ָ�룬д����--��������ָ��
* usAddress �Ĵ�����ʼ��ַ
* usNRegs �Ĵ�������
* eMode ������ʽ��������д
* @retval eStatus �Ĵ���״̬
*/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
//����״̬
eMBErrorCode eStatus = MB_ENOERR;
//�����Ĵ�������
int16_t iNDiscrete = ( int16_t )usNDiscrete;
//ƫ����
uint16_t usBitOffset;

//�жϼĴ���ʱ�����ƶ���Χ��
if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
{
//���ƫ����
usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );

while( iNDiscrete > 0 )
{
*pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
iNDiscrete -= 8;
usBitOffset += 8;
}

}
else
{
eStatus = MB_ENOREG;
}
return eStatus;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred. 
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None 
  */ 
void assert_failed(uint8_t* file, uint32_t line) 
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */ 
    
  /* Infinite loop */ 
  while (1) 
  { 
  } 
}
#else
void __aeabi_assert(const char * x1, const char * x2, int x3)
{
}
#endif



