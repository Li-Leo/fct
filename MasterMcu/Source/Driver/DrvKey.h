/*********************************************************************************
 * $Header: DrvEncoder.h
 * Descripton:
 *		This file is the head file of the key moudle.
 * Original Author: Kevin Shi
 ********************************************************************************/
#ifndef DRVKEY_H
#define DRVKEY_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

typedef enum 
{
	E_KEY_MAIN = 0,	//Ö÷
	E_KEY_PLUS,		//£«
	E_KEY_MINUS,	//£­
	E_KEY_MAX,
} TE_KEY;

#define SCAN_KEY_MAIN_STATUS	P1IN
#define SCAN_KEY_PLUS_STATUS	P2IN
#define SCAN_KEY_MINUS_STATUS	P2IN

#define KEY_SCAN_MILLI_SECOND	5

//the time key pressed to fit the TE_PRESS_STATUS.
#define KEY_PRESS_TIME 50
#define KEY_PRESS_HOLD_500MS_TIME 510
#define KEY_LONG_PRESS_TIME 1010
#define KEY_PRESS_HOLD_2S_TIME 1010
#define KEY_PRESS_HOLD_5S_TIME 5010
#define KEY_PRESS_HOLD_20S_TIME 20010
#define KEY_PRESS_STUCK_TIME 30010
#define KEY_PRESS_FIRST_RELEASE_TIME 0xFFFF


extern T_U16 g_key_hold_time[E_KEY_MAX];

void DrvKeyInitial(void);		//°´¼ü³õÊ¼»¯
void DrvKeyGetState(void);	//¼ì²â°´¼ü×´Ì¬
void DrvKeyPostClearState(void);	//Çå³ý°´¼ü×´Ì¬

#endif /* DRVKEY_H */

