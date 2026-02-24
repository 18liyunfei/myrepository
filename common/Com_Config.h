#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__




#include "main.h"




typedef enum{



    COMMON_OK,
    COMMON_ERROR,
}CommonStatus;

typedef struct{

    uint8_t uid[33];
    //gps定位信息
    double lon; //经度
    uint8_t londir; //经度方向

    double lat; //纬度
    uint8_t latdir; //纬度方向




    //定位时间
    uint8_t datetime[21];




    //计步信息
    uint32_t stepcount;



    //最终发送的数据
    uint8_t data[512];
    uint16_t data_len;








}
upload_data;






#endif /* __COM_CONFIG_H__ */



