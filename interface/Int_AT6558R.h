#ifndef __INT_AT6558R_H__
#define __INT_AT6558R_H__
#include "main.h"

#include "usart.h"
#define GPS_ful_buffer_len_max 1024


//硬件接口层完成两个操作：1.初始化配置芯片 2.读数据

void Int_AT6558R_Init(void);


void Int_AT6558R_ReadGPS(void);

void Int_AT6558R_Enter_Low(void);

void Int_AT6558R_Exit_Low(void);




#endif /* __INT_AT6558R_H__ */



