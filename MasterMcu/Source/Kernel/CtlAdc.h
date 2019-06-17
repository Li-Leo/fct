/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlAdc.h
*
* DESCRIPTION:
*   kernel layer, ADC module head file
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/

#ifndef CTLADC_H
#define CTLADC_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

#define ADC_GET_AVERAGE_NUMBER	20
#define FULL_MEASURE_RANGE_VOLTAGE	25
#define MAX_ADC_GET_VALUE	4096
#define RATE_BETWEEN_ADC_GET_VALUE_AND_VOLTAGE	(MAX_ADC_GET_VALUE * 10 / FULL_MEASURE_RANGE_VOLTAGE)

#define BATTERY_FUNCTION_CHK	1000

//adc refer voltage(mV), temporary value
#define ADC_REF_VOLTAGE 3000 
//adc max digit code = 2^(adc's bits)
#define ADC_MAX_DIGIT_CODE (MAX_ADC_GET_VALUE -1)

//get voltage(mV) from ADC value, the voltage's unit is mV
#define M_GetVoltageFromAdc(adc_value) ((T_U32)(adc_value)*ADC_REF_VOLTAGE/ADC_MAX_DIGIT_CODE)

//get ADC value from voltage(mV)
#define M_GetAdcFromVoltage(voltage_value) ((T_U32)(voltage_value)*ADC_MAX_DIGIT_CODE/ADC_REF_VOLTAGE)

/* Define the channels of ADC here. */
typedef enum
{
    E_ADC_CHANNEL_BATT_VOL,
	E_ADC_CHANNEL_BATT_PERF_VOL,
    E_ADC_CHANNEL_S_VOL_CAL,
	E_ADC_CHANNEL_MOTOR_VOL,
	E_ADC_CHANNEL_MAX,
} TE_ADC_CHANNEL;

T_U16 AdcGetValue(TE_ADC_CHANNEL cha);
TS_ADC_INFO AdcGetAverageValue();

#endif	/* CTLADC_H */

