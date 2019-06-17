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
#include "..\App\Init.h"
#include "DrvPower.h"

extern T_U8 g_rx_data[8];
extern T_U8 g_feedback_data[8];
extern volatile T_U8 g_receive_finished;

typedef enum
{
  E_SPI_M_S_COM,
  E_SPI_MAX,
} TE_SPI_CHANNEL;

void DrvSpixInitial(TE_SPI_CHANNEL cha);
TE_BOOLEAN DrvSpixWaitFinish();
void DrvSpiEnable(TE_SPI_CHANNEL cha);
void DrvSpiDisable(TE_SPI_CHANNEL cha);

#endif
