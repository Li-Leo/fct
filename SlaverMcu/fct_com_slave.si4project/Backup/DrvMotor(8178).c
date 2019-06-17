/************************************************
* COPYRIGHT 2016 
*
* NAME: DrvMotor.c
*
* REVISION HISTORY:
* 	Created by Kevin Shi at 04/15/2016
* 	Modified by Kevin Shi at 04/21/2016
*
* END OF FILE HEADER
************************************************/

#include "DrvMotor.h"
#include "DrvPower.h"
#include "DrvAdc.h"
#include "Common.h"

extern TS_ADC_INFO g_adc_value;
T_U8 g_motor_dir;

void DrvMotorInitial(void)
{
	//MOTOR_POWER_OFF;
	MOTOR_SLEEP;
}

void DrvMotorSetRun(TE_DIRECTION dir)
{
  if (dir == E_MOVE_BACKWARD){
    MOTOR_RUN_BACKWORD;
    //MOTOR_RUN_FORWORD_OFF;
  }
  else if (dir == E_MOVE_FORWARD){
    MOTOR_RUN_FORWORD;
    //MOTOR_RUN_BACKWORD_OFF;
  }
}

void DrvMotorSetStopDriver(void)
{
    MOTOR_BREAK;
}

void DrvMotorSetSleep(void)
{
    //MOTOR_POWER_OFF;
	MOTOR_SLEEP;
}

T_U16 pulse_counter=0;                         // counter
T_U16 begin, end;
T_U32 time;
T_U8  flag_Hz_done=0;
float hz=0;
T_U16 g_hz_value;                              // hz result of motor
T_U16 g_carry_cnter=0;
T_U8  g_motor_current_done=0;

void DrvMotorCurrentInit(void)
{
  P5SEL1 &= ~BIT0;
  P5SEL0 |= BIT0 ;                             // P5.0 TA1.CCI1A
  P5DIR  &= ~BIT0;                             // P5.0 input
  
  TA1CCTL1 |= CM_1 | CCIS_0 | SCS | CAP | CCIE;// Capture rising edge,
                                               // Use CCI1A
                                               // Synchronous capture,
                                               // Enable capture mode,
                                               // Enable capture interrupt

  TA1CTL |= TASSEL__SMCLK|ID__2;               // 1/2
  TA1EX0 |= TAIDEX_4;                          // 1/5  500k/5=100kHz
}

//enable motor currnet detection
void DrvMotorCurrentEnable(void)
{
    //clear the counter
    TA1R = 0;
    // Enable capture mode,
    // Enable capture interrupt
    TA1CCTL1 |= CAP | CCIE;
    TA1CTL |= MC__CONTINUOUS|TAIE;                  // Start timer in continuous mode    

    pulse_counter = 0;
    g_hz_value = 0;
    flag_Hz_done = 0;
    g_motor_current_done = 0;
}
//disable motor currnet detection
void DrvMotorCurrentDisable(void)
{
    // Disable capture mode,
    // Disable capture interrupt
    TA1CCTL1 &= ~(CAP | CCIE);
    TA1CTL &= ~(MC__CONTINUOUS|TAIE);               // stop timer
}
    
// Timer0_A3 CC1-2, TA Interrupt Handler
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
{
  switch (__even_in_range(TA1IV, TA1IV_TAIFG)){
    case TA1IV_TA1CCR1:       

      if(pulse_counter < 1){
        begin=TA1CCR1;
        flag_Hz_done = 0;
        
      }
      
      if(pulse_counter >= MOTOR_PULSE_GET){ 
        end=TA1CCR1;
        flag_Hz_done=1;
      }
      pulse_counter++;
      break;
    case TA1IV_TA1CCR2:
      break;
    case TA1IV_TA1IFG:
      if(pulse_counter < MOTOR_PULSE_GET && pulse_counter >= 1)
      { 
        g_carry_cnter ++;
      }
      break;
    default:
      break;
  }
  
  if(flag_Hz_done){
    g_motor_current_done=1;
    flag_Hz_done=0;        
    pulse_counter=0;
        
    if(g_carry_cnter<1)
      time=end-begin;
    else
    {
      if(end >= begin)
      {
        time=MAX_T_U16*g_carry_cnter+end-begin;
      } 
      else
      {
        time=MAX_T_U16*(g_carry_cnter-1)+MAX_T_U16-begin+end;
      }
    }
    g_carry_cnter=0;
    
    hz=(float)MOTOR_PULSE_GET/((float)time/100000);
    g_hz_value=(T_U16)(hz*100);
    LPM1_EXIT;
  }
}

/***************************   END OF FILE   **********************************/
