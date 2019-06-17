/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlTimer.c
*
* DESCRIPTION:
*   define some function interfaces of timer module
*
* REVISION HISTORY:
*
* END OF FILE HEADER
************************************************/
#include "CtlTimer.h"
#include "CtlTime.h"
#include "CtlEvent.h"
#include <assert.h>

/************************************************
* Declaration
************************************************/
typedef struct  
{
    T_U32 expire_time; //when expire_time is arrived, call the handle
    T_S32 timer_size;// the timer's size, unit:ms
    TF_TIMER_FUNC handle_func;
    TE_BOOLEAN is_activated; //is timer is activated
    TE_BOOLEAN is_repeat; // is timer is repeat
    TE_BOOLEAN is_screen_timer;/*is screen's timer? if it is, when exit screen, this id will auto stop*/
}TS_TIMER;

/************************************************
* Variable 
************************************************/
//timer manager
static TS_TIMER g_timer_mgr[E_TIMER_MAX];

/************************************************
* Function 
************************************************/

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   bind handle for timer id
*
* PROTOTYPE:    
*   void CtlTimerBindHandle( TE_TIMER_ID id,  TF_TIMER_FUNC func );
*
* ARGUMENT:
*   id:
*   func:
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerBindHandle( TE_TIMER_ID id,  TF_TIMER_FUNC func )
{
    assert(id<E_TIMER_MAX);
    g_timer_mgr[id].handle_func = func;
    g_timer_mgr[id].is_screen_timer = E_FALSE;
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   bind handle for timer id, the timer id is relate with screen
*   So when exit screen, the id will auto stop
*
* PROTOTYPE:    
*   void CtlTimerBindScreenHandle( TE_TIMER_ID id,  TF_TIMER_FUNC func );
*
* ARGUMENT:
*   id:
*   func:
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerBindScreenHandle( TE_TIMER_ID id,  TF_TIMER_FUNC func )
{
    assert(id<E_TIMER_MAX);
    g_timer_mgr[id].handle_func = func;
    g_timer_mgr[id].is_screen_timer = E_TRUE;
}
/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   start a timer(previous timer will be clear),
*   when milliseconds from now is pass, the handle function will be called.
*   Note: the timer only run once,
*   if you want to run again, please call it again at handle function.
*
* PROTOTYPE:    
*   void CtlTimerSet( TE_TIMER_ID id, T_U32 milliseconds );
*
* ARGUMENT:
*   id:
*   milliseconds:
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerSet( TE_TIMER_ID id, T_S32 milliseconds )
{
    T_U32 curr_time = CtlTimeSincePowerOn();

    assert(milliseconds > 0);
    assert( id<E_TIMER_MAX);
    
    #if MSECS_PER_TICK!=1
    //the timer value must multiples of one tick.
    if(milliseconds % MSECS_PER_TICK != 0)
    {
       milliseconds += MSECS_PER_TICK - milliseconds % MSECS_PER_TICK; 
    }
    #endif

    //set the expire time
    if(MAX_T_U32 - curr_time >= (T_U32)milliseconds)
    {
        g_timer_mgr[id].expire_time = curr_time + (T_U32)milliseconds;
    }
    else
    {
        g_timer_mgr[id].expire_time = (T_U32)milliseconds - (MAX_T_U32 - curr_time) - 1;
    }
    //active the timer
    g_timer_mgr[id].is_repeat = E_FALSE;
    g_timer_mgr[id].is_activated = E_TRUE;
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   same as CtlTimerSet,but it runs repeat,
*   the handle function will be called each milliseconds.
*   if you want to stop, please call CtlTimerClear.
*
* PROTOTYPE:    
*   void CtlTimerSetRepeat( TE_TIMER_ID id, T_U32 milliseconds );
*
* ARGUMENT:
*   id:
*   milliseconds:
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerSetRepeat( TE_TIMER_ID id, T_S32 milliseconds )
{
    //active the timer
    CtlTimerSet(id, milliseconds);
    //set it as repeat
    g_timer_mgr[id].timer_size = milliseconds;
    g_timer_mgr[id].is_repeat = E_TRUE;
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   stop the timer
*
* PROTOTYPE:    
*   void CtlTimerClear(TE_TIMER_ID id);
*
* ARGUMENT:
*   id:
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerClear( TE_TIMER_ID id )
{
    assert(id<E_TIMER_MAX);
    //deactive the timer
    g_timer_mgr[id].is_activated = E_FALSE;
    g_timer_mgr[id].is_repeat = E_FALSE;
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   check if the timer active
*
* PROTOTYPE:    
*   TE_BOOLEAN CtlTimerIsExist(TE_TIMER_ID id);
*
* ARGUMENT:
*   id:
*
* RETURN:
*   
* END OF FUNCTION HEADER
************************************************/
TE_BOOLEAN CtlTimerIsExist( TE_TIMER_ID id )
{
    assert(id<E_TIMER_MAX);
    if(g_timer_mgr[id].is_activated)
    {
        return E_TRUE;
    }
    else
    {
        return E_FALSE;
    }
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   stop all screen timer, called when exit screen
*
* PROTOTYPE:    
*   void CtlTimerClearAllOfScreen();
*
* ARGUMENT:
*
* RETURN:
*   
* END OF FUNCTION HEADER
************************************************/
void CtlTimerClearAllOfScreen(void)
{
    T_S32 i;
    for (i=0; i<E_TIMER_MAX; i++)
    {
        if(g_timer_mgr[i].is_screen_timer)
        {
            //clear timer
            CtlTimerClear((TE_TIMER_ID)i);
        }
    }
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   clear all timer
*
* PROTOTYPE:    
*   void CtlTimerClearAll();
*
* ARGUMENT:
*
* RETURN:
*   
* END OF FUNCTION HEADER
************************************************/
void CtlTimerClearAll(void)
{
    T_S32 i;
    for (i=0; i<E_TIMER_MAX; i++)
    {
        //clear timer
        CtlTimerClear((TE_TIMER_ID)i);
    }
}

/***********************************************
* COPYRIGHT 2016.
*
* Description:
*   handle timer event
*
* PROTOTYPE:    
*   void CtlTimerHandle();
*
* ARGUMENT:
*   
* RETURN:
*
* END OF FUNCTION HEADER
************************************************/
void CtlTimerHandle(void)
{
    T_U32 curr_time = CtlTimeSincePowerOn();
    T_S32 i;
    for (i=0; i<E_TIMER_MAX; i++)
    {
        //if timer is active and expire, clear the timer and call the handle
        if(g_timer_mgr[i].is_activated &&
            M_IsTimeAfterOrEq(curr_time, g_timer_mgr[i].expire_time) 
            )
        {
            if (g_timer_mgr[i].is_repeat)
            {
                //set timer again if need repeat
                CtlTimerSetRepeat((TE_TIMER_ID)i, g_timer_mgr[i].timer_size);
            }
            else
            {
                //clear timer
                CtlTimerClear((TE_TIMER_ID)i);
            }
            //call handle func
            if (g_timer_mgr[i].handle_func)
            {
                g_timer_mgr[i].handle_func();

                //after timer handle may be have some event, so need call event handle at once
                CtlEventHandleAll();
            }
        }
    }
}

