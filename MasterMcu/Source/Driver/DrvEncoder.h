/*******************************************************************************
**文件名:DrvEncoder.h
**
**功能:
**
**作者:HJH
**
**日期:2016-04-11
**
**修改内容:
**
**作者:
**
**日期:
**
*******************************************************************************/
#ifndef DRV_ENCODER_H
#define DRV_ENCODER_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"
extern T_U32 g_encoder_counter;
extern T_U32 g_encoder_expect_counter;
extern T_U8 g_encoder_need_stop_flag;
extern T_U8 g_encoder_is_monitor;
extern T_U16 g_encoder_hz_result;
extern TE_BOOLEAN g_encoder_is_hz_result_ready;


#define ENCODER_NUMBER_GET  100

#define ENCODER_STATE           (~(P1IN&BIT0))//结果为真，则表示检测到有效编码信号

#define ENCODER_INT_ENA         (P1IE |= BIT0)//P1.0中断使能
#define ENCODER_INT_DIS         (P1IE &= ~BIT0)//P1.0中断禁止

#define PORT1_INT_FLAG_CLR      (P1IFG &= 0x00)//清除端口1所有中断标志
#define ENCODER_INT_FLAG        P1IFG&BIT0//编码器中断标志获取
#define MAIN_INT_FLAG           P1IFG&BIT1//主按键中断标志获取
#define PULSE_INT_FLAG          P1IFG&BIT2//心跳信号中断标志获取

void DrvEncoderInitial(void);
void DrvEncoderEnable(void);   
void DrvEncoderDisable(void);   
//call it can disable motor pos monitor at this running
void DrvEncoderDisableMotorPosMonitorOnce();

T_U32 DrvGetEncoderCounter(void);
void DrvClearEncoderCounter(void);

//1:wakeup by port, 0: not wakeup by port
T_U8 DrvPortWakeupFlag();
//clear the flag as 0
void DrvPortClearWakeupFlag();


#endif /* DRV_ENCODER_H */
