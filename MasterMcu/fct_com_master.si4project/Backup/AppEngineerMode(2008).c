#include "string.h"
#include "AppEngineerMode.h"
#include "stdio.h"
#include "string.h"
#include "CtlTimer.h"
//#include "AppDelivery.h"
#include "DrvRTC.h"
#include "CtlData.h"
//#include "AppSetInfuPara.h"
//#include "AppFactory.h"
#include "CtlSpiComm.h"
#include "stdlib.h"
#include "CtlMotor.h"
//#include "AppCommon.h"
#include "DrvTimer.h"
#include "AppFCT.h"

/************************************************
* Declaration
************************************************/
#define MAX_SIZE_OF_RECEIVE_STR 60
#define CMD_REPEAT_RUN_TIME 2000  //each 2s to run cmd if repeat param is give
typedef  T_S32 (*TF_ENGINEER_CMD_HANDLER)(const T_CHAR* cmd, T_CHAR *params[], T_S32 param_size);
typedef struct  
{
    T_CHAR command[28];
    TF_ENGINEER_CMD_HANDLER handler;
    T_CHAR description[128];
}TS_ENGINEER_CMD_HANDLER;

static void AppEngineerModeParseCmd(T_CHAR* cmd);

extern T_U16 g_power_up_cause;

/************************************************
* Variable 
************************************************/
static T_CHAR g_app_engineer_receive_str[MAX_SIZE_OF_RECEIVE_STR];
static T_S32 g_app_engineer_receive_index = 0;
static T_CHAR g_app_engineer_last_receive_str[MAX_SIZE_OF_RECEIVE_STR];
//static TE_BOOLEAN g_app_engineer_is_enable= E_FALSE;
static TE_BOOLEAN g_app_engineer_is_enable_cmd = E_FALSE;
static const TS_ENGINEER_CMD_HANDLER* g_app_engineer_handlers_ptr = NULL;


/************************************************
* Function 
************************************************/

void AppEngineerModeRepeatRun(void)
{
    //T_CHAR tmp_receive_str[MAX_SIZE_OF_RECEIVE_STR];
    
    //strcpy(tmp_receive_str, g_app_engineer_last_receive_str);
    AppEngineerModeParseCmd(g_app_engineer_last_receive_str);   
}

//all command function
static T_S32 AppEngineerModeHelp(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    const TS_ENGINEER_CMD_HANDLER* p;
    p = g_app_engineer_handlers_ptr;

    printf("all cmds:\n");
    while(p->command[0] != 0)
    {
        printf("%-28s", p->command);
        if (p->description[0] != 0)
        {
            printf(" %s", p->description);
        }
        putchar('\n');
        p++;
    }
    
    return 0;
}

static T_S32 AppEngineerModeExit(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    //do not receive data
    g_app_engineer_is_enable_cmd = E_FALSE;

    return 0;
}

static T_S32 AppEngineerModeRepeatLastCmd(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    CtlTimerSetRepeat(E_TIMER_ENGINEER_MODE_REPEAT_CMD, CMD_REPEAT_RUN_TIME);
    AppEngineerModeRepeatRun();
    return 0;
}

