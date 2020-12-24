#ifndef _UART4_H
#define _UART4_H
/*--------------------串口4功能实现头函数-------------*/
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
#include "mygps.h"
#include "stm32f4xx_it.h"
#define UART4_TXBUFF_SIZE   1024   //定义串口4 发送缓冲区大小 1024字节
#define UART4_RXBUFF_SIZE   1024              //定义串口4 接收缓冲区大小 1024字节
extern unsigned int uart4_RxCounter;          //外部声明，其他文件可以调用该变量
extern char uart4_RxBuff[UART4_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
void uart4_Init(u32 bound);
#define EN_UART4_RX 1
#endif
