/************************************************
* COPYRIGHT 2016 
*
* NAME: ctl_display_main.h
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
 * 
* END OF FILE HEADER
************************************************/


#ifndef DRVDISPLAY_H
#define DRVDISPLAY_H

#include "..\TypeDefine.h"
#include "fonts.h"

#define PAGE_NUMBER_MAX	3
#define PAGE_NUMBER_MIN	0
#define ROW_NUMBER_MAX	31
#define ROW_NUMBER_MIN	0
#define COL_NUMBER_MAX	127
#define COL_NUMBER_MIN	0

#define CHINESE_WORD_LENGTH	32
#define ASCII_CHAR_LENGTH	16
#define ICON_ARRAY_LENGTH	48
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define TOP_LINE_Y 0 
#define MIDDLE_LINE_Y (DISPLAY_HEIGHT/4)
#define BOTTOM_LINE_Y (DISPLAY_HEIGHT/2)


#define M_DisplaySetFirstLineDirty() DisplaySetDirty(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT/2)
#define M_DisplaySetSecondLineDirty() DisplaySetDirty(0, DISPLAY_HEIGHT/2, DISPLAY_WIDTH, DISPLAY_HEIGHT/2)
#define M_DisplaySetAllDirty() DisplaySetDirty(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT)

#define M_DisplayPrint if(IsEnableOutput(E_MODULE_DISPLAY))printf

typedef enum
{
	E_PAGE_OFFSET_TYPE_TOP,		//The offset rows should be set blank.
	E_PAGE_OFFSET_TYPE_BOTTOM,	//The offset rows should be filled.
	E_PAGE_OFFSET_TYPE_MAX,
} TE_PAGE_OFFSET_TYPE;

typedef enum
{
	E_DRAW_LCD_TYPE_CHINESE_WORD,	//Chinese word, 16x16.
	E_DRAW_LCD_TYPE_ASCII_CHAR,		//Ascii char, 8x16.
	E_DRAW_LCD_TYPE_ICON_ARRAY,		//Icon of dose and battery, 24x16.
	E_DRAW_LCD_TYPE_MAX,
} TE_DRAW_LCD_TYPE;

typedef enum
{
	E_TEXT_POSITION_LINE_TOP,		//Draw text at line top, page 1&2.
	E_TEXT_POSITION_LINE_MIDDLE,	//Draw text at line middle, page 2&3.
	E_TEXT_POSITION_LINE_BOTTOM,	//Draw text at line bottom, page 3&4.
	E_TEXT_POSITION_MAX,
} TE_TEXT_POSITION;

