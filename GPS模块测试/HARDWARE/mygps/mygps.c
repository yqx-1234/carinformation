#include "mygps.h"
#include "sys.h"
#include "usart.h"
#include "string.h"
#include "stm32f4xx_it.h"  //包含需要的头文件
/////////////////////////////////////////////////////////////////////////////////////////////////////
//GPS数据解析函数
////////////////////////////////////////////////////////////////////////////////////////////////////
void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;  //存储当前帧中当前功能序和下一功能序内容
	char i = 0;
	if(Save_Data.isGetData){//接受数据是否完成
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer); //打印帧数据
		for (i = 0 ; i <= 6 ; i++){
			if (i == 0){
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL) //是否找到间隔符","
					errorLog(1);	//解析错误
			}
			else{
				subString++;//去掉数据首位所带的","
				if ((subStringNext = strstr(subString, ",")) != NULL){
					char usefullBuffer[2]; //储存有效性标志位，V:数据无效，A:数据有效
					switch(i){
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取数据有效性标志
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
						default:break;
					}
					subString = subStringNext;
					Save_Data.isParseData = true;//解析数据标志位置位，表示数据解析完成
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

			}
			else{
					errorLog(2);	//解析错误
			}
		}
	 }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//GPS数据串口输出。 The Designer:MR.杨
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void printGpsBuffer()
{
	if (Save_Data.isParseData){//判断数据是否解析完成
		Save_Data.isParseData = false;
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");
		if(Save_Data.isUsefull){//判断数据是否有效
			Save_Data.isUsefull = false;
			
			printf("Save_Data.latitude = ");//打印纬度
			printf(Save_Data.latitude);
			printf("\r\n");
			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);     
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);//打印经度
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
if (Save_Data.isParseData){//判断数据是否解析完成
		Save_Data.isParseData = false;
		u1_printf("Save_Data.UTCTime = ");
		u1_printf(Save_Data.UTCTime);
		u1_printf("\r\n");
		if(Save_Data.isUsefull){//判断数据是否有效
			Save_Data.isUsefull = false;
			
			u1_printf("Save_Data.latitude = ");//打印纬度
			u1_printf(Save_Data.latitude);
			u1_printf("\r\n");
			u1_printf("Save_Data.N_S = ");
			u1_printf(Save_Data.N_S);     
			u1_printf("\r\n");

			u1_printf("Save_Data.longitude = ");
			u1_printf(Save_Data.longitude);//打印经度
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