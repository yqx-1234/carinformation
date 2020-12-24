/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ����602Wifi���ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //������Ҫ��ͷ�ļ�
#include "fourG.h"	    //������Ҫ��ͷ�ļ�
//#include "iic.h"        //������Ҫ��ͷ�ļ�
//#include "24c02.h" 		//������Ҫ��ͷ�ļ�
#include "delay.h"	    //������Ҫ��ͷ�ļ�
#include "usart1.h"	    //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "key.h"        //������Ҫ��ͷ�ļ�
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
char fourG_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
	
/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void fourG_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //ʹ��PA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //׼������PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;         //����100Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   	//��ͨ�����ʽ
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;     //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;        //��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //����PA4
	RESET_IO(1);                                              //��λIO���ߵ�ƽ
}
/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char fourG_SendCmd(char *cmd, int timeout)
{
	fourG_RxCounter=0;                           //WiFi������������������                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);     //���WiFi���ջ����� 
	fourG_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		delay_ms(100);                          //��ʱ100ms
		if(strstr(fourG_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������fourG_Reset                               */
/*�������ã�4Gģ������                               */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char fourG_Reset(int timeout)
{
	//RESET_IO(0);                                    //��λIO���͵�ƽ
	delay_ms(500);                                    //��ʱ500ms
  fourG_printf("AT+Z\r\n");                          //����ָ��		
	//RESET_IO(1);                                    //��λIO���ߵ�ƽ	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                              //��ʱ100ms
		if(strstr(fourG_RX_BUF,"G405tf"))             //������յ�G405tf��ʾ��λ�ɹ�
			break;       						    //��������whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
	else return 0;		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}

/*-------------------------------------------------*/
/*��������fourG_callMode*/
/*��������:4Gģ�����͸��ģʽ                      */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char fourG_callMode(int timeout)
{		
	fourG_RxCounter=0;                               //WiFi������������������                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);         //���WiFi���ջ����� 
	fourG_printf("AT+ENTM\r\n"); //����ָ��	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(fourG_RX_BUF,"OK")) //������յ�WIFI GOT IP��ʾ�ɹ�
			break;       						    //��������whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}

/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
char fourG_Connect_Server(int timeout)
{	
	char a=0;
	fourG_RxCounter=0;                              //WiFi������������������                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);       //���WiFi���ջ�����   
	fourG_printf("AT+SOCKA=TCP,%s,%d\r\n",ServerIP,ServerPort);//�������ӷ�����ָ��
	while(timeout--){                               //�ȴ���ʱ���
		delay_ms(100);                                //��ʱ100ms	
		if(strstr(fourG_RX_BUF ,"aliyuncs"))          //������ܵ�CONNECT��ʾ���ӳɹ�
			break; 		                                  //����whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��  
	}
	u1_printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 1;                   //��ʱ���󣬷���1
	else
	  return 0;
}
/*-------------------------------------------------*/
/*��������fourG_commandMode                      */
/*��  ����timeout           */
/*����ֵ��0����ȷ   ����������     */
/*���ܣ�����ATָ��ģʽ*/
/*-------------------------------------------------*/
char fourG_commandMode(int timeout)
{
	
	fourG_RxCounter=0;                           //4G������������������                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);     //���4G���ջ�����     
	while(timeout--){		//�ȴ���ʱʱ�䵽0
		fourG_printf("+++");
		delay_ms(1000); 		//��ʱ1s
		fourG_printf("a");
		delay_ms(1000); 		//��ʱ1s
		if(strstr(fourG_RX_BUF,"ok"))     //������ڽ��ܵ�ok��ʾ�ɹ�����ATָ��ģʽ
			break;                              //����whileѭ��  
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��  
	}	
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //��ʱ���󣬷���1
	return 0;                                   //��ȷ����0
}


/*-------------------------------------------------*/
/*��������4G���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/

char fourG_Connect_IoTServer(void)
{
	u1_printf("׼������ATָ��ģʽ\r\n");	//������ʾ����
	if(fourG_commandMode(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����ATָ��ģʽʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 1;                                      //����1
	}else u1_printf("�ɹ�����ATָ��ģʽ\r\n");                   //������ʾ����
	u1_printf("׼����λģ��\r\n");	//������ʾ����
	if(fourG_Reset(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 2;                                      //����1
	}else u1_printf("��λ�ɹ�\r\n");                   //������ʾ����
	u1_printf("׼������ATָ��ģʽ\r\n");	//������ʾ����
	if(fourG_commandMode(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����ATָ��ģʽʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 3;                                      //����1
	}else u1_printf("�ɹ�����ATָ��ģʽ\r\n");                   //������ʾ����
	u1_printf("׼������TCP����\r\n");                  //������ʾ����
	if(fourG_Connect_Server(50)){                      //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����TCP����ʧ�ܣ�׼������\r\n");      //���ط�0ֵ������if��������ʾ����
		return 4;                                        //����2
	}else u1_printf("����TCP���ӳɹ�\r\n");            //������ʾ����
	u1_printf("׼������ģ�飬����͸��ģʽ\r\n");       //������ʾ����
	if(fourG_Reset(50)){                               //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
		return 5;                                      //����8
	}else u1_printf("����͸���ɹ�\r\n");               //������ʾ����
	return 0;                                          //��ȷ����0
	
}
