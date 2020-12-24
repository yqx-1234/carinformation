#ifndef _UART4_H
#define _UART4_H
/*--------------------����4����ʵ��ͷ����-------------*/
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
#define UART4_TXBUFF_SIZE   1024   //���崮��4 ���ͻ�������С 1024�ֽ�
#define UART4_RXBUFF_SIZE   1024              //���崮��4 ���ջ�������С 1024�ֽ�
extern unsigned int uart4_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char uart4_RxBuff[UART4_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
void uart4_Init(u32 bound);
#define EN_UART4_RX 1
#endif
