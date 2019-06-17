#include "AppCommon.h"
#include "CtlMotor.h"
#include "CtlSpiComm.h"
#include "CtlEvent.h"
#include "DrvEncoder.h"
#include "DrvRTC.h"
#include "CtlEvent.h"
#include "CtlTimer.h"
#include "DrvPower.h"


//extern TS_RECORD op_rec;
void AppStartRun(TE_DIRECTION direction, T_S32 expect_encoder)
{
    T_U16 data1,data2;
    
    data1 = expect_encoder>>16;
    data2 = (T_U16)expect_encoder;
    if(direction == E_MOVE_FORWARD)
    {
        data1 |= 0x8000; 
    }
    else
    {
        data1 &= 0x7FFF; 
    }
    CtlMotorStart(direction, expect_encoder);
    SendSpiCommandToSlaveEx(HEAD_SC,COMM_MR,data1, data2);
}
void AppStopRun()
{
	CtlMotorStopEx(E_MOTOR_STOP, E_MOTOR_STOP_BY_USER, 0);
}

void ExcuteSleep()
{
	//SaveSystemParameters();
	//P1IE |= BIT1;
	SendSpiCommandToSlave(HEAD_SC,COMM_SS,0);
    DelayMs(3); //let the cmd send finish

    M_CommonPrint("sleep\n");
    //enter low power mode
	DrvPowerEnterLowMode();
}

void ExcuteHallLocationCal()
{
	//g_is_hall_cal_finished = E_FALSE;    
    //g_encoder_expect_counter =400 * (T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
    //CtlMotorStart(E_MOVE_BACKWARD, DISTANCE_MAX_ENCODER);
    DrvEncoderDisableMotorPosMonitorOnce();
    g_motor_is_allow_stop_by_other_cpu = E_TRUE;
    SendSpiCommandToSlave(HEAD_SC,COMM_TH,E_MOVE_BACKWARD);
}

