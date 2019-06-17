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
*   Calculate the crc16 code according to the data.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	N/A
************************************************/
//	void CalculateCrcCode(TS_SPI_COMMAND *spi_command)
//	{
//	  T_U8  pcrc[6];
//	  T_U8* p = pcrc;
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
//	void ConvertSpiStringToCommand(T_U8 *receive_data, TS_SPI_COMMAND *spi_command)
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

/***********************************************
* Description:
*   Receive Spi Command From Master MCU.
* Argument:
*	spi_command: data packet of spi command.
* Return:
*	TS_SPI_COMMAND spi_com
************************************************/
// TS_SPI_COMMAND ReceiveSpiCommandFromMaster(T_U8 *one_packet_buff)
// {
//   TS_SPI_COMMAND spi_com={0,0,0,0};

//   ConvertSpiStringToCommand(one_packet_buff,&spi_com);
//   return spi_com;
// }
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
* Description:
*   Save Data To Frame
* Argument:
*
* Return:
*
************************************************/
//	void SaveDataToFrame(TE_SPI_SAVE_LOAD_TYPE type)
//	{
//	  switch(type)
//	  {
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERING_DOSE:
//	    //FramWriteByte(&g_dose_value,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_SHOULD,1);              //"DD"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_THICKNESS:
//	    //FramWriteByte(&g_sys_para_thickness,(T_U32 *)FRAM_ADDR_SYS_PARA_THICKNESS,1);        //"TH"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_DOSE:
//	    //FramWriteByte(&g_sys_para_max_day_total,(T_U32 *)FRAM_ADDR_SYS_PARA_DAY_TOTAL,1);    //"DT"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_SINGLE_MAX_DOSE:
//	    //FramWriteByte(&g_sys_para_max_inf_doze,(T_U32 *)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE,1);  //"SM"
//	
//	    /*�������������ĸ� �洢�ṹ������������*/
//	  case E_SPI_SAVE_LOAD_TYPE_TIME:
//	    SaveRec(g_rec_record);    //"TI"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_RECORD:
//	    SaveRec(g_rec_record);   //"DR"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_RECORD:
//	    SaveRec(g_rec_record);   //"TR"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_STOP_DELI_RECORD:
//	    SaveRec(g_rec_record);   //"SR"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_OPERATION_RECORD:
//	    SaveRec(g_rec_record);   //"OR"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_RECORD_POINTER:
//	    SaveRec(g_rec_record);   //"RP"
//	    break;
//	  /************     ����       ***********/
//	
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_STATUS:
//	    //FramWriteByte(&g_delivery_state,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_STATUS,1);     //"DS"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_MACHINE_STATUS:
//	    //FramWriteByte(&g_delivery_state,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_STATUS,1);/**/ //"MS"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_DOSE_NUMBER:   /*������*///"DN"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_BATT_NUMBER:   /*������*/   //"BN"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_POWER:
//	   // FramWriteByte( &g_delivery_state,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_POWER,1); /*����&g_delivery_state ����*/
//	    break;   //"DP"
//	  case E_SPI_SAVE_LOAD_TYPE_CALI_MILLIMETER:
//	    //FramWriteByte(&g_delivery_state,(T_U32 *)FRAM_ADDR_SYS_PARA_MOTOR_CAL,1);     //"CM"
//	    break;
//	  case E_SPI_SAVE_LOAD_TYPE_CALI_CIRCLE:
//	    //FramWriteByte( &g_delivery_state,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_CAL_RAT,1); //"CC"
//	    break;
//	  }
//	}
/***********************************************
* Description:
*   Load Data from Frame
* Argument:
*
* Return:
*
************************************************/
//	void LoadDataFromFrame(TE_SPI_SAVE_LOAD_TYPE type)
//	{
//	  switch(type)
//	  {
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERING_DOSE:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_SHOULD,1);
//	    break;     //"DD"
//	  case E_SPI_SAVE_LOAD_TYPE_THICKNESS:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_THICKNESS,1);
//	    break;     //"TH"
//	  case E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_DOSE:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DAY_TOTAL,1);
//	    break;     //"DT"
//	  case E_SPI_SAVE_LOAD_TYPE_SINGLE_MAX_DOSE:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_MAX_INF_DOSE,1);
//	    break;     //"SM"
//	
//	  /*�������������ĸ� �洢�ṹ������������*/
//	  case E_SPI_SAVE_LOAD_TYPE_TIME:
//	    SaveRec(g_rec_record);break;                                   //"TI"
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_RECORD:
//	    SaveRec(g_rec_record);break;                                   //"DR"
//	  case E_SPI_SAVE_LOAD_TYPE_DAY_TOTAL_RECORD:
//	    SaveRec(g_rec_record);break;                                   //"TR"
//	  case E_SPI_SAVE_LOAD_TYPE_STOP_DELI_RECORD:
//	    SaveRec(g_rec_record);break;                                   //"SR"
//	  case E_SPI_SAVE_LOAD_TYPE_OPERATION_RECORD:
//	    SaveRec(g_rec_record);break;                                   //"OR"
//	  case E_SPI_SAVE_LOAD_TYPE_RECORD_POINTER:SaveRec(g_rec_record);break; //"RP"
//	  /************     ����       ***********/
//	
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_STATUS:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_STATUS,1);
//	    break;                                                                  //"DS"
//	  case E_SPI_SAVE_LOAD_TYPE_MACHINE_STATUS:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_STATUS,1);
//	    break; /*����״̬Ϊ��ע״̬�����ߺ���һ��DS�ظ�*/                       //"MS"
//	  case E_SPI_SAVE_LOAD_TYPE_DOSE_NUMBER:
//	    /*ʣ��������û�б�Ҫ�浽FRAM����*/  break;                              //"DN"
//	  case E_SPI_SAVE_LOAD_TYPE_BATT_NUMBER:
//	    /*ʣ��������û�б�Ҫ�浽FRAM����*/  break;                              //"BN"
//	  case E_SPI_SAVE_LOAD_TYPE_DELIVERY_POWER:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_POWER,1);
//	    break;                                                                   //"DP"
//	  case E_SPI_SAVE_LOAD_TYPE_CALI_MILLIMETER:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_MOTOR_CAL,1);
//	    break;                                                                   //"CM"
//	  case E_SPI_SAVE_LOAD_TYPE_CALI_CIRCLE:
//	    //FramReadByte(g_frame_read_data_buff,(T_U32 *)FRAM_ADDR_SYS_PARA_DELI_CAL_RAT,1);
//	    break;                                                                                  //"CC"
//	  default:
//	    break;
//	  }
//	}
/***********************************************
*   description:
*       Analysis Send Para Command
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
//	T_U32 cali_result;
//	void AnalysisSendParaCommand(TS_SPI_COMMAND command)
//	{
//	  T_U32 temp_data=0,cali_circle=0;
//	  temp_data=command.data;
//	  
//	  switch(command.command)
//	  {
//	  case COMM_MD:
//	    break;
//	  case COMM_DD:  //save Dose address not defined
//	  	g_deliver_dose_value = temp_data;
//		WriteToFram(FRAM_ADDR_SYS_PARA_DELI_SHOULD,&temp_data,1);
//	    break;
//	  case COMM_TH:  //save thickness
//	  	g_sys_para_thickness = (TE_DOSE_THICKNESS)temp_data;
//		WriteToFram(FRAM_ADDR_SYS_PARA_THICKNESS,&temp_data,1);
//	    break;
//	  case COMM_DT:  //save day total dose
//	    WriteToFram(FRAM_ADDR_SYS_PARA_DAY_TOTAL,&temp_data,1);
//	    break;
//	  case COMM_SM:  //save single max dose
//	    WriteToFram(FRAM_ADDR_SYS_PARA_MAX_INF_DOSE,&temp_data,1);
//	    break;
//	//not configured yet
//	  case COMM_SC:  //Stop-deli Code
//	    g_rec_record.rec_type=E_RECORD_STOP_DELI;
//	    g_rec_record.rec_data=(T_U8)command.data;
//	    break;
//	  case COMM_OC:  //Operation Code
//	    g_rec_record.rec_type=E_RECORD_OPERATION;
//	    g_rec_record.rec_para=(T_U8)command.data;
//	    break;
//	  case COMM_DS:  //Delivery Status
//	    //g_delivery_state=command.data;
//	    break;
//	//
//	  case COMM_MS:     //Machine Status
//	    break;
//	  case COMM_DA:     //Dose Min Alarm
//	    WriteToFram(FRAM_ADDR_SYS_PARA_DOSE_ALARM,&temp_data,1);
//	    break;
//	  case COMM_BA:     //Batt Min Alarm
//	    WriteToFram(FRAM_ADDR_SYS_PARA_BATT_ALARM,&temp_data,1);
//	    break;
//	  case COMM_DP:     //Delivery Power
//	    WriteToFram(FRAM_ADDR_SYS_PARA_DELI_POWER,&temp_data,1);
//	    break;
//	  case COMM_SI:     //Save Rec Index
//	    break;
//	  case COMM_CM:     //Calibration Millimeter
//	    if(temp_data == PARA_CM_MAIN_HOLD){             //main hold
//	      CtlMotorStop();
//	      
//	      WriteToFram(FRAM_ADDR_SYS_PARA_MOTOR_CAL,&cali_result,1);
//	      ReadFromFram(FRAM_ADDR_SYS_PARA_MOTOR_CAL,&cali_circle,1);
//	      
//	      if(cali_result == cali_circle)
//	        SetSpiCommandToMaster(HEAD_FP,COMM_CM,cali_result,0);
//	      else
//	        SetSpiCommandToMaster(HEAD_FP,COMM_CM,0,0);
//	      
//	      cali_result=0;
//	      g_encoder_counter=0;
//	      g_encoder_expect_counter=0;
//	    }
//	    else if(temp_data == PARA_CM_MAIN_PRESS){             //main pressed
//	      g_encoder_counter=0;
//	      g_encoder_expect_counter=200*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;//20mm
//	      
//	      CtlMotorStartEx(E_MOVE_BACKWARD, E_TRUE, E_TRUE, E_TRUE);                //plate move backward
//	    }
//	
//	    break;
//	  case COMM_CR:                                        //Calibration Rate
//	    WriteToFram(FRAM_ADDR_SYS_PARA_DELI_CAL_RAT,&temp_data,1);
//	    break;
//	  default:
//	    break;
//	  }
//	}
/***********************************************
*   description:
*       Analysis Send Time Command
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
//	TS_TIME_INFO time_rx_from_master;
//	void AnalysisSendTimeCommand(TS_SPI_COMMAND command)
//	{
//	  switch(command.command)
//	    {
//	    case COMM_YR:
//	      time_rx_from_master.year=command.data;
//	      break;
//	      
//	    case COMM_MH:
//	      time_rx_from_master.month=command.data;
//	      break;
//	    case COMM_DY:
//	      time_rx_from_master.day=command.data;
//	      break;
//	    case COMM_HR:
//	      time_rx_from_master.hour=command.data;
//	      break;
//	    case COMM_ME:
//	      time_rx_from_master.minute=command.data;
//	      break;
//	    case COMM_SD:
//	      time_rx_from_master.second=command.data;
//	      break;
//	    default:
//	      break;
//	    }
//	}
//	void CheckIsMeetHall()
//	{
//	    if(g_motor_is_meet_hall)
//	    {
//	        CtlTimerClear(E_TIMER_CHECK_IS_MEET_HALL);
//	        SetSpiCommandToMaster(HEAD_FC,COMM_TH,0,0);//tell master have meet hall
//	    }
//	    if(g_motor_is_running == E_FALSE)
//	    {
//	        CtlTimerClear(E_TIMER_CHECK_IS_MEET_HALL);
//	    }
//	}
/***********************************************
*   description:
*       Analysis Send  Command
*   parameters:
*       N/A
*   return:
*       none
***********************************************/
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
//	  case COMM_SD:                          //Save Data
//	    break;
//	  case COMM_LD:                          //Load Data
//	    break;
  case COMM_SS:                          //Set mcu Sleep
    //P2OUT |= BIT2;
    //__delay_cycles(20000);
    //P2OUT &= ~BIT2;
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
//	  case COMM_SW:                          //Set Wakeup
//	    //DrvPowerOn();
//	    break;
//	  case COMM_AL:                          //Auto Location
//		g_encoder_counter=0;
//		g_encoder_expect_counter=400*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
//	    CtlMotorStartEx(E_MOVE_BACKWARD, E_TRUE, E_TRUE, E_TRUE);	
//	    SetSpiCommandToMaster(HEAD_FC,COMM_MR,0,0);
//	    break;
//	  case COMM_TC:                          //Transmission calibration
//	    M_Assert(0);
//		g_encoder_expect_counter=400*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
//	    CtlMotorStart(E_MOVE_FORWARD);	
//	    break;
//	  case COMM_SI:                          //Set Initial
//	    break;
//	  case COMM_DS:                          //Deliver Start 
//	    M_Assert(0);
//	  	//g_deliver_dose_value = 10;
//		//g_sys_para_thickness = E_DOSE_THICKNESS_200;
//	  	//g_delivery_state = CtlDeliveryStart(g_deliver_dose_value, g_sys_para_thickness);
//	  	g_encoder_expect_counter=400*(T_U32)MOTOR_SHIFTING_TO_CIRCLE/10;
//	    CtlMotorStart(E_MOVE_FORWARD);
//	    break;
//	  case COMM_AD:                          //Sampling AD 20 times
//	    M_Assert(0);
//	//	    DrvAdcEnable();
//	//	    while(!g_adc_completed_flag);        //wait for convertion finished
//	//	    g_adc_completed_flag=0;
//	//	    adc_value=AdcGetAverageValue();
//	    break;
  
//	  case COMM_MC:               //motor current calibration(Occlusion calibration)
//	      M_Assert(0);
//	      //finish calibration
//	      SetSpiCommandToMaster(HEAD_FC,COMM_MC,g_hz_value,1);
//	      //CtlMotorStop();
//	      g_hz_value=0;
//	      g_encoder_counter=0;
//	      g_encoder_expect_counter=0;
//	    break; 
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
    if(command.data == 0xa5a5 && command.data2 == 0x5a5a)
        SetSpiCommandToMaster(HEAD_FC,0x55aa,0x5a5a,0xa5a5);
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
