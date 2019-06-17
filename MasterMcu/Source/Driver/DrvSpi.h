/*******************************************************************************
**�ļ���:DrvSpi.h
**
**����:����"DrvSpi.c�ļ���ʹ�õĺꡢö�ٱ�����"
**
**����:HJH
**
**����:2016-04-16
**
**�޸�����:	Adjust the codes according to the Coding Standard.
**
**����:Kevin Shi
**
**����:04/21/2016
**
*******************************************************************************/
#ifndef  DRVSPI_H
#define DRVSPI_H

#include <msp430fr5989.h>
#include "TypeDefine.h"

#define SPI_COMPLETE_FLAG ~(UCB0STATW & BIT0)

typedef enum
{
  E_SPI_M_S_COM,//����CPUͨ�ŵ�SPI�ӿ�
  E_SPI_MAX,//ָʾö�ٱ�����ȡֵ������
} TE_SPI_CHANNEL;

void DrvSpixInitial(TE_SPI_CHANNEL cha);
void DrvSpixWriteByte(T_U8 tx_data);
T_U8 DrvSpixReadByte(void);
TE_BOOLEAN DrvSpixWaitFinish();
void DrvSpiEnable(TE_SPI_CHANNEL cha);
void DrvSpiDisable(TE_SPI_CHANNEL cha);

#endif
