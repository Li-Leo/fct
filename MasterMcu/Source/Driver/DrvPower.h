/*******************************************************************************
**文件名:DrvPower.h
**
**功能:
**
**作者:HJH
**
**日期:2016-04-13
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#ifndef DRVPOWER_H
#define DRVPOWER_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

#include "DrvLcd.h"
#include "DrvPort.h"


#define MOTOR_POWER_ON  (P4OUT |= BIT3)		//打开直流电机电源
#define MOTOR_POWER_OFF (P4OUT &= ~BIT3)	//关闭直流电机电源

typedef enum
{
  E_MOTOR_POWER_OFF = 0,
  E_MOTOR_POWER_ON,
  E_MOTOR_POWER_MAX
} TE_MOTOR_POWER_STATE;

void DrvInitPowerControl(void);
void DrvPowerSleep(void);
void DrvPowerStandby(void);
void DrvPowerOn(void);

void DrvPowerEnterLowMode(void); /* setting system into low power mode */
void DrvPowerExitLowMode(void);  /* setting system exit low power mode */




#endif /* DRVPOWER_H */

