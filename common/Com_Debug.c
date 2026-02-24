#include "Com_Debug.h"

void Com_Debug_Init(void)
{
    //MX_USART1_UART_Init();//初始化串口
}



int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

