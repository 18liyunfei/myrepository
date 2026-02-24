#include   "Com_Tool.h"


void Com_Delay_us(uint16_t us){//1us就是72次钟,一次循环是n次，那么循环n*temp就等于us
    uint32_t temp=(us*72)/11;
    while(temp--){
    
    }
}
void Com_Delay_ms(uint16_t ms){
    HAL_Delay(ms);
}
void Com_Delay_s(uint16_t s){
    while(s--){
        HAL_Delay(1000);
    }   
}


