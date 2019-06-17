/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlInterrupt.h
*
* DESCRIPTION:
*   Interrupt control module head file
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLINTERRUPT_H
#define CTLINTERRUPT_H

#include<msp430fr5989.h>
#include"TypeDefine.h"

void InterruptCtlInit(void);
void InterruptCtlEnable(void);
void InterruptCtlDisable(void);

#endif /* CTLINTERRUPT_H */

