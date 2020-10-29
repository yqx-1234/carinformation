#include "timer2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "usart1.h"
void timer2_Enable_5s(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//���嶨ʱ�����ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;//�����ж����ȼ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);//ʹ��TIM2��ʱ��ʱ��
	
	
	TIM_DeInit(TIM2);//��ʼ��TIM2��ֵ��Ĭ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;//��ʱ��ʱ�ӷ�ƵΪ84M/(8400-1+1)=10kHz
	TIM_TimeBaseInitStructure.TIM_Period=42000-1;//�Զ���װ��ֵΪ42000-1+1����ʱ5s
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
  	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//��������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;  //��ʱ��2����ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //ʹ��ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;       //��Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //�������жϱ�־�
  u1_printf("����3\r\n");
	//TIM_Cmd(TIM2,ENABLE);  //ʹ�ܶ�ʱ��2
}
