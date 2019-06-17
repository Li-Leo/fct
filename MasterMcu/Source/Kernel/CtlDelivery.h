/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlDelivery.h
*
* DESCRIPTION:
*   delivery module head file
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#ifndef CTLDELIVERY_H
#define CTLDELIVERY_H

#include "..\TypeDefine.h"
#include "..\Driver\DrvEncoder.h"
#include "CtlKey.h"

extern T_U32 g_delivery_dest_encoder;
extern T_U32 g_delivery_deviation_encoder;
extern T_U32 g_delivery_actural_encoder;
extern T_U32 g_delivery_expect_encoder;

/* ------------------ definitions ------------------ */
/* Define the state of motor */
typedef enum
{
	/* ----- stop ----- */
	E_DELI_STATE_STOP,
	/* ----- normal infusion ----- */
	//只有正常输注时的才是这个状态, 校准或自动定位时不会设置这个输注状态
	E_DELI_STATE_RUNNING,  
	/* ----- occlusion ----- */
	E_DELI_STATE_PAUSE,
	/* ----- sleep ----- */
	E_DELI_STATE_POWEROFF,
	E_DELI_STATE_MAX,
} TE_DELIVERY_STATE;

typedef enum
{
	E_DOSE_THICKNESS_100,
	E_DOSE_THICKNESS_200,
	E_DOSE_THICKNESS_500,
	E_DOSE_THICKNESS_MAX,
} TE_DOSE_THICKNESS;

#define RATE_OF_FLOAT_TO_INT	1000

#define DOSE_THICKNESS_100	100 //unit: ug/ml
#define DOSE_THICKNESS_200	200
#define DOSE_THICKNESS_500	500

#define REDUCTION_RATIO_MOTOR	256				//电机减速比
#define REDUCTION_RATIO_REDUCER_CASING	3		//减速箱减速比
#define TOTAL_REDUCTION_RADIO (REDUCTION_RATIO_MOTOR * REDUCTION_RATIO_REDUCER_CASING)
#define MOTOR_SHIFTING_TO_CIRCLE	(TOTAL_REDUCTION_RADIO * 4)	//电机每推动1毫米对应的编码器转动圈数(丝杆转2圈)

#define CONSTANT_PI		(T_U32)31416
#define CONSTANT_RADIUS_OF_RESERVOIR	6  //unit:6mm
#define SECTIONAL_AREA (CONSTANT_PI*CONSTANT_RADIUS_OF_RESERVOIR*CONSTANT_RADIUS_OF_RESERVOIR/10000.0) //mm^2
//correct factor, if the precision is 2%, S*S_factor*L=V*(1+2%), S_factor=(1+2%)*S_pre_factor
#define SECTIONAL_AREA_FACTOR (0.993)
//with correct factor
#define FIXED_SECTIONAL_AREA (SECTIONAL_AREA*SECTIONAL_AREA_FACTOR) //mm^2

#define CONSTANT_SQUARE	(T_U32)(CONSTANT_PI * CONSTANT_RADIUS_OF_RESERVOIR * CONSTANT_RADIUS_OF_RESERVOIR)
#define CONSTANT_RATE_1	(T_U32)(MOTOR_SHIFTING_TO_CIRCLE * 100000 / CONSTANT_SQUARE)
#define CONSTANT_REMAINDER	(T_U32)(MOTOR_SHIFTING_TO_CIRCLE * 100000 % CONSTANT_SQUARE)
#define CONSTANT_RATE_2	(T_U32)(CONSTANT_REMAINDER * 1000 / CONSTANT_SQUARE)
#define CONSTANT_RATE	(T_U32)(CONSTANT_RATE_1 * 1000 + CONSTANT_RATE_2)	//271623,已放大1000倍并取余
                                     //encoder interrupts per plate move 1mm
#define DOSE_TO_CIRCLE_THICKNESS_100	(T_U32)(CONSTANT_RATE * 10)	//每输注1单位剂量编码器转动圈数，已放大10000倍
#define DOSE_TO_CIRCLE_THICKNESS_200	(T_U32)(DOSE_TO_CIRCLE_THICKNESS_100 / 2)
#define DOSE_TO_CIRCLE_THICKNESS_500	(T_U32)(DOSE_TO_CIRCLE_THICKNESS_100 / 5)

#define SINGLE_DELIVERY_DOSE_MAX	50
#define DAY_TOTAL_DELI_DOSE_REMIND	160
#define DAY_TOTAL_DELI_DOSE_RANGE	1000

#define CIRCLES_PER_MINUTE_MOTOR	2010			//标称带负载时每分钟电机转动圈数,注意: 只有这里是电机圈数,其他地方都是encoder数
#define CIRCLES_PER_MINUTE_MOTOR_NO_LOAD	10400			//标称不带负载时每分钟电机转动圈数,注意: 只有这里是电机圈数,其他地方都是encoder数
#define CIRCLES_PER_MINUTE_ENCODER	(CIRCLES_PER_MINUTE_MOTOR * 2)
#define CIRCLES_PER_SECOND_ENCODER	(CIRCLES_PER_MINUTE_ENCODER / 60)

#define M_VolumeToEncoder(volume_ml)((volume_ml)*1000.0* MOTOR_SHIFTING_TO_CIRCLE       \
        /FIXED_SECTIONAL_AREA)   //unit: ml
#define M_EncoderToVolume(encoder) (FIXED_SECTIONAL_AREA        \
        *(encoder)/MOTOR_SHIFTING_TO_CIRCLE/1000)   //unit: ml


#define ONE_DOSE_FIXED_FACTOR (1.021) 

/* --------------- external functions --------------- */

/* init */
TE_BOOLEAN CtlDeliveryInit(void);

TE_DELIVERY_STATE CtlDeliveryStart(T_S32 expect_encoder, T_U16 deli_dose, TE_DOSE_THICKNESS thickness);
TE_DELIVERY_STATE CtlDeliveryStop(void);
TE_BOOLEAN IsOcclusion(void);
//TE_BOOLEAN CtlAutoLocation(void);
//T_U8 CtlManualLocation(TE_DIRECTION dir);
TE_DELIVERY_STATE CtlGetDeliveryState(void);
void CtlSetDeliveryState (TE_DELIVERY_STATE deli_state);
TS_DELI_PARA GetDeliveryPara(T_U8 index);
TE_BOOLEAN CheckDeliveryPara(TS_DELI_PARA deli_para);
TE_BOOLEAN SetDeliveryPara(TS_DELI_PARA deli_para);
TE_BOOLEAN CheckThicknessPara(T_U16 thickness);
TE_BOOLEAN CheckMaxSingleDeliveryDosePara(T_U16 dose);
TE_BOOLEAN CheckDayTotalDeliveryDoseOverrun(T_U16 dose);
TE_BOOLEAN CheckDayTotalDeliveryDoseOutRange(T_U16 dose);
T_U32 GetRotationNumber();
T_U16 GetActualDeliveryInfusionVolume();
T_U16 GetMotorCurrent();

#endif /* CTLDELIVERY_H */

