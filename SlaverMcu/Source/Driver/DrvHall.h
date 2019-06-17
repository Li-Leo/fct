/*******************************************************************************
**文件名:DrvHall.h
**
**功能:
**
**作者:HJH
**
**日期:2016-04-11
**
**修改内容:	Adjust the codes according to the Coding Standard.
**
**作者:Kevin Shi
**
**日期:04/21/2016
**
*******************************************************************************/
#ifndef DRVHALL_H
#define DRVHALL_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

/******************************************************************************/  
#define HALL_POWER_ON	(P4OUT |= BIT0)		//打开霍尔
#define HALL_POWER_OFF	(P4OUT &= ~BIT0)	//关闭霍尔

#define MILIMETER  15      // 1.5 milimeter

/******************************************************************************/
typedef enum //定义霍尔传感器状态
{
  E_HALL_LOW = 0,
  E_HALL_HIGH=4,
  E_HALL_MAX,
} TE_HALL_STATE;

typedef enum //定义返回霍尔传感器的状态
{
  E_NOT_ARRIVAL_HALL_POSITION= 0,
  E_ARRIVAL_HALL_POSITION,
  E_HALL_POSITION_MAX,
} TE_HALL_RETURN;

typedef enum
{
  E_HALL_POWER_OFF = 0,
  E_HALL_POWER_ON,
  E_HALL_POWER_MAX
} TE_HALL_POWER_STATE;


void DrvHallSetPower(T_U8 hall_state);
void DrvHallInit(void);
void DrvHallEnable(void);
void DrvHallDisable(void);
TE_HALL_RETURN DrvHallGetState(void);

#endif	/* DRVBEEP_H */

/***************************   END OF FILE   **********************************/