static T_S32 AppEngineerModeTime(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    Ds1302TimeRead(); 
	printf("%04d-%02d-%02d %02d:%02d:%02d\n", (T_S16)g_current_time.year,
        (T_S16)g_current_time.month, (T_S16)g_current_time.date, 
        (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second);
    return 0;
}

static T_S32 AppEngineerModeEnableOutput(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    T_SINT i;

    for(i=0; i<param_size; i++)
    {
        if(SetModuleOutputByName(params[i], E_TRUE))
        {
            printf("%s module set success\n", params[i]);
        }
    }

    return 0;
}

static T_S32 AppEngineerModeDisableOutput(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    T_SINT i;

    for(i=0; i<param_size; i++)
    {
        if(SetModuleOutputByName(params[i], E_FALSE))
        {
            printf("%s module set success\n", params[i]);
        }
    }

    return 0;
}

// static T_S32 AppEngineerModeSetNextDeliveryTime(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {
//     Ds1302TimeRead();
//     g_next_deliver_time = g_current_time;
//     g_next_deliver_time.second = g_current_time.second+5;
//     if(g_next_deliver_time.second>=60)
//     {
//         g_next_deliver_time.minute++;     
//         g_next_deliver_time.second -= 60;
//     }
//     if(g_next_deliver_time.minute>=60)
//     {
//         g_next_deliver_time.hour++;     
//         g_next_deliver_time.minute -= 60;
//     }
// 	if (g_next_deliver_time.hour > 23)
// 	{
// 		g_next_deliver_time.hour -= 24;
// 		g_next_deliver_time.week++;
//         if(g_next_deliver_time.week>7)
//         {
//            g_next_deliver_time.week = 1; 
//         }
// 	}		
//     printf("set next delivery time: week:%ld, %02lu:%02lu:%02lu\n", 
//         g_next_deliver_time.week-1,
//         g_next_deliver_time.hour, g_next_deliver_time.minute, g_next_deliver_time.second);


//     return 0;
// }

// static T_S32 AppEngineerModeStartDelivery(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {
//     //for test
//     ExecDelivery(g_sys_para_thickness, g_deliver_dose_value);

//     return 0;
// }

static T_S32 AppEngineerModeReadData(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    if(param_size==1 && strcmp(params[0],"plan")==0)
    {   
        printf("infusion plan\n");
        for(T_SINT idx = 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx ++)
    	{
    		if(g_sys_para_infu_info[idx].dose!=0)
    		{
                printf("%02d:%02d %d/10ug\n", idx*15/60,idx*15%60, (T_S16)(g_sys_para_infu_info[idx].dose));
    		}
    	}
        printf("\n");
    }
    else if(param_size==1 && strcmp(params[0],"infusion_rec")==0)
    {
        
    }
    else
    {
        printf("factory data:\n");
        printf("len encoder: %lu=%u*0.001ml, hall to end len:%u*0.001mm\n", 
            g_motor_cali_circle, EncoderToVolume(g_motor_cali_circle), g_cali_hal_to_end_len);
        //printf("hall pos: %u/1000 ml\n", TOOLING_HALL_CAL_VOLUME + EncoderToVolume(g_motor_cali_circle));
		
        printf("min occlusion: %u/100 Hz, min motor occlusion speed:%uHz\n", 
            g_occl_cal_min, g_occl_speed_cal_min);        
        printf("max occlusion: %u/100 Hz, max motor occlusion speed:%uHz\n", 
            g_occl_cal_max, g_occl_speed_cal_max); 		
        //printf("dose alarm limt:%d, battery alarm limt:%d");
        printf("battery offset: %d\n", g_battery_voltage_offset);
        printf("SN: %lu, hardware ver:%lu\n", 
            g_serial_number, g_hardware_ver);
        printf("Common Pass: %u\n", g_password_common); 
        // printf("\n");
        // printf("user data\n");
        // printf("reset flag: %d\n", g_mcu_abnormal_reset_state);
        // printf("last_delivert_start_motor_pos:%lu\n", g_deliver_start_motor_pos);
        // printf("delivery switch:%d\n", g_deliver_switch);
        // printf("thickness:%u ug/ml\n", GetThicknessValue(g_sys_para_thickness));
        // printf("once max infusion dose:%u ug\n", g_sys_para_max_inf_dose);
        // printf("curr motor pos:%lu\n", g_motor_position);
        // printf("motor pos saved 1 sec:%lu, encoder:%lu\n", 
        //     *((T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START),*((T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START+1)
        //         );
        // printf("motor pos saved 2 sec:%lu, encoder:%lu\n", 
        //     *((T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START+3),*((T_U32*)FRAM_ADDR_MOTOR_POS_CNT_START+4)
        //         );
        
        // printf("dose left: %u/10 ug, volume left: %u/1000 ml\n", RemainDose(), EncoderToVolume(g_motor_position));
        // printf("hall pos: %u/1000 ml\n", TOOLING_HALL_CAL_VOLUME + EncoderToVolume(g_motor_cali_circle));
        // Ds1302TimeRead(); 
    	// M_CommonPrint("curr time: %04d-%02d-%02d %02d:%02d:%02d\n", (T_S16)g_current_time.year,
        //     (T_S16)g_current_time.month, (T_S16)g_current_time.date, 
        //     (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second);
        // printf("next delivery time: week %d, %02u:%02u:%02u, %u/10ug\n",
        //     (T_S16)(g_next_deliver_time.week-1),
        //     (T_U16)g_next_deliver_time.hour,
        //     (T_U16)g_next_deliver_time.minute,
        //     (T_U16)g_next_deliver_time.second, g_deliver_dose_value);
    }

    return 0;
}

// static T_S32 AppEngineerModeSetFastPlan(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {
// 	for(int idx= 0; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx++)
// 	{
//         g_sys_para_infu_info[idx].index = idx;
//         g_sys_para_infu_info[idx].dose = g_sys_para_max_inf_dose*10;
// 		SaveInfusionDose(idx + 1);
// 	}

//     return 0;
// }

static T_S32 AppEngineerModeOutputSensor(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
	printf("real battery %d\n", g_batt_number);
    printf("calibration battery %d\n", g_batt_number+g_battery_voltage_offset);

    return 0;
}

static T_S32 AppEngineerModeCancelSleep(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
	CtlTimerClear(E_TIMER_AUTO_SLEEP);
    CtlTimerSet(E_TIMER_NO_OPERATION_SLEEP, (T_S32)1000*60*30);

    return 0;
}

static T_S32 AppEngineerModeCalBattery(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    T_SINT offset;

    offset = 3000 - g_batt_number;

    g_battery_voltage_offset = offset;
    SaveSysPara(E_SYSTEM_PAPA_BATTERY_VOLLTAGE_OFFSET, 0);
    printf("battery calibration finish.\n");
    printf("curr battery voltage:%dmV, the offset:%dmV\n", g_batt_number, offset);

    return 0;
}

static T_S32 AppEngineerModeTestWatchdog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    while(1)
    {
      ;
    }
}

