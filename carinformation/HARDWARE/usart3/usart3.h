#ifndef _USART3_H
#define _USART3_H
/*--------------------����3����ʵ��ͷ����-------------*/
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
#define USART3_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�
#define USART3_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�
extern unsigned int usart3_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char usart3_RxBuff[USART3_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
void usart3_Init(u32 bound);
void u3_printf(char*,...);
#define EN_USART3_RX 1
#endif
