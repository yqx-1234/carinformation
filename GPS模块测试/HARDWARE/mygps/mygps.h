#ifndef __MYGPS_H
#define __MYGPS_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "string.h"
#include "usart.h"
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
void parseGpsBuffer(void);
//void printGpsBuffer(void);
void mygps_Data(void);
void errorLog(int num);

//如果想串口中断接收，请不要注释以下宏定义
#endif


