/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
#include "stm32f4xx.h"     //包含需要的头文件
//#include "stm32f4xx_it.h"  //包含需要的头文件
#include "main.h"          //包含需要的头文件
#include "delay.h"         //包含需要的头文件
#include "usart1.h"        //包含需要的头文件
#include "usart2.h"        //包含需要的头文件
//#include "timer1.h"        //包含需要的头文件
#include "timer4.h"        //包含需要的头文件
#include "fourG.h"	       //包含需要的头文件
#include "timer3.h"        //包含需要的头文件
#include "led.h"           //包含需要的头文件
#include "mqtt.h"          //包含需要的头文件
#include "usart3.h"        //包含需要的头文件
#include "gcan600.h"
/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
		if(Connect_flag==0){                                //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
			if(USART2->DR){                                 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //保存到缓冲区	
				Usart2_RxCounter ++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}		
		}else{		                                        //反之Connect_flag等于1，连接上服务器了	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart2_RxCounter == 0){    					//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter ++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
} 

/*-------------------------------------------------*/
/*函数名：串口3接收中断函数                        */
/*参数：无                                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		if(USART2->DR){
			usart3_RxBuff[usart3_RxCounter]=USART3->DR;
			usart3_RxCounter++;
		}
		
	}
	
}
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

	void TIM4_IRQHandler(void){
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter);  //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                   //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                   //记录数据长度低字节
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                  //指针下移
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //指针归位到缓冲区开头
		Usart2_RxCounter = 0;                                         //串口2接收数据量变量清零
		TIM_SetCounter(TIM3, 0);                                      //清零定时器6计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				  //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			  //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		switch(Ping_flag){                               //判断Ping_flag的状态
			case 0:										 //如果Ping_flag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 					 //添加Ping报文到发送缓冲区  
					break;
			case 1:										 //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					TIM3_ENABLE_2S(); 					 //我们将定时器6设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区  
					break;
			case 2:										 //如果Ping_flag等于2，说明还没有收到服务器回复
			case 3:				                         //如果Ping_flag等于3，说明还没有收到服务器回复
			case 4:				                         //如果Ping_flag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区 
					break;
			case 5:										 //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					Connect_flag = 0;                    //连接状态置0，表示断开，没连上服务器
					TIM_Cmd(TIM3,DISABLE);               //关TIM3 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除TIM3溢出中断标志 	
	}
}
/*------------------------------------------*/
/*函数名：定时器2中断服务函数*              */
/*函数作用：定时器定时5s采集传感器数据并打包成MQTT报文数据*/
/*参数：无                                  */
/*返回值：无                                */
/*------------------------------------------*/
void TIM2_IRQHandler(void){
	u32 i=0; //定义两个循环变量
	u32 j=0;
	u8 k=0;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET){   //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	  
		u1_printf("调试14\r\n");
		//u1_printf("接收车速数据时发生错误\r\n");
		//	GCAN600_Data();  //调试用
    u3_printf("ATPID=13\r\n");
		for(i=0;i<4200;i--)
		  for(j=0;j<20000;j--); //循环延时约0.5s
		//if(strstr(usart3_RxBuff,"PID13")){
		if(k){
		  GCAN600_Data();
			usart3_RxCounter=0;
			data_Flag=0;
		}
		else if(k){
		  u3_printf("ATPID=13\r\n");
			for(i=0;i<4200;i--)
				for(j=0;j<20000;j--);
		  if(strstr(usart3_RxBuff,"PID13")){         //第一次读取车速数据不成功，重新发送查询命令
			  GCAN600_Data();
				usart3_RxCounter=0;
				data_Flag=0;
			}
    }
    else{
      u1_printf("接收车速数据时发生错误\r\n");
			GCAN600_Data();  //调试用
      data_Flag=1;
		  usart3_RxCounter=0;
    }
		
 /*---------------采集冷却液温度信息------------------*/			 
		u3_printf("ATPID=05\r\n");
			for(i=0;i<4200;i--)
				for(j=0;j<20000;j--);  //延时约0.5s
		if(strstr(usart3_RxBuff,"PID05")){
			GCAN600_Data();
			data_Flag=0;
			usart3_RxCounter=0;
		}
		else if(strstr(usart3_RxBuff,"PID05")==0){
			u3_printf("ATPID=47\r\n");
			for(i=0;i<4200;i--)
				for(j=0;j<20000;j--);
			if(strstr(usart3_RxBuff,"PID47")){
				GCAN600_Data();
				data_Flag=0;
				usart3_RxCounter=0;
			}
   }
   else{
     u1_printf("接收冷却液温度数据时发生错误\r\n");	
     data_Flag=1;
   }		
/*----------------读取汽车当前油量信息-------------*/	 
		u3_printf("ATPID=47\r\n");
			for(i=0;i<4200;i--)
				for(j=0;j<20000;j--);
		if(strstr(usart3_RxBuff,"PID47")){
			GCAN600_Data();
			data_Flag=0;
			usart3_RxCounter=0;
		}
		else if(strstr(usart3_RxBuff,"PID05")==0){
			u3_printf("ATPID=47\r\n");
			for(i=0;i<4200;i--)
				for(j=0;j<20000;j--);
			if(strstr(usart3_RxBuff,"PID47")){
				GCAN600_Data();
				data_Flag=0;
				usart3_RxCounter=0;
			}
   }
   else{
     u1_printf("接收数据错误\r\n");	
     data_Flag=1;
		 usart3_RxCounter=0;
   }					 				 
  }
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      //清除TIM2溢出中断标志 
}





/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
