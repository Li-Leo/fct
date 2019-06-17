/*******************************************************************************
**�ļ���:CtlSpiComm.c  (Master MCU)
**
**����:��-��оƬͨ��ģ��
**
**����:Kevin Shi
**
**����:2016-04-28
**
*******************************************************************************/
#include "CtlSpiComm.h"
#include "CtlData.h"
#include "CtlTimer.h"
#include "CtlTime.h"
#include "DrvUart.h"
#include "CtlComm.h"
#include "string.h"
#include "AppComm.h"

/***********************************************
* Description:
*   Initialize the spi communication moudle.
* Argument:
*	N/A
* Return:
*	N/A
************************************************/
void SpiCommInit(void)
{
	//DrvSpixInitial(E_SPI_M_S_COM);
}
/***********************************************
* Description:
*   crc16 code calculation
* Argument:
*	T_U8* data_p, T_U8 length
* Return:
*	crc16 code
************************************************/
T_U16 crc16( T_U8* data_p, T_U8 length)
{
  T_U8 x;
  T_U16 crc = 0xFFFF;

  while (length--){
    x = (crc >> 8) ^ (*data_p++);
    x ^= x>>4;
    crc = (crc << 8) ^ ((T_U16)(x << 12))
          ^ ((T_U16)(x <<5)) ^ ((T_U16)x);
  }
  return crc;
}
/***********************************************
* Description:
*   Calculate the crc16 code according to the data.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	N/A
************************************************/
//	void CalculateCrcCode(TS_SPI_COMMAND *spi_command)
//	{
//		T_U8  pcrc[6];
//		T_U8* p = pcrc;
//	
//	  *p = (T_U8)((spi_command->head & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command->head & 0x00FF);
//	  p++;
//	
//	  *p = (T_U8)((spi_command->command & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command->command & 0x00FF);
//	  p++;
//	
//	  *p = (T_U8)((spi_command->data & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command->data & 0x00FF);
//	  p++;
//	  spi_command->crc_code = crc16(pcrc, 6);
//	}

/***********************************************
* Description:
*   Convert Spi Command To String.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	Converted string.
************************************************/
//	void ConvertSpiCommandToString(TS_SPI_COMMAND spi_command, T_U8 *p)
//	{
//	  *p = (T_U8)((spi_command.head & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command.head & 0x00FF);
//	  p++;
//	
//	  *p = (T_U8)((spi_command.command & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command.command & 0x00FF);
//	  p++;
//	
//	  *p = (T_U8)((spi_command.data & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command.data & 0x00FF);
//	  p++;
//	
//	  *p = (T_U8)((spi_command.crc_code & 0xFF00)>>8);
//	  p++;
//	  *p = (T_U8)(spi_command.crc_code & 0x00FF);
//	  p++;
//	}

/***********************************************
* Description:
*   Convert Spi Command To String.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	Converted string.
************************************************/
//	void ConvertSpiStringToCommand(T_U8 *receive_data,TS_SPI_COMMAND* spi_command)
//	{
//	  spi_command->head = (*receive_data<<8);
//	  receive_data++;
//	  spi_command->head |=((*receive_data));
//	  receive_data++;
//	
//	  spi_command->command = (*receive_data<<8);
//	  receive_data++;
//	  spi_command->command |=((*receive_data));
//	  receive_data++;
//	
//	  spi_command->data = (*receive_data<<8);
//	  receive_data++;
//	  spi_command->data |=((*receive_data));
//	  receive_data++;
//	
//	  spi_command->crc_code = (*receive_data<<8);
//	  receive_data++;
//	  spi_command->crc_code |=((*receive_data));
//	}

#ifdef _DEBUG
/***********************************************
* Description:
*   Receive Spi Command From Slave MCU.
* Argument:
*	 none
* Return:
*	spi_rx_command: data packet of spi command.
************************************************/
TS_SPI_COMMAND ReceiveSpiCommandFromSlave()
{
  T_U32 curr_time = CtlTimeSincePowerOn();

  memset(&gg_last_cmd_return, 0, sizeof(gg_last_cmd_return));
  while(1)
  {
      //handle new received data from usart
      if (UartGetNewDataFlag(E_UART0))
      {
          UartClearNewDataFlag(E_UART0);
          CommHandleNewReceiveData(E_COMM_PC_UART);
      }
      if (UartGetNewDataFlag(E_UART1))
      {
          UartClearNewDataFlag(E_UART1);
          CommHandleNewReceiveData(E_COMM_INTERNAL_UART);
      }

      if(gg_last_cmd_return.command != 0)
      {
        break;
      }
      if(CtlTimeElapse(curr_time)>=5000)
      {
        M_CommPrint("recv cmd respond time out\n");
        break;
      }
  }


  return gg_last_cmd_return;
}
#endif
/***********************************************
* Description:
*   Send Spi Command To Slave MCU.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	Success - 1; otherwise - 0
************************************************/
void SendSpiCommandToSlave(T_U16 head, T_U16 command, T_U16 data)
{
  SendSpiCommandToSlaveEx(head, command, data, 0);
}

