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
	E_STRING_HINT_CAL_TIME,				//��У׼���ڼ�ʱ��
	E_STRING_HINT_CHANGE_BATT,			//�뻻���
	E_STRING_ASK_EXECUTE,				//ִ����
	E_STRING_INFUSION,					//��ע
	E_STRING_ON,						//��
	E_STRING_OFF,						//��
	E_STRING_ASK_LOCATION_EXECUTE,		//��λִ����
	E_STRING_HINT_NO_CONNECT_TO_BODY,	//�����������壡
	E_STRING_MANUAL,					//�ֶ�
	E_STRING_MILLILITER,				//����
	E_STRING_CAL_TIME,					//Уʱ
	E_STRING_HOUR,						//ʱ
	E_STRING_MINUTE,					//��
	E_STRING_CAL_DATE,					//У����
	E_STRING_YEAR,						//��
	E_STRING_MONTH,						//��
	E_STRING_DAY,						//��
	E_STRING_SET_INFUSION,				//��ע����
	E_STRING_PRESET_PARAMETER,			//Ԥ�����
	E_STRING_PRESET_PARAMETER_INTRO1,	//ÿ1.5h
	E_STRING_PRESET_PARAMETER_INTRO2,	//��ע10ug
	E_STRING_PRESET_PARAMETER_COMFIRM,	//����Ԥ������ɹ�
	E_STRING_MANUAL_SET_PARAMETER,		//�ֶ��趨
	E_STRING_PREVIOUS,					//�ϴ�
	E_STRING_THIS_TIME,					//����
	E_STRING_DAYS_TOTAL_INFUSION,		//����ע����
	E_STRING_LOCK,						//��  ��
	E_STRING_WRONG_PRESS,				//�������� !
	E_STRING_SET_PARAMETER,				//�����趨
	E_STRING_THICKNESS,					//Ũ��
	E_STRING_PER_MILLILITER,			//ÿ����
	E_STRING_MAX_INFUSION_SET,			//�������
	E_STRING_AT_EVERY_TURN,				//ÿ��
	E_STRING_MICROGRAMME,				//΢��
	E_STRING_INFUSION_RECORD,			//��ע��¼
	E_STRING_DAYS_TOTAL_INFU_RECORD,	//������
	E_STRING_STOP_INFUSION_RECORD,		//ͣ���¼
	E_STRING_OPERATION_RECORD,			//������¼
	E_STRING_LOCATION_CAL_MENU,			//��λ����У׼
	E_STRING_LOCATION_CAL_REMIND_LINE1,	//��+-���Ƶ��
	E_STRING_LOCATION_CAL_REMIND_LINE2,	//����ȷ��
	E_STRING_LOCATION_CAL_RESULT_LINE1,	//��λ����У׼���
	E_STRING_LOCATION_CAL_RESULT_LINE2,	//У׼����:
	E_STRING_OCCLUSION_CAL_MENU,		//����У׼
	E_STRING_OCCLUSION_CAL_REMIND_LINE1,//ѹ���ﵽ��׼
	E_STRING_OCCLUSION_CAL_REMIND_LINE2,//����ȷ��
	E_STRING_OCCLUSION_CAL_RESULT_LINE1,//����У׼���
	E_STRING_OCCLUSION_CAL_RESULT_LINE2,//��������:
	E_STRING_RESTORE_TO_FACTORY_MENU,	//�ָ���������
	E_STRING_RESTORE_TO_FACTORY_COMFIRM,//�ָ��������óɹ�
	E_STRING_SET_AGING_MODE_MENU,		//�����ϻ�ģʽ
	E_STRING_SET_AGING_MODE_COMFIRM,	//�ϻ�ģʽ�������
	E_STRING_SHOW_DEVICE_INFO_MENU,		//��ʾ�豸��Ϣ
	E_STRING_SHOW_DEVICE_INFO_LIST_L1,	//Ӳ���汾��
	E_STRING_SHOW_DEVICE_INFO_LIST_L2,	//����汾��
	E_STRING_SHOW_DEVICE_INFO_LIST_L3,	//��Ʒ���к�
	E_STRING_EXIT_FCT,					//�˳�����ģʽ
	E_STRING_DOSE_LEFT,					//ʣ��ҩ��
	E_STRING_NOT_ENOUGH_TO_DELIVER,		//������ע
	E_STRING_ALARM_DAY_TOTAL_OVERRUN,	//ֵ����
	E_STRING_ALARM_DAY_TOTAL_OUT_RANGE,	//ֵ����Χ
	E_STRING_ALARM_BATT_LOW,			//��ص�����
	E_STRING_ALARM_DOSE_EMPTY,			//!!ҩ�Ѿ�
	E_STRING_ALARM_CHANGE_BATT,			//!!�뻻���
	E_STRING_ALARM_DOSE_LOW,			//ʣ��ҩ����
	E_STRING_ALARM_DELIVER_FINISHED,	//Ԥ����ע���
	E_STRING_ALARM_OCCLUSION,			//!!����
	E_STRING_ALARM_STOP_DELIVER,		//!!ͣ��
	E_STRING_ALARM_BATT_EMPTY,			//!������
	E_STRING_ALARM_SYSTEM_ERROR1,		//ϵͳ���� 01
	E_STRING_ALARM_SYSTEM_ERROR2,		//ϵͳ���� 02
	E_STRING_ALARM_SYSTEM_ERROR3,		//ϵͳ���� 03
	E_STRING_ALARM_SYSTEM_ERROR4,		//ϵͳ���� 04
	E_STRING_ALARM_SYSTEM_ERROR5,		//ϵͳ���� 05
	E_STRING_ALARM_SYSTEM_ERROR6,		//ϵͳ���� 06
	E_STRING_ALARM_SYSTEM_ERROR7,		//ϵͳ���� 07
	E_STRING_ALARM_SYSTEM_ERROR8,		//ϵͳ���� 08
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




