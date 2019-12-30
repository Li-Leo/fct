/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlSpiComm.c (Slave MCU)
*
* DESCRIPTION:
*   slave Spi handling module
*
* REVISION HISTORY:
*	Modified by Li at 08/23/2016
*
* END OF FILE HEADER
************************************************/
#include "CtlSpiComm.h"
#include "CtlData.h"
#include "CtlAdc.h"
#include "DrvFram.h"
#include "DrvMotor.h"
#include "DrvSpi.h"
#include "DrvHall.h"
#include "DrvTimer.h"
#include "DrvPower.h"
#include "CtlComm.h"
#include "CtlMotor.h"
#include "CtlTimer.h"
#include "AppMonitor.h"
#include "AppAlarm.h"

TS_RECORD g_rec_record;
T_U16 g_adc_cha;//adc value

extern TS_ADC_INFO g_adc_value;
extern TE_DELIVERY_STATE g_delivery_state;
extern T_U16 g_deliver_dose_value;
extern TE_DOSE_THICKNESS g_sys_para_thickness;
extern T_U8 volatile g_adc_completed_flag;

extern T_U32 g_encoder_counter ;        // counter (defined at DrvEncoder.c)
extern T_U32 g_encoder_expect_counter;  // store the finished counter value
extern T_U8  g_motor_finished_flag;
extern T_U16 g_hz_value;                // hz result of motor
extern T_U8 g_feedback_hz[8];
extern T_S16 g_mcu_reset_flag;


T_U32 g_com_receive = 0;

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

  while(length--){
    x = (crc >> 8) ^ (*data_p++);
    x ^= x>>4;
    crc = (crc << 8) ^ ((T_U16)(x << 12))
          ^ ((T_U16)(x <<5)) ^ ((T_U16)x);
  }
  return crc;
}

/***********************************************
* Description:
*   Set spi command to be sent to Master MCU.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*
************************************************/
void SetSpiCommandToMaster(T_U16 head, T_U16 command, T_U16 data, T_U16 data2)
{
  TS_SPI_COMMAND spi_send_command;
  //T_U8 feedback_buff[8];

  spi_send_command.sop1 = head>>8;
  spi_send_command.sop2 = head&0xFF;
  spi_send_command.command=command;
  spi_send_command.data=data;
  spi_send_command.data2 = data2;
  spi_send_command.crc_code = crc16((T_U8*)&spi_send_command, 8);
  
  CommSendBytes(E_COMM_INTERNAL_UART, (T_U8*)&spi_send_command, 10);
  M_CommPrint("send type[%c] cmd[%c%c] respond,data1[%u],data2[%u]\n", 
    spi_send_command.sop2, command>>8, command&0xFF, data, data2);
}

