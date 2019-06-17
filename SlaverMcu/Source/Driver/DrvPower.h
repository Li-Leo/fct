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

//#include "DrvLcd.h"
#include "DrvPort.h"

extern T_U8 g_enter_low_power_state;

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

//enable the power for hall and motor current
void DrvPowerEnableHallAndMotorCurrent();
//disable the power for hall and motor current
void DrvPowerDisableHallAndMotorCurrent();


void DrvLowPower(void);  /* setting system into low power mode */


#endif /* DRVPOWER_H */


