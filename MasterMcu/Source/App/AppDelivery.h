/************************************************
* COPYRIGHT 2016.
*
* NAME: AppDelivery.h
*
* DESCRIPTION:
*   Delivery module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/22/2016
*
* END OF FILE HEADER
************************************************/

#ifndef APP_DELIVERY_H
#define APP_DELIVERY_H

#include "..\TypeDefine.h"
#include "CtlDelivery.h"

#define MAX_MINUTE_IN_ONE_DAY (24*60)
#define MAX_SECS_IN_ONE_DAY ((T_S32)24*60*60)
#define WAKE_UP_TIME_BEFORE_SECS 10

extern TS_TIME_INFO g_next_deliver_time;
extern TE_BOOLEAN g_is_delivery_dose_matched;
//extern TE_BOOLEAN g_is_delivery_stopped;
extern TE_BOOLEAN g_is_need_sleep_after_delivery_stopped;
//the total delivery dose today when start delivery
extern T_U16 g_delivery_today_total_dose_when_start;



//return the next deliver point[0~95],
//if return -1, it is no deliver point
T_S16 GetNextDeliverPoint(T_U16 *next_dose_value, T_S16 *week);


//void SetNextWakeTime(TS_TIME_INFO next_time);
//TS_TIME_INFO GetNextWakeTime();
//return the next delivery seconds from now
T_S32 GetNextDeliverySecsFromNow();
//TE_BOOLEAN IsTimeUp();
void ExecDelivery(TE_DOSE_THICKNESS thickness, T_U16 dose);
T_U16 GetDeliveredDose();
TE_BOOLEAN IsUptoDose();
void TermDelivery();
void SetNextDeliTimePoint();
T_S32 ConvertTimeToSecond(TS_TIME_INFO *time_point);

//calc the diff seconds by the week, hour, minute, seconds
T_S32 DiffDateTimeInOneDay(TS_TIME_INFO *new_time, TS_TIME_INFO *old_time);

//reduce the dest time by secs, only change week, hour, minute, seconds
void ReduceSecsInOneDay(TS_TIME_INFO *dest, T_S32 secs);
void ConvertSecsToTimeInOneDay(TS_TIME_INFO *dest, T_S32 secs);


#endif /* APP_DELIVERY_H */

