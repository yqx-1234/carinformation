#ifndef _USART3_H
#define _USART3_H
/*--------------------串口3功能实现头函数-------------*/
//#include "sys.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "sys.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdint.h"
#define USART3_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节
#define USART3_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern unsigned int usart3_RxCounter;          //外部声明，其他文件可以调用该变量
extern char usart3_RxBuff[USART3_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
void usart3_Init(u32 bound);
void u3_printf(char*,...);
#define EN_USART3_RX 1
#endif
