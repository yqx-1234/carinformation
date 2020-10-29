#include "timer2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "usart1.h"
void timer2_Enable_5s(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//¶¨Òå¶¨Ê±Æ÷ÅäÖÃ½á¹¹Ìå
	NVIC_InitTypeDef NVIC_InitStructure;//¶¨ÒåÖĞ¶ÏÓÅÏÈ¼¶ÅäÖÃ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);//Ê¹ÄÜTIM2¶¨Ê±Æ÷Ê±ÖÓ
	
	
	TIM_DeInit(TIM2);//³õÊ¼»¯TIM2µÄÖµ³ÉÄ¬ÈÏÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;//¶¨Ê±Æ÷Ê±ÖÓ·ÖÆµÎª84M/(8400-1+1)=10kHz
	TIM_TimeBaseInitStructure.TIM_Period=42000-1;//×Ô¶¯ÖØ×°ÔØÖµÎª42000-1+1£¬¶¨Ê±5s
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1·ÖÆµ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
  	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//¿ªÆôÒç³öÖĞ¶Ï
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;  //¶¨Ê±Æ÷2Òç³öÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //Ê¹ÄÜÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//ÇÀÕ¼ÓÅÏÈ¼¶Îª1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;       //ÏìÓ¦ÓÅÏÈ¼¶Îª2
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //Çå³ıÒç³öÖĞ¶Ï±êÖ¾Î
  u1_printf("µ÷ÊÔ3\r\n");
	//TIM_Cmd(TIM2,ENABLE);  //Ê¹ÄÜ¶¨Ê±Æ÷2
}