typedef enum
{
	E_STRING_HINT_CAL_TIME,				//请校准日期及时间
	E_STRING_HINT_CHANGE_BATT,			//请换电池
	E_STRING_ASK_EXECUTE,				//执行吗？
	E_STRING_INFUSION,					//输注
	E_STRING_ON,						//开
	E_STRING_OFF,						//关
	E_STRING_ASK_LOCATION_EXECUTE,		//定位执行吗？
	E_STRING_HINT_NO_CONNECT_TO_BODY,	//请勿连接人体！
	E_STRING_MANUAL,					//手动
	E_STRING_MILLILITER,				//毫升
	E_STRING_CAL_TIME,					//校时
	E_STRING_HOUR,						//时
	E_STRING_MINUTE,					//分
	E_STRING_CAL_DATE,					//校日期
	E_STRING_YEAR,						//年
	E_STRING_MONTH,						//月
	E_STRING_DAY,						//日
	E_STRING_SET_INFUSION,				//输注设置
	E_STRING_PRESET_PARAMETER,			//预设参数
	E_STRING_PRESET_PARAMETER_INTRO1,	//每1.5h
	E_STRING_PRESET_PARAMETER_INTRO2,	//输注10ug
	E_STRING_PRESET_PARAMETER_COMFIRM,	//设置预设参数成功
	E_STRING_MANUAL_SET_PARAMETER,		//手动设定
	E_STRING_PREVIOUS,					//上次
	E_STRING_THIS_TIME,					//本次
	E_STRING_DAYS_TOTAL_INFUSION,		//日输注总量
	E_STRING_LOCK,						//锁  定
	E_STRING_WRONG_PRESS,				//您按错了 !
	E_STRING_SET_PARAMETER,				//参数设定
	E_STRING_THICKNESS,					//浓度
	E_STRING_PER_MILLILITER,			//每毫升
	E_STRING_MAX_INFUSION_SET,			//最大设置
	E_STRING_AT_EVERY_TURN,				//每次
	E_STRING_MICROGRAMME,				//微克
	E_STRING_INFUSION_RECORD,			//输注纪录
	E_STRING_DAYS_TOTAL_INFU_RECORD,	//日总量
	E_STRING_STOP_INFUSION_RECORD,		//停输纪录
	E_STRING_OPERATION_RECORD,			//操作纪录
	E_STRING_LOCATION_CAL_MENU,			//定位开关校准
	E_STRING_LOCATION_CAL_REMIND_LINE1,	//按+-控制电机
	E_STRING_LOCATION_CAL_REMIND_LINE2,	//按主确认
	E_STRING_LOCATION_CAL_RESULT_LINE1,	//定位开关校准完成
	E_STRING_LOCATION_CAL_RESULT_LINE2,	//校准参数:
	E_STRING_OCCLUSION_CAL_MENU,		//阻塞校准
	E_STRING_OCCLUSION_CAL_REMIND_LINE1,//压力达到标准
	E_STRING_OCCLUSION_CAL_REMIND_LINE2,//按主确认
	E_STRING_OCCLUSION_CAL_RESULT_LINE1,//阻塞校准完成
	E_STRING_OCCLUSION_CAL_RESULT_LINE2,//阻塞电流:
	E_STRING_RESTORE_TO_FACTORY_MENU,	//恢复出厂设置
	E_STRING_RESTORE_TO_FACTORY_COMFIRM,//恢复出厂设置成功
	E_STRING_SET_AGING_MODE_MENU,		//设置老化模式
	E_STRING_SET_AGING_MODE_COMFIRM,	//老化模式设置完成
	E_STRING_SHOW_DEVICE_INFO_MENU,		//显示设备信息
	E_STRING_SHOW_DEVICE_INFO_LIST_L1,	//硬件版本号
	E_STRING_SHOW_DEVICE_INFO_LIST_L2,	//软件版本号
	E_STRING_SHOW_DEVICE_INFO_LIST_L3,	//产品序列号
	E_STRING_EXIT_FCT,					//退出工厂模式
	E_STRING_DOSE_LEFT,					//剩余药量
	E_STRING_NOT_ENOUGH_TO_DELIVER,		//不够输注
	E_STRING_ALARM_DAY_TOTAL_OVERRUN,	//值超限
	E_STRING_ALARM_DAY_TOTAL_OUT_RANGE,	//值超范围
	E_STRING_ALARM_BATT_LOW,			//电池电量低
	E_STRING_ALARM_DOSE_EMPTY,			//!!药已尽
	E_STRING_ALARM_CHANGE_BATT,			//!!请换电池
	E_STRING_ALARM_DOSE_LOW,			//剩余药量低
	E_STRING_ALARM_DELIVER_FINISHED,	//预设输注完成
	E_STRING_ALARM_OCCLUSION,			//!!阻塞
	E_STRING_ALARM_STOP_DELIVER,		//!!停输
	E_STRING_ALARM_BATT_EMPTY,			//!电量低
	E_STRING_ALARM_SYSTEM_ERROR1,		//系统故障 01
	E_STRING_ALARM_SYSTEM_ERROR2,		//系统故障 02
	E_STRING_ALARM_SYSTEM_ERROR3,		//系统故障 03
	E_STRING_ALARM_SYSTEM_ERROR4,		//系统故障 04
	E_STRING_ALARM_SYSTEM_ERROR5,		//系统故障 05
	E_STRING_ALARM_SYSTEM_ERROR6,		//系统故障 06
	E_STRING_ALARM_SYSTEM_ERROR7,		//系统故障 07
	E_STRING_ALARM_SYSTEM_ERROR8,		//系统故障 08
	E_STRING_MAX
} TE_STRING_ID;
typedef struct
{
    T_SINT x;
    T_SINT y;
    T_SINT x_size;
    T_SINT y_size;
}TS_RECT;


//flush the display cache to lcd, x_start:0~127, y_start:0~31
void DisplayFlush(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height);

//flush the dirty area to lcd
void DisplayFlushDirty();
//is exist dirty area
TE_BOOLEAN DisplayIsDirty();
//set dirty area
void DisplaySetDirty(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height);




/*在屏上画一条竖线*/
extern void DrawLineColumn(T_U8 col, T_U8 row_start, T_U8 row_end);		

/*在屏上画一条竖线*/
extern void DrawLineRow(T_U8 row, T_U8 col_start, T_U8 col_end);		

/*在屏上画一个矩形*/
extern void DrawRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end);	

/*在屏上画一个矩形并填充*/
extern void FillRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end);

/*在第一行显示文字*/
extern void DrawTextAtLineOne(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*在第二行显示文字*/
extern void DrawTextAtLineTwo(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*跨行在屏中间显示文字*/
extern void DrawTextAtMiddleLine(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*以grid_x为起点在第一行显示文字并闪烁*/
extern void DrawTextAtLineOneWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*以grid_x为起点在第二行显示文字并闪烁*/
extern void DrawTextAtLineTwoWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*以grid_x为起点在中间行显示文字并闪烁*/
extern void DrawTextAtMiddleLineWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*以grid_x为起点在第二行显示闪烁的图标*/
extern void DrawFlashIconAtLineTwo(T_U8 grid_x, T_U8 const* buffer);

/*清除屏幕第二行显示*/
void ClearBottomLine();

//clear all display
void DisplayClearAll();
//clear the display, x_start:0~127, y_start:0~31
void DisplayClear(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height);


void DisplaySetFontLib(const TS_FONT_INFO* font_lib);
//get the string's width
T_S16 DisplayGetXSize(const T_CHAR* str);
/*显示文字*/
void DisplayTextAtPos(T_U8 x_start, T_U8 y_start, const T_CHAR* str);
//display at center of x
void DisplayTextAtXCenter(T_U8 y_start, const T_CHAR* str);

void DisplayInit();
#endif /* DRVDISPLAY_H */

