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

#define BATTERY_PERFORMANCE_CHK_LIMIT   0xff    //电池性能检测通过下限值
#define BATT_CHK_ENABLE (P4OUT |= BIT3)		//电池性能检测使能
#define BATT_CHK_DISABLE (P4OUT &= ~BIT3)	//电池性能检测使能
/************************************************
* Declaration
************************************************/
void SysInit();
void ParaInit();
void Initial();
void InitDriver();

#endif /* INIT_H */

