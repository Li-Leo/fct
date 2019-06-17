/************************************************
* COPYRIGHT 2016.
*
* NAME: ctl_key.c
*
* DESCRIPTION:
*   define some function interfaces of key module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlKey.h"
#include "CtlEvent.h"
#include "CtlTimer.h"
#include "CtlData.h"
#include "..\Driver\DrvKey.h"
#include "..\Driver\DrvRtc.h"
//#include "..\App\AppAlarm.h"
//#include "..\App\AppuiScreen.h"
#include "..\TypeDefine.h"
#include <string.h>
#include <assert.h>

/************************************************
* Declaration
************************************************/

//key event handle
typedef struct  
{
    //TE_KEY key;
    TF_KEY_EVENT_FUNC handle[E_KEY_EVENT_MAX];
} TS_KEY_HANDLE;

//combination key event handle
typedef struct  
{
    //TE_COMBINATION_KEY combination_key;
    TF_KEY_EVENT_FUNC handle[E_KEY_EVENT_MAX];
} TS_KEY_COMBINATION_HANDLE;

//combination key map
typedef struct  
{
    TE_KEY key1;
    TE_KEY key2;
    TE_KEY key3;
    TE_COMBINATION_KEY combination_key;
} TS_KEY_COMBINATION_MAP;

/************************************************
* Variable 
************************************************/
//all key handles
TS_KEY_HANDLE g_key_handles[E_KEY_MAX];
//all combination key handles
TS_KEY_COMBINATION_HANDLE g_combination_key_handles[E_COMBINATION_KEY_MAX];
//is someone combination key pressed
TE_BOOLEAN g_combination_key_is_pressed[E_COMBINATION_KEY_MAX];
//combination key map
const TS_KEY_COMBINATION_MAP g_combination_key_map[E_COMBINATION_KEY_MAX] = 
{
    {E_KEY_PLUS, E_KEY_MAIN, E_KEY_MINUS, E_COMBINATION_KEY_ENTER_FACTORY_MODE}
};
//monitor funs
TF_KEY_MONITOR_FUNC g_key_monitor_funs[E_KEY_MONITOR_EVENT_MAX];

//is the key block when one key is pressed
TE_BOOLEAN g_key_is_block_when_one_key_pressed[E_KEY_MAX];

TE_EVENT g_key_pre_event[E_KEY_MAX]={E_EVENT_NULL};

//the key current handle
TE_KEY g_key_current_handle_key;

//the valid pressed key. it is the first pressed key. the later pressed keys are not valid.
TE_KEY g_key_current_valid_pressed_key = E_KEY_MAX;

TE_KEY g_key_current_pressed_key = E_KEY_MAX;

extern TS_TIME_INFO g_current_time;
extern TS_RECORD_POINT g_record_point;
//extern TE_SCREEN_ID g_current_ui;

/************************************************
* Function 
************************************************/
void CtlNoOperation()
{
	CtlEventPost(E_EVENT_NO_OPERATION, 1);
}

void CtlKeyScan(void)
{
  //TE_EVENT event;
  //T_U8 key;
  
  DrvKeyGetState();
/*
  for(key=0;key<E_KEY_MAX;key++)
  {
    event = CtlKeyState((TE_KEY)key);
      
    if((event!=E_EVENT_NULL)&&(g_key_pre_event[key] != event))
    {
        g_key_pre_event[key] = event;
		
		if(event != E_EVENT_KEY_STUCK)
		{
        	CtlEventPost(event, (TE_KEY)key);
//				if (AlarmIsSet(E_ALARM_KEY_STUCK_ID))
//					AlarmClear(E_ALARM_KEY_STUCK_ID);
		}
		else if (g_current_ui != E_SCREEN_MANU_LOCA)
		{
            T_U32 data;
			AlarmSet(E_ALARM_KEY_STUCK_ID);
			switch((TE_KEY)key)
			{
				case E_KEY_MAIN:
					data = 9;
					break;
				case E_KEY_PLUS:
					data = 10;
					break;
				case E_KEY_MINUS:
					data = 11;
					break;
				default:
					data = 0;
					break;
			}
			SaveRec(E_RECORD_OPERATION, 91, data);
		}
		
		if(event != E_EVENT_KEY_RELEASED)
		{
			g_key_current_pressed_key = (TE_KEY)key;
		}

        if(event == E_EVENT_KEY_PRESSED)
        {
			CtlEventPost(E_EVENT_KEY_ANY_KEY_PRESSED, 1);            
        }
        else if(event == E_EVENT_KEY_RELEASED)
        {
			CtlEventPost(E_EVENT_KEY_ANY_KEY_RELEASED, 1);            
        }
    }
  }

*/
}

