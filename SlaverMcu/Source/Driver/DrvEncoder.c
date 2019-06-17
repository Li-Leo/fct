
#include "DrvEncoder.h"
#include "DrvMotor.h"
#include "CtlMotor.h"

T_U32 g_encoder_counter = 0;           // counter
T_U32 g_encoder_expect_counter = 0;    // store the finished counter value
//T_U8  g_motor_finished_flag=0;

void DrvEncoderInitial(void)
{
  P1SEL1 &= ~BIT0;
  P1SEL0 |=  BIT0;    // P1.0 TA0.CCI1A
  P1DIR  &= ~BIT0;    // P1.0 input
  //ENCODER_ENABLE;     // power on encoder
  
  TA0CCTL1 |= CM_1 | CCIS_0 | SCS | CAP | CCIE;// Capture rising edge,
                                               // Use CCI2A
                                               // Synchronous capture,
                                               // Enable capture mode,
                                               // Enable capture interrupt

  TA0CTL = TASSEL__SMCLK ;                     // Use SMCLK as clock source,
                                               // Start timer in continuous mode 
}

//enable encoder
void DrvEncoderEnable(void)
{
    // Enable capture mode,
    // Enable capture interrupt
    TA0CCTL1 |= CAP | CCIE;
    TA0CTL |= MC__CONTINUOUS;              // start timer, continuous
}
//disable encoder
void DrvEncoderDisable(void)
{

    // Disable capture mode,
    // Disable capture interrupt
    TA0CCTL1 &= ~(CAP | CCIE);
    TA0CTL &= ~MC__CONTINUOUS;             // stop timer
}


// Timer0_A3 CC1-2, TA Interrupt Handler
#pragma vector = TIMER0_A1_VECTOR
  __interrupt void Timer0_A1_ISR(void)
{
  switch (__even_in_range(TA0IV, TA0IV_TAIFG)) {
    case TA0IV_TA0CCR1:
      
      g_encoder_counter++;
      if(g_encoder_counter >= g_encoder_expect_counter)
      {
        g_motor_is_reach_expect_encoder = E_TRUE;
        DrvMotorSetStopDriver();
        LPM1_EXIT;
      }
    
      break;
    case TA0IV_TA0CCR2:
      break;
    case TA0IV_TA0IFG:
      break;
    default:
      break;
  }
}

/***************************   END OF FILE   **********************************/
