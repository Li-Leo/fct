/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlKey.h
*
* DESCRIPTION:
*   define some function interfaces of key module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLKEY_H
#define CTLKEY_H

#include "..\TypeDefine.h"
#include "..\Driver\DrvKey.h"
#include "CtlEvent.h"

#define	KEY_STABILIZE_TIME	20
#define	NO_OPERATION_SLEEP_MS	((T_S32)30 * (T_S32)1000)   //300

/************************************************
* Declaration
************************************************/
//all combination key
typedef enum
{
    E_COMBINATION_KEY_ENTER_FACTORY_MODE = 0,
    E_COMBINATION_KEY_MAX
} TE_COMBINATION_KEY;

//all key event 
typedef enum
{
    E_KEY_EVENT_PRESSED = 0,
    E_KEY_EVENT_RELEASED,
    E_KEY_EVENT_HELD,  
    E_KEY_EVENT_HELD_2S,  
    E_KEY_EVENT_MAX
} TE_KEY_EVENT;

//all monitor event
typedef enum
{
    E_KEY_MONITOR_EVENT_BEFORE_KEY_PRESS = 0,
    E_KEY_MONITOR_EVENT_AFTER_KEY_PRESS,
    E_KEY_MONITOR_EVENT_AFTER_KEY_RELEASE,    
    E_KEY_MONITOR_EVENT_MAX
} TE_KEY_MONITOR_EVENT;

typedef TE_BOOLEAN (*TF_KEY_EVENT_FUNC)(void);
typedef void (*TF_KEY_MONITOR_FUNC)(TE_KEY key, TE_BOOLEAN is_key_handled);

//key init
void CtlKeyInit(void);

//bind handle function for key event
void CtlKeyEventBindHandle(TE_KEY key, TE_KEY_EVENT event, TF_KEY_EVENT_FUNC fun);

//bind handle function for combination key event,
//Note: it only support pressed and released event
void CtlKeyCombEventBindHandle(TE_COMBINATION_KEY key, TE_KEY_EVENT event, TF_KEY_EVENT_FUNC fun);

//clear all key handle(include combination key)
void CtlKeyClearAllHandle(void);

//clear combination key handle
void CtlKeyClearAllCombHandle(void);

//clear one key's handle
void CtlKeyClearHandle(TE_KEY key);

//is key pressed
TE_BOOLEAN CtlKeyIsPressed(TE_KEY key);

//is any key pressed except one key(you can use E_KEY_MAX to check any key)
TE_BOOLEAN CtlKeyIsAnyKeyPressedExcept(TE_KEY key);

//is key stuck
TE_BOOLEAN CtlKeyIsStuck(TE_KEY key);

//get key state
TE_EVENT CtlKeyState(TE_KEY key);

//get the key's hold time
T_S32 CtlKeyHeldTime(TE_KEY key);

//bind handle function to monitor key
void CtlKeyMonitorEventBindHandle(TE_KEY_MONITOR_EVENT event, TF_KEY_MONITOR_FUNC fun);

void CtlKeyScan(void);

//set all key blocked when other key is pressed
void CtlKeySetAllBlockedWhenKeyPressed(void);

#endif /* CTLKEY_H */

