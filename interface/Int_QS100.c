

#include "Int_QS100.h"


#define IOT_BUFF_LEN_MAX 128
uint8_t IOT_RxBuff[IOT_BUFF_LEN_MAX];
uint16_t IOT_RxLen=0;


#define IOT_FULL_BUFF_LEN_MAX 128
uint8_t IOT_FULL_RxBuff[IOT_FULL_BUFF_LEN_MAX];
uint16_t IOT_FULL_RxLen=0;





void Int_QS100_wkup(void){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_SET);
    Com_Delay_ms(5);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_RESET);
    Com_Delay_ms(5);
}



void Int_QS100_Reset(void){
    uint8_t *cmd="AT+RB\r\n";
    HAL_UART_Transmit(&huart3, cmd, strlen((char *)cmd), 1000); 
    

    HAL_UART_Receive(&huart3, IOT_RxBuff, IOT_BUFF_LEN_MAX, 3000);
    IOT_RxLen=strlen((char *)IOT_RxBuff);
    if(IOT_RxLen>0){
        DEBUG_PRINTF("QS100初始化完成\r\n");
        DEBUG_PRINTF("%s",IOT_RxBuff);
        memset(IOT_RxBuff,0,IOT_BUFF_LEN_MAX);
        IOT_RxLen=0;
    }   
    Com_Delay_s(3);
}
void Int_QS100_Sendcmd(uint8_t *cmd){
    memset(IOT_FULL_RxBuff,0,IOT_FULL_BUFF_LEN_MAX);
    IOT_FULL_RxLen=0;

    HAL_UART_Transmit(&huart3, cmd, strlen((char *)cmd), 1000); 

    
    do{
        HAL_UARTEx_ReceiveToIdle(&huart3, IOT_RxBuff, IOT_BUFF_LEN_MAX, &IOT_RxLen, 3000);
        


        if(IOT_RxLen>0){


            memcpy(&IOT_FULL_RxBuff[IOT_FULL_RxLen],IOT_RxBuff,IOT_RxLen);

            IOT_FULL_RxLen+=IOT_RxLen;

            memset(IOT_RxBuff,0,IOT_BUFF_LEN_MAX);
            IOT_RxLen=0;

            
           
        }

    }while(strstr((char *)IOT_FULL_RxBuff,"OK")==NULL&&strstr((char *)IOT_FULL_RxBuff,"ERROR")==NULL);


        DEBUG_PRINTF("QS100 CMD RX:%s",IOT_FULL_RxBuff);

        DEBUG_PRINTF("++++++++++++++++++++++++++++++\r\n");
       

    
}


