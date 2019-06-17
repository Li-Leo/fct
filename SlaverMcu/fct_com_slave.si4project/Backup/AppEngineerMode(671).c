#include "DrvAdc.h"
#include "stdio.h"
#include "CtlAdc.h"
#include "CtlSpiComm.h"

#include "AppEngineerMode.h"
#include "stdio.h"
#include "string.h"
#include "CtlTimer.h"
#include "CtlTime.h"
#include "AppMonitor.h"
#include "AppAlarm.h"

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

static T_S32 g_app_engineer_com_test_send = 0;


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
	printf("%lums\n", CtlTimeSincePowerOn());
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

static T_S32 AppEngineerModeOutputSensor(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    TS_ADC_INFO adc_value={0,0,0,0};
    
    DrvBattVoltChkEn();
    DrvBattPerfChkEn();
    
    DrvAdcEnable();
    while(!g_adc_completed_flag);        //wait for convertion finished
    g_adc_completed_flag = 0;
    adc_value=AdcGetAverageValue();    
    DrvAdcDisable();
    printf("battery:%d,batt_perf:%d,s_vol_cal:%d,motor_vol:%d\n", 
    adc_value.batt_vol_channal_a12, adc_value.batt_perf_vol_channal_a13,
    adc_value.s_vol_cal_channal_a14, adc_value.motor_vol_channal_a15);
    printf("battery:%.3fv,batt_perf:%.3fv,s_vol_cal:%.3fv,motor_vol:%.3fv\n", 
    M_GetVoltageFromAdc(adc_value.batt_vol_channal_a12)/1000.0,
    M_GetVoltageFromAdc(adc_value.batt_perf_vol_channal_a13)/1000.0,
    M_GetVoltageFromAdc(adc_value.s_vol_cal_channal_a14)/1000.0,
    M_GetVoltageFromAdc(adc_value.motor_vol_channal_a15)/1000.0
        );


    return 0;
}

static T_S32 AppEngineerModeTestWatchdog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    while(1)
    {
    }

    return 0;
}

static T_S32 AppEngineerModeStopWatchdog(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    AlarmStop();
    //stop monitor slave
    ExternSysMonitorStop();


    return 0;
}

static T_S32 AppEngineerModePowerupCause(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    printf("power up cause: 0x%X\n", g_power_up_cause);
    return 0;
}


void test_com(void)
{
    SetSpiCommandToMaster(HEAD_FC,0x55aa,0x5a5a,0xa5a5);
    g_app_engineer_com_test_send++;
    printf("send: %ld\n",g_app_engineer_com_test_send);
}

static T_S32 AppEngineerModeTestCom(const T_CHAR* cmd, T_CHAR* params[], T_S32 param_size)
{
    g_app_engineer_com_test_send = 0;

    CtlTimerBindHandle(E_TIMER_COMMON_DELAY, test_com);
    CtlTimerSetRepeat(E_TIMER_COMMON_DELAY, 50);

    printf("test starts\n");
    return 0;
}


//all cmd handlers
const static TS_ENGINEER_CMD_HANDLER g_app_engineer_cmd_handlers[] =
{
    {"help", AppEngineerModeHelp},
    {"exit", AppEngineerModeExit,"exit engineer mode"},
    {"repeat", AppEngineerModeRepeatLastCmd,"repeat last command"},
    {"time", AppEngineerModeTime,"show current time"},
    {"enable_output", AppEngineerModeEnableOutput,"usage: enable_output module"},
    {"disable_output", AppEngineerModeDisableOutput,"usage: disable_output module"},
    {"sensor", AppEngineerModeOutputSensor,"output sensor's value"},
    {"test_watchdog", AppEngineerModeTestWatchdog,"test watchdog"},
    {"stop_watchdog", AppEngineerModeStopWatchdog,"stop watchdog"},
    {"power_up_cause", AppEngineerModePowerupCause,"show power up cause"},
    {"test_com", AppEngineerModeTestCom,"test uart"},

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
