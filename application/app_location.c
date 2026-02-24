#include"app_location.h"
#include "Com_Config.h"

#include <stdlib.h>
#include "math.h"




upload_data my_upload_data={0};

extern uint8_t GPS_FUL_buffer[GPS_ful_buffer_len_max];
extern uint16_t GPS_FUL_buffer_len;



void UTC_to_BeijingTime(double utc_time_float, const char* gps_date, char* beijing_time_str)
{
    // 1. 拆分UTC浮点时间为 时、分、秒
    int utc_hour = (int)(utc_time_float / 10000);        // 235316.0 → 23
    double temp_min = utc_time_float - floor(utc_time_float / 10000) * 10000; // 235316.0 - 23*10000 = 5316.0
    int utc_min  = (int)(temp_min / 100); // 5316.0 / 100 = 53.16 → 53
    
    // 修复：用floor+减法替代 %100
    double utc_sec = temp_min - floor(temp_min / 100) * 100; // 

    // 2. 转换为北京时间（UTC+8），处理跨天
    int bj_hour = utc_hour + 8;
    int day_offset = 0; // 日期偏移（0=当天，1=次日）
    if (bj_hour >= 24) {
        bj_hour -= 24;
        day_offset = 1;
    }

    // 3. 拆分GPS日期为 日、月、年（数值）
    int day  = (gps_date[0] - '0') * 10 + (gps_date[1] - '0'); // 02 → 2
    int mon  = (gps_date[2] - '0') * 10 + (gps_date[3] - '0'); // 07 → 7
    int year = (gps_date[4] - '0') * 10 + (gps_date[5] - '0'); // 11 → 11

    // 4. 日期跨天修正（简单版：每月按30天算，精准版可替换为月份天数表）
    const int month_days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // 精准天数表
    if (day_offset == 1) {
        day += 1;
        // 精准判断当月天数（适配28/30/31天）
        if (day > month_days[mon]) {
            day = 1;
            mon += 1;
            if (mon > 12) {
                mon = 1;
                year += 1;
            }
        }
    }

    // 5. 拼接北京时间字符串（格式：2011-07-03 07:53:16）
    sprintf(beijing_time_str, "20%02d-%02d-%02d %02d:%02d:%02.0f",
            year, mon, day,
            bj_hour, utc_min, utc_sec);
}

void app_location_get_gps_Data(void){
    //鑾峰彇瀹氫綅鏁版嵁锛?
    //閫昏緫锛氭寰幆淇濊瘉鏁版嵁鏈夋晥
    Int_AT6558R_Init();

    char *valid_data=NULL;




    while(1){
        Int_AT6558R_ReadGPS();

        valid_data=strstr(( char *)GPS_FUL_buffer,"$GNRMC");

        valid_data="$GNRMC,081321.000,A,2900.10293,N,11431.06545,E,3.81,214.99,190226,,,E,V*02";

        

        //  $GPRMC,235316.000,A,2959.9925,S,12000.0090,E,0.009,75.020,020711,,,A*45
        if(valid_data==NULL){


            continue;//娌℃湁鎵惧埌锛岀户缁惊鐜?
        }


        DEBUG_PRINTF("%s\n",valid_data);
        DEBUG_PRINTF("step5\r\n");


        char valid=0;

        DEBUG_PRINTF("step6\r\n");
        int ret=sscanf(valid_data,"%*[^A V]%c", &valid);


        DEBUG_PRINTF("step7\r\n");

        DEBUG_PRINTF("valid=%c,ret=%d\n",valid,ret);


        if(valid=='A' && ret==1){
            DEBUG_PRINTF("成功获取有效定位数据\n");
            double lon; //经度
    uint8_t londir; //经度方向

    double lat; //纬度
    uint8_t latdir; //纬度方向

    uint8_t datetime[21];

    double utctime;

    sscanf(valid_data,"%*[^,],%lf,%*[^,],%lf,%c,%lf,%c,%*[^,],%*[^,],%s",&utctime,&lat,(char *)&latdir,&lon,(char *)&londir,datetime);
    DEBUG_PRINTF("utctime=%lf\n",utctime);
    DEBUG_PRINTF("lat=%lf,latdir=%c\n",lat,latdir);
    DEBUG_PRINTF("lon=%lf,londir=%c\n",lon,londir);
    DEBUG_PRINTF("datetime=%s\n",datetime);



    my_upload_data.lat=(int)lat/100+(lat-(int)(lat/100)*100)/60;
    my_upload_data.latdir=latdir;
    // DEBUG_PRINTF("lat:%lf\n",my_upload_data.lat);
    // DEBUG_PRINTF("latdir:%c\n",my_upload_data.latdir);

    my_upload_data.lon=(int)lon/100+(lon-(int)(lon/100)*100)/60;
    my_upload_data.londir=londir;
    // DEBUG_PRINTF("lon:%lf\n",my_upload_data.lon);
    // DEBUG_PRINTF("londir:%c\n",my_upload_data.londir);


    UTC_to_BeijingTime(utctime, (const char*)datetime, (char*)my_upload_data.datetime);

   // sprintf((char *)my_upload_data.datetime,"20%c%c-%c%c-%c%c",datetime[4],datetime[5],datetime[2],datetime[3],datetime[0],datetime[1]);

    DEBUG_PRINTF("解析数据为:date:%s,lat:%lf,%c,lon:%lf,%c\n",my_upload_data.datetime,my_upload_data.lat,my_upload_data.latdir,my_upload_data.lon,my_upload_data.londir);

            break;



        
    }
    DEBUG_PRINTF("获取到无效的定位数据，10秒后重新获取\n");

    Com_Delay_s(10);



}
//解析获取到的定位数据
//valid_data="$GPRMC,235316.000,A,2959.9925,S,12000.0090,E,0.009,75.020,020711,,,A*45";

    

}



