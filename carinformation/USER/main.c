#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stm32f4xx.h"
#include "mqtt.h"
#include "usart1.h"
#include "usart2.h"
#include "fourG.h"
#include "key.h"
#include "timer3.h"
#include "timer4.h"
#include "commu.h"
#include "string.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "gcan600.h"
//������Ϣ�ɼ�ģ��

int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	Usart1_Init(9600);              //����1���ܳ�ʼ����������9600
	Usart2_Init(115200);            //����2���ܳ�ʼ����������115200	
	TIM4_Init(500,7200);            //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	usart3_Init(9600);              //����3���ܳ�ʼ����������9600
	timer2_Enable_5s();
	u1_printf("����\r\n");
	TIM_Cmd(TIM2,DISABLE);
	fourG_ResetIO_Init();           //��ʼ��4Gģ��ĸ�λIO
	AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
	delay_ms(40000);
	while(1)
	{
   communication();
	}
}