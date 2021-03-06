/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlDelivery.c
*
* DESCRIPTION:
*   delivery module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlDelivery.h"
#include "CtlAdc.h"
#include "CtlData.h"
#include "DrvEncoder.h"
#include "DrvMotor.h"
#include "CtlMotor.h"

//************global variables******************
extern TE_DELIVERY_STATE g_delivery_state;
extern TE_BOOLEAN g_occlusion_state;
extern TS_DELI_PARA g_sys_para_infu_info[TOTAL_DELIVERY_TIME_POINT_NUMBER];
extern T_U16 g_sys_para_max_inf_dose;
extern TE_DOSE_THICKNESS g_sys_para_thickness;
extern T_U32 g_encoder_expect_counter;
extern T_U32 g_encoder_counter;
/***********************************************
*   description:
*       delivery initialization
*
*   parameters:
*       N/A
*   return:       
*       E_TURE/E_FALSE
***********************************************/
TE_BOOLEAN CtlDeliveryInit(void)
{
	//T_U16 index;

	DrvMotorInitial();
	g_delivery_state = E_DELI_STATE_STOP;
	
	/*if(!LoadSysPara(E_SYSTEM_PARA_THICKNESS, 0))
		return E_FALSE;	
	
	if(!LoadSysPara(E_SYSTEM_PARA_MAX_INFU_DOSE, 0))
		return E_FALSE;
	
	for(index = 0; index < TOTAL_DELIVERY_TIME_POINT_NUMBER; index++)
		if(!LoadSysPara(E_SYSTEM_PARA_INFU_INFO, (index+1)))
			return E_FALSE;*/
		
	return E_TRUE;
}

/***********************************************
*   description:
*       control delivery start
*
*   parameters:
*       deli_dose: delivery dose, uint:ug
*       thickness: thickness of the dose, uint:ug/ml
*   return:       
*       motor's delivery state     
***********************************************/
TE_DELIVERY_STATE CtlDeliveryStart(T_U16 deli_dose, TE_DOSE_THICKNESS thickness)
{
	T_U32 circle;					//编码器需转圈数
	switch(thickness)
	{
		case E_DOSE_THICKNESS_100:
			circle = DOSE_TO_CIRCLE_THICKNESS_100;
			break;
		case E_DOSE_THICKNESS_200:
			circle = DOSE_TO_CIRCLE_THICKNESS_200;
			break;
		case E_DOSE_THICKNESS_500:
			circle = DOSE_TO_CIRCLE_THICKNESS_500;
			break;
		default:
			break;
	}
	circle = circle * (deli_dose/10) / 1000;		//该数据为真实圈数的10倍
	if (circle % 10 < 5)
		circle = circle / 10;
	else
		circle = circle / 10 + 1;
	g_encoder_expect_counter = circle;
	CtlMotorStart(E_MOVE_FORWARD);
	return E_DELI_STATE_RUNNING;
}

/***********************************************
*   description:
*       control delivery stop
*
*   parameters:
*       
*   return:       
*       motor's delivery state 
***********************************************/
TE_DELIVERY_STATE CtlDeliveryStop(void)
{
	//DrvMotorSetStopPower();
	CtlMotorStop();
	    
    return E_DELI_STATE_STOP;
}

/***********************************************
* Description:
*	Judge whether the motor is occlusion.
* Argument:
*   N/A
* Return:
*   E_TURE/E_FALSE
************************************************/
TE_BOOLEAN IsOcclusion(void)
{
	return g_occlusion_state;
}

/***********************************************
* Description:
*	Get the delivery state of motor.
* Argument:
*	N/A
* Return:
*	delivery state 
************************************************/
TE_DELIVERY_STATE CtlGetDeliveryState(void)
{
	return g_delivery_state;
}

/***********************************************
* Description:
*	Set the delivery state of motor.
* Argument:
*	deli_state:
* Return:
*	N/A
************************************************/
void CtlSetDeliveryState (TE_DELIVERY_STATE deli_state)
{    
	g_delivery_state = deli_state;
}

/***********************************************
*   description:
*		Get the delivery parameter.
*   parameters:
*       index: a certain delivery time point
*   return:       
*       delivery parameter
***********************************************/
TS_DELI_PARA GetDeliveryPara(T_U8 index)
{
	TS_DELI_PARA deli_para;
	deli_para.index = index;
	if(LoadSysPara(E_SYSTEM_PARA_INFU_INFO, (index+1)))
		deli_para.dose = g_sys_para_infu_info[index].dose;
	return deli_para;
}

/***********************************************
*   description:
*       Check the validity of the delivery parameter
*   parameters:
*       deli_para: delivery parameter
*   return:       
*       E_TURE/E_FALSE
***********************************************/
TE_BOOLEAN CheckDeliveryPara(TS_DELI_PARA deli_para)
{
	if (deli_para.index > (TOTAL_DELIVERY_TIME_POINT_NUMBER -1))
		return E_FALSE;
	if (deli_para.dose > g_sys_para_max_inf_dose)
		return E_FALSE;
	return E_TRUE;
}

/***********************************************
*   description:
*		Set the delivery parameter.
*   parameters:
*       deli_para: delivery parameter
*   return:       
*       E_TURE/E_FALSE
***********************************************/
TE_BOOLEAN SetDeliveryPara(TS_DELI_PARA deli_para)
{
	g_sys_para_infu_info[deli_para.index].dose = deli_para.dose;
	if(SaveSysPara(E_SYSTEM_PARA_INFU_INFO, (deli_para.index + 1)))
		return E_TRUE;
	else
		return E_FALSE;
}

/***********************************************
*   description:
*       Check the validity of the thickness parameter
*   parameters:
*       thickness: thickness parameter
*   return:       
*       E_TURE/E_FALSE
***********************************************/
TE_BOOLEAN CheckThicknessPara(T_U16 thickness)
{
	if ((thickness != DOSE_THICKNESS_100) &&
		(thickness != DOSE_THICKNESS_200) &&
		(thickness != DOSE_THICKNESS_500))
		return E_FALSE;
	else
		return E_TRUE;
}

/***********************************************
*   description:
*       get the rotation number which encoder counted
*   parameters:
*       N/A
*   return:       
*       the value of counter
***********************************************/
T_U32 GetRotationNumber()
{
    return g_encoder_counter * RATE_OF_FLOAT_TO_INT / 2;
}

/***********************************************
*   description:
*       get the actual delivery dose
*   parameters:
*       N/A
*   return:       
*       actual delivery dose
***********************************************/
T_U16 GetActualDeliveryInfusionVolume()
{
	T_U16 actual_dose;
	T_U32 rotation_num = GetRotationNumber();
	TE_DOSE_THICKNESS thickness = g_sys_para_thickness;
	if (thickness == E_DOSE_THICKNESS_100)
		actual_dose = (T_U16)(rotation_num / DOSE_TO_CIRCLE_THICKNESS_100);
	else if (thickness == E_DOSE_THICKNESS_200)
		actual_dose = (T_U16)(rotation_num / DOSE_TO_CIRCLE_THICKNESS_200);
	else if (thickness == E_DOSE_THICKNESS_500)
		actual_dose = (T_U16)(rotation_num / DOSE_TO_CIRCLE_THICKNESS_500);
	return actual_dose;
}


/***********************************************
*   description:
*       get motor current
*   parameters:
*       N/A
*   return:       
*       the value of motor current
***********************************************/
//	T_U16 GetMotorCurrent()
//	{
//	    //TS_ADC_INFO adc_value = AdcGetAverageValue();
//	    //return adc_value.motor_cur_vol_channal_a12;
//	  
//	}


