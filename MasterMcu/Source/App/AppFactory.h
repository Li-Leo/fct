/************************************************
* COPYRIGHT 2016.
*
* NAME: AppFactory.h
*
* DESCRIPTION:
*   Application layer, head file of in-factory aging module
*
* REVISION HISTORY:
*	Created by Li at 06/20/2016
*	Modified and fixed errors by Kevin Shi at 08/09/2016.
*
* END OF FILE HEADER
************************************************/

#ifndef APP_FACTORY_H
#define APP_FACTORY_H

#include "TypeDefine.h"
#include "CtlData.h"
//#include "AppSetInfuPara.h"
//#include "CtlAdc.h"
#include "DrvEncoder.h"
#include "CtlKey.h"
#include "Fonts.h"
//#include "AppuiScreen.h"
#include "CtlDelivery.h"

#define TOOLING_HALL_CAL_VOLUME	2860	//工装2.860ml位置,已乘1000
#define HALL_TO_END_MILIMETER	1500	//hall到底的长度 ,unit: 0.001mm
#define AGING_MODE_DELIVER_DOSE	150		//老化模式每次输注剂量，已乘10
#define AGING_MODE_DELI_PERIOD	6		//老化模式输注间隔，单位15分钟
#define AGING_MODE_THICKNESS	E_DOSE_THICKNESS_200
#define TRANS_CAL_FREQ_DELTA_MAX	200	//temporary

typedef struct
{
	T_U8 string_buff[256];
    //TE_STRING_ID string_type;
} TS_STRING_DISPLAY;
extern TS_STRING_DISPLAY g_String;

void SetAgingMode(void);
void ExcuteHallLocationCal();
void SendPressPlusButtonSignal(TE_BOOLEAN is_run_or_stop);
void SendPressMinusButtonSignal(TE_BOOLEAN is_run_or_stop);
void FinishHallLocationCal();
//T_U16 ExcuteOcclusionCal();
T_U16 GetCalCurrent();
void copy_string(T_U8 *dst, const T_U8 *src, T_U16 len);

void ExcuteTransmissionCal();
void TransmissionCalOnGetMotorCurrent(T_U16 data);



void GetDeviceId();
void GetSoftwareVerInfo();
void GetHardwareVerInfo();
void ShowDeviceInfo();

void DeleteAllInfusionRecord();
void DeleteAllDaysTotalRecord();
void DeleteAllInfusionStopRecord();
void DeleteAllOperationRecord();
void DeleteAllSysPara();

void FactoryReset();

#endif	/* APP_FACTORY_H */

/****************************************************************
 *******************     end of file    *************************
****************************************************************/

