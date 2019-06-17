/*******************************************************************************
**�ļ���:DrvBeep.h
**
**����:
**
**����:HJH
**
**����:2016-04-11
**
**�޸�����:	Adjust the codes according to the Coding Standard.
**
**����:Kevin Shi
**
**����:04/21/2016
**
*******************************************************************************/
#ifndef DRVBEEP_H
#define DRVBEEP_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

/******************************************************************************/  
#define BEEP_ON		(P3OUT |= BIT1)		//�򿪷�����
#define BEEP_OFF	(P3OUT &= ~BIT1)	//�رշ�����
/******************************************************************************/
#define ZD_ON		(P3OUT |= BIT2)		//�򿪷�����
#define ZD_OFF		(P3OUT &= ~BIT2)	//�رշ�����
/******************************************************************************/
typedef enum //���������״̬
{
  E_BUZZ_OFF = 0,
  E_BUZZ_ON,
  E_BUZZ_MAX,
} TE_BUZZ_STATE;
/******************************************************************************/
typedef enum//�����񶯵��״̬
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