static T_S32 AppEngineerModeFakeMotorPos(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_motor_position = VolumeToEncoder(5000);
    SaveMotorPos(E_TRUE);

    return 0;
}

static T_S32 AppEngineerModeStopWatchdog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    //let slave not monitor me
    SendSpiCommandToSlave(HEAD_SC, COMM_WD, 0);
    //stop monitor slave
    //ExternSysMonitorStop();


    return 0;
}

static T_S32 AppEngineerModeRun(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    T_U16 dose = 100;
    
    if(param_size == 1)
    {
        dose = atoi(params[0]);
    }

    //ExecDelivery(g_sys_para_thickness, dose);

    return 0;
}


static T_S32 AppEngineerModeRunBack(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    T_U16 dose = 100;
    
    if(param_size == 1)
    {
        dose = atoi(params[0]);
    }

    T_U32 expect_circle;

    expect_circle = DoseToEncoder(g_sys_para_thickness, dose);
    AppStartRun(E_MOVE_BACKWARD, expect_circle);

    return 0;
}

//static T_S32 AppEngineerModeDeliveryDetailInfo(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
//{
//    printf("encoder:%lu, current:%dhz, encoder speed:%dhz\n", 
//        g_encoder_counter, g_curr_motor_current, g_encoder_hz_result);
//
//    return 0;
//}

// static T_S32 AppEngineerModeTestLog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {
//     T_SINT i;

//     //stop watchdog
//     SendSpiCommandToSlave(HEAD_SC, COMM_WD, 0);
//     //write log
//     for(i=0; i<2001; i++)
//     {
//         printf("write %d log\n", i);
//         SaveRec(E_RECORD_INFUSION, i, i+1);
//         SaveRec(E_RECORD_DAYS_TOTAL, i, i+1);
//         SaveRec(E_RECORD_STOP_DELI, i, i+1);
//         SaveRec(E_RECORD_OPERATION, i, i+1);
//     }
//     printf("write finish\n");
//     //destory one log
//     *(T_U8*)FRAM_ADDR_INFUSION_RECORD_START = 0x55;

//     return 0;
// }

// static T_S32 AppEngineerModeReadLog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {
//     TE_RECORD_TYPE log_type = E_RECORD_OPERATION;
    
//     if(param_size == 1)
//     {
//         T_SINT i = atoi(params[0]);
//         switch(i)
//         {
//             case 1:
//                 log_type = E_RECORD_INFUSION;
//                 break;
//             case 2:
//                 log_type = E_RECORD_DAYS_TOTAL;
//                 break;
//             case 3:
//                 log_type = E_RECORD_STOP_DELI;
//                 break;
//         }
//     }

    
//     TS_RECORD rc;
//     T_SINT index;
//     T_SINT i=0;

