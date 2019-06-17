/*******************************************************************************
**�ļ���:DrvPort.c
**
**����:�˿ڳ�ʼ�����˿ڹ������á��˿ڷ�������
**
**����:
**
**����:2016-03-01
**
**�޸�����:
**
**����:
**
**����:
**
*******************************************************************************/
#include "DrvPort.h"
/*******************************************************************************
**������:void PortInit()
**
**����:�˿ڳ�ʼ�����˿ڹ���ѡ�񡢶˿ڷ�������,�����ڵ�һ���·
**
**���ߣ�HJH
**
**���ڣ�2016-03-01
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
/*
void PortInit(void)
{
  P1SELC = 0xff;//�Ĵ���P1SEL1��P1SEL0�Ĵ�������ͬʱ�ı�
  P1SEL1 = 0x00;//P1.0-P1.3����I/O��
  P1SEL0 = 0xf0;//P1.4-P1.7����SPI��
  P1DIR = 0x70;//P1.4-P1.6��������ڣ�P1.0-P1.3��P1.7���������
  P1IES = 0x01;//P1.0���ж�ѡ���½��ش�����������������������
//------------------------------------------------------------------------------
  P2SELC = 0xff;//�Ĵ���P2SEL1��P2SEL0�Ĵ�������ͬʱ�ı�
  P2SEL1 = 0x03;//P2.0,P2.1�����ڶ�ģ�鹦��(UCA0TXD,UCA0RXD)
  P2SEL0 = 0x00;//P2.2-P2.7����I/O��
  P2DIR = 0x1d;//P2.0,P2.2-P2.4�������;P2.1,P2.5-P2.7��������
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
  P3SEL1 = 0x00;//P3.0-P3.7����I/O��
  P3SEL0 = 0x00;
  P3DIR = 0xc7;//P3.0-P3.2,P3.6,P3.7�������;P3.3-P3.5��������
//------------------------------------------------------------------------------ 
  P4SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
  P4SEL1 = 0x00;//P4.0-P4.3����I/O��
  P4SEL0 = 0x00;
  P4DIR = 0x04;//P4.2�������;P4.0,P4.1,P4.3��������
//------------------------------------------------------------------------------
  P5SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3����I/O��
  P5DIR = 0x0e;//P5.1-P5.3�������;P5.0��������
//------------------------------------------------------------------------------ 
  P9SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  P9SEL1 = 0x00;//P9.0-P9.7����I/O��
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7�������
//------------------------------------------------------------------------------ 
  PJSELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  PJSEL1 = 0x00;//PJ.0-PJ.3 ����JTAG����
  PJSEL0 = 0x0f;
  PJDIR = 0x71;//PJ.0,PJ.5�������;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7��������
}

*/
/*******************************************************************************
**������:void PortInit()
**
**����:�˿ڳ�ʼ�����˿ڹ���ѡ�񡢶˿ڷ�������,�����ڵڶ����·
**
**���ߣ�HJH
**
**���ڣ�2016-06-14
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void PortInit(void)
{
  P1SEL1 = 0x00;//P1.0-P1.3����I/O��
  P1SEL0 = 0x00;//P1.4-P1.7����I/O��
  P1DIR  = 0xff;//P1.4-P1.6��������ڣ�P1.0-P1.3��P1.7���������
  P1DIR  &= ~(BIT0|BIT1|BIT2);
  P1OUT =0;
  P1IES = 0x02;//P1.0��P1.1:�½����ж�
  P1IE = 0;    //�ر�P1�����ж�
//------------------------------------------------------------------------------
  P2SEL1 = 0x00;//P2.0,P2.1�ֱ�����BSL�������صķ��ͺͽ�������
  P2SEL0 = 0x00;//P2.0-P2.7����I/O��
  P2DIR = 0x3c;//P2.2-P2.5�������;P2.0-P2.1,P2.6-P2.7��������
  P2IE = 0;    //�ر�P2�����ж�
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
  P3SEL1 = 0x00;//P3.4��P3.5��������CUA1TXD��UCA1RXD���ܣ�������������I/O��
  P3SEL0 = 0x00;
  P3DIR = 0xdf;//P3.5�������룬���������������
  P3OUT =0;
  P3IE = 0;    //�ر�P3�����ж�
//------------------------------------------------------------------------------  
  P4SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
  P4SEL1 = 0x00;//P4.0-P4.7����I/O��
  P4SEL0 = 0x00;
  P4DIR = 0x0C;//P4.0-P4.7�������
  P4OUT &= ~(BIT2|BIT3);
  P4IE = 0;    //�ر�P4�����ж�
//------------------------------------------------------------------------------  
  P5SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3����I/O��
  P5DIR = 0x0f;//P5.0-P5.3�������
//------------------------------------------------------------------------------  
  P9SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  P9SEL1 = 0x00;//P9.0-P9.7����I/O��
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7�������
//------------------------------------------------------------------------------  
  PJSELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  PJSEL1 = 0x00;//PJ.0-PJ.3 ����JTAG����
  PJSEL0 = 0x0f;
  PJDIR = 0x71;//PJ.0,PJ.5�������;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7��������
}
/*
void PortWakeup(void)
{
  P1SEL1 = 0x00; //P1.0-P1.3����I/O��
  P1SEL0 = 0xf0; //P1.4-P1.7����SPI��
  
  P1DIR  = 0;
  P1IFG  = 0;
  P1IE   = 0;
  
//------------------------------------------------------------------------------
  P2SEL1 = 0x00; //P2.0,P2.1�ֱ�����BSL�������صķ��ͺͽ�������
  P2SEL0 = 0x00; //P2.0-P2.7����I/O��
  P2DIR  = 0;    //P2.2-P2.5�������;P2.0-P2.1,P2.6-P2.7��������
  P2DIR  |= 0x0f;
//------------------------------------------------------------------------------  

  P3SEL1 = 0x00;
  P3SEL0 = 0x00;
  P3DIR  = 0xff;//P3.5�������룬���������������
  P3OUT  = 0;
//------------------------------------------------------------------------------  
  P4SEL1 = 0x00;//P4.0-P4.7����I/O��
  P4SEL0 = 0x00;
  P4DIR = 0xff;//P4.0-P4.7�������
  P4OUT = 0;
//------------------------------------------------------------------------------  
  P5SEL1 = 0x00;
  P5SEL0 = 0x00;//P5.0-P5.3����I/O��
  P5DIR  = 0xff;//P5.0-P5.3�������
  P5OUT  = 0;
//------------------------------------------------------------------------------  

  P9SEL1 = 0x00;//P9.0-P9.7����I/O��
  P9SEL0 = 0x00;
  P9DIR = 0xff;//P9.0-P9.7�������
  P9OUT = 0;
//------------------------------------------------------------------------------  

  PJSEL1 = 0x00;//PJ.0-PJ.3 ����JTAG����
  PJSEL0 = 0x0f;
  PJDIR  = 0x71;//PJ.0,PJ.5�������;PJ.1-PJ.3,PJ.4,PJ.6,PJ.7��������
}
*/
/*******************************************************************************
**������:void PortLowPower()
**
**����:�Զ˿ڽ��е͹������ã���Ҫ�����ص�����ʹ�ܿ��ء�δ�ö˿����õ�,�����ڵ�һ���
**
**·
**
**���ߣ�
**
**���ڣ�2016-06-13
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
/*
void PortLowPower(void)
{
  P1SELC = 0xff;//�Ĵ���P1SEL1��P1SEL0�Ĵ�������ͬʱ�ı�
  P1SEL1 = 0x00;//P1������ͨ��I/O
  P1SEL0 = 0x00;//
  P1DIR = 0x08;//P1.3����(����)����Ϊ�����P1��������������Ϊ��������
//  P1IES = 0x01;//P1.0-P1.2���ж�ѡ���½��ش���
//------------------------------------------------------------------------------
  P2SELC = 0xff;//�Ĵ���P2SEL1��P2SEL0�Ĵ�������ͬʱ�ı�
  P2SEL1 = 0x00;//P2������ͨ��I/O
  P2SEL0 = 0x00;//
  P2DIR = 0x10;//P2.4����Ϊ�����P2��������������Ϊ��������
  P2OUT &= ~BIT4;//P2.4����͵�ƽ���ص������Դ 
//------------------------------------------------------------------------------  
  P3SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
  P3SEL1 = 0x00;//P3������ͨ��I/O
  P3SEL0 = 0x00;
  P3DIR = 0x07;//P3.0-P3.2���������P3����������������������
  P3OUT &= ~(BIT0|BIT1|BIT2);//P3.0-P3.2����͵�ƽ���رձ���ơ�
                            //���������񶯵��
//------------------------------------------------------------------------------  
  P4SELC = 0xff;//�Ĵ���P4SEL1��P4SEL0�Ĵ�������ͬʱ�ı�
  P4SEL1 = 0x00;//P4������ͨ��I/O
  P4SEL0 = 0x00;
  P4DIR = 0x02;//P4.2�����P4����������������������
  P4OUT &= ~BIT2;//P4.2����͵�ƽ����ֹ��翪��ʹ��
//------------------------------------------------------------------------------  
  P5SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
  P5SEL1 = 0x00;//P5������ͨ��I/O
  P5SEL0 = 0x00;//
  P5DIR = 0x00;//P5��������������
//------------------------------------------------------------------------------  
  P9SELC = 0xff;//�Ĵ���P9SEL1��P9SEL0�Ĵ�������ͬʱ�ı�
  P9SEL1 = 0x00;//P9������ͨ��I/O
  P9SEL0 = 0x00;
  P9DIR = 0x00;//P9��������������
//------------------------------------------------------------------------------  
  PJSELC = 0xff;//�Ĵ���PJSEL1��PJSEL0�Ĵ�������ͬʱ�ı�
  PJSEL1 = 0x00;//PJ.0-PJ.3 ����JTAG����
  PJSEL0 = 0x0f;
  PJDIR = 0x01;//PJ.0�������;PJ.1-PJ.7������������
}
*/
/*******************************************************************************
**������:void PortLowPower()
**
**����:�Զ˿ڽ��е͹������ã���Ҫ�����ص�����ʹ�ܿ��ء�δ�ö˿����õ�,�����ڵڶ����
**
**·
**
**���ߣ�
**
**���ڣ�2016-06-14
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
//	void PortLowPower(void)
//	{
//	  P1SELC = 0xff;//�Ĵ���P1SEL1��P1SEL0�Ĵ�������ͬʱ�ı�
//	  P1SEL1 = 0x00;//P1������ͨ��I/O
//	  P1SEL0 = 0x00;//
//	  P1DIR = 0x08;//P1.3��������Ϊ�������ֹ��SLAVE MCU��λʹ�ܣ�P1��������������Ϊ��
//	               //������
//	  P1OUT = 0x00;
//	//------------------------------------------------------------------------------
//	  P2SELC = 0xff;//�Ĵ���P2SEL1��P2SEL0�Ĵ�������ͬʱ�ı�
//	  P2SEL1 = 0x00;//P2������ͨ��I/O
//	  P2SEL0 = 0x00;//
//	  P2DIR = 0x00;//P2����������Ϊ��������
//	//------------------------------------------------------------------------------  
//	  P3SELC = 0xff;//�Ĵ���P3SEL1��P3SEL0�Ĵ�������ͬʱ�ı�
//	  P3SEL1 = 0x00;//P3������ͨ��I/O
//	  P3SEL0 = 0x00;
//	  P3DIR = 0x0f;//P3.0-P3.3���������P3����������������������
//	  P3OUT &= ~(BIT0|BIT1|BIT2);//P3.0-P3.2����͵�ƽ���رձ���ơ�
//	                            //���������񶯵��
//	//------------------------------------------------------------------------------  
//	  P4SELC = 0xff;//�Ĵ���P4SEL1��P4SEL0�Ĵ�������ͬʱ�ı�
//	  P4SEL1 = 0x00;//P4������ͨ��I/O
//	  P4SEL0 = 0x00;
//	  P4DIR = 0x03;//P4.1��P4.2�����P4����������������������
//	  P4OUT &= ~(BIT1|BIT2);//P4.1��P4.2����͵�ƽ���ֱ��ֹ�����Դ�͹�翪��ʹ��
//	  
//	//------------------------------------------------------------------------------  
//	  P5SELC = 0xff;//�Ĵ���P5SEL1��P5SEL0�Ĵ�������ͬʱ�ı�
//	  P5SEL1 = 0x00;//P5������ͨ��I/O
//	  P5SEL0 = 0x00;//
//	  P5DIR = 0x00;//P5��������������
//	 
//	//------------------------------------------------------------------------------  
//	  P9SELC = 0xff;//�Ĵ���P9SEL1��P9SEL0�Ĵ�������ͬʱ�ı�
//	  P9SEL1 = 0x00;//P9������ͨ��I/O
//	  P9SEL0 = 0x00;
//	  P9DIR = 0x00;//P9��������������
//	//------------------------------------------------------------------------------  
//	  PJSELC = 0xff;//�Ĵ���PJSEL1��PJSEL0�Ĵ�������ͬʱ�ı�
//	  PJSEL1 = 0x00;//PJ.0-PJ.3 ����JTAG����
//	  PJSEL0 = 0x0f;
//	  PJDIR = 0x01;//PJ.0�������;PJ.1-PJ.7������������
//	  
//	}