/***********************************************
* COPYRIGHT 2016 
*
* Description:
*   set key state if it is pressed
*   Note: can only called by CtlKeyScan
*
* PROTOTYPE:    
*   TE_EVENT CtlKeyState(TE_KEY key);
*
* ARGUMENT:
*   TE_KEY key:
*   
*   
* RETURN:
*    TE_EVENT event;
*   
* END OF FUNCTION HEADER
************************************************/
TE_EVENT CtlKeyState(TE_KEY key)
{
    TE_EVENT event;
    
    if(g_key_hold_time[key] == 0)
    {
      event = E_EVENT_NULL;  
    }
    else if(g_key_hold_time[key] == KEY_PRESS_FIRST_RELEASE_TIME)
    {
        //first released
        event = E_EVENT_KEY_RELEASED;
    }
    else
    {
        if(g_key_hold_time[key] < KEY_PRESS_HOLD_500MS_TIME)
        {
          event = E_EVENT_KEY_PRESSED;  
        }
        else if((g_key_hold_time[key] >= KEY_PRESS_HOLD_500MS_TIME)&&(g_key_hold_time[key] < KEY_LONG_PRESS_TIME))
        {
          event = E_EVENT_KEY_HELD;
        }
        else  
        {
          event = E_EVENT_KEY_HELD;
          //hold >= 1 second
          if(g_key_hold_time[key] >= KEY_LONG_PRESS_TIME)
          {
              //
          }
        
          //hold >= 2 second
          if(g_key_hold_time[key] >= KEY_PRESS_HOLD_2S_TIME)
          {
			event = E_EVENT_KEY_HELD_2S;
          }
          
          //hold >= 5 second
          if(g_key_hold_time[key] >= KEY_PRESS_HOLD_5S_TIME)
          {
            //
          }
        
          //hold >= 20 second
          if(g_key_hold_time[key] >= KEY_PRESS_HOLD_20S_TIME)
          {
            //
          }
        
          //hold >= 30 second
          if(g_key_hold_time[key] >= KEY_PRESS_STUCK_TIME)
          {
            event = E_EVENT_KEY_STUCK;
          }
       }
    }
    return event;
}

/***********************************************
* Description:
*   get the key's hold time
* Argument:
*   key:
*
* Return:
*
************************************************/
T_S32 CtlKeyHeldTime(TE_KEY key)
{
    if(g_key_hold_time[key] == KEY_PRESS_FIRST_RELEASE_TIME)
    {
        return 0;
    }
    else
    {
        return g_key_hold_time[key];
    }
}

/***********************************************
* Description:
*   used to let the key press is OK if the key's other event is exist, then can play bip
* Argument:
*
* Return:
*
************************************************/
static TE_BOOLEAN CtlKeyOnPress(void)
{
    T_S32 i;
    //if the key's other event is exist, return true
    for(i=E_KEY_EVENT_RELEASED; i<E_KEY_EVENT_MAX; i++)
    {
        if (g_key_handles[g_key_current_handle_key].handle[i] != NULL)
        {
            return E_TRUE;
        }
    }

    //if the combination key's other event is exist, return true
    for(i=E_COMBINATION_KEY_ENTER_FACTORY_MODE; i<E_COMBINATION_KEY_MAX; i++)
    {
        if (g_combination_key_handles[i].handle[E_KEY_EVENT_PRESSED] != NULL
            ||g_combination_key_handles[i].handle[E_KEY_EVENT_RELEASED]!=NULL)
        {
            if(g_combination_key_map[i].key1==g_key_current_handle_key
                ||g_combination_key_map[i].key2==g_key_current_handle_key)
            {
                return E_TRUE;
            }
        } 
    }

    return E_FALSE;
}


/***********************************************
* Description:
*   get how many key is pressed
* Argument:
*
* Return:
*
************************************************/
static T_S32 CtlKeyPressedCounter(void)
{
    T_S32 pressed_key_total = 0;
    T_S32 i;

    //calc how many keys is pressed
    for(i=0; i<E_KEY_MAX; i++)
    {
        if(CtlKeyIsPressed((TE_KEY)i))
        {
            pressed_key_total++;
        }
    }

    return pressed_key_total;
}

