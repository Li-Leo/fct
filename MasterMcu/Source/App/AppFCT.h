#ifndef APPFCT_H
#define APPFCT_H

#include "..\TypeDefine.h"

#include <msp430fr5989.h>
#include "CtlKey.h"
#include "CtlTime.h"
#include "CtlTimer.h"
#include "..\Kernel\CtlDisplay.h"
#include "..\Kernel\Fonts.h"
#include "..\Driver\DrvLcd.h"
#include "..\Driver\DrvRtc.h"
#include "CtlComm.h"
#include "intrinsics.h"
#include "CtlMotor.h"
#include "CtlData.h"
#include "AppCommon.h"
#include "AppFactory.h"
#include "AppEngineerMode.h"
//#include "CtlCommon.h"
#include "DrvKey.h"
#include "DrvBeep.h"
#include "DrvUart.h"
#include "DrvEncoder.h"
#include "CtlSpiComm.h"
#include "DrvPower.h"
#include "DrvLcd.h"
#include "DrvTimer.h"
#include "DrvSpi.h"
#include "DrvEncoder.h"
#include "DrvUart.h"
#include "Init.h"
#include "CtlDelivery.h"
#include "AppDelivery.h"
#include "version.h"


#define MAX_SIZE_OF_ONE_LINE    16
#define TEST_COM_INTERVAL_MS    30
#define TEST_PULSE_INTERVAL_MS  30
#define TEST_COM_EXPECT_COUNT   5000

typedef enum
{
    E_FCT_TEST_CASE_VERSION,
    E_FCT_TEST_CASE_SN,
    E_FCT_TEST_CASE_KEY,
    E_FCT_TEST_CASE_LCD,
    E_FCT_TEST_CASE_BUZZER,
    E_FCT_TEST_CASE_VIBRATOR,
    E_FCT_TEST_CASE_COMM,
    E_FCT_TEST_CASE_PULSE,
    E_FCT_TEST_CASE_MOTOR,
    E_FCT_TEST_CASE_HALL,
    E_FCT_TEST_CASE_ENCODER,
    E_FCT_TEST_CASE_MOTOR_CURRENT,
    E_FCT_TEST_CASE_ADC,
    E_FCT_TEST_CASE_RTC,
    E_FCT_TEST_CASE_SLEEP_CURRENT,
    E_FCT_TEST_CASE_WORKING_CURRENT,
    E_FCT_TEST_CASE_NORMAL_CURRENT,
    E_FCT_TEST_CASE_BACKLIGHT_ON_CURRENT,
    E_FCT_TEST_CASE_PRECISION_TEST,
    E_FCT_TEST_CASE_MAX,
}TE_FCT_CASE;

typedef enum
{
    E_FCT_RESULT_NULL,
    E_FCT_RESULT_PASS,
    E_FCT_RESULT_FAIL,
}TE_FCT_RESULT;

typedef void (*TF_FCT_CASE_FUNC)(T_CHAR* case_name);

typedef struct TS_FCT_CASE_STRUCT
{
    TF_FCT_CASE_FUNC fun;
    T_CHAR case_name[MAX_SIZE_OF_ONE_LINE];
    T_CHAR print_name[32];
    TE_FCT_RESULT result; 
}TS_FCT_CASE;

static TS_FCT_CASE g_fct_case[E_FCT_TEST_CASE_MAX];
extern T_U16 g_battery_voltage;
extern TE_BOOLEAN g_battery_voltage_is_ready;
extern TE_BOOLEAN g_motor_current_is_ready;
extern T_U16 g_curr_motor_current;
extern TE_BOOLEAN g_is_already_meet_hall;
extern TE_BOOLEAN g_is_use_continue_mode;
extern TE_BOOLEAN g_is_get_mode_cmd;
extern T_U16 g_case_num;
extern void TestLcd();
void FCTTestStart();

#endif

/****************************************************************
 *******************     end of file    *************************
****************************************************************/


