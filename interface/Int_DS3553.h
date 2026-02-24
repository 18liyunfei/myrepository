#ifndef __DS3553_H__
#define __DS3553_H__
#include "main.h"
#include "i2c.h"


#define DS3553_ADDR 0x27
#define DS3553_ADDR_R 0x4F
#define DS3553_ADDR_W 0x4E


#define DS3553_CS_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
#define DS3553_CS_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);





void Int_DS3553_ReadReg(uint8_t reg_addr, uint8_t *reg_data);
void Int_DS3553_WriteReg(uint8_t reg_addr, uint8_t reg_data);
void  Int_DS3553_Get_Step(uint32_t *step);
void    Int_DS3553_Init(void);







#endif /* __DS3553_H__ */