/***********************************************
* Description:
*   handle key event
* Argument:
*   key:
*   event:
*   combination_key_handle_valid: 
*       TRUE: it is combination key and the combination key has valid handle function
*       FALS: it is not combination key or the combination key has unvalid handle function 
*
* Return:
*
************************************************/
static void CtlKeyHandle(TE_KEY key, TE_KEY_EVENT event, TE_BOOLEAN combination_key_handle_valid)
{
    TE_BOOLEAN is_handled = E_FALSE;
    TF_KEY_EVENT_FUNC event_fun;
    
    assert(key<E_KEY_MAX && event<E_KEY_EVENT_MAX);

    g_key_current_handle_key = key;

    //call monitor fun
    if (event == E_KEY_EVENT_PRESSED && g_key_monitor_funs[E_KEY_MONITOR_EVENT_BEFORE_KEY_PRESS])
    {
        g_key_monitor_funs[E_KEY_MONITOR_EVENT_BEFORE_KEY_PRESS](key, E_FALSE);
    }

    event_fun = g_key_handles[key].handle[event];

    //if the key is not the valid pressed key and the event is not release, check if it can handle
    if(event_fun!=NULL && event!=E_KEY_EVENT_RELEASED && key!=g_key_current_valid_pressed_key)
    {
        //check if it is combination key and the combination key has valid handle function
        //if yes, no need handle the event and if the handle function is exist, it need set handle result as TRUE to play a beep
        if(combination_key_handle_valid)
        {
            is_handled = E_TRUE;
            event_fun = NULL; 
        }
        //check if this is blocked when exist valid pressed key
        else if(g_key_is_block_when_one_key_pressed[key])
        {
            //if this key is blocked, do not call the handler
            event_fun = NULL;
        }
    }
        
    //handle the key
    if(event_fun != NULL)
    {
        is_handled = event_fun();
    }

    //call monitor fun
    if (event == E_KEY_EVENT_PRESSED && g_key_monitor_funs[E_KEY_MONITOR_EVENT_AFTER_KEY_PRESS])
    {
        g_key_monitor_funs[E_KEY_MONITOR_EVENT_AFTER_KEY_PRESS](key, is_handled);
    }
    if (event == E_KEY_EVENT_RELEASED && g_key_monitor_funs[E_KEY_MONITOR_EVENT_AFTER_KEY_RELEASE])
    {
        g_key_monitor_funs[E_KEY_MONITOR_EVENT_AFTER_KEY_RELEASE](key, is_handled);
    }
}

/***********************************************
* Description:
*   handle combination key event
* Argument:
*   combination_key:
*   event:
*
* Return:
*
************************************************/
static void CtlKeyCombinationHandle(TE_COMBINATION_KEY combination_key, TE_KEY_EVENT event)
{
    TF_KEY_EVENT_FUNC event_fun;
    
    assert(combination_key<E_COMBINATION_KEY_MAX && event<E_KEY_EVENT_MAX);

    event_fun = g_combination_key_handles[combination_key].handle[event];
   
    if(event_fun != NULL)
    {
        event_fun();
    }
}

/***********************************************
* Description:
*   handle key press
* Argument:
*   param:
*
* Return:
*
************************************************/
static void CtlKeyPressHandle(T_U32 param)
{
    const TS_KEY_COMBINATION_MAP *combination_key = NULL;
    T_U16 i;
    TE_BOOLEAN is_combination_pressed = E_FALSE;
    T_S32 pressed_key_counter  = CtlKeyPressedCounter();

    //if press just two keys, check if it is combination key
    if(pressed_key_counter == 2)
    {
        //check if combination key is pressed
        for (i=0; i<sizeof(g_combination_key_map)/sizeof(g_combination_key_map[0]); i++)
        {
            combination_key = &g_combination_key_map[i];
            //check if two key is pressed 
            if((param == combination_key->key1) && CtlKeyIsPressed(combination_key->key2))
            {
                //another key is already pressed,so combination key is pressed
                is_combination_pressed = E_TRUE;
            }
            else if((param == combination_key->key2) && CtlKeyIsPressed(combination_key->key1))
            {
                //another key is already pressed,so combination key is pressed
                is_combination_pressed = E_TRUE;
            }
            if (is_combination_pressed)
            {
                g_combination_key_is_pressed[combination_key->combination_key] = E_TRUE;
                break;
            }
        }
    }
    else if(pressed_key_counter == 1)
    {
        g_key_current_valid_pressed_key = (TE_KEY)param;
    }

    //if it is combination key and it has bind fun, handle the key with null event fun
    if(combination_key && is_combination_pressed && 
        (g_combination_key_handles[combination_key->combination_key].handle[E_KEY_EVENT_PRESSED]
         ||g_combination_key_handles[combination_key->combination_key].handle[E_KEY_EVENT_RELEASED])
        )
    {
        CtlKeyHandle((TE_KEY)param, E_KEY_EVENT_PRESSED, E_TRUE);
        CtlKeyCombinationHandle(combination_key->combination_key, E_KEY_EVENT_PRESSED);
        return;
    }
    else
    {
        CtlKeyHandle((TE_KEY)param, E_KEY_EVENT_PRESSED, E_FALSE);
    }

}

