/*******************************************************************************
**�ļ���:DrvEncoder.h
**
**����:
**
**����:HJH
**
**����:2016-04-11
**
**�޸�����:
**
**����:
**
**����:
**
*******************************************************************************/
#ifndef DRV_ENCODER_H
#define DRV_ENCODER_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

void DrvEncoderInitial(void);
//void DrvEncoderSetSleep(void);
void DrvEncoderEnable(void);
void DrvEncoderDisable(void);

#endif /* DRV_ENCODER_H */
