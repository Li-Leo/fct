
#include "DrvAdc.h"
#include "CtlAdc.h"
TS_ADC_INFO g_adc_data[ADC_GET_AVERAGE_NUMBER];
T_U8 volatile g_adc_completed_flag=0;
static T_U8 g_adc_sample_count =0;


void AdcInit(void)
{
  P9SEL1 |= BIT4|BIT5|BIT6|BIT7;  //P9.4-P9.7 AD input
  P9SEL0 &= ~(BIT4|BIT5|BIT6|BIT7);
  P9DIR  &= ~(BIT4|BIT5|BIT6|BIT7);


  while(REFCTL0 & REFGENBUSY);   // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_3 + REFON;  //REFCTL0 |= REFVSEL_0 + REFON;
                                 // Enable internal 2.5 V reference

  ADC12CTL0 &= ~ADC12ENC;        //disable AD conversion
  ADC12CTL0 |= ADC12SHT1_3 + ADC12SHT0_3 + ADC12ON + ADC12MSC;//每个采样通道的采样保持时间为16个AD时
  //钟周期

  ADC12CTL1 |= ADC12SHP + ADC12SSEL_3 + ADC12CONSEQ_3;//采样和保持脉冲来自采样定
  //时器，smclk
  ADC12CTL2 |= ADC12RES_2;      //12bit AD
  ADC12CTL3 |= ADC12CSTARTADD_0;//start from CH 0

  ADC12MCTL0 |= ADC12INCH_12 + ADC12VRSEL_1;  //channel A12  battery voltage
  ADC12MCTL1 |= ADC12INCH_13 + ADC12VRSEL_1;  //channel A13  battery performance
  ADC12MCTL2 |= ADC12INCH_14 + ADC12VRSEL_1;  //channel A14  S_VOL_CAL
  ADC12MCTL3 |= ADC12INCH_15 + ADC12VRSEL_1+ ADC12EOS;  //channel A15  motor voltage

  while(!(REFCTL0 & REFGENRDY));

  //ADC12CTL0 |= ADC12ENC;
  ADC12IER0 |= ADC12IE3;
}

void DrvAdcEnable(void)
{
  //while(REFCTL0 & REFGENBUSY);       // If ref generator busy, WAIT
  //REFCTL0 |= REFON;
  //while(!(REFCTL0 & REFGENRDY));

  //ADC12CTL0 |= ADC12ON;
  g_adc_sample_count = 0;
  g_adc_completed_flag = 0;
  ADC12CTL0 |= ADC12ENC | ADC12SC;
}

void DrvAdcDisable(void)
{
  P3OUT &= ~BIT2;
  P4OUT &= ~BIT3;

  ADC12CTL0 &= ~(ADC12ENC|ADC12SC);    //stop conversion
  //ADC12CTL0 &= ~ADC12ON;
}

void DrvAdcDeinit(void)
{
  DrvAdcDisable();

  while(REFCTL0 & REFGENBUSY);   // If ref generator busy, WAIT
  REFCTL0 &= ~REFON;
}


void DrvBattVoltChkEn(void)
{
  P3OUT |= BIT2;                 //batt_v_che_en
}
void DrvBattPerfChkEn(void)
{
  P4OUT |= BIT3;                 //batt_perf_che_en
}


#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  if(g_adc_sample_count<ADC_GET_AVERAGE_NUMBER){
    g_adc_data[g_adc_sample_count].batt_vol_channal_a12=ADC12MEM0;
    g_adc_data[g_adc_sample_count].batt_perf_vol_channal_a13=ADC12MEM1;
    g_adc_data[g_adc_sample_count].s_vol_cal_channal_a14=ADC12MEM2;
    g_adc_data[g_adc_sample_count].motor_vol_channal_a15=ADC12MEM3;
    g_adc_sample_count++;
  }
  if(g_adc_sample_count>=ADC_GET_AVERAGE_NUMBER){
    g_adc_completed_flag=1;
    DrvAdcDisable();
    //LPM1_EXIT;
  }
}

/***************************   END OF FILE   **********************************/