/***********************************************
* Description:
*   handle key release
* Argument:
*   param:
*
* Return:
*
************************************************/
static void CtlKeyReleaseHandle(T_U32 param)
{
    const TS_KEY_COMBINATION_MAP *combination_key;
    T_U16 i;
    //TE_BOOLEAN is_combination_released = E_FALSE;

    //if the valid key release, reset it 
    if(param == g_key_current_valid_pressed_key)
    {
        g_key_current_valid_pressed_key = E_KEY_MAX;
    }

    CtlKeyHandle((TE_KEY)param, E_KEY_EVENT_RELEASED, E_FALSE);

    //check if combination key is released
    for (i=0; i<sizeof(g_combination_key_map)/sizeof(g_combination_key_map[0]); i++)
    {
        combination_key = &g_combination_key_map[i];
        //is_combination_released = E_FALSE;
        if(g_combination_key_is_pressed[combination_key->combination_key])
        {
            //check if key is one of combination key
            if((param == combination_key->key1) || (param == combination_key->key2))
            {
                //it is, so combination key is released
                g_combination_key_is_pressed[combination_key->combination_key] = E_FALSE;
                CtlKeyCombinationHandle(combination_key->combination_key, E_KEY_EVENT_RELEASED);
            }
        }
    }
}

/***********************************************
* Description:
*   handle key hold
* Argument:
*   param:
*
* Return:
*
************************************************/
static void CtlKeyHoldHandle(T_U32 param)
{
    CtlKeyHandle((TE_KEY)param, E_KEY_EVENT_HELD, E_FALSE);
}

/***********************************************
* Description:
*   handle key hold 500ms
* Argument:
*   param:
*
* Return:
*
************************************************/
static void CtlKeyHold2sHandle(T_U32 param)
{
    CtlKeyHandle((TE_KEY)param, E_KEY_EVENT_HELD_2S, E_FALSE);
}

/***********************************************
* Description:
*   key init
* Argument:
*
* Return:
*
************************************************/
void CtlKeyInit(void)
{
    CtlEventBindHandle(E_EVENT_KEY_PRESSED, CtlKeyPressHandle);
    CtlEventBindHandle(E_EVENT_KEY_RELEASED, CtlKeyReleaseHandle);
    CtlEventBindHandle(E_EVENT_KEY_HELD, CtlKeyHoldHandle);
    CtlEventBindHandle(E_EVENT_KEY_HELD_2S, CtlKeyHold2sHandle);
    
    //CtlTimerBindHandle(E_TIMER_SCAN_KEY, CtlKeyScan);
    //CtlTimerSetRepeat(E_TIMER_SCAN_KEY,5);
	//CtlTimerBindHandle(E_TIMER_NO_OPERATION_SLEEP, CtlNoOperation);
    //CtlTimerSet(E_TIMER_NO_OPERATION_SLEEP,NO_OPERATION_SLEEP_MS);
}

/***********************************************
* Description:
*   bind handle function for key event
* Argument:
*   key:
*   event:
*   fun:
*
* Return:
*
************************************************/
void CtlKeyEventBindHandle(TE_KEY key, TE_KEY_EVENT event, TF_KEY_EVENT_FUNC fun)
{
    assert(key<E_KEY_MAX && event<E_KEY_EVENT_MAX);
    g_key_handles[key].handle[event] = fun;
    //if bind valid fun on event, need set pressed event handle not null
    if (fun != NULL && event!=E_KEY_EVENT_PRESSED)
    {
        if(g_key_handles[key].handle[E_KEY_EVENT_PRESSED]==NULL)
        {
            g_key_handles[key].handle[E_KEY_EVENT_PRESSED] = CtlKeyOnPress;
        }
    }
}

