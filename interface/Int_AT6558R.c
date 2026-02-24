#include "Int_AT6558R.h"

#include "Com_Debug.h"
#define GPS_buffer_len_max 512
uint8_t GPS_buffer[GPS_buffer_len_max];
uint16_t GPS_buffer_len;




uint8_t GPS_FUL_buffer[GPS_ful_buffer_len_max];
uint16_t GPS_FUL_buffer_len;




#define AT6558R_FREQ "PCAS02,1000"
#define AT6558R_MODE "PCSA04,3"


uint8_t AT6558R_Cmd_Buffer[64];//需要计算校验和
void AT6558R_SendCmd(char* cmd){

    uint8_t temp=cmd[0];


    for(uint8_t i=1;cmd[i]!='\0';i++){
        temp^=cmd[i];
    }
    sprintf((char*)AT6558R_Cmd_Buffer,"$%s*%02X\r\n",cmd,temp);
    HAL_UART_Transmit(&huart2, AT6558R_Cmd_Buffer, strlen((char*)AT6558R_Cmd_Buffer), 1000);



}


void Int_AT6558R_Init(void){
//初始化驱动，串口usart2和pb3
    //修改参数
    // MX_USART2_UART_Init();
//设置波特率，模式，发命令
    //AT6558R_SendCmd(AT6558R_FREQ);
   // AT6558R_SendCmd(AT6558R_MODE);


}


void Int_AT6558R_ReadGPS(void){

    //HAL_UARTEx_ReceiveToIdle(&huart2, GPS_buffer, GPS_buffer_len_max, 1000);
    
// $GNGGA,,,,,,0,00,25.5,,,,,,*64
// $GNGLL,,,,,,V,N*7A
// $GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,1*01
// $GNGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5,4*04
// $GPGSV,1,1,00,0*65
// $BDGSV,1,1,00,0*74
// $GNRMC,,V,,,,,,,,,,N,V*37
// $GNVTG,,,,,,,,,N*2E
// $GNZDA,,,,,,*56
// $GPTXT,01,01,01,ANTENNA OPEN*25

//为了提升一个健壮性和效率
//思路1.使用toidle，避免空闲等待
//思路2.使用缓冲区，避免数据丢失

//逻辑：只有完成一次完成的接收才进行打印数据


    memset(GPS_FUL_buffer,0,GPS_ful_buffer_len_max);
    GPS_FUL_buffer_len=0;
    DEBUG_PRINTF("step1");
    while(1){
        if(strstr((char *)GPS_FUL_buffer,"$GNGGA")!=NULL && strstr((char *)GPS_FUL_buffer,"$GPTXT")!=NULL){

            //找到完整数据

            break;
        }

        memset(GPS_buffer,0,GPS_buffer_len_max);
        //没有完整数据，继续接收
        HAL_UARTEx_ReceiveToIdle(&huart2,GPS_buffer, GPS_buffer_len_max,&GPS_buffer_len, 1000);
        DEBUG_PRINTF("step2");
        if(GPS_buffer_len>0){
            //有数据，拼接
            memcpy(&GPS_FUL_buffer[GPS_FUL_buffer_len],GPS_buffer,GPS_buffer_len);

            GPS_FUL_buffer_len+=GPS_buffer_len;

            memset(GPS_buffer,0,GPS_buffer_len_max);

            GPS_buffer_len=0;
            DEBUG_PRINTF("step3");




    }
       
    }
    DEBUG_PRINTF("step4");

    DEBUG_PRINTF("\n%s",GPS_FUL_buffer);






}

void Int_AT6558R_Enter_Low(void){
    //AT6558R进入低功耗模式：除了VDD_BK引脚外的电源输入全部拉低即可
    HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_RESET);
    //
    //拉低稳压器ce片选
}

void Int_AT6558R_Exit_Low(void){
    //AT6558R退出低功耗模式
    HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_SET);
    Com_Delay_ms(100);
    //拉高稳压器ce片选
}