void app_location_get_step_Data(void){


    Int_DS3553_Init();
    Int_DS3553_Get_Step(&my_upload_data.stepcount);
    DEBUG_PRINTF("stepcount=%d\n",my_upload_data.stepcount);





}



void app_location_Data_to_JSON(void){
    sprintf((char *)my_upload_data.uid,"%08X%08X%08X",HAL_GetUIDw2(),HAL_GetUIDw1(),HAL_GetUIDw0());
    cJSON *json=cJSON_CreateObject();

    // cJSON_AddStringToObject(json,"uid",(char *)my_upload_data.uid);
    // cJSON_AddNumberToObject(json,"lat",my_upload_data.lat);
    // cJSON_AddStringToObject(json,"latdir",(char *)&my_upload_data.latdir);
    // cJSON_AddNumberToObject(json,"lon",my_upload_data.lon);
    // cJSON_AddStringToObject(json,"londir",(char *)&my_upload_data.londir);
    // cJSON_AddStringToObject(json,"datetime",(char *)my_upload_data.datetime);
    // cJSON_AddNumberToObject(json,"stepcount",my_upload_data.stepcount);
    //这里有存在地址传递位数对齐的问题

    // 临时缓冲区：存单个字符+结束符（长度2足够）
    char latdir_str[2] = {0};
    char londir_str[2] = {0};

    // 把单个字符赋值到缓冲区（自动补\0）
    latdir_str[0] = my_upload_data.latdir;
    londir_str[0] = my_upload_data.londir;

    // 修正cJSON添加逻辑（传缓冲区，不是直接传单个字符地址）
    cJSON_AddStringToObject(json,"uid",(char *)my_upload_data.uid);
    cJSON_AddNumberToObject(json,"lat",my_upload_data.lat);
    cJSON_AddStringToObject(json,"latdir",latdir_str); // 改这里
    cJSON_AddNumberToObject(json,"lon",my_upload_data.lon);
    cJSON_AddStringToObject(json,"londir",londir_str); // 改这里
    cJSON_AddStringToObject(json,"datetime",(char *)my_upload_data.datetime);
    cJSON_AddNumberToObject(json,"stepcount",my_upload_data.stepcount);


    

    char *json_string=cJSON_PrintUnformatted(json);
    memcpy(my_upload_data.data,(uint8_t *)json_string,strlen(json_string));


    my_upload_data.data_len=strlen(json_string);
    DEBUG_PRINTF("json_string=%s\n",json_string);

    cJSON_Delete(json);//释放栈



    free(json_string); //释放堆






}




void app_location_send_data(void){
    CommonStatus ret=Int_QS100_SendData(my_upload_data.data,my_upload_data.data_len);
    if(ret==COMMON_OK){
        DEBUG_PRINTF("数据发送成功\n");
    }else{
        DEBUG_PRINTF("数据发送失败\n");





}
}



