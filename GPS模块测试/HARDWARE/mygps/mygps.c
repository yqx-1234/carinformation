#include "mygps.h"
#include "sys.h"
#include "usart.h"
#include "string.h"
#include "stm32f4xx_it.h"  //������Ҫ��ͷ�ļ�
/////////////////////////////////////////////////////////////////////////////////////////////////////
//GPS���ݽ�������
////////////////////////////////////////////////////////////////////////////////////////////////////
void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;  //�洢��ǰ֡�е�ǰ���������һ����������
	char i = 0;
	if(Save_Data.isGetData){//���������Ƿ����
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer); //��ӡ֡����
		for (i = 0 ; i <= 6 ; i++){
			if (i == 0){
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL) //�Ƿ��ҵ������","
					errorLog(1);	//��������
			}
			else{
				subString++;//ȥ��������λ������","
				if ((subStringNext = strstr(subString, ",")) != NULL){
					char usefullBuffer[2]; //������Ч�Ա�־λ��V:������Ч��A:������Ч
					switch(i){
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡ������Ч�Ա�־
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W
						default:break;
					}
					subString = subStringNext;
					Save_Data.isParseData = true;//�������ݱ�־λ��λ����ʾ���ݽ������
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

			}
			else{
					errorLog(2);	//��������
			}
		}
	 }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//GPS���ݴ�������� The Designer:MR.��
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void printGpsBuffer()
{
	if (Save_Data.isParseData){//�ж������Ƿ�������
		Save_Data.isParseData = false;
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");
		if(Save_Data.isUsefull){//�ж������Ƿ���Ч
			Save_Data.isUsefull = false;
			
			printf("Save_Data.latitude = ");//��ӡγ��
			printf(Save_Data.latitude);
			printf("\r\n");
			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);     
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);//��ӡ����
			printf("\r\n");
			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}
*/
void errorLog(int num)
{
	
	while (1){
	  	printf("ERROR%d\r\n",num);
	}
}

void mygps_Data(void){
	char *temp_Data;
	float i=17;
if (Save_Data.isParseData){//�ж������Ƿ�������
		Save_Data.isParseData = false;
		u1_printf("Save_Data.UTCTime = ");
		u1_printf(Save_Data.UTCTime);
		u1_printf("\r\n");
		if(Save_Data.isUsefull){//�ж������Ƿ���Ч
			Save_Data.isUsefull = false;
			
			u1_printf("Save_Data.latitude = ");//��ӡγ��
			u1_printf(Save_Data.latitude);
			u1_printf("\r\n");
			u1_printf("Save_Data.N_S = ");
			u1_printf(Save_Data.N_S);     
			u1_printf("\r\n");

			u1_printf("Save_Data.longitude = ");
			u1_printf(Save_Data.longitude);//��ӡ����
			u1_printf("\r\n");
			u1_printf("Save_Data.E_W = ");
			u1_printf(Save_Data.E_W);
			u1_printf("\r\n");
		}
		else{
			u1_printf("GPS DATA is not usefull!\r\n");
		}
		
	}	


}