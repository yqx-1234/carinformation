#include "gcan600.h"
void GCAN600_Data(void){
	char *temp_Data;
	float i=17;
	char temp_Buff[TBUFF_UNIT];
	if(strstr(usart3_RxBuff,"PID13")){
	char *temp=usart3_RxBuff+6;
	memcpy(temp_Data,temp,strlen(temp));
	u1_printf("开始将车速信息进行打包\r\n");
	sprintf(temp_Buff,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CarSpeed1\":%s},\"version\":\"1.0.0\"}",temp_Data);
	MQTT_PublishQs0(P_TOPIC_NAME,temp_Buff,strlen(temp_Buff));
	}
	else if(strstr(usart3_RxBuff,"PID5")){
	char *temp=usart3_RxBuff+5;
	memcpy(temp_Data,temp,strlen(temp));
	u1_printf("开始将冷却液温度信息进行打包\r\n");
	sprintf(temp_Buff,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CoolingTemperature1\":%s},\"version\":\"1.0.0\"}",temp_Data);
	MQTT_PublishQs0(P_TOPIC_NAME,temp_Buff,strlen(temp_Buff));
	}
	else if(strstr(usart3_RxBuff,"PID47")){
		char *temp=usart3_RxBuff+6;
	memcpy(temp_Data,temp,strlen(temp));
	u1_printf("开始将油量信息进行打包\r\n");
	sprintf(temp_Buff,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CarPower1\":%s},\"version\":\"1.0.0\"}",temp_Data);
	MQTT_PublishQs0(P_TOPIC_NAME,temp_Buff,strlen(temp_Buff));
	}
	else
	u1_printf("警告：此次数据打包过程出现问题：无数据打包\r\n");
	temp_Data="15";
	sprintf(temp_Buff,"{\"method\":\"thing.event.property.post\",\"id\":\"203302324\",\"params\":{\"CarPower1\":%s},\"version\":\"1.0.0\"}",temp_Data);
	MQTT_PublishQs0(P_TOPIC_NAME,temp_Buff,strlen(temp_Buff));	
}

