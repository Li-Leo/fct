/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlDelivery.h
*
* DESCRIPTION:
*   motor module head file
*
* REVISION HISTORY:
*	Created by ryan at 11/23/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlMotor.h"
#include "drvpower.h"
#include "common.h"
//#include "AppAlarm.h"
#include "CtlDelivery.h"
#include "CtlSpiComm.h"
#include "CtlData.h"
#include "CtlTimer.h"
#include "CtlTime.h"
#include "CtlMotor.h"

#define CHECK_ENCODER_STOP_MS 100
#define MOTOR_MAX_STOP_TIME 2000

TE_DIRECTION g_motor_running_dir = E_MOVE_FORWARD;
TE_MOTOR_STATE g_motor_state = E_MOTOR_STATE_SMAX; 
TE_MOTOR_STATE g_motor_need_to_state = E_MOTOR_STATE_SMAX; 

TE_BOOLEAN g_motor_is_allow_stop_by_other_cpu;
TE_MOTOR_STOP_CAUSE g_motor_stop_cause;
T_S16 g_motor_stop_cause_of_alarm;

//TE_BOOLEAN g_motor_is_encoder_stoping = E_FALSE;
static T_U32 g_motor_last_encoder;
static T_U32 g_motor_first_stop_encoder;
static T_U32 g_motor_stoping_time;


void CtlMotorPrintStopInfor()
{
    M_MotorPrint("motor stoped, curr pos[%lu]\n", g_motor_position);
    M_MotorPrint("have run %lu encoder, expect encoder[%lu], first stop encoder:%lu\n", 
        g_encoder_counter, g_encoder_expect_counter, g_motor_first_stop_encoder);
    switch(g_motor_stop_cause)
    {
        case E_MOTOR_STOP_BY_USER:
            break;
        case E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET:
            M_MotorPrint("stop cause: finish target\n");
            break;
        case E_MOTOR_STOP_BY_POS_ZERO:
            M_MotorPrint("stop cause: arrive pos 0\n");
            break;
        case E_MOTOR_STOP_BY_ALARM:
            switch(g_motor_stop_cause_of_alarm)
            {
                // case E_ALARM_BATTERY_EXHAUSTION_ID:
                //     M_MotorPrint("stop cause: battery exhaustion\n");        
                //     break;
                // case E_ALARM_DOSE_EMPTY_ID:
                //     M_MotorPrint("stop cause: dose empty\n");        
                //     break;
                // case E_ALARM_OCCLUSION_ID:
                //     M_MotorPrint("stop cause: occlusion\n");        
                //     break;
                default:
                    M_MotorPrint("stop cause: alarm[%d]\n", g_motor_stop_cause_of_alarm);        
                    break;
            }
            
            break;     
        case E_MOTOR_STOP_BY_MEET_HALL:
            M_MotorPrint("stop cause: meet hall\n");
            break;
        case E_MOTOR_STOP_BY_AMCU_NOT_ACTIVE:
            M_MotorPrint("stop cause: amcu not active\n");
            break;
        case E_MOTOR_STOP_BY_OVERRUN:
            M_MotorPrint("stop cause: over run\n");
            break;
        default:
            M_MotorPrint("stop cause: [%d]\n", g_motor_stop_cause); 
            break;
    }
}
    

void CtlMotorCheckEncoderStop()
{
    TE_BOOLEAN is_need_stop = E_FALSE;
    
    if(g_motor_last_encoder != g_encoder_counter)
    {
        g_motor_last_encoder = g_encoder_counter;
        M_MotorPrint("continue run: %lu\n",g_motor_last_encoder);
    }
    else
    {
        is_need_stop = E_TRUE;
    }

    if(!is_need_stop && CtlTimeElapse(g_motor_stoping_time)>=MOTOR_MAX_STOP_TIME)
    {
        //if time out, force stop
        is_need_stop = E_TRUE;
        M_MotorPrint("motor stop time out, force stop!\n");
    }
    
    if(is_need_stop)
    {
        //g_motor_is_encoder_stoping = E_FALSE;
	    DrvEncoderDisable();
        MOTOR_POWER_OFF;        
        g_motor_state = g_motor_need_to_state;
        CtlTimerClear(E_TIMER_CHECK_ENCODER_STOP);

        CtlMotorPrintStopInfor();
        //force to write the motor pos
        SaveMotorPos(E_TRUE);
        if(g_motor_state == E_MOTOR_PAUSE)
        {
            CtlEventPost(E_EVENT_ENTER_PAUSE_STATE, 0);
        }
        else
        {
            CtlEventPost(E_EVENT_ENTER_STOP_STATE, 0);
        }
        CtlEventPost(E_EVENT_MOTOR_STOP, g_motor_stop_cause);
        //CtlEventPost(E_EVENT_ENCODER_STOP, 0);
    }
}

