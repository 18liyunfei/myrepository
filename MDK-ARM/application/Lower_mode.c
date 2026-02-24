#include "Lower_mode.h"





void Enter_Lower_Mode(void){
    Int_AT6558R_Enter_Low();
    Int_QS100_Enter_Low();



    RTC_TimeTypeDef mytime;
    RTC_AlarmTypeDef myalarm;

    HAL_RTC_GetTime(&hrtc, &mytime, RTC_FORMAT_BIN);//获取当前时间

    myalarm.AlarmTime.Hours=mytime.Hours;
    myalarm.AlarmTime.Minutes=mytime.Minutes;
    myalarm.AlarmTime.Seconds=mytime.Seconds+20;


    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);//清除唤醒标志�?
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);//清除待机标志�?

    HAL_RTC_SetAlarm(&hrtc, &myalarm, RTC_FORMAT_BIN);//设置闹钟中断，唤醒主控芯�?



    DEBUG_PRINTF("����͹���");


    HAL_Delay(100);
    






    HAL_PWR_EnterSTANDBYMode();//主控芯片进入低功�?







}
void Exit_Lower_Mode(void){
    Int_AT6558R_Exit_Low();
    Int_QS100_Exit_Low();
    DEBUG_PRINTF("�˳��͹���");


}


