/*******************************************************************************
**文件名:DrvBeep.h
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
#ifndef DRVBEEP_H
#define DRVBEEP_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

/******************************************************************************/  
#define BEEP_ON		(P3OUT |= BIT1)		//打开蜂鸣器
#define BEEP_OFF	(P3OUT &= ~BIT1)	//关闭蜂鸣器
/******************************************************************************/
#define ZD_ON		(P3OUT |= BIT2)		//打开蜂鸣器
#define ZD_OFF		(P3OUT &= ~BIT2)	//关闭蜂鸣器
/******************************************************************************/
typedef enum //定义蜂鸣器状态
{
  E_BUZZ_OFF = 0,
  E_BUZZ_ON,
  E_BUZZ_MAX,
} TE_BUZZ_STATE;
/******************************************************************************/
typedef enum//定义振动电机状态
{
  E_ZD_OFF = 0,
  E_ZD_ON,
  E_ZD_MAX,
} TE_ZD_STATE;

void DrvBuzzSetState(T_U8 buzz_state);
T_U8 DrvBuzzGetState(void);
void DrvVibMotorSetState(T_U8 vibrating_motor_state);
T_U8 DrvVibMotorGetState(void);

#endif	/* DRVBEEP_H */