//start motor
void CtlMotorStart(TE_DIRECTION direction, T_S32 expect_encoder)
{
    if(direction == E_MOVE_FORWARD)
    {
        M_MotorPrint("motor start forward, expect encoder[%ld], curr pos[%lu]\n", 
           expect_encoder, g_motor_position);
    }
    else
    {
        M_MotorPrint("motor start backword,expect encoder[%ld], curr pos[%lu]\n", 
        expect_encoder, g_motor_position);
    }
    //if now is not running, post run event to notify user
    if(!CtlMotorIsRunning() || CtlMotorIsStoping())
    {
        CtlEventPost(E_EVENT_ENTER_RUN_STATE, 0);        
    }
    CtlTimerClear(E_TIMER_CHECK_ENCODER_STOP);

    //clear encoder count
    g_encoder_counter = 0;
    //set expect encoder
    g_encoder_expect_counter = expect_encoder;
        
	g_motor_running_dir = direction;
    g_motor_state = E_MOTOR_RUN;
    g_motor_stop_cause = E_MOTOR_STOP_BY_USER;
    g_motor_stop_cause_of_alarm = -1;      
    g_motor_is_allow_stop_by_other_cpu = E_FALSE;
	MOTOR_POWER_ON;
    DrvEncoderEnable();
}

//stop motor
void CtlMotorStopInternal(TE_MOTOR_STATE state)
{
    if(g_motor_state == E_MOTOR_RUN)
    {
        //T_U32 last_count;
        M_MotorPrint("motor stopping\n");
        SendSpiCommandToSlave(HEAD_SC,COMM_MS,0);
        g_motor_last_encoder = g_encoder_counter;
        g_motor_first_stop_encoder = g_encoder_counter;
        g_motor_need_to_state = state;
        g_motor_stoping_time = CtlTimeSincePowerOn();
        //g_motor_first_stop_encoder = g_encoder_counter;
        //g_motor_is_encoder_stoping = E_TRUE;
        CtlTimerSetRepeat(E_TIMER_CHECK_ENCODER_STOP, CHECK_ENCODER_STOP_MS);
        //last_count = g_encoder_counter;
        //DelayMs(100);
        //M_MotorPrint("continue run: %ld\n",g_encoder_counter-last_count);
        CtlEventPost(E_EVENT_MOTOR_STOPPING, 0);
    }
    else
    {
        DrvEncoderDisable();
	    MOTOR_POWER_OFF;
        if(state != g_motor_state)
        {
            if(state == E_MOTOR_PAUSE)
            {
                CtlEventPost(E_EVENT_ENTER_PAUSE_STATE, 0);
            }
            else
            {
                CtlEventPost(E_EVENT_ENTER_STOP_STATE, 0);
            }
            g_motor_state = state;
        }
    }
}


//stop motor
void CtlMotorStop()
{   
    CtlMotorStopInternal(E_MOTOR_STOP);
}

//pause motor
void CtlMotorPause()
{
    CtlMotorStopInternal(E_MOTOR_PAUSE);
}

//stop/pause by the cause
void CtlMotorStopEx(TE_MOTOR_STATE state, TE_MOTOR_STOP_CAUSE cause, T_SINT detail_alarm)
{
    g_motor_stop_cause = cause;
    g_motor_stop_cause_of_alarm = detail_alarm;
    CtlMotorStopInternal(state);    
}

//motor direction
TE_DIRECTION CtlMotorDirection()
{
	return g_motor_running_dir;
}

//is motor runing
TE_BOOLEAN CtlMotorIsRunning()
{
    if(g_motor_state == E_MOTOR_RUN)
    {
        return E_TRUE;
    }
    else
    {
        return E_FALSE;
    }
}

//is motor stoping
TE_BOOLEAN CtlMotorIsStoping()
{
    if(CtlTimerIsExist(E_TIMER_CHECK_ENCODER_STOP))
    {
        return E_TRUE;
    }
    else
    {
        return E_FALSE;
    }
}


void CtlMotorInit()
{
    CtlTimerBindHandle(E_TIMER_CHECK_ENCODER_STOP, CtlMotorCheckEncoderStop);
}
