#include <CtlComm.h>
#include <CtlSpiComm.h>
#include "DrvUart.h"
#include "string.h"
#include "AppEngineerMode.h"
#include "CtlTimer.h"

static T_U8 g_comm_internal_packet[10];
static T_S16 g_comm_receive_pos_at_internal_packet;

void CommOnRecvFromInternalComTimeOut()
{
    g_comm_receive_pos_at_internal_packet = 0;
    M_CommPrint("recv cmd timeout\n");    
}
T_U16 g_rec_counter;
void CommOnRecvOneByteFromInternalCom(T_U8 ch)
{
    //CtlTimerSet(E_TIMER_CMD_TIME_TOO_LONG, 1000);
    
    printf("0x%x ", ch);
//	if (g_comm_receive_pos_at_internal_packet == 0 && ch == 0xA5)
//	{
//		//control command from master 
//		g_comm_internal_packet[g_comm_receive_pos_at_internal_packet] = ch;
//		g_comm_receive_pos_at_internal_packet++;
//	}
	//else 
  //      if (g_comm_receive_pos_at_internal_packet>0)
	{
		g_comm_internal_packet[g_comm_receive_pos_at_internal_packet] = ch;
		g_comm_receive_pos_at_internal_packet++;
		if (g_comm_receive_pos_at_internal_packet==10)
		{
			TS_SPI_COMMAND command;
            T_U16 crc_result;

            g_rec_counter++;
            printf("rec=%d\n", g_rec_counter);

			//receive finish, reset pos
			g_comm_receive_pos_at_internal_packet = 0;
            
            //check the checksum is right
            crc_result = crc16(g_comm_internal_packet, 8);
            if(memcmp(&crc_result, g_comm_internal_packet+8, 2)==0)
            {
                //the checksum is right
                memcpy(&command, g_comm_internal_packet, 10);
                printf("recv type[%d] cmd[%c%c],data[%d]\n", command.sop2, command.command>>8, command.command&0xFF, command.data);
                //AnalysisCommmandFromMaster(command);
            }
            else
            {
                printf("receive cmd checksum fail\n");    
            }
            CtlTimerClear(E_TIMER_CMD_TIME_TOO_LONG);

    		
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

void CommInit()
{
    CtlTimerBindHandle(E_TIMER_CMD_TIME_TOO_LONG, CommOnRecvFromInternalComTimeOut);    
}