/***********************************************
*   description:
*       Analysis Send  Command
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void test_com_save_recv(void)
{
    *(T_U32 *)FRAM_ADDR_FCT_RECV = g_com_receive;
}

void AnalysisSendCommand(TS_SPI_COMMAND command)
{
  switch(command.command)
  {
  case COMM_MR: //motor start
    {
        TE_DIRECTION direction;
        T_U16 data = command.data&0x7FFF;

        if(command.data & 0x8000)
        {
            direction = E_MOVE_FORWARD;
        }
        else
        {
            direction = E_MOVE_BACKWARD;
        }
        g_encoder_expect_counter = ((T_U32)data<<16)|command.data2;
        CtlMotorStart(direction);
        SetSpiCommandToMaster(HEAD_FC,COMM_MR,0,0);
  }
    break;
  case COMM_TH:  //go to hall
    g_encoder_expect_counter = 400*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
    CtlMotorStartEx((TE_DIRECTION)command.data, E_TRUE, E_TRUE, E_TRUE);
    //CtlTimerBindHandle(E_TIMER_CHECK_IS_MEET_HALL, CheckIsMeetHall);
    //CtlTimerSetRepeat(E_TIMER_CHECK_IS_MEET_HALL, 10);
    SetSpiCommandToMaster(HEAD_FC,COMM_MR,0,0);
    break;
  case COMM_MS:                          //motor stop
    CtlMotorStop();
    g_encoder_counter=0;
    g_encoder_expect_counter=0;
    SetSpiCommandToMaster(HEAD_FC,COMM_MS,0,0);
    break;
  case COMM_SS:
    DrvLowPower();
    DelayMs(2);
    SetSpiCommandToMaster(HEAD_FC,COMM_SS,0,0);
    if(ExternSysMonitorIsRuning() || CtlTimerIsExist(E_TIMER_WAITING_MCU_RESET))
    {
        ExternSysMonitorStop();
        ExternSysMonitorStart(); //restart            
    }

    PulseSignalSendStart();//restart send signal
    break;
  case COMM_WD: //watchdog set
    if(command.data == 0)
    {
        M_CommonPrint("master let monitor stop\n");
        ExternSysMonitorStop();
        AlarmStop();
    }
    else
    {
        M_CommonPrint("master let monitor start\n");
        ExternSysMonitorStart();
    }
    break;
  case COMM_PU: //power up
    SetSpiCommandToMaster(HEAD_FC,COMM_PU,g_mcu_reset_flag,0);
    g_mcu_reset_flag = 0;
    ExternSysMonitorStart();
    break;
  case 0x55aa: //fct test comm
    if(command.data == 0xa5a5 && command.data2 == 0x5a5a){
        SetSpiCommandToMaster(HEAD_FC,0x55aa,0x5a5a,0xa5a5);
        g_com_receive ++;
        CtlTimerBindHandle(E_TIMER_COM_TEST, test_com_save_recv);
        CtlTimerSetRepeat(E_TIMER_COM_TEST, 2000);
    }
    break;
  default:
    M_Assert(0);
    break;
  }
}
/***********************************************
*   description:
*       Analysis Send Query Command
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void AnalysisSendQueryCommand(TS_SPI_COMMAND command)
{
  //T_U32 temp_data=0;
  //temp_data=command.data;

  switch(command.command)
  {
//	  case COMM_DS:   //Delivery Status
//	    WriteToFram(FRAM_ADDR_SYS_PARA_DELI_STATUS,&temp_data,1);
//	    break;
  //case COMM_PS: //Pulse Signal
   // break;
  case COMM_AD:   //get AD result
      {
        TS_ADC_INFO adc_value={0,0,0,0};
        /*  enable adc input channel*/
        if(command.data == E_ADC_CHANNEL_BATT_VOL)
          {DrvBattVoltChkEn();}
        else if(command.data == E_ADC_CHANNEL_BATT_PERF_VOL)
          {DrvBattPerfChkEn();}
        
        DrvAdcEnable();
        while(!g_adc_completed_flag);        //wait for convertion finished
        g_adc_completed_flag = 0;
        adc_value=AdcGetAverageValue();    
        DrvAdcDisable();

        switch(command.data)
        {   
        case E_ADC_CHANNEL_MOTOR_VOL:
          adc_value.motor_vol_channal_a15=(T_U16)(adc_value.motor_vol_channal_a15/4096.0*2.5*1000);
          SetSpiCommandToMaster(HEAD_FQ,COMM_AD,command.data, adc_value.motor_vol_channal_a15);
          break;
        case E_ADC_CHANNEL_BATT_VOL:
          //printf("%d\n",adc_value.batt_vol_channal_a12);
          adc_value.batt_vol_channal_a12=(T_U16)(adc_value.batt_vol_channal_a12/4096.0*2.5*1000);
          SetSpiCommandToMaster(HEAD_FQ,COMM_AD,command.data, adc_value.batt_vol_channal_a12);
          break;
        case E_ADC_CHANNEL_BATT_PERF_VOL:
          adc_value.batt_perf_vol_channal_a13=(T_U16)(adc_value.batt_perf_vol_channal_a13/4096.0*2.5*1000);
          SetSpiCommandToMaster(HEAD_FQ,COMM_AD,command.data, adc_value.batt_perf_vol_channal_a13);
          break;
        default:
          break;
        }
      }
    break;
//	  case COMM_AS:  //Alarm Status
//	    break;
//	  case COMM_AF:  //Abnormal Flag
//	    WriteToFram(FRAM_ADDR_SYS_PARA_MCU_RES_FLAG,&temp_data,1);
//	    break;
//	  case COMM_MP:  //Motor Position
//	    WriteToFram(FRAM_ADDR_MOTOR_POS_REC_START,&temp_data,1);
//	    break;
  case COMM_MC:
    SetSpiCommandToMaster(HEAD_FQ,COMM_MC,g_hz_value,1);
    break;
  default:
    break;
  }
}

/***********************************************
*   description:
*       Analysis Commmand From Master
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
void AnalysisCommmandFromMaster(TS_SPI_COMMAND spi_rx_command)
{
  T_U16 head;
  
  head = (spi_rx_command.sop1<<8) + spi_rx_command.sop2;
  switch(head)
  {
//	  case HEAD_SP:
//	    AnalysisSendParaCommand(spi_rx_command);
//	    break;
//	  case HEAD_ST:
//	    AnalysisSendTimeCommand(spi_rx_command);
//	    break;
  case HEAD_SC:
    AnalysisSendCommand(spi_rx_command);
    break;
  case HEAD_SQ:
    AnalysisSendQueryCommand(spi_rx_command);
    break;
  default:
    break;
  }
}

/*********************    end of file  **************************/
