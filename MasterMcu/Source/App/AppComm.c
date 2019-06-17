#include "AppComm.h"
//#include "AppUiScreen.h"
#include "CtlData.h"
//#include "AppUiHandle.h"
//#include "AppFactory.h"
#include "AppMonitor.h"
#include "init.h"
#include "common.h"
#include "CtlComm.h"
#include "CtlMotor.h"
#include "AppFCT.h"

typedef void (*TF_COMM_HANDLER)(T_U16 cmd, T_U16 data1, T_U16 data2);

typedef struct
{
    T_U16 cmd;
    TF_COMM_HANDLER handler;
}TS_COMM_HANDLER;

TS_SPI_COMMAND gg_last_cmd_return;
extern TE_BOOLEAN g_is_comm_test_pass;
extern T_U32 g_com_test_receive_counter;

T_U16 g_curr_motor_current;
TE_BOOLEAN g_is_already_meet_hall;


/*******************************send param respond*************************************************/
//	void AppCommOnSetParamCalibrationMMRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
//	{
//	    AppCalLocSwiRemindScreenOnGetMM(data1);
//	}

/*******************************send cmd respond*************************************************/
//	void AppCommOnSetCmdCalMotorCurrentRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
//	{
//	    AppCalOcclRemindScreenOnGetMotorCurrent(data1);
//	}

void AppCommOnSetCmdToHallRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
	g_is_already_meet_hall = E_TRUE;
    //M_CommonPrint("Slaver meet hall\n");
    //AppCalLocSwiRemindScreenOnMeetHall(data1);
    //AppAutoLocaScreenOnMeetHall(data1);
}

void AppCommOnSetCmdSleepRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    M_CommonPrint("Slaver wake up\n");
}
void AppCommOnSetCmdMotorRunRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    M_CommonPrint("Slaver running\n");
}

void AppCommOnSetCmdMotorStopRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    if(data1 == 1)
    {
        M_CommonPrint("Slaver stopping\n");
        if(!CtlMotorIsStoping() && CtlMotorIsRunning())
        {
            CtlMotorStopEx(E_MOTOR_STOP, E_MOTOR_STOP_BY_FINISH_EXPECT_TARGET, 0);
        }
    }
    else
    {
        M_CommonPrint("Slaver stopped\n");        
    }
}

void AppCommOnSetCmdPowerUpRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    M_CommonPrint("Slaver power up too\n");
    if(data1 == 1)
    {
        M_CommonPrint("master is reset by slaver\n");
        SaveRec(E_RECORD_OPERATION, 81, 0);      
    }
}

void AppCommOnCommTestRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    if(data1 == 0x5a5a && data2 == 0xa5a5)
    {
        //g_is_comm_test_pass=E_TRUE;
        g_com_test_receive_counter++;
    }
}
/*******************************send query respond*************************************************/
void AppCommOnQueryMotorCurrentRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
	g_curr_motor_current = data1;
    //AppCalOcclRemindScreenOnGetMotorCurrent(data1);
    //TransmissionCalOnGetMotorCurrent(data1);
    //MonitorOnGetMotorCurrent(data1);
}

void AppCommOnQueryAdcRespond(T_U16 cmd, T_U16 data1, T_U16 data2)
{
    //BatteryPerformanceTest(data1, data2);
    //MonitorOnGetAdc(data1, data2);

    if(data1 == E_ADC_CHANNEL_BATT_VOL)
    {
        g_battery_voltage=data2*2;
        g_battery_voltage_is_ready=E_TRUE;
    }
}


//	const static TS_COMM_HANDLER g_comm_param_responds[] = 
//	{
//	    {COMM_CM, AppCommOnSetParamCalibrationMMRespond},
//	};

static const  TS_COMM_HANDLER g_comm_cmd_responds[] = 
{
    //{COMM_MC, AppCommOnSetCmdCalMotorCurrentRespond},
    {COMM_TH, AppCommOnSetCmdToHallRespond},
    {COMM_SS, AppCommOnSetCmdSleepRespond},
    {COMM_MR, AppCommOnSetCmdMotorRunRespond},
    {COMM_MS, AppCommOnSetCmdMotorStopRespond},
    {COMM_PU, AppCommOnSetCmdPowerUpRespond},
    {COMM_TEST, AppCommOnCommTestRespond},
};
static const TS_COMM_HANDLER g_comm_query_responds[] = 
{
    {COMM_MC, AppCommOnQueryMotorCurrentRespond},
    {COMM_AD, AppCommOnQueryAdcRespond},
};

void AppCommHandleCmdReturn(TS_SPI_COMMAND *ret_cmd)
{
    const TS_COMM_HANDLER* handler = NULL;
    T_SINT handler_size = 0;
    T_U16 head;

    head = (ret_cmd->sop1<<8)|ret_cmd->sop2;
	gg_last_cmd_return = *ret_cmd;
    switch(head)
    {
//	        case HEAD_FP:
//	            M_Assert(0);
//	            //handler = g_comm_param_responds;
//	            //handler_size = M_NumOfArr(g_comm_param_responds);
//	            break;
//	        case HEAD_FT:
//	            M_Assert(0);
//	            break;
        case HEAD_FC:
            handler = g_comm_cmd_responds;
            handler_size = M_NumOfArr(g_comm_cmd_responds);
            break;
        case HEAD_FQ:
            handler = g_comm_query_responds;
            handler_size = M_NumOfArr(g_comm_query_responds);
            break;
        default:
            M_Assert(0);
            break;
    }


    for(T_SINT i=0; i<handler_size; i++)
    {
        if(handler[i].cmd == ret_cmd->command)
        {
            if(handler[i].handler != NULL)
            {
               handler[i].handler(ret_cmd->command, ret_cmd->data, ret_cmd->data2);
               //printf("type[%d] cmd[%c%c] \n", ret_cmd->sop2,
                    //(T_CHAR)(ret_cmd->command>>8), (T_CHAR)(ret_cmd->command&0xFF)); //M_CommPrint
            }
            else
            {
                M_CommPrint("type[%d] cmd[%c%c] respond not have handler!\n", ret_cmd->sop2,
                    (T_CHAR)(ret_cmd->command>>8), (T_CHAR)(ret_cmd->command&0xFF)); //
                M_Assert(0);
            }
            break;
        }
    }
}
