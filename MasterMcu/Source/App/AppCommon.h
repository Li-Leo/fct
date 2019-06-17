#ifndef APPCOMMON_H
#define APPCOMMON_H
#include "common.h"
//#include "AppAlarm.h"

#define DISTANCE_MAX_ENCODER 400*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10

void AppStartRun(TE_DIRECTION direction, T_S32 expect_encoder);
void AppStopRun();
void ExcuteSleep();
void ExcuteHallLocationCal();


#endif
