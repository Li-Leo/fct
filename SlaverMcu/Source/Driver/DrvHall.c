/************************************************
* COPYRIGHT 2016.
*
* NAME: DrvHall.c  (Slave MCU)
*
* DESCRIPTION:
*   Slave MCU hall module in the driver layer.
*
* REVISION HISTORY:
*
* END OF FILE HEADER
************************************************/
#include "DrvHall.h"
#include "DrvMotor.h"
#include "CtlDelivery.h"
#include "CtlMotor.h"

extern T_U32 g_encoder_counter ;  // counter (defined at DrvEncoder.c)
extern T_U32 g_encoder_expect_counter   ;  // store the finished counter value
extern T_U8 g_enable_key_plus_minus;
extern T_U32 cali_result;

/***********************************************
*   description:
*       Hall Power control
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvHallSetPower(T_U8 hall_state)
{
  if(hall_state == E_HALL_POWER_ON)
  {
    HALL_POWER_ON;
  }
  else
  {
    HALL_POWER_OFF;
  }
}
/***********************************************
*   description:
*      DrvHallGetState
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
TE_HALL_RETURN DrvHallGetState(void)
{
  if((P1IN & BIT2) == E_HALL_LOW)
  {
    return E_ARRIVAL_HALL_POSITION;
  }
  else
  {
    return E_NOT_ARRIVAL_HALL_POSITION;
  }
}
/***********************************************
*   description:
*       Hall initialization
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvHallInit(void)
{
  P1SEL1 &= ~BIT2;   
  P1SEL0 &= ~BIT2;  
  P1DIR  &= ~BIT2;     //P1.2  input
  
  //P1IES  &= ~BIT2;     //P1.2: 1 falling edge, 0 rising edge
  //P1IFG  =  0;         //clear interrupt  
  //P1IE   |= BIT2;      //P1.2£ºinterrupt enable
}
/***********************************************
*   description:
*       Hall initialization
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvHallEnable(void)
{  

  P1IFG =  0;        //clear interrupt  
  P1IE  |= BIT2;     //P1.2£ºenable interrupt
}

/***********************************************
*   description:
*       Hall initialization
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void DrvHallDisable(void)
{  
  
  P1IE  &= ~BIT2;     //P1.2£ºdisable interrupt
  P1IFG =  0;         //clear interrupt
}
/***********************************************
*   description:
*       Port 1 interrupt handler(Hall)
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
/* Port 1 interrupt service routine  */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  switch(__even_in_range(P1IV, P1IV_P1IFG7))
  {
    case P1IV_NONE: 
      break;
    case P1IV_P1IFG0:
      break;
    case P1IV_P1IFG1:
      break;
    case P1IV_P1IFG2:
      P1IFG &= ~BIT2;                  //Clear P1.2 IFG
      //g_motor_is_meet_hall = E_TRUE;
      //g_motor_stop_cause = E_MOTOR_STOP_BY_MEET_HALL;
      //CtlMotorStop();               
      //cali_result=g_encoder_counter;   //save calbration result      
      //g_encoder_expect_counter=MILIMETER*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
      //CtlMotorStart(E_MOVE_BACKWARD);  //plate move backward
      LPM1_EXIT;
      break;
    case P1IV_P1IFG3:
      break;
    case P1IV_P1IFG4:
      P1IFG  =  0;         //clear interrupt  
      P1IE   &= ~BIT4;     //P1.4£ºdisable interrupt 

      LPM3_EXIT;           // exit low power mode 3, wakeup MCU
      break;
    case P1IV_P1IFG5:
      break;
    case P1IV_P1IFG6:
      break;
    case P1IV_P1IFG7:
      break;
    default: break;
  }
}
/*********************    end of file  **************************/
