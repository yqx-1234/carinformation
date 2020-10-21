#include "timer2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
void timer2_enable_5s(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);//使能TIM2定时器时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//定义定时器配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;//定义中断优先级配置
	
	TIM_DeInit(TIM2);//初始化TIM2的值成默认值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;//定时器时钟分频为84M/(8400-1+1)=10kHz
	TIM_TimeBaseInitStructure.TIM_Period=42000-1;//自动重装载值为42000-1+1，定时5s
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit
  	

}