//     switch(log_type)
//     {
//         case E_RECORD_INFUSION:
//             printf("infusion log: [%u]\n", g_record_counter.de_rec_counter);
//             break;
//         case E_RECORD_DAYS_TOTAL:
//             printf("day total log: [%u]\n", g_record_counter.dt_rec_counter);
//             break;    
//         case E_RECORD_STOP_DELI:
//             printf("stop infusion log: [%u]\n", g_record_counter.sd_rec_counter);
//             break;    
//         case E_RECORD_OPERATION:
//             printf("operation log: [%u]\n", g_record_counter.op_rec_counter);
//             break;    
//     }
//     rc.rec_type = log_type;
//     index = RecFirstIndex(log_type);
//     while(index != -1)
//     {
//         i++;
//         rc.rec_id = index;
//         if(LoadRec(&rc))
//         {
//             printf("%04d: %04d-%02d-%02d %02d:%02d:%02d: para[%lu], data[%lu]\n",
//                 i, (T_SINT)rc.rec_time.year, (T_SINT)rc.rec_time.month, (T_SINT)rc.rec_time.date,
//                 (T_SINT)rc.rec_time.hour, (T_SINT)rc.rec_time.minute, (T_SINT)rc.rec_time.second,
//                 rc.rec_para, rc.rec_data);
//         }
//         else
//         {
//             printf("%04d: log crc wrong\n", i);
//         }
//         index = RecNextIndex(log_type, index);
//     }

//     return 0;
// }

// static T_S32 AppEngineerModeShowScreen(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {   
//     if(param_size == 1)
//     {
//         T_SINT i = atoi(params[0]);
//         //DisplayUI((TE_SCREEN_ID)i);
//     }

//     return 0;
// }

// static T_S32 AppEngineerModeCurrScreen(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
// {   
//     //printf("current screen id:%d\n", GetCurrentUI());

//     return 0;
// }

//static T_S32 AppEngineerModeTestSleepTimer(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
//{   
//    //let slave not monitor me
//    SendSpiCommandToSlave(HEAD_SC, COMM_WD, 0);
//    
//    TS_TIME_INFO time1,time2;
//    
//    printf("start test wakeup timer\n");
//    Ds1302TimeRead(); 
//    printf("curr time: %02d:%02d:%02d\n", 
//                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
//    time1 = g_current_time;
//    g_auto_wakeup_time.week = 0;
//    DrvTimerEnable(E_TIMER_CHANNEL_A1);
//    g_auto_wakeup_timer_ms = 60000;
//    DelayMs(60000);
//    DrvTimerDisable(E_TIMER_CHANNEL_A1);
//    Ds1302TimeRead(); 
//    printf("curr time: %02d:%02d:%02d\n", 
//                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
//    time2 = g_current_time;
//    printf("should pass 60s, actual pass %ldms, %lds\n", g_timer_pass_count, DiffDateTimeInOneDay(&time2, &time1));
//
//    //TS_TIME_INFO time1,time2;
//    
//    printf("start test wakeup timer when sleep\n");
//    DrvTimerDisable(E_TIMER_CHANNEL_A2);
//    Ds1302TimeRead(); 
//    printf("curr time: %02d:%02d:%02d\n", 
//                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
//    time1 = g_current_time;
//    g_auto_wakeup_time.week = 0;
//    DrvTimerEnable(E_TIMER_CHANNEL_A1);
//    g_auto_wakeup_timer_ms = 60000;
//    //DelayMs(60000);
//    __bis_SR_register(LPM3_bits + GIE);      // LPM3
//    DrvTimerDisable(E_TIMER_CHANNEL_A1);
//    Ds1302TimeRead(); 
//    printf("curr time: %02d:%02d:%02d\n", 
//                    (T_S16)g_current_time.hour, (T_S16)g_current_time.minute, (T_S16)g_current_time.second); 
//    time2 = g_current_time;
//    printf("should pass 60s, actual pass %ldms, %lds\n", g_timer_pass_count, DiffDateTimeInOneDay(&time2, &time1));
//    DrvTimerEnable(E_TIMER_CHANNEL_A2);
//    return 0;
//}

static T_S32 AppEngineerModeClearData(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    if(param_size==1 && strcmp(params[0],"factory")==0)
    {
        *(T_U32 *)FRAM_ADDR_SYS_PARA_FACTORY_FLAG = 0;
        printf("finish clear factory data, please reset\n");    
    }
    // else
    // {
    //     FactoryReset();
    //     printf("finish clear user data, please reset\n");
    // }

    return 0;
}

