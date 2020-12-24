#ifndef __MYGPS_H
#define __MYGPS_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "string.h"
#include "usart.h"
#include "stm32f4xx_it.h"  //������Ҫ��ͷ�ļ�
//////////////////////////////////////////////////////////////////////////////////	 
//GPSģ�麯���⣬designer��MR.��
////////////////////////////////////////////////////////////////////////////////// 	
#define false 0
#define true 1

//�������鳤��
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

//����봮���жϽ��գ��벻Ҫע�����º궨��
#endif