/*�����ϻ�һ������*/
extern void DrawLineColumn(T_U8 col, T_U8 row_start, T_U8 row_end);		

/*�����ϻ�һ������*/
extern void DrawLineRow(T_U8 row, T_U8 col_start, T_U8 col_end);		

/*�����ϻ�һ������*/
extern void DrawRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end);	

/*�����ϻ�һ�����β����*/
extern void FillRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end);

/*�ڵ�һ����ʾ����*/
extern void DrawTextAtLineOne(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*�ڵڶ�����ʾ����*/
extern void DrawTextAtLineTwo(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*���������м���ʾ����*/
extern void DrawTextAtMiddleLine(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*��grid_xΪ����ڵ�һ����ʾ���ֲ���˸*/
extern void DrawTextAtLineOneWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*��grid_xΪ����ڵڶ�����ʾ���ֲ���˸*/
extern void DrawTextAtLineTwoWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*��grid_xΪ������м�����ʾ���ֲ���˸*/
extern void DrawTextAtMiddleLineWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type);

/*��grid_xΪ����ڵڶ�����ʾ��˸��ͼ��*/
extern void DrawFlashIconAtLineTwo(T_U8 grid_x, T_U8 const* buffer);

/*�����Ļ�ڶ�����ʾ*/
void ClearBottomLine();

//clear all display
void DisplayClearAll();
//clear the display, x_start:0~127, y_start:0~31
void DisplayClear(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height);


void DisplaySetFontLib(const TS_FONT_INFO* font_lib);
//get the string's width
T_S16 DisplayGetXSize(const T_CHAR* str);
/*��ʾ����*/
void DisplayTextAtPos(T_U8 x_start, T_U8 y_start, const T_CHAR* str);
//display at center of x
void DisplayTextAtXCenter(T_U8 y_start, const T_CHAR* str);

void DisplayInit();
#endif /* DRVDISPLAY_H */

