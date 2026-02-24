#ifndef __INT_QS100_H__
#define __INT_QS100_H__


#define HTTP_SERVER_IP "112.125.89.8"
#define HTTP_SERVER_PORT 33993
#define SEQUENCE 1

#include "main.h"
#include "Com_Config.h"
#include "gpio.h"

void Int_QS100_Init(void);
void Int_QS100_wkup(void);


void Int_QS100_Reset(void);
CommonStatus Int_QS100_SendData(uint8_t *data,uint16_t len);


void Int_QS100_Sendcmd(uint8_t *cmd);

void Int_QS100_Enter_Low(void);


void Int_QS100_Exit_Low(void);











#endif /* __INT_QS100_H__ */






