/************************************************
* COPYRIGHT 2016 
*
* NAME: DrvMotor.h
*
* REVISION HISTORY:
* 	Created by Kevin Shi at 04/15/2016
* 	Modified by Kevin Shi at 04/21/2016
*
* END OF FILE HEADER
************************************************/

#ifndef	DRVMOTOR_H
#define	DRVMOTOR_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

/******************************************************************************/  
#define MOTOR_RUN_FORWORD	{PJOUT &= ~BIT7;PJOUT |= BIT6;}		 //Set the motor run FORWORD
//#define MOTOR_RUN_FORWORD_OFF	{PJOUT &= ~BIT7;PJOUT &= ~BIT6;  //Stop run FORWORD


#define MOTOR_RUN_BACKWORD	{PJOUT |= BIT7;PJOUT &= ~BIT6;}		 //Set the motor run BACKWORD
//#define MOTOR_RUN_BACKWORD_OFF {PJOUT &= ~BIT7;PJOUT &= ~BIT6; //Stop run BACKWORD

#define MOTOR_SLEEP {PJOUT &= ~(BIT7|BIT6);}	//sleep
#define MOTOR_BREAK {PJOUT |= BIT7|BIT6;}	    //break motor


#define REDUCTION_RATIO_MOTOR	256				//电机减速比
#define REDUCTION_RATIO_REDUCER_CASING	3		//减速箱减速比
#define REDUCTION_RATIO_TOTAL (REDUCTION_RATIO_MOTOR * REDUCTION_RATIO_REDUCER_CASING)
#define CIRCLES_PER_MINUTE		2010			//标称带负载时每分钟电机转动圈数

#define MOTOR_PULSE_GET	1                       // number of pulse get for frequency calculation
/******************************************************************************/
typedef enum
{
    E_MOVE_BACKWARD,
    E_MOVE_FORWARD,
    E_MOVE_MAX,
}TE_DIRECTION;

extern void DrvMotorInitial(void);
extern void DrvMotorSetRun(TE_DIRECTION dir);
//extern void DrvMotorSetStopPower(void);
extern void DrvMotorSetStopDriver(void);
extern void DrvMotorSetSleep(void);
extern T_U32 DrvMotorCurrentDetect(void);
extern TE_DIRECTION DrvMotorGetState(void);
void DrvMotorCurrentInit(void);
void DrvMotorCurrentEnable(void);
void DrvMotorCurrentDisable(void);

#endif  /* DRVMOTOR_H */
