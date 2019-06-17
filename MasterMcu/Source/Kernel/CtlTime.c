/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlTime.c
*
* DESCRIPTION:
*   define some function interfaces of time module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/05/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlTime.h"

/************************************************
* Declaration
************************************************/

/************************************************
* Variable 
************************************************/
//the time elapse from power on to now, used for count.
//unit: ms
extern T_U32 g_time_ms_elapse;
TE_BOOLEAN g_time_is_tick_handled = E_FALSE;

/************************************************
* Function 
************************************************/

/************************************************
* COPYRIGHT 2016.
*
* DESCRIPTION:
*   when 1 tick elapsed, call it, only called by the timer interrupt
*
* PROTOTYPE: 
*   void CtlTimeOnOneTick();
*
* ARGUMENT:
*
* RETURN:
*   
* END OF FUNCTION HEADER
************************************************/
void CtlTimeOnOneTick(T_S16 time_ms)
{
    //add one tick
    g_time_ms_elapse += time_ms;
	if (g_time_ms_elapse == MAX_T_U32)
		g_time_ms_elapse = 0;
    //set tick as not handled
    g_time_is_tick_handled = E_FALSE;
}

/***********************************************
* COPYRIGHT 2016. 
*
* Description:
*   calc the elapsed time from old_time to now, 
*   unit: ms
*
* PROTOTYPE:    
*   T_U32 CtlTimeElapse(T_U32 old_ms_time);
*
* ARGUMENT:
*   old_ms_time: unit=ms
*   
* RETURN:
*   the elapsed time (unit: ms)
* END OF FUNCTION HEADER
************************************************/
T_U32 CtlTimeElapse(T_U32 old_ms_time)
{
    if(g_time_ms_elapse >= old_ms_time)
    {
        return g_time_ms_elapse-old_ms_time;
    }
    else
    {
        return MAX_T_U32-old_ms_time+g_time_ms_elapse+1;
    }
}

/***********************************************
* COPYRIGHT 2016. 
*
* Description:
*   calc the elapsed time from old_ms_time to new_ms_time, unit: ms
*
* PROTOTYPE:    
*   T_U32 CtlTimeDiff( T_U32 new_ms_time, T_U32 old_ms_time );
*
* ARGUMENT:
*   new_ms_time: unit=ms
*   old_ms_time: unit=ms
*   
* RETURN:
*   the elapsed time (unit: ms)
* END OF FUNCTION HEADER
************************************************/
T_U32 CtlTimeDiff( T_U32 new_ms_time, T_U32 old_ms_time )
{
    if(new_ms_time >= old_ms_time)
    {
        return new_ms_time-old_ms_time;
    }
    else
    {
        return MAX_T_U32-old_ms_time+new_ms_time+1;
    }
}

/***********************************************
* COPYRIGHT 2016. 
*
* Description:
*   get current time,it will recount when it reach MAX_T_U32 millisecond 
*   unit: ms
*
* PROTOTYPE:    
*   T_U32 CtlTimeCurrent();
*
* ARGUMENT:
*   
* RETURN:
*   current time (unit: ms)
* END OF FUNCTION HEADER
************************************************/
T_U32 CtlTimeSincePowerOn(void)
{
    return g_time_ms_elapse;
}

/***********************************************
* Description:
*   after tick occurred, is the tick handled
* Argument:
*
* Return:
*   E_TRUE: it is handled
*   E_FALSE: it is not handled
************************************************/
TE_BOOLEAN CtlTimeIsTickHandled( void )
{
    return g_time_is_tick_handled;
}

/***********************************************
* Description:
*   set the tick handled or not handled
* Argument:
*   is_handled:  
*           E_TRUE:set it is handled
*           E_FALSE: set it is not handled
*
* Return:
*
************************************************/
void CtlTimeSetTickHandled( TE_BOOLEAN is_handled )
{
    g_time_is_tick_handled = is_handled;
}

