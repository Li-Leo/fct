/*******************************************************************************
**�ļ���:DrvHall.h
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
#ifndef DRVHALL_H
#define DRVHALL_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

/******************************************************************************/  
#define HALL_POWER_ON	(P4OUT |= BIT0)		//�򿪻���
#define HALL_POWER_OFF	(P4OUT &= ~BIT0)	//�رջ���

#define MILIMETER  15      // 1.5 milimeter

/******************************************************************************/
typedef enum //�������������״̬
{
  E_HALL_LOW = 0,
  E_HALL_HIGH=4,
  E_HALL_MAX,
} TE_HALL_STATE;

typedef enum //���巵�ػ�����������״̬
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
