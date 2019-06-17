/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlAlarm.h
*
* DESCRIPTION:
*   alarm module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/11/2016
*
* END OF FILE HEADER
************************************************/

#ifndef CTLALARM_H
#define CTLALARM_H

#include "..\TypeDefine.h"

#define PLAY_MS 200
#define WAIT_MS 100

/* -------------external functions ------------- */
void SetPlayNone();
void SetPlayOnce();
void SetPlayOnceNoVibration();
void SetPlayTwice();

#endif /* CTLALARM_H */

