#ifndef __APP_LOCATION_H__
#define __APP_LOCATION_H__


#include "main.h"

#include "Int_AT6558R.h"
#include "Int_QS100.h"
#include "Int_DS3553.h"
#include "cJSON.h"




void app_location_get_gps_Data(void);



void app_location_get_step_Data(void);



void app_location_Data_to_JSON(void);




void app_location_send_data(void);























#endif 







