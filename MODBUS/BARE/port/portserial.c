#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "rs485.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
/**
* @brief 控制接收和发送状态
* @param xRxEnable 接收使能、
* xTxEnable 发送使能
* @retval None
*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{

if(xRxEnable)
{
USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//使能接收和接收中断
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//MAX485操作 低电平为接收模式
GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}
else
{
USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
//MAX485操作 高电平为发送模式
GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

if(xTxEnable)
{
//使能发送完成中断
USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
//prvvUARTTxReadyISR();
}
else
{
//禁止发送完成中断
USART_ITConfig(USART2, USART_IT_TC, DISABLE);
GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}

}
/**
* @brief 串口初始化
* @param ucPORT 串口号
* ulBaudRate 波特率
* ucDataBits 数据位
* eParity 校验位 
* @retval None
*/
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
(void)ucPORT; //不修改串口
(void)ucDataBits; //不修改数据位长度
(void)eParity; //不修改校验格式

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

//使能USART2，GPIOA
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA,D时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

//GPIOA2 USART2_Tx
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
GPIO_Init(GPIOA, &GPIO_InitStructure);
//GPIOA.3 USART2_Rx
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮动输入
GPIO_Init(GPIOA, &GPIO_InitStructure);

USART_InitStructure.USART_BaudRate = ulBaudRate; //只修改波特率
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
//USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 switch(eParity)
    {
    case MB_PAR_NONE:USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
    case MB_PAR_ODD:USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
		break;
    case MB_PAR_EVEN:USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
		break;
    default:break;
    }


//串口初始化
USART_Init(USART2, &USART_InitStructure);
//使能USART2
USART_Cmd(USART2, ENABLE);
//vMBPortSerialEnable(FALSE,FALSE);
//USART_ClearFlag(USART2,USART_FLAG_TC);
	
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//设定USART2 中断优先级
NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

//最后配置485发送和接收模式
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//GPIOD.2
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOD, &GPIO_InitStructure); 

return TRUE;
}



/*xMBPortSerialPutByte和xMBPortSerialGetByte两个函数用于串口发送和接收数据，在这里只要调用STM32的库函数即可。*/
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	//发送数据
USART_SendData(USART2, ucByte);
while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
   	//接受数据
	*pucByte = USART_ReceiveData(USART2);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
//mb.c eMBInit函数中
//pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
//发送状态机
pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
/*FreeModbus协议栈通过串口中断接收一帧数据，用户需在串口接收中断中回调prvvUARTRxISR()函数；*/
static void prvvUARTRxISR( void )
{
//mb.c eMBInit函数中
//pxMBFrameCBByteReceived = xMBRTUReceiveFSM
//接收状态机
pxMBFrameCBByteReceived();
}


//void USART2_IRQHandler(void)
//{
////发生接收中断
//if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//{
//prvvUARTRxISR(); 
////清除中断标志位 
//USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//}

////发生完成中断
//if(USART_GetITStatus(USART2, USART_IT_TC)== SET)
//{
//prvvUARTTxReadyISR();
////清除中断标志
//USART_ClearITPendingBit(USART2, USART_IT_TC);
//}
//}

void
EnterCriticalSection( void )
{
  __disable_irq();
}

void
ExitCriticalSection( void )
{
  __enable_irq();
}
