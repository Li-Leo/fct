#ifndef CTLCOMM_H
#define CTLCOMM_H
#include "common.h"
typedef enum 
{
	E_COMM_INTERNAL_UART,
	E_COMM_PC_UART,
}TE_COMM;

#define M_CommPrint if(IsEnableOutput(E_MODULE_COMM))printf

void CommHandleNewReceiveData(TE_COMM com);
void CommSendBytes(TE_COMM com, const T_U8* data, T_S32 size);
#endif
