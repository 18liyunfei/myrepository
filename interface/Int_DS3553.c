



#include "Int_DS3553.h"



void Int_DS3553_ReadReg(uint8_t reg_addr, uint8_t *reg_data){




    DS3553_CS_L

    Com_Delay_ms(5);

    

    //方法一
    HAL_I2C_Mem_Read(&hi2c1,DS3553_ADDR_R,reg_addr,I2C_MEMADD_SIZE_8BIT,reg_data,1,1000);
    


    DS3553_CS_H

    Com_Delay_ms(12);


    //方法二

    //HAL_I2C_Master_Transmit(&hi2c1,DS3553_ADDR_W,0x01,1,1000);
    // HAL_I2C_Master_Receive(&hi2c1,DS3553_ADDR_R,&reg_data,1,1000);






}


void Int_DS3553_WriteReg(uint8_t reg_addr, uint8_t reg_data){

    

    DS3553_CS_L

    Com_Delay_ms(5);

    

    //方法一
    HAL_I2C_Mem_Write(&hi2c1,DS3553_ADDR_W,reg_addr,I2C_MEMADD_SIZE_8BIT,&reg_data,1,1000);
   



    DS3553_CS_H

    Com_Delay_ms(12);




}


void    Int_DS3553_Init(void){



    //MX_I2C1_Init();初始化
    uint8_t id=0xff;
    Int_DS3553_ReadReg(0X01,&id);
    DEBUG_PRINTF("id = %x", id);
    
    //读id


    //初始化设置参数


    //1.关闭计步脉冲输出
    //2.选择模式10
    uint8_t temp=~(0x01<<4);
    temp|=0x10;
   

    Int_DS3553_WriteReg(0x3C,temp);







}



void  Int_DS3553_Get_Step(uint32_t *step){
//读寄存器
    uint8_t step_l=0x00;
    uint8_t step_h=0x00;
    uint8_t step_m=0x00;


    Int_DS3553_ReadReg(0xC4,&step_l);
    Int_DS3553_ReadReg(0xC5,&step_m);
    Int_DS3553_ReadReg(0xC6,&step_h);

    *step=(step_h<<16)|(step_m<<8)|step_l;









}




