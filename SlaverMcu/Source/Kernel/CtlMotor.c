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
#include "DrvMotor.h"
#include "DrvHall.h"
#include "DrvEncoder.h"
#include "common.h"
#include "CtlDelivery.h"
#include "CtlTimer.h"
#include "CtlSpiComm.h"

TE_DIRECTION g_motor_running_dir = E_MOVE_FORWARD;
TE_BOOLEAN g_motor_is_running = E_FALSE;
TE_BOOLEAN g_motor_is_meet_hall = E_FALSE;
TE_BOOLEAN g_motor_is_reach_expect_encoder = E_FALSE;
TE_MOTOR_STOP_CAUSE g_motor_stop_cause;
TE_BOOLEAN g_motor_is_need_check_hall = E_FALSE;


extern T_U32 g_encoder_counter;
extern T_U32 g_encoder_expect_counter;
extern T_U16 g_motor_position;

//	void CtlMotorCheckifReachExpectEncoder()
//	{
//	    if(g_motor_is_reach_expect_encoder)
//	    {
//	        DrvMotorSetStopDriver();
//	        M_CommonPrint("finish encoder, stop\n");
//	        SetSpiCommandToMaster(HEAD_FC,COMM_MS,1,0);
//	        CtlMotorStop();
//	    }
//	}

//start motor
void CtlMotorStart(TE_DIRECTION direction)
{
    CtlMotorStartEx(direction, E_TRUE, E_TRUE, E_FALSE);
}

void CtlMotorStartEx(TE_DIRECTION direction, 
    TE_BOOLEAN is_enable_encode, TE_BOOLEAN is_enable_motorcurrent,
    TE_BOOLEAN is_enable_hall)
{
    if(direction == E_MOVE_FORWARD)
    {
        M_MotorPrint("motor start forward, expect encoder[%ld], curr pos[%u*0.001mm]\n", 
            g_encoder_expect_counter, g_motor_position);
    }
    else
    {
        M_MotorPrint("motor start backword,expect encoder[%ld], curr pos[%u*0.001mm]\n", 
            g_encoder_expect_counter, g_motor_position);
    }
    //clear encoder count
    g_encoder_counter = 0;
    g_motor_stop_cause = E_MOTOR_STOP_BY_USER;
	g_motor_running_dir = direction;    
    g_motor_is_running = E_TRUE; 
    g_motor_is_meet_hall = E_FALSE;
    g_motor_is_need_check_hall = E_FALSE;
    g_motor_is_reach_expect_encoder = E_FALSE;
    //CtlTimerBindHandle(E_TIMER_CHECK_IS_REACH_EXPECT_ENCODER, CtlMotorCheckifReachExpectEncoder);
    //CtlTimerSetRepeat(E_TIMER_CHECK_IS_REACH_EXPECT_ENCODER, 5);
    
    if(is_enable_encode)
    {
	    DrvEncoderEnable();
    }
    else
    {
        DrvEncoderDisable();
    }

    //enable or disable the power
    if(is_enable_motorcurrent || is_enable_hall)
    {
        DrvPowerEnableHallAndMotorCurrent();
    }
    else
    {
        DrvPowerDisableHallAndMotorCurrent();
    }
    if(is_enable_motorcurrent)
    {
        DrvMotorCurrentEnable();
    }
    else
    {
        DrvMotorCurrentDisable();
    }
    if(is_enable_hall)
    {
        g_motor_is_need_check_hall = E_TRUE;
        //must delay to turn on hall, it need some time to be ready or it will enter interrupt at once
        DelayMs(1);
        DrvHallEnable();
    }
    else
    {
        DrvHallDisable();
    }

    
    DrvMotorSetRun(direction);
}
//stop motor
void CtlMotorStop()
{
    T_U32 curr = g_encoder_counter;
    
    DrvMotorSetStopDriver();
    if(g_motor_is_running)
    {
        DelayMs(50);
        M_MotorPrint("have run %lu encoder after break\n", g_encoder_counter-curr);
    }
    else
    {
        M_MotorPrint("already stopped\n");
    }

    
    if(g_motor_is_running)
    {
        M_MotorPrint("motor stop, curr pos[%u*0.001mm]\n", g_motor_position);
        M_MotorPrint("have run %lu encoder, expect encoder[%lu]\n", 
            g_encoder_counter, g_encoder_expect_counter);
        switch(g_motor_stop_cause)
        {
            case E_MOTOR_STOP_BY_USER:
                break;
            case E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET:
                M_MotorPrint("stop cause: finish target\n");
                break;
            case E_MOTOR_STOP_BY_MEET_HALL:
                M_MotorPrint("stop cause: meet hall\n");
                break;
            default:
                M_MotorPrint("stop cause: [%d]\n", g_motor_stop_cause); 
                break;
        }
    }    
    CtlTimerClear(E_TIMER_CHECK_IS_REACH_EXPECT_ENCODER);
	DrvEncoderDisable();
    
    DrvPowerDisableHallAndMotorCurrent();
	DrvMotorCurrentDisable();
    DrvHallDisable();
    DrvMotorSetSleep();
    g_motor_is_running = E_FALSE;
}

//motor direction
TE_DIRECTION CtlMotorDirection()
{
	return g_motor_running_dir;
}

