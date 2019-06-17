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

#define BATTERY_PERFORMANCE_CHK_LIMIT   0xff    //������ܼ��ͨ������ֵ
#define BATT_CHK_ENABLE (P4OUT |= BIT3)		//������ܼ��ʹ��
#define BATT_CHK_DISABLE (P4OUT &= ~BIT3)	//������ܼ��ʹ��
/************************************************
* Declaration
************************************************/
void SysInit();
void ParaInit();
void Initial();
void InitDriver();

#endif /* INIT_H */

