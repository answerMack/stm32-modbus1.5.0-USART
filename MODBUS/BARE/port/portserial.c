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
* @brief ���ƽ��պͷ���״̬
* @param xRxEnable ����ʹ�ܡ�
* xTxEnable ����ʹ��
* @retval None
*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{

if(xRxEnable)
{
USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//ʹ�ܽ��պͽ����ж�
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//MAX485���� �͵�ƽΪ����ģʽ
GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}
else
{
USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
//MAX485���� �ߵ�ƽΪ����ģʽ
GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

if(xTxEnable)
{
//ʹ�ܷ�������ж�
USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
//prvvUARTTxReadyISR();
}
else
{
//��ֹ��������ж�
USART_ITConfig(USART2, USART_IT_TC, DISABLE);
GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}

}
/**
* @brief ���ڳ�ʼ��
* @param ucPORT ���ں�
* ulBaudRate ������
* ucDataBits ����λ
* eParity У��λ 
* @retval None
*/
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
(void)ucPORT; //���޸Ĵ���
(void)ucDataBits; //���޸�����λ����
(void)eParity; //���޸�У���ʽ

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

//ʹ��USART2��GPIOA
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA,Dʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

//GPIOA2 USART2_Tx
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
GPIO_Init(GPIOA, &GPIO_InitStructure);
//GPIOA.3 USART2_Rx
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
GPIO_Init(GPIOA, &GPIO_InitStructure);

USART_InitStructure.USART_BaudRate = ulBaudRate; //ֻ�޸Ĳ�����
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


//���ڳ�ʼ��
USART_Init(USART2, &USART_InitStructure);
//ʹ��USART2
USART_Cmd(USART2, ENABLE);
//vMBPortSerialEnable(FALSE,FALSE);
//USART_ClearFlag(USART2,USART_FLAG_TC);
	
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//�趨USART2 �ж����ȼ�
NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

//�������485���ͺͽ���ģʽ
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//GPIOD.2
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOD, &GPIO_InitStructure); 

return TRUE;
}



/*xMBPortSerialPutByte��xMBPortSerialGetByte�����������ڴ��ڷ��ͺͽ������ݣ�������ֻҪ����STM32�Ŀ⺯�����ɡ�*/
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	//��������
USART_SendData(USART2, ucByte);
while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
   	//��������
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
//mb.c eMBInit������
//pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
//����״̬��
pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
/*FreeModbusЭ��ջͨ�������жϽ���һ֡���ݣ��û����ڴ��ڽ����ж��лص�prvvUARTRxISR()������*/
static void prvvUARTRxISR( void )
{
//mb.c eMBInit������
//pxMBFrameCBByteReceived = xMBRTUReceiveFSM
//����״̬��
pxMBFrameCBByteReceived();
}


//void USART2_IRQHandler(void)
//{
////���������ж�
//if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//{
//prvvUARTRxISR(); 
////����жϱ�־λ 
//USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//}

////��������ж�
//if(USART_GetITStatus(USART2, USART_IT_TC)== SET)
//{
//prvvUARTTxReadyISR();
////����жϱ�־
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
