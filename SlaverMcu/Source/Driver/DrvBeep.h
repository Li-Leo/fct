
#ifndef DRV_BEEP_
#define DRV_BEEP_

#include <msp430fr5989.h>
#include "TypeDefine.h"

/******************************************************************************/  
#define BEEP_ON         P2OUT |= BIT2;   //beep on  
#define BEEP_OFF        P2OUT &= ~BIT2;  //beep off
/******************************************************************************/
#define ZD_ON           P3OUT |= BIT6;   //vibrator on
#define ZD_OFF          P3OUT &= ~BIT6;  //vibrator off
/******************************************************************************/
typedef enum //定义蜂鸣器状态
{
  E_BUZZ_ON = 0,
  E_BUZZ_OFF,
  E_BUZZ_MAX,
}BUZZ_STATE;
/******************************************************************************/
typedef enum//定义振动电机状态
{
  E_ZD_ON = 0,
  E_ZD_OFF,
  E_ZD_MAX,
}ZD_STATE;

void DrvBuzzSetState(T_U8 buzz_state);
T_U8 DrvBuzzGetState(void);
void DrvVibMotorSetState(T_U8 vibrating_motor_state);
T_U8 DrvVibMotorGetState(void);

#endif
  
 

