/************************************************
* COPYRIGHT 2016.
*
* NAME: DrvBeep.c  (Slave MCU)
*
* DESCRIPTION:
*   Slave MCU beep and vibrator module in the driver layer.
*
* REVISION HISTORY:
*	
* END OF FILE HEADER
************************************************/
#include "DrvBeep.h"
/*******************************************************************************
**函数名:void DrvBuzzSetState(T_U8 buzz_state)
**
**功能:控制蜂鸣器打开和关闭。若buzz_state为buzz_on,则打开蜂鸣器；若buzz_state为
**
**buzz_off,则关闭蜂鸣器。
**
**作者：HJH
**
**日期：2016-04-11
**
*******************************************************************************/
void DrvBuzzSetState(T_U8 buzz_state)
{
  if(buzz_state == E_BUZZ_ON)
  {
    BEEP_ON;
  }
  else
  {
    BEEP_OFF;
  }
}
/*******************************************************************************
**函数名:T_U8 DrvBuzzGetState(void)
**
**功能:获取蜂鸣器当前的工作状态，返回值为蜂鸣器工作状态
**
**作者：HJH
**
**日期：2016-04-11
**
*******************************************************************************/
T_U8 DrvBuzzGetState(void)
{
  if((P3OUT & BIT1) != 0)
  {
    return E_BUZZ_ON;
  }
  else
  {
    return E_BUZZ_OFF;
  }
}
/*******************************************************************************
**函数名:void DrvVibMotorSetState(T_U8 vibrating_motor_state)
**
**功能:控制振动电机打开和关闭
**
**作者：HJH
**
**日期：2016-04-11
**
*******************************************************************************/
void DrvVibMotorSetState(T_U8 vibrating_motor_state)
{
  if(vibrating_motor_state == E_ZD_ON)
  {
    ZD_ON;
  }
  else
  {
    ZD_OFF;
  }
}
/*******************************************************************************
**函数名:T_U8 DrvVibMotorGetState(void)
**
**功能:获取振动电机当前的工作状态,返回值为振动电机工作状态
**
**作者：HJH
**
**日期：2016-04-11
**
*******************************************************************************/
T_U8 DrvVibMotorGetState(void)
{
  if((P3OUT & BIT2) != 0)
  {
    return E_ZD_ON;
  }
  else
  {
    return E_ZD_OFF;
  }
}
