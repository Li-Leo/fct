/************************************************
* COPYRIGHT 2016.
*
* NAME: Init.h
*
* DESCRIPTION:
*   Define the interfaces of Initial module.
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/14/2016
*
* END OF FILE HEADER
************************************************/
#ifndef INIT_H
#define INIT_H

#include "..\TypeDefine.h"

#define BATT_CHK_TIME 3

/************************************************
* Declaration
************************************************/
void SysInit();
void ParaInit();
void ShowStartupPage();
void Initial();
void InitDriver();
void RecordInitToZero();
void BatteryPerformanceTest(T_U16 data1, T_U16 data2);
void GetBatteryPerfVolt(void);

#endif /* INIT_H */

