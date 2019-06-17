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
#include "DrvMotor.h"

#define M_MotorPrint if(IsEnableOutput(E_MODULE_MOTOR))printf



typedef enum
{
    E_MOTOR_STOP_BY_USER,
    E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET,
    E_MOTOR_STOP_BY_MEET_HALL,
}TE_MOTOR_STOP_CAUSE;

extern TE_DIRECTION g_motor_running_dir;
extern TE_BOOLEAN g_motor_is_running;
extern TE_BOOLEAN g_motor_is_meet_hall;
extern TE_BOOLEAN g_motor_is_reach_expect_encoder;
extern TE_MOTOR_STOP_CAUSE g_motor_stop_cause;
extern TE_BOOLEAN g_motor_is_need_check_hall;


//start motor, default it will enable encode and motorcurrent
void CtlMotorStart(TE_DIRECTION direction);

void CtlMotorStartEx(TE_DIRECTION direction, 
    TE_BOOLEAN is_enable_encode, TE_BOOLEAN is_enable_motorcurrent,
    TE_BOOLEAN is_enable_hall);

//stop motor
void CtlMotorStop();

//motor direction
TE_DIRECTION CtlMotorDirection();
#endif /* CTLMOTOR_H */

