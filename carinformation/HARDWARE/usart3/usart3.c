#include "usart3.h"
unsigned int usart3_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char usart3_RxBuff[USART3_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据
/*---------------------------------------*/
/*初始化串口2                            */
/*函数名：usart3_Init(u32 bound)         */
/*参数：无                               */
/*---------------------------------------*/
void usart3_Init(u32 bound){
   GPIO_InitTypeDef  GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能串口时钟
	
	 USART_DeInit(USART3);//复位串口3
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);//设置PB10与PB11复用功能串口1
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断管理优先级分组为2，2位抢占优先级，2位响应优先级
	
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11; //引脚设置位Pin10与pin11
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;          //模式设置为复用功能
	 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;        //推挽输出模式
	 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;     //上拉模式
	 GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed; //IO速率50Mhz
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	 USART_InitStructure.USART_BaudRate=bound;  //波特率设置为bound
	 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //无硬件流控制方式
	 USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//设置接收端和发送端
	 USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验位
	 USART_InitStructure.USART_StopBits=USART_StopBits_1;//1个停止位
	 USART_InitStructure.USART_WordLength=USART_WordLength_8b;//八个数据有效位
	 USART_Init(USART3,&USART_InitStructure);//初始化串口1
	 
	  
	
#if EN_USART3_RX	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//串口接收中断使能
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;//中断通道为USART3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//响应优先级为0
	NVIC_Init(&NVIC_InitStructure);//中断初始化
#endif
  USART_Cmd(USART3,ENABLE);//串口使能

}

/*---------------------------------------*/
/*串口3打印函数                        */
/*函数名：u3_printf(u32 bound)         */
/*参数：无                               */
/*---------------------------------------*/
__align(8) char usart3_TxBuff[USART3_TXBUFF_SIZE]; 
void u3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(usart3_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)usart3_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = usart3_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
}