void Int_QS100_Init(void){
    MX_GPIO_Init();


    Int_QS100_Exit_Low();


    Int_QS100_Sendcmd("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
    
    

    Int_QS100_Reset();

    


  


    Int_QS100_Sendcmd("ATE1\r\n");


    

    







}




CommonStatus Int_QS100_Getip(void){
    //查询是否处于附着状态
    Int_QS100_Sendcmd("AT+CGATT?\r\n");
    if(strstr((char *)IOT_FULL_RxBuff,"+CGATT:1")==NULL){
        return COMMON_ERROR;
    }
    return COMMON_OK;
}




CommonStatus Int_QS100_Create_Client(int8_t * socket){

    Int_QS100_Sendcmd("AT+NSOCR=STREAM,6,0,0\r\n");

    if(strstr((char *)IOT_FULL_RxBuff,"OK")==NULL){
        return COMMON_ERROR;
    }
//+NSOCR:0
    *socket=(uint8_t)strstr((char *)IOT_FULL_RxBuff,"+NSOCR:")[7]-'0';
    
    

    return COMMON_OK;
}

CommonStatus Int_QS100_Connect_Server(int8_t *socket,char * ip,uint16_t port){
    //参数为socket_id，IP，port
    //cmd:AT+NSOCO=0,139.224.112.6,10005
    uint8_t cmd[64]={0};
    sprintf((char *)cmd,"AT+NSOCO=%d,%s,%d\r\n",*socket,ip,port);
    Int_QS100_Sendcmd(cmd);

    if(strstr((char *)IOT_FULL_RxBuff,"OK")==NULL){
        return COMMON_ERROR;
    }

    return COMMON_OK;

}


CommonStatus Int_QS100_Close_Client(int8_t *socket){
    uint8_t cmd[32]={0};
    sprintf((char *)cmd,"AT+NSOCL=%d\r\n",*socket);
    Int_QS100_Sendcmd(cmd);

    if(strstr((char *)IOT_FULL_RxBuff,"OK")==NULL){
        return COMMON_ERROR;
    }

    return COMMON_OK;

}



CommonStatus Int_QS100_SendData_To_Server(int8_t *socket,uint8_t *data,uint16_t len){
    //发送数据到服务端
    //AT+NSOSD=0,2,4444,0x200,1
    //传入socket_id,要发送数据的长度（十进制），数据内容（十六进制字符串），flag，sequence

    //将uint_type数据转换为十六进制字符串
    uint16_t hex_len=len*2+1;
    uint8_t hex_data[hex_len];
    memset(hex_data,0,hex_len);
    for(int i=0;i<len;i++){
        sprintf((char *)&hex_data[i*2],"%02X",data[i]);
    }
    uint8_t cmd[512]={0};

    sprintf((char *)cmd,"AT+NSOSD=%d,%d,%s,0x200,%d\r\n",*socket,len,hex_data,SEQUENCE);

    Int_QS100_Sendcmd(cmd);
    Com_Delay_s(1);

    memset(cmd,0,512);
    sprintf((char *)cmd,"AT+SEQUENCE=%d,%d\r\n",*socket,SEQUENCE);
    char temp='5';
    
    // for(int16_t i=0;i<IOT_FULL_RxLen;i++){
    //     printf("%d:%c\r\n",i,IOT_FULL_RxBuff[i]);

        //此处成功找到下标为19代表返回值
    // }
    while(temp!='0'&&temp!='1'){
        Int_QS100_Sendcmd(cmd);
        temp=IOT_FULL_RxBuff[IOT_FULL_RxLen-9];
        
        }
    if(temp=='1'){
        DEBUG_PRINTF("接收数据成功");
    }else{
        DEBUG_PRINTF("接收数据失败");
        return COMMON_ERROR;
    }
        
    



    return COMMON_OK;


}
    

CommonStatus  Int_QS100_SendData(uint8_t *data,uint16_t len){
    //1.判断是否能链接外网

    CommonStatus status1=Int_QS100_Getip();
    int8_t count1=5;
    while(status1==COMMON_ERROR&&count1>0){
        status1=Int_QS100_Getip();
        count1--;
        Com_Delay_s(1);
    }
    if(count1==0){
        DEBUG_PRINTF("附着失败");
        return COMMON_ERROR;

    }else{
        DEBUG_PRINTF("附着成功");
    }




    //2.创建客户端
    int8_t socket=0xFF;
    CommonStatus status2=Int_QS100_Create_Client(&socket);
    int8_t count2=5;
    while(status2==COMMON_ERROR&&count2>0){
        status2=Int_QS100_Create_Client(&socket);
        count2--;
        Com_Delay_s(1);
    }
    if(count2==0){
        DEBUG_PRINTF("创建客户端失败");
        return COMMON_ERROR;
    }else{
        DEBUG_PRINTF("socket_id:%d",socket);
        DEBUG_PRINTF("创建客户端成功");
    }

    

    //链接服务器

    

    CommonStatus status3=Int_QS100_Connect_Server(&socket,HTTP_SERVER_IP,HTTP_SERVER_PORT);
    int8_t count3=5;
    while(status3==COMMON_ERROR&&count3>0){
        status3=Int_QS100_Connect_Server(&socket,HTTP_SERVER_IP,HTTP_SERVER_PORT);
        count3--;
        Com_Delay_s(1);
    }
    if(count3==0){
        DEBUG_PRINTF("连接服务端失败");
        Int_QS100_Close_Client(&socket);
        return COMMON_ERROR;
        }
    else{
        DEBUG_PRINTF("连接服务端成功");
        }
    //3.进入es状态


    //4.发送数据


    int8_t count5=5;
    CommonStatus status5=Int_QS100_SendData_To_Server(&socket,data,len);
    while (status5==COMMON_ERROR&&count5>0)
    {
        status5=Int_QS100_SendData_To_Server(&socket,data,len);
        count5--;
        Com_Delay_s(1);
    }
    if(count5==0){
        DEBUG_PRINTF("发送数据失败");
        Int_QS100_Close_Client(&socket);
        return COMMON_ERROR;
    }
    else{
        DEBUG_PRINTF("发送数据成功");
    }
    
    //5.关闭客户端




    
    int8_t count4=5;
    CommonStatus status4=Int_QS100_Close_Client(&socket);
    while(status4==COMMON_ERROR&&count4>0){
        status4=Int_QS100_Close_Client(&socket);
        count4--;
        Com_Delay_s(1);
    }
    if(count4==0){
        DEBUG_PRINTF("关闭客户端失败");
        return COMMON_ERROR;
    }
    else{
        DEBUG_PRINTF("关闭客户端成功");
    }
    Com_Delay_ms(3);

    return COMMON_OK;
}


void Int_QS100_Enter_Low(void){
    //QS100进入低功耗模式
    Int_QS100_Sendcmd("AT+FASTOFF=0\r\n");
    Com_Delay_ms(5);
}


void Int_QS100_Exit_Low(void){
    //QS100退出低功耗模式
    Int_QS100_wkup();
    Com_Delay_ms(5);
}










