#include <CtlComm.h>
#include <CtlSpiComm.h>
#include "DrvUart.h"
#include "AppComm.h"
#include "stdlib.h"
#include "string.h"
#include "AppEngineerMode.h"
static T_U8 g_comm_internal_packet[10];
static T_S16 g_comm_receive_pos_at_internal_packet;

void CommOnRecvOneByteFromInternalCom(T_U8 ch)
{
	if (g_comm_receive_pos_at_internal_packet == 0 && ch == 0x46)
	{
		//control command from master 
		g_comm_internal_packet[g_comm_receive_pos_at_internal_packet] = ch;
		g_comm_receive_pos_at_internal_packet++;
	}
	else if (g_comm_receive_pos_at_internal_packet>0)
	{
		g_comm_internal_packet[g_comm_receive_pos_at_internal_packet] = ch;
		g_comm_receive_pos_at_internal_packet++;
		if (g_comm_receive_pos_at_internal_packet==10)
		{
			TS_SPI_COMMAND command;
            T_U16 crc_result;

			//receive finish, reset pos
			g_comm_receive_pos_at_internal_packet = 0;

            //check the checksum is right
            crc_result = crc16(g_comm_internal_packet, 8);
            if(memcmp(&crc_result, g_comm_internal_packet+8, 2)==0)
            {
                //the checksum is right
                memcpy(&command, g_comm_internal_packet, 10);
                M_CommPrint("recv type[%c] cmd respond[%c%c],data1[%u],data2[%u]\n", 
                    command.sop2, (T_CHAR)(command.command>>8), (T_CHAR)(command.command&0xFF), command.data,command.data2);

                AppCommHandleCmdReturn(&command);
            }
            else
            {
                M_CommPrint("receive cmd checksum fail\n");    
            }
		}
	}
}


void CommHandleNewReceiveData(TE_COMM com)
{
	T_S32 buff_size;
	TE_UART_PORT port;


	if(com == E_COMM_INTERNAL_UART)
	{
		port = E_UART1;
	}
	else
	{
		port = E_UART0;
	}
	buff_size = UartReceivedBufferSize(port);


    if (buff_size>0)
    {
        T_S32 i;
        T_U8 a;
        for(i=0; i<buff_size; i++)
        {
            //get the receive char
            a = UartReceiveByte(port);

            if (com == E_COMM_INTERNAL_UART)
            {
            	CommOnRecvOneByteFromInternalCom(a);
            }
            else
            {
                AppEngineerModeOnReceiveByte(a);
            }
        }
    }
}

void CommSendBytes(TE_COMM com, const T_U8* data, T_S32 size)
{

	if(com == E_COMM_INTERNAL_UART)
	{
		Uart1SendData(data, size);
	}
	else
	{
		Uart0SendData(data, size);
	}

    
}