/***********************************************
* Description:
*   bind handle function for combination key event,
*   Note: it only support pressed and released event
* Argument:
*   key:
*   event:
*   fun:
*
* Return:
*
************************************************/
void CtlKeyCombEventBindHandle(TE_COMBINATION_KEY key, TE_KEY_EVENT event, TF_KEY_EVENT_FUNC fun)
{
    assert(key<E_COMBINATION_KEY_MAX && event<E_KEY_EVENT_MAX);
    assert(event ==E_KEY_EVENT_PRESSED || event==E_KEY_EVENT_RELEASED);
    g_combination_key_handles[key].handle[event] = fun;

    //if bind valid fun on event, need set the two key's pressed event handle not null
    if (fun != NULL)
    {
        TE_KEY alone_key;
        
        alone_key = g_combination_key_map[key].key1;
        if(g_key_handles[alone_key].handle[E_KEY_EVENT_PRESSED]==NULL)
        {
            g_key_handles[alone_key].handle[E_KEY_EVENT_PRESSED] = CtlKeyOnPress;
        }

        alone_key = g_combination_key_map[key].key2;
        if(g_key_handles[alone_key].handle[E_KEY_EVENT_PRESSED]==NULL)
        {
            g_key_handles[alone_key].handle[E_KEY_EVENT_PRESSED] = CtlKeyOnPress;
        }
    }
}

/***********************************************
* Description:
*   clear all key handle
* Argument:
*
* Return:
*
************************************************/
void CtlKeyClearAllHandle(void)
{
    memset(g_key_handles, 0, sizeof(g_key_handles));
    memset(g_combination_key_handles, 0, sizeof(g_combination_key_handles));
}

/***********************************************
* Description:
*   clear combination key handle
* Argument:
*
* Return:
*
************************************************/
void CtlKeyClearAllCombHandle(void)
{
    memset(g_combination_key_handles, 0, sizeof(g_combination_key_handles));
}

/***********************************************
* Description:
*   clear one key's handle
* Argument:
*
* Return:
*
************************************************/
void CtlKeyClearHandle(TE_KEY key)
{
    memset(g_key_handles[key].handle, 0, sizeof(g_key_handles[key].handle));
}

/***********************************************
* Description:
*   is the key pressed
* Argument:
*   key:
*
* Return:
*
************************************************/
TE_BOOLEAN CtlKeyIsPressed(TE_KEY key)
{
  TE_BOOLEAN is_pressed = E_FALSE;
 
  assert(key<E_KEY_MAX);
  
  
  if((g_key_hold_time[key] > 500))//&&(g_key_hold_time[key] != KEY_PRESS_FIRST_RELEASE_TIME)
  {
    is_pressed = E_TRUE;
  }
    
  return is_pressed;
}

/***********************************************
* Description:
*   is any key pressed except one key
* Argument:
*   key:
*
* Return:
*
************************************************/
TE_BOOLEAN CtlKeyIsAnyKeyPressedExcept( TE_KEY key )
{
    T_S32 i;
    for (i=0; i<E_KEY_MAX; i++)
    {
        //check if key is pressed
        if (i!=key && CtlKeyIsPressed((TE_KEY)i))
        {
            return E_TRUE;
        }
    }
    return E_FALSE;
}

/***********************************************
* Description:
*   is the key stuck
* Argument:
*   key:
*
* Return:
*
************************************************/
TE_BOOLEAN CtlKeyIsStuck(TE_KEY key)
{
  TE_BOOLEAN is_stuck = E_FALSE;
 
  assert(key<E_KEY_MAX);
  
  
  if((g_key_hold_time[key] >= KEY_PRESS_STUCK_TIME) &&
  	(g_key_hold_time[key] !=KEY_PRESS_FIRST_RELEASE_TIME))
  {
    is_stuck = E_TRUE;
  }
  
  return is_stuck;
}


/***********************************************
* Description:
*   bind handle function to monitor key
* Argument:
*   event:
*   fun:
*
* Return:
*
************************************************/
void CtlKeyMonitorEventBindHandle( TE_KEY_MONITOR_EVENT event, TF_KEY_MONITOR_FUNC fun )
{
    assert(event<E_KEY_MONITOR_EVENT_MAX);
    g_key_monitor_funs[event] = fun;
}

/***********************************************
* Description:
*   set all key blocked when other key is pressed
* Argument:
*
* Return:
*
************************************************/
void CtlKeySetAllBlockedWhenKeyPressed(void)
{
    T_S32 i;

    for(i=0; i<E_KEY_MAX; i++)
    {
        g_key_is_block_when_one_key_pressed[i] = E_TRUE;
    }
}

