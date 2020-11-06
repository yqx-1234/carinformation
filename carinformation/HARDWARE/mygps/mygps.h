#ifndef __MYGPS_H
#define __MYGPS_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "string.h"
#include "uart4.h"
#include "stm32f4xx_it.h"  //包含需要的头文件
//////////////////////////////////////////////////////////////////////////////////	 
//GPS模块函数库，designer：MR.杨
////////////////////////////////////////////////////////////////////////////////// 	
#define false 0
#define true 1

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;
void parseGpsBuffer(void);
//void printGpsBuffer(void);
void mygps_Data(void);
void errorLog(int num);
//如果想串口中断接收，请不要注释以下宏定义
#endif


