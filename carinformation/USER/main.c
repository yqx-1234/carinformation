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
//车辆信息采集模块

int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	Usart1_Init(9600);              //串口1功能初始化，波特率9600
	Usart2_Init(115200);            //串口2功能初始化，波特率115200	
	TIM4_Init(500,7200);            //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
	usart3_Init(9600);              //串口3功能初始化，波特率9600
	timer2_Enable_5s();
	u1_printf("调试\r\n");
	TIM_Cmd(TIM2,DISABLE);
	fourG_ResetIO_Init();           //初始化4G模块的复位IO
	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
	delay_ms(40000);
	while(1)
	{
   communication();
	}
}