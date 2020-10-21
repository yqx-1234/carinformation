#include "usart3.h"
void usart3_Init(u32 bound){
   GPIO_InitTypeDef  GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ�ܴ���ʱ��
	
	 USART_DeInit(USART3);//��λ����3
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);//����PB10��PB11���ù��ܴ���1
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϹ������ȼ�����Ϊ2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11; //��������λPin10��pin11
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;          //ģʽ����Ϊ���ù���
	 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;        //�������ģʽ
	 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;     //����ģʽ
	 GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed; //IO����50Mhz
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	 USART_InitStructure.USART_BaudRate=bound;  //����������Ϊbound
	 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //��Ӳ�������Ʒ�ʽ
	 USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//���ý��ն˺ͷ��Ͷ�
	 USART_InitStructure.USART_Parity=USART_Parity_No;//����żУ��λ
	 USART_InitStructure.USART_StopBits=USART_StopBits_1;//1��ֹͣλ
	 USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�˸�������Чλ
	 USART_Init(USART3,&USART_InitStructure);//��ʼ������1
	 
	  
	
#if EN_USART1_RX	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//���ڽ����ж�ʹ��
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;//�ж�ͨ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//��Ӧ���ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
#endif
  USART_Cmd(USART3,ENABLE);//����ʹ��

}