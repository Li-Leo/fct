/*******************************************************************************
**�ļ���:DrvEncoder.h
**
**����:
**
**����:HJH
**
**����:2016-04-11
**
**�޸�����:
**
**����:
**
**����:
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

#define ENCODER_STATE           (~(P1IN&BIT0))//���Ϊ�棬���ʾ��⵽��Ч�����ź�

#define ENCODER_INT_ENA         (P1IE |= BIT0)//P1.0�ж�ʹ��
#define ENCODER_INT_DIS         (P1IE &= ~BIT0)//P1.0�жϽ�ֹ

#define PORT1_INT_FLAG_CLR      (P1IFG &= 0x00)//����˿�1�����жϱ�־
#define ENCODER_INT_FLAG        P1IFG&BIT0//�������жϱ�־��ȡ
#define MAIN_INT_FLAG           P1IFG&BIT1//�������жϱ�־��ȡ
#define PULSE_INT_FLAG          P1IFG&BIT2//�����ź��жϱ�־��ȡ

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
