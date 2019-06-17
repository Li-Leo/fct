
#ifndef DRV_ADC_
#define DRV_ADC_

#include<msp430fr5989.h>
#include"TypeDefine.h"
#include "CtlAdc.h"

extern T_U8 volatile g_adc_completed_flag;

void AdcInit(void);
void DrvAdcEnable(void);
void DrvAdcDisable(void);
void DrvAdcDeinit(void);
void DrvBattVoltChkEn(void);
void DrvBattPerfChkEn(void);

#endif
