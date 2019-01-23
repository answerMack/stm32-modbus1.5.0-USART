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
/*输入寄存器*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
/*---------------------------------------------------------------------------*/
//保持寄存器起始地址
#define REG_HOLDING_START 0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS 10
static USHORT   usRegHoldingStart = REG_HOLDING_START;
//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS]
= {0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e,0x2020,0x1010};
/*---------------------------------------------------------------------------*/
/*线圈寄存器*/
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
//初始化 RTU模式 从机地址为10 USART2 9600 无校验
//eMBInit(MB_RTU, 0x01, 0x01, 38400, MB_PAR_NONE);
//启动FreeModbus 
//eMBEnable(); 
while (1)
{
	key=KEY_Scan(0);
	if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;//填充发送缓冲区
 			}
			RS485_Send_Data(rs485buf,5);//发送5个字节 									   
		}		 
		RS485_Receive_Data(rs485buf,&key);
		if(key)//接收到有数据
		{
			if(key>5)key=5;//最大是5个数据.
 			for(i=0;i<key;i++);			//显示数据
			printf("jieshoudaoshuju ");
 		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			t=0;
			cnt++;	
		}	
		
//FreeMODBUS不断查询
 //(void)eMBPoll(); 
	
}
}

/**
* @brief 保持寄存器处理函数，保持寄存器可读，可读可写
* @param pucRegBuffer 读操作时--返回数据指针，写操作时--输入数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
eMBRegisterMode eMode )
{
//错误状态
eMBErrorCode eStatus = MB_ENOERR;
//偏移量
int16_t iRegIndex;

//判断寄存器是不是在范围内
if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
{
//计算偏移量
iRegIndex = ( int16_t )( usAddress - REG_HOLDING_START);

switch ( eMode )
{
//读处理函数 
case MB_REG_READ:
while( usNRegs > 0 )
{
*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
*pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
iRegIndex++;
usNRegs--;
}
break;

//写处理函数 
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
//返回错误状态
eStatus = MB_ENOREG;
}

return eStatus;
}


/**
* @brief 输入寄存器处理函数，输入寄存器可读，但不可写。
* @param pucRegBuffer 返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* @retval eStatus 寄存器状态
*/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int16_t iRegIndex;
//查询是否在寄存器范围内
//为了避免警告，修改为有符号整数
if( ( (int16_t)usAddress >= REG_INPUT_START ) \
&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
{
//获得操作偏移量，本次操作起始地址-输入寄存器的初始地址
iRegIndex = ( int16_t )( usAddress - REG_INPUT_START );
//逐个赋值
while( usNRegs > 0 )
{
//赋值高字节
*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
//赋值低字节
*pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
//偏移量增加
iRegIndex++;
//被操作寄存器数量递减
usNRegs--;
}
}
else
{
//返回错误状态，无寄存器 
eStatus = MB_ENOREG;
}
    return eStatus;
}



/**
* @brief 线圈寄存器处理函数，线圈寄存器可读，可读可写
* @param pucRegBuffer 读操作---返回数据指针，写操作--返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
eMBRegisterMode eMode )
{
//错误状态
eMBErrorCode eStatus = MB_ENOERR;
//寄存器个数
int16_t iNCoils = ( int16_t )usNCoils;
//寄存器偏移量
int16_t usBitOffset;

//检查寄存器是否在指定范围内
if( ( (int16_t)usAddress >= REG_COILS_START ) &&
( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
{
//计算寄存器偏移量
usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
switch ( eMode )
{
//读操作
case MB_REG_READ:
while( iNCoils > 0 )
{
*pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
iNCoils -= 8;
usBitOffset += 8;
}
break;

//写操作
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
* @brief 开关输入寄存器处理函数，开关输入寄存器，可读
* @param pucRegBuffer 读操作---返回数据指针，写操作--返回数据指针
* usAddress 寄存器起始地址
* usNRegs 寄存器长度
* eMode 操作方式，读或者写
* @retval eStatus 寄存器状态
*/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
//错误状态
eMBErrorCode eStatus = MB_ENOERR;
//操作寄存器个数
int16_t iNDiscrete = ( int16_t )usNDiscrete;
//偏移量
uint16_t usBitOffset;

//判断寄存器时候再制定范围内
if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
{
//获得偏移量
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