void SendSpiCommandToSlaveEx(T_U16 head, T_U16 command, T_U16 data, T_U16 data2)
{
  TS_SPI_COMMAND spi_send_command;

  spi_send_command.sop1 = head>>8;
  spi_send_command.sop2 = head&0xFF;
  spi_send_command.command=command;
  spi_send_command.data=data;
  spi_send_command.data2 = data2; 

  spi_send_command.crc_code = crc16((T_U8*)&spi_send_command, 8);
  
  CommSendBytes(E_COMM_INTERNAL_UART, (T_U8*)&spi_send_command, 10);
  M_CommPrint("send type[%d] cmd[%c%c],data[%u]\n", spi_send_command.sop2, (T_CHAR)(command>>8), (T_CHAR)(command&0xFF), data);
  #if 0
  for(int i=0;i<8; i++)
  {
    M_CommPrint("0x%02x ", ((T_U8*)&spi_send_command)[i]);
  }
  M_CommPrint("\n");
  #endif    
}

#ifdef _DEBUG
/***********************************************
* Description:
*    Get ADC Result From Slave
* Argument:
*	   ADC_CHANNEL
* Return:
*	   T_U16 adc result
************************************************/
T_U16 GetADCResultFromSlave(TE_ADC_CHANNEL ch)
{
  T_U16 ad12_result=0;
  //TS_SPI_COMMAND g_spi_rx_command={0,0,0,0};

  switch(ch)
  {
  case E_ADC_CHANNEL_MOTOR_VOL:
    SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_MOTOR_VOL);
    ad12_result=(ReceiveSpiCommandFromSlave()).data;
    break;
  case E_ADC_CHANNEL_BATT_VOL:
    SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_BATT_VOL);
    ad12_result=(ReceiveSpiCommandFromSlave()).data;
    break;
  case E_ADC_CHANNEL_BATT_PERF_VOL:
    SendSpiCommandToSlave(HEAD_SQ,COMM_AD,E_ADC_CHANNEL_BATT_PERF_VOL);
    ad12_result=(ReceiveSpiCommandFromSlave()).data;
    break;
  default:
    break;
  }
  return ad12_result;
}
#endif
/*void AnalysisRcvParaCommand(TS_SPI_COMMAND command)
{
    switch(command.command)
    {
      case COMM_MD:
        break;
      case COMM_DD:   //save Dose address not defined
        break;
      case COMM_TH:  //save thickness
        break;
      case COMM_DT:  //save day total dose
        break;
      case COMM_SM:  //save single max dose
        break;
      case COMM_SC:  //Stop-deli Code
        break;
      case COMM_OC:  //Operation Code
        break;
      case COMM_DS:  //Delivery Status
        break;
      case COMM_MS:     //Machine Status
        break;
      case COMM_DA:     //Dose Min Alarm
        break;
      case COMM_BA:     //Batt Min Alarm
        break;
      case COMM_DP:     //Delivery Power
        break;
      case COMM_SI:     //Save Rec Index
        break;
      case COMM_CM:     //Calibration Millimeter
        break;
      case COMM_CR:     //Calibration Rate
        break;
      default:
        break;
    }
}

void AnalysisRcvTimeCommand(TS_SPI_COMMAND command)
{
  switch(command.command)
  {
  case COMM_YR:
  break;
  case COMM_MH:
  break;
  case COMM_DY:
  break;
  case COMM_HR:
  break;
  case COMM_ME:
  break;
  case COMM_SD:
  break;
  default:break;
  }
}

void AnalysisRcvCommand(TS_SPI_COMMAND command)
{
  switch(command.command)
  {
  case COMM_MR:
  case COMM_MS:
  case COMM_SD:
  case COMM_LD:
  case COMM_SS:
  case COMM_SW:
  case COMM_SC:
  case COMM_SI:
  case COMM_SD:
  case COMM_AD:
  default:break;
  }
}

void AnalysisRcvQueryCommand(TS_SPI_COMMAND command)
{
  switch(command.command)
  {
  case COMM_DS:
    break;
  case COMM_PS:
    break;
  case COMM_AD:
    break;
  case COMM_AS:
    break;
  case COMM_AF:
    break;
  case COMM_MP:
    break;
  default:
    break;
  }
}
void AnalysisPackagesFromSlave()
{
  TS_SPI_COMMAND spi_readback={0,0,0,0};

  switch( spi_readback.head)
  {
    case HEAD_FP:
      break;
    case HEAD_FT:
      break;
    case HEAD_FC:
      break;
    case HEAD_FQ:
      break;
         default:break;
  }
}*/
	
