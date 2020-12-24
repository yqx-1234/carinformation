/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ����Wifi���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __FOURG_H
#define __FOURG_H

#include "usart2.h"	    //������Ҫ��ͷ�ļ�

#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4����WiFi�ĸ�λ

#define fourG_printf       u2_printf           //����2���� 4Gģ��
#define fourG_RxCounter    Usart2_RxCounter    //����2���� 4Gģ��
#define fourG_RX_BUF       Usart2_RxBuff       //����2���� 4Gģ��
#define fourG_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //����2���� 4Gģ��


void fourG_ResetIO_Init(void);
char fourG_SendCmd(char *cmd, int timeout);
char fourG_Reset(int timeout);
char fourG_JoinAP(int timeout);
char fourG_Connect_Server(int timeout);
char fourG_commandMode(int timeout);
char fourG_WaitAP(int timeout);
char fourG_Connect_IoTServer(void);


#endif


