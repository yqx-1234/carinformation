#include "timer2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
void timer2_enable_5s(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);//ʹ��TIM2��ʱ��ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//���嶨ʱ�����ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;//�����ж����ȼ�����
	
	TIM_DeInit(TIM2);//��ʼ��TIM2��ֵ��Ĭ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;//��ʱ��ʱ�ӷ�ƵΪ84M/(8400-1+1)=10kHz
	TIM_TimeBaseInitStructure.TIM_Period=42000-1;//�Զ���װ��ֵΪ42000-1+1����ʱ5s
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit
  	

}