static T_S32 AppEngineerModePowerupCause(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    printf("power up cause: 0x%X\n", g_power_up_cause);
    return 0;
}
static T_S32 AppEngineerModeFCTContinueMode(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    g_is_use_continue_mode=E_TRUE;
    g_is_get_mode_cmd=E_TRUE;
    g_case_num=13;
    return 0;
}
static T_S32 AppEngineerModeFCTMenuMode(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    g_is_use_continue_mode=E_FALSE;
    g_is_get_mode_cmd=E_TRUE;
    g_case_num=12;
    return 0;
}

static T_S32 AppEngineerModeFCTCase0(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    g_case_num=0;
    return 0;
}

static T_S32 AppEngineerModeFCTCase1(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    g_case_num=1;
    return 0;
}
static T_S32 AppEngineerModeFCTCase2(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{   
    g_case_num=2;
    return 0;
}

static T_S32 AppEngineerModeFCTCase3(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=3;
    return 0;
}

static T_S32 AppEngineerModeFCTCase4(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=4;
    return 0;
}

static T_S32 AppEngineerModeFCTCase5(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{

    g_case_num=5;
    return 0;
}

static T_S32 AppEngineerModeFCTCase6(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=6;
    return 0;
}
static T_S32 AppEngineerModeFCTCase7(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{

    g_case_num=7;
    return 0;
}

static T_S32 AppEngineerModeFCTCase8(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=8;
    return 0;
}

static T_S32 AppEngineerModeFCTCase9(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=9;
    return 0;
}

static T_S32 AppEngineerModeFCTCase10(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_case_num=10;
    return 0;
}

//all cmd handlers
static const TS_ENGINEER_CMD_HANDLER g_app_engineer_cmd_handlers[] =
{
    {"help", AppEngineerModeHelp},
    {"exit", AppEngineerModeExit,"exit engineer mode"},
    {"repeat", AppEngineerModeRepeatLastCmd,"repeat last command"},
    {"time", AppEngineerModeTime,"show current time"},
    {"enable_output", AppEngineerModeEnableOutput,"usage: enable_output module"},
    {"disable_output", AppEngineerModeDisableOutput,"usage: disable_output module"},
    // {"set_next_time", AppEngineerModeSetNextDeliveryTime,"set next time(add 5s) to delivery"},
    // {"start_delivery", AppEngineerModeStartDelivery,"start delivery at once"},
    {"read_data", AppEngineerModeReadData,"read saved data"},
    // {"set_fast_plan", AppEngineerModeSetFastPlan,"set fast plan"},
    {"sensor", AppEngineerModeOutputSensor,"output sensor's value"},
    {"cancel_sleep", AppEngineerModeCancelSleep,"cancel sleep"},
    {"cal_battery", AppEngineerModeCalBattery,"calibration battery use fixed 3v power"},
    {"test_watchdog", AppEngineerModeTestWatchdog,"test watchdog"},
    {"stop_watchdog", AppEngineerModeStopWatchdog,"stop watchdog"},
    {"fake_motor_pos", AppEngineerModeFakeMotorPos,"fake motor pos as a big value"},
    {"run", AppEngineerModeRun,"usage: run [dose], the unit is 0.1ug, default it will run 10 dose"},
    {"run_back", AppEngineerModeRunBack,"usage: run_back [dose], the unit is 0.1ug, default it will run 10 dose"},
    //{"delivery_detail", AppEngineerModeDeliveryDetailInfo, "output curr delivery detail"},
    //{"test_log", AppEngineerModeTestLog, "test log full and crc wrong"},
    // {"read_log", AppEngineerModeReadLog, 
    //     "usage: read_log [param], param:1=>infusion, 2=>day total, 3=>stop del, other=>operator"},
    // {"show_screen", AppEngineerModeShowScreen, "show_screen screen_id"},
    // {"curr_screen", AppEngineerModeCurrScreen, "show current screen_id"},
    //{"test_sleep_timer", AppEngineerModeTestSleepTimer, "test sleep timer precision"},
    {"clear_data", AppEngineerModeClearData, "usage: clear_data [factory], used to clear user or factory data"},
    {"power_up_cause", AppEngineerModePowerupCause,"show power up cause"},
    {"c", AppEngineerModeFCTContinueMode},
    {"m", AppEngineerModeFCTMenuMode},
    {"0", AppEngineerModeFCTCase0},
    {"1", AppEngineerModeFCTCase1},
    {"2", AppEngineerModeFCTCase2},
    {"3", AppEngineerModeFCTCase3},
    {"4", AppEngineerModeFCTCase4},
    {"5", AppEngineerModeFCTCase5},
    {"6", AppEngineerModeFCTCase6},
    {"7", AppEngineerModeFCTCase7},
    {"8", AppEngineerModeFCTCase8},
    {"9", AppEngineerModeFCTCase9},
    {"10", AppEngineerModeFCTCase10},

    //below must at end
    {"", NULL}   
};

/***********************************************
* Description:
*   parse all params from the cmd
* Argument:
*   cmd:
*   params:
*
* Return:
*   the params count
************************************************/
static T_S32 AppEngineerModeParseParams(T_CHAR* cmd, char* params[], T_S32 params_max)
{
    T_S32 param_index = 0;

    while(cmd != NULL && param_index<params_max)
    {
        //get one param
        params[param_index] = cmd;
        param_index++;

        //find the ' '
        cmd = strchr(cmd, ' ');
        if(cmd != NULL)
        {
            *cmd = 0;
            cmd++;
        }
        else
        {
            break;
        }
    }
    return param_index;
}

/***********************************************
* Description:
*   parse cmd str
* Argument:
*   cmd:
*
* Return:
*
************************************************/
static void AppEngineerModeParseCmd(T_CHAR* cmd)
{          
    if (strcmp(cmd, "enable_cmd weichuang")==0)
    {
        printf("engineer cmd enable, input help to get avalid cmd\n");
        g_app_engineer_is_enable_cmd = E_TRUE;
        return;
    }
    else if(g_app_engineer_is_enable_cmd)
    {
        T_S32 ret = 0;
        T_CHAR *params[10];
        T_S32 param_size = 0;
        const TS_ENGINEER_CMD_HANDLER* p;
        T_CHAR *work_str;
        T_CHAR cmd_back[MAX_SIZE_OF_RECEIVE_STR];
        
        strcpy(cmd_back, cmd);

        //find the ' '
        work_str = strchr(cmd, ' ');
        if(work_str != NULL)
        {
            *work_str = 0;
            work_str++;
        }
        param_size = AppEngineerModeParseParams(work_str, params, 10);
        if(strcmp(cmd, "repeat") != 0)
        {
            strcpy(g_app_engineer_last_receive_str, cmd_back);
        }
        p = g_app_engineer_handlers_ptr;

        //find the cmd handler
        while(p->command[0] != 0)
        {
            if (strcmp(p->command, cmd)==0)
            {
                ret = p->handler(cmd, params, param_size);
                break;
            }
            p++;
        }


        //if not find cmd handler, return -1
        if (p->command[0] == 0)
        {
            ret = -1;
            if(g_app_engineer_is_enable_cmd)
            {
                puts("not valid cmd");
            }
        }
        else if(ret != 0)
        {
            printf("cmd return error code:%d\n", (T_S16)ret);
        }

    }
}


/***********************************************
* Description:
*   handle char received from UART
* Argument:
*   ch:
*   is_handled_ok_at_pre_handler:
*
* Return:
*
************************************************/
TE_BOOLEAN AppEngineerModeOnReceiveByte(T_U8 ch)
{
    //if previous handler handle it OK, reset the index
    if (ch == '\r' || ch=='\n') //cmd end char
    {
        g_app_engineer_receive_str[g_app_engineer_receive_index] = 0;
        if(g_app_engineer_receive_index>0)
        {
            //stop last cmd repeat run
            CtlTimerClear(E_TIMER_ENGINEER_MODE_REPEAT_CMD);
            //parse the cmd
            AppEngineerModeParseCmd(g_app_engineer_receive_str);
        }
        //reset the index
        g_app_engineer_receive_index = 0;
    }
    else
    {       

        //receive cmd srt
        g_app_engineer_receive_str[g_app_engineer_receive_index] = ch;
        g_app_engineer_receive_index++;
        if(g_app_engineer_receive_index >=MAX_SIZE_OF_RECEIVE_STR-1)
        {
            g_app_engineer_receive_index = 0;
        }
    }
    return E_TRUE;
}


/***********************************************
* Description:
*
* Argument:
*
* Return:
*
************************************************/
void AppEngineerModeInit( void )
{
    g_app_engineer_handlers_ptr = g_app_engineer_cmd_handlers;
    CtlTimerBindHandle(E_TIMER_ENGINEER_MODE_REPEAT_CMD, AppEngineerModeRepeatRun);
//#ifdef _DEBUG	
    g_app_engineer_is_enable_cmd = E_TRUE;
//#endif
}
