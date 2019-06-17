/*******************************************************************************
**文件名:DrvEncoder.h
**
**功能:
**
**作者:HJH
**
**日期:2016-04-11
**
**修改内容:
**
**作者:
**
**日期:
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
