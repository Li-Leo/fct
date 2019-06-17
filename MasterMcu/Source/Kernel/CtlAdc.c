/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlAdc.c
*
* DESCRIPTION:
*   kernel layer, adc module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#include <msp430fr5989.h>
#include "CtlAdc.h"

#if 0

//************global variables******************
TS_ADC_INFO g_adc_value = {0,0,0,0,0};
static T_U32 g_motor_vol[ADC_GET_AVERAGE_NUMBER] = {0};
static T_U32 g_motor_cur[ADC_GET_AVERAGE_NUMBER] = {0};
static T_U32 g_batt_vol[ADC_GET_AVERAGE_NUMBER] = {0};
static T_U32 g_batt_perf[ADC_GET_AVERAGE_NUMBER] = {0};

/***********************************************
*   description:
*       get all 5 channels adc value
*   parameters:
*       N/A
*   return:       
*       the pointed channel's adc value
***********************************************/
T_U16 AdcGetValue(TE_ADC_CHANNEL cha)
{
	T_U16 ret_value = 0;
	//g_adc_value = DrvAdcDetect();
	switch(cha)
	{
		case E_ADC_CHANNEL_MOTOR_VOL:
			ret_value = g_adc_value.motor_vol_channal_a2;		
			break;
		case E_ADC_CHANNEL_MOTOR_CUR:			
			ret_value = g_adc_value.motor_cur_vol_channal_a12;
			break;
		case E_ADC_CHANNEL_BATT_VOL:			
			ret_value = g_adc_value.batt_vol_channal_a13;
			break;
		case E_ADC_CHANNEL_BATT_PERF_VOL:			
			ret_value = g_adc_value.batt_perf_vol_channal_a14;
			break;
		case E_ADC_CHANNEL_RESERVED:
		default:
			break;
	}
	return ret_value;
}

/***********************************************
* Description:
*	a local function to calculate the average value of 20 number.
* Argument:
*   N/A
* Return:
*	the average value
************************************************/
static TS_ADC_INFO CalcAverageValue()
{
	T_U16 index;
	T_U32 motor_vol_avg, motor_cur_avg, batt_vol_avg, batt_perf_avg;
	T_U32 motor_vol_min = g_motor_vol[0];
	T_U32 motor_vol_max = g_motor_vol[0];
	T_U32 motor_vol_sum = g_motor_vol[0];
	T_U32 motor_cur_min = g_motor_cur[0];
	T_U32 motor_cur_max = g_motor_cur[0];
	T_U32 motor_cur_sum = g_motor_cur[0];
	T_U32 batt_vol_min = g_batt_vol[0];
	T_U32 batt_vol_max = g_batt_vol[0];
	T_U32 batt_vol_sum = g_batt_vol[0];
	T_U32 batt_perf_min = g_batt_perf[0];
	T_U32 batt_perf_max = g_batt_perf[0];
	T_U32 batt_perf_sum = g_batt_perf[0];
	TS_ADC_INFO adc_value_avg;

	for(index = 1; index < ADC_GET_AVERAGE_NUMBER; index++)
	{
	/* Pick up the max and min value from 20 numbers */
		if(g_motor_vol[index] < motor_vol_min)
			motor_vol_min = g_motor_vol[index];
		if(g_motor_vol[index] > motor_vol_max)
			motor_vol_max = g_motor_vol[index];
		if(g_motor_cur[index] < motor_cur_min)
			motor_cur_min = g_motor_cur[index];
		if(g_motor_cur[index] > motor_cur_max)
			motor_cur_max = g_motor_cur[index];
		if(g_batt_vol[index] < batt_vol_min)
			batt_vol_min = g_batt_vol[index];
		if(g_batt_vol[index] > batt_vol_max)
			batt_vol_max = g_batt_vol[index];
		if(g_batt_perf[index] < batt_perf_min)
			batt_perf_min = g_batt_perf[index];
		if(g_batt_perf[index] > batt_perf_max)
			batt_perf_max = g_batt_perf[index];
		
	/* Calculate the sum of 20 values */
		motor_vol_sum += g_motor_vol[index];
		motor_cur_sum += g_motor_cur[index];
		batt_vol_sum += g_batt_vol[index];
		batt_perf_sum += g_batt_perf[index];
	}

	/* Get rid of the min and max value, and then calculate the avg */
	motor_vol_sum -= (motor_vol_min + motor_vol_max);
	motor_cur_sum -= (motor_cur_min + motor_cur_max);
	batt_vol_sum -= (batt_vol_min + batt_vol_max);
	batt_perf_sum -= (batt_perf_min + batt_perf_max);

	motor_vol_avg = motor_vol_sum / (ADC_GET_AVERAGE_NUMBER - 2);
	motor_cur_avg = motor_cur_sum / (ADC_GET_AVERAGE_NUMBER - 2);
	batt_vol_avg = batt_vol_sum / (ADC_GET_AVERAGE_NUMBER - 2);
	batt_perf_avg = batt_perf_sum / (ADC_GET_AVERAGE_NUMBER - 2);

	adc_value_avg.motor_vol_channal_a2 = motor_vol_avg;
	adc_value_avg.motor_cur_vol_channal_a12 = motor_cur_avg;
	adc_value_avg.batt_vol_channal_a13 = batt_vol_avg;
	adc_value_avg.batt_perf_vol_channal_a14 = batt_perf_avg;

	return adc_value_avg;
}

/***********************************************
*   description:
*       get all 5 channels adc average value
*   parameters:
*       N/A
*   return:       
*       the structure of adc value
***********************************************/
TS_ADC_INFO AdcGetAverageValue()
{
	TS_ADC_INFO adc_value[ADC_GET_AVERAGE_NUMBER];
	TS_ADC_INFO adc_value_avg;
	T_U16 index;
	for(index = 0; index < ADC_GET_AVERAGE_NUMBER; index++)
	{
		//adc_value[index] = DrvAdcDetect();
		g_motor_vol[index] = adc_value[index].motor_vol_channal_a2;
		g_motor_cur[index] = adc_value[index].motor_cur_vol_channal_a12;
		g_batt_vol[index] = adc_value[index].batt_vol_channal_a13;
		g_batt_perf[index] = adc_value[index].batt_perf_vol_channal_a14;
	}
	adc_value_avg = CalcAverageValue();

	return adc_value_avg;
}

#endif

