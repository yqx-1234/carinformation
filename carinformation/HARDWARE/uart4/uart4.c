#include "uart4.h"
#include "mygps.h"
char uart4_RxBuff[UART4_RXBUFF_SIZE];
unsigned int uart4_RxCounter; 

void uart4_Init(u32 bound){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);//使能串口时钟
	
	USART_DeInit(UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);//设置PA0与PA1复用功能串口1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断管理优先级分组为2，2位抢占优先级，2位响应优先级
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//串口传输速率50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//设置接收端和发送端
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件控制流方式
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_BaudRate=bound;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//八个数据有效位
	USART_Init(UART4,&USART_InitStructure);
	
	#if EN_UART4_RX
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//串口接收中断使能
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	#endif
	USART_Cmd(UART4,ENABLE);//中断使能
}
