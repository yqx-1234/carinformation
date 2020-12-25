#include "sys.h"
#include "usart.h"	
char rxdatabufer;
u16 point1 = 0;

_SaveData Save_Data;
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif 
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound)
{
    //GPIO�˿�����
   GPIO_InitTypeDef GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 USART_DeInit(USART1);//����1���ø�λ
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//��PA9��PA10���ù���ӳ�䵽����1��
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1��GPIOAʱ��
   //USART1_TX   PA.9;USART1_RX PA10
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //PA9��PA10
   GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//����ģʽ
	 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//����ģʽ
	 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//��������ģʽ
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 


	CLR_Buf();//��ջ���
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//��������
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //���				
		}	
				
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	
		
		
		
		
		
		
// 		USART_RX_STA|=0x8000;	//���������
// 		if((USART_RX_STA&0x8000)==0)//����δ���
// 		{
// 			if(USART_RX_STA&0x4000)//���յ���0x0d
// 			{
// 				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
// 				else USART_RX_STA|=0x8000;	//��������� 			//bit31�����Ƿ���յ�0x0a(\n)
// 			}
// 			else //��û�յ�0X0D
// 			{	
// 				if(Res==0x0d)USART_RX_STA|=0x4000;						//bit30�����Ƿ���յ�0x0d(\r)
// 				else
// 				{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
// 				}		 
// 			}
// 		}   		 
   } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}


u8 Hand(char *a)                   // ��������ʶ����
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
  point1 = 0;                    
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

#endif	


/*-------------------------------------------------*/
/*����2��ʼ������                    */
/*����:bound:������                           */
/*����ֵ����                                    */
/*-------------------------------------------------*/
/*
void Usart2_Init(u32 bound)
{  
 //��ؽṹ�嶨��	
  GPIO_InitTypeDef GPIO_InitStructure;     
	USART_InitTypeDef USART_InitStructure;   
#if USART2_RX_ENABLE                         //��Ϊ0��ʹ���жϹ���
	NVIC_InitTypeDef NVIC_InitStructure;     //�ж����ȼ�����
#endif

#if USART2_RX_ENABLE                                 //��Ϊ0�򲻶����ж����ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //����ʱ������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   //GPIOAʱ������
	 USART_DeInit(USART2);                                               //��λ����2��������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);//PA2��PA3���ù���ӳ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;              //PA2��PA3
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //IO����Ϊ100M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	          //���ù���
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;              //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //��ʼ��GPIO�˿�
   
	
	USART_InitStructure.USART_BaudRate = bound;                                    //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8bit����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
#if USART2_RX_ENABLE               												   //�Ƿ�ʹ�ܽ��չ���
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               
#else                                                                              
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           
#endif        
    USART_Init(USART2, &USART_InitStructure);                                      

#if USART2_RX_ENABLE  	         					        //��ʹ�ܽ��չ��ܣ������ж�����
	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);         
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	                        
#endif  

	USART_Cmd(USART2, ENABLE);                             
}
*/
