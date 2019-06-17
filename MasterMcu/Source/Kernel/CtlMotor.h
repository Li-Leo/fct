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
#ifndef CTLMOTOR_H
#define CTLMOTOR_H

#include "TypeDefine.h"
#include "DrvEncoder.h"

#define M_MotorPrint if(IsEnableOutput(E_MODULE_MOTOR))printf


typedef enum
{
    E_MOTOR_STOP_BY_USER,
    E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET,
    E_MOTOR_STOP_BY_POS_ZERO,
    E_MOTOR_STOP_BY_ALARM,
    E_MOTOR_STOP_BY_MEET_HALL,
    E_MOTOR_STOP_BY_AMCU_NOT_ACTIVE,
    E_MOTOR_STOP_BY_OVERRUN,
    E_MOTOR_STOP_BY_POS_MAX,
}TE_MOTOR_STOP_CAUSE;

typedef enum
{
    E_MOTOR_STOP,
    E_MOTOR_PAUSE,
    E_MOTOR_RUN,
    E_MOTOR_STATE_SMAX,
}TE_MOTOR_STATE;

extern TE_DIRECTION g_motor_running_dir;
extern TE_BOOLEAN g_motor_is_allow_stop_by_other_cpu;
extern TE_MOTOR_STOP_CAUSE g_motor_stop_cause;
extern T_S16 g_motor_stop_cause_of_alarm;


//start motor
void CtlMotorStart(TE_DIRECTION direction, T_S32 expect_encoder);

//stop motor
void CtlMotorStop();

//pause motor
void CtlMotorPause();

//stop/pause by the cause
void CtlMotorStopEx(TE_MOTOR_STATE state, TE_MOTOR_STOP_CAUSE cause, T_SINT detail_alarm);


//motor direction
TE_DIRECTION CtlMotorDirection();

//is motor runing
TE_BOOLEAN CtlMotorIsRunning();


//is motor stoping
TE_BOOLEAN CtlMotorIsStoping();


void CtlMotorInit();

#endif /* CTLMOTOR_H */

