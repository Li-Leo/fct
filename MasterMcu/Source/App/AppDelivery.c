/************************************************
* COPYRIGHT 2016.
*
* NAME: AppDelivery.c
*
* DESCRIPTION:
*   Delivery module in the application layer.
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/22/2016
*
* END OF FILE HEADER
************************************************/
//#include "AppAlarm.h"
#include "AppDelivery.h"
//#include "AppEventHandle.h"
//#include "AppSetInfuPara.h"
//#include "AppuiScreen.h"
#include "..\Driver\DrvRtc.h"
#include "..\Driver\DrvTimer.h"
#include "..\Driver\DrvEncoder.h"
#include "..\Kernel\CtlTimer.h"
#include "..\Kernel\CtlTime.h"
#include "..\Kernel\CtlData.h"
#include "..\Kernel\CtlDelivery.h"
#include "DrvUart.h"
#include "CtlData.h"
//#include "AppSysState.h"
#include "AppFactory.h"
#include "string.h"
#include "common.h"

//TE_BOOLEAN g_is_delivery_dose_matched = E_FALSE;
////TE_BOOLEAN g_is_delivery_stopped = E_FALSE;
//TE_BOOLEAN g_is_need_sleep_after_delivery_stopped = E_FALSE;
////if week is 0, not delivery at any time
////avalid is week, hour, minute, second
//TS_TIME_INFO g_next_deliver_time;
////TE_BOOLEAN is_next_day;
//extern T_U16 aging_deliver_counter;
//extern TE_BOOLEAN g_is_in_aging_mode;
//extern TE_SCREEN_ID last_ui;
//
////the total delivery dose today when start delivery
//T_U16 g_delivery_today_total_dose_when_start = 0;
//
//
////convert time to delivery point[0~95] 
//static T_S16 TimeToDeliveryPoint(TS_TIME_INFO current_time)
//{
//	T_U16 deli_point;
//	switch(current_time.hour)
//	{
//		case 0:
//			deli_point = 0;
//			break;
//		case 1:
//			deli_point = 4;
//			break;
//		case 2:
//			deli_point = 8;
//			break;
//		case 3:
//			deli_point = 12;
//			break;
//		case 4:
//			deli_point = 16;
//			break;
//		case 5:
//			deli_point = 20;
//			break;
//		case 6:
//			deli_point = 24;
//			break;
//		case 7:
//			deli_point = 28;
//			break;
//		case 8:
//			deli_point = 32;
//			break;
//		case 9:
//			deli_point = 36;
//			break;
//		case 10:
//			deli_point = 40;
//			break;
//		case 11:
//			deli_point = 44;
//			break;
//		case 12:
//			deli_point = 48;
//			break;
//		case 13:
//			deli_point = 52;
//			break;
//		case 14:
//			deli_point = 56;
//			break;
//		case 15:
//			deli_point = 60;
//			break;
//		case 16:
//			deli_point = 64;
//			break;
//		case 17:
//			deli_point = 68;
//			break;
//		case 18:
//			deli_point = 72;
//			break;
//		case 19:
//			deli_point = 76;
//			break;
//		case 20:
//			deli_point = 80;
//			break;
//		case 21:
//			deli_point = 84;
//			break;
//		case 22:
//			deli_point = 88;
//			break;
//		case 23:
//			deli_point = 92;
//			break;
//		default:
//			deli_point = 0;
//			break;
//	}
//	
//	if(current_time.minute < 15)
//		deli_point += 0;
//	else if(current_time.minute >= 15 && current_time.minute < 30)
//		deli_point += 1;
//	else if(current_time.minute >= 30 && current_time.minute < 45)
//		deli_point += 2;
//	else if(current_time.minute >= 45 && current_time.minute <= 59)
//		deli_point += 3;
//
//	return deli_point;
//}
//
////return the next deliver point[0~95],
////if return -1, it is no deliver point
//T_S16 GetNextDeliverPoint(T_U16 *next_dose_value, T_S16 *week)
//{
//	T_S16 current_time_point;
//	T_S16 idx;
//    T_S16 ret = -1;
//
//    *next_dose_value = 0;
//	Ds1302TimeRead();
//    *week = g_current_time.week;
//	current_time_point = TimeToDeliveryPoint(g_current_time);
//
//    //find next point after curr point
//	for(idx = current_time_point+1; idx < TOTAL_DELIVERY_TIME_POINT_NUMBER; idx++)
//	{
//		if (g_sys_para_infu_info[idx].dose != 0)
//		{
//			*next_dose_value = g_sys_para_infu_info[idx].dose;
//            ret = idx;
//			break;
//		}
//	}
//
//    //if not find, find before point
//	if(idx >= TOTAL_DELIVERY_TIME_POINT_NUMBER)
//	{
//    	for(idx = 0; idx <= current_time_point; idx++)
//    	{
//    		if (g_sys_para_infu_info[idx].dose != 0)
//    		{
//    			*next_dose_value = g_sys_para_infu_info[idx].dose;
//    			(*week)++;
//                if(*week>7)
//                {
//                    *week = 1;
//                }
//                ret = idx;
//    			break;
//    		}
//    	}
//	}
//    
//	return ret;
//}

T_S32 ConvertTimeToSecond(TS_TIME_INFO *time_point)
{
	T_S32 second;
	second = (time_point->hour * 60 + time_point->minute) * 60 + time_point->second;
	return second;
}

//	static T_U32 GetTheRateOfDoseToEncoderNumber(TE_DOSE_THICKNESS thickness)
//	{
//		T_U32 dose_to_circle;
//		switch(thickness)
//		{
//			case E_DOSE_THICKNESS_100:
//				dose_to_circle = DOSE_TO_CIRCLE_THICKNESS_100;
//				break;
//			case E_DOSE_THICKNESS_200:
//				dose_to_circle = DOSE_TO_CIRCLE_THICKNESS_200;
//				break;
//			case E_DOSE_THICKNESS_500:
//				dose_to_circle = DOSE_TO_CIRCLE_THICKNESS_500;
//				break;
//			default:
//				dose_to_circle = DOSE_TO_CIRCLE_THICKNESS_200;
//				break;
//		}
//		return dose_to_circle;
//	}

/***********************************************
* Description:
*   do common thing while auto wake up.
* Argument:
*   param:
*
* Return:
*
************************************************/
//	static void AppTimerHanldeAutoWakeUp(void)
//	{
//	    return;
//	}

//	void SetNextWakeTime(TS_TIME_INFO next_time)
//	{
//		T_U32 time_period, curr_time_to_sec, next_time_to_sec;
//		T_U32 day_second_max = ((T_U32)MAX_MINUTE_IN_ONE_DAY * 60);
//		Ds1302TimeRead();
//		curr_time_to_sec = ConvertTimeToSecond(g_current_time);
//		next_time_to_sec = ConvertTimeToSecond(next_time);
//		
//		/* the next delivery point is at the same day */
//		if (g_current_time.date == next_time.date)
//		{
//			time_period = next_time_to_sec - curr_time_to_sec;
//		}
//		/* the next delivery point is at the next day */
//		else if (g_current_time.date < next_time.date)
//		{
//			time_period = day_second_max - curr_time_to_sec + next_time_to_sec;
//			//SaveAllDeliParaIntoOperRec();
//		}
//	
//		g_auto_wakeup_timer_ms = time_period * 1000;
//		//g_auto_wakeup_timer_ms = 2000;
//	}

//	TS_TIME_INFO GetNextWakeTime()
//	{
//		Ds1302TimeRead(); 
//		TS_TIME_INFO next_wakeup_time = g_current_time;
//		if(g_is_in_aging_mode == E_FALSE)
//		{
//			T_S16 next_deli_time_point;
//	        TE_BOOLEAN is_next_day;
//	        
//			//current_time_point = TimeToDeliveryPoint(g_current_time);
//			next_deli_time_point = GetNextDeliverPoint(&g_deliver_dose_value, &is_next_day);
//			if(next_deli_time_point == -1)
//			{
//				next_wakeup_time.hour = 25;
//				return next_wakeup_time;
//			}
//			
//			/************* Ceremonial codes blow ********************/
//			next_wakeup_time.second = (60 - ADVANCE_WAKE_UP_TIME);
//			g_next_deliver_time.second = 0;
//	
//			if (next_deli_time_point > 0)
//				next_wakeup_time.hour = (next_deli_time_point - 1) / 4;
//			else
//				next_wakeup_time.hour = 23;
//			g_next_deliver_time.hour = next_deli_time_point / 4;
//	
//			switch(next_deli_time_point % 4)
//			{
//				case 0:
//					next_wakeup_time.minute = 59;
//					g_next_deliver_time.minute = 0;
//					break;
//				case 1:
//					next_wakeup_time.minute = 14;
//					g_next_deliver_time.minute = 15;
//					break;
//				case 2:
//					next_wakeup_time.minute = 29;
//					g_next_deliver_time.minute = 30;
//					break;
//				case 3:
//					next_wakeup_time.minute = 44;
//					g_next_deliver_time.minute = 45;
//					break;
//				default:
//					next_wakeup_time.minute = 59;
//					g_next_deliver_time.minute = 0;
//					break;			
//			}
//			if (is_next_day == E_TRUE)
//				next_wakeup_time.date++;
//		}
//		/*************** Ceremonial codes finish ********************/
//		else
//		{
//			if(aging_deliver_counter == 1)
//				next_wakeup_time.second += 25;
//			else
//				next_wakeup_time.second += 45;
//		  	//g_next_deliver_time.second += 20;
//			if(next_wakeup_time.second > 59)
//			{
//				next_wakeup_time.second %= 60;
//				next_wakeup_time.minute ++;
//				if (next_wakeup_time.minute > 59)
//				{
//					next_wakeup_time.minute %= 60;
//					next_wakeup_time.hour ++;
//					if(next_wakeup_time.hour > 23)
//					{
//						next_wakeup_time.hour %= 24;
//						next_wakeup_time.date ++;
//					}
//				}		
//			}
//		}
//		/************* Test codes blow ********************
//		next_wakeup_time = g_current_time;
//		next_wakeup_time.second += 13;
//	  	g_next_deliver_time.second += 20;
//		if(g_next_deliver_time.second > 59)
//		{
//			g_next_deliver_time.second = 0;
//			if (g_next_deliver_time.minute < 16)
//				g_next_deliver_time.minute ++;
//		}
//		*************** Test codes finish ********************/
//	
//		return next_wakeup_time;
//	}

//return the next delivery seconds from now, if return negative, the next delivery point is passed
//T_S32 GetNextDeliverySecsFromNow()
//{
//    T_S32 diff_secs;
//    
//    if(g_next_deliver_time.week == 0)
//    {
//        diff_secs = MAX_T_S32;
//    }
//    else
//    {
//        Ds1302TimeRead(); 
//        diff_secs = DiffDateTimeInOneDay(&g_next_deliver_time, &g_current_time);
//
//        if(diff_secs<0)
//        {
//            M_CommonPrint("delivery point passed!\n");
//            M_CommonPrint("next delivery time: week:%ld, %02lu:%02lu:%02lu\n", 
//                g_next_deliver_time.week-1,
//                g_next_deliver_time.hour, g_next_deliver_time.minute, g_next_deliver_time.second);
//            M_CommonPrint("curr time: week:%ld, %02lu:%02lu:%02lu\n", 
//                g_current_time.week-1,
//                g_current_time.hour, g_current_time.minute, g_current_time.second);
//
//        }
//    }
//    
//	return diff_secs;
//}
//
////when encoder finish, record the histroy
//void DeliveryOnEncoderStop(T_U32 param)
//{
//	//T_U32 temp, rate;	
//    TS_RECORD  day_total_rec;
//    T_U16 today_deli_total;
//    T_U16 dose_actual;
//    
//	dose_actual = EncoderToDose(g_sys_para_thickness, g_encoder_counter);
//	SaveRec(E_RECORD_INFUSION, dose_actual, 0);
//    M_CommonPrint("delivery stop: dose[%u*0.1ug]\n", dose_actual);
//	if(g_record_counter.dt_rec_counter > 0)
//	{               
//        day_total_rec.rec_id = RecLastIndex(E_RECORD_DAYS_TOTAL);
//        day_total_rec.rec_type = E_RECORD_DAYS_TOTAL;
//        LoadRec(&day_total_rec);
//	}
//	else
//	{
//        memset(&day_total_rec, 0, sizeof(day_total_rec));
//	}
//	
//	//LoadSysPara(E_SYSTEM_PARA_DAY_TOTAL, 0);
//    today_deli_total = day_total_rec.rec_para;
//	Ds1302TimeRead();
//    if(day_total_rec.rec_time.year == g_current_time.year &&
//       day_total_rec.rec_time.month == g_current_time.month&&
//       day_total_rec.rec_time.date == g_current_time.date)
//	{
//        //if already record, change the reocrd
//		today_deli_total += dose_actual;
//		//g_sys_para_actual_day_total=today_deli_total;
//		//SaveSysPara(E_SYSTEM_PARA_DAY_TOTAL, 0);
//		day_total_rec.rec_time = g_current_time;
//		day_total_rec.rec_para = today_deli_total;
//		g_record_counter.dt_rec_counter--;
// 		SaveRecEx(&day_total_rec);
//
//	}
//	else
//	{
//        //g_sys_para_actual_day_total = dose_actual;
//		//SaveSysPara(E_SYSTEM_PARA_DAY_TOTAL, 0);
//		SaveRec(E_RECORD_DAYS_TOTAL, dose_actual, 0);
//        
//	}
////		if(g_dose_number>dose_actual)
////		{
////		    g_dose_number -= dose_actual;
////		}
//	//SaveSysPara(E_SYSTEM_PARA_DOSE_LEFT, 0);
//	if (RemainDose() < GetInfuDose())
//	{
//
//		//last_ui = GetCurrentUI();
//		//AlarmSet(E_ALARM_REMIND_DOSE_LEFT_LESS_THAN_INFU_VOL_ID);
//		//AlarmShow(E_ALARM_REMIND_DOSE_LEFT_LESS_THAN_INFU_VOL_ID);
//		g_remind_alarm_status = E_ALARM_REMIND_DOSE_LEFT_LESS_THAN_INFU_VOL_ID;
//		//DisplayUI(E_SCREEN_SHOW_REMIND_INFO);
//		SaveRec(E_RECORD_OPERATION, 83, RemainDose());
//	}
//
////	    if(RemainDose()== 0)
////		{
////	        SaveRec(E_RECORD_STOP_DELI, 18, 0);
////		}
//}
//
//void ExecDelivery(TE_DOSE_THICKNESS thickness, T_U16 dose)
//{
//	T_U32 expect_circle;
//    TE_ALARM_ID alarm_id = 0;
//    //AlarmShowFirstOfHighestPriority();
//    TS_RECORD tmp_day_total_rec;
//    T_S16 stop_cause = 0;
//
//    tmp_day_total_rec.rec_type = E_RECORD_DAYS_TOTAL;
//    tmp_day_total_rec.rec_id = RecLastIndex(E_RECORD_DAYS_TOTAL);
//	LoadRec(&tmp_day_total_rec);
//    Ds1302TimeRead();
//	if(tmp_day_total_rec.rec_time.year == g_current_time.year &&
//       tmp_day_total_rec.rec_time.month == g_current_time.month&&
//       tmp_day_total_rec.rec_time.date == g_current_time.date)
//	{
//        g_delivery_today_total_dose_when_start = tmp_day_total_rec.rec_para;
//	}
//    else
//    {
//        g_delivery_today_total_dose_when_start = 0;
//    }
//        
//	assert(alarm_id <= E_ALARM_COUNT);
//    if(alarm_id >= E_ALARM_KEY_STUCK_ID && alarm_id<E_ALARM_COUNT)
//    {
//        stop_cause = -1;
//    }
//    else if(alarm_id == E_ALARM_BATTERY_EXHAUSTION_ID)
//    {
//        stop_cause = 19;
//    }
//	else if((alarm_id == E_ALARM_OCCLUSION_ID))
//    {
//        stop_cause = 16;
//    } 
//	else if(g_deliver_switch == E_FALSE && g_is_in_aging_mode==E_FALSE)
//	{
//        stop_cause = 17;
//	}
//	else if(RemainDose()== 0)
//	{
//        stop_cause = 18;
//	}
//    else if(g_delivery_today_total_dose_when_start >= GetTotalDose() && g_is_in_aging_mode==E_FALSE)
//    {
//        stop_cause = 20;
//    }
//
//    //check if can delivery
//    if(stop_cause != 0)
//    {
//        if(stop_cause != -1)
//        {
//            //add a stop infusion record
//            SaveRec(E_RECORD_STOP_DELI, stop_cause, 0);
//        }
//        M_CommonPrint("delivery can not start, cause:[%d]\n", stop_cause);
//        SetNextDeliTimePoint();
//        return;
//    }
//
//	//GetDeliveredDose();
//	expect_circle = DoseToEncoder(thickness, dose);
//    if(dose == 10)
//    {
//        expect_circle*=ONE_DOSE_FIXED_FACTOR;
//    }
//	//g_encoder_expect_counter = expect_circle;
//	//bonus_period = expect_circle/CIRCLES_PER_SECOND_ENCODER;
//
//	//Print(g_motor_position, "position", 0);
//    M_CommonPrint("delivery start: expect dose[%u*0.1ug], thickness[%uug/ml]\n",
//        dose, GetThicknessValue(thickness));
//	CtlDeliveryStart(expect_circle, dose, thickness);
//    //CtlTimerSet(E_TIMER_CHECK_BONUS, bonus_period*1000);
//	
//	//save the delivery start state 
//	g_deliver_start_motor_pos = g_motor_position;
//	SaveSysPara(E_SYSTEM_PARA_DELI_START_MOTOR_POS, 0);
//    g_deliver_is_running = 1;
//    SaveSysPara(E_SYSTEM_PARA_DELI_IS_RUNNING, 0);
//    
//    //CtlEventBindHandle(E_EVENT_ENCODER_STOP, DeliveryOnEncoderStop);
//}
//
//T_U16 GetDeliveredDose()
//{
//	//if(!IsUptoDose())
//		//g_deliver_dose_value += (g_deli_expected - g_deli_actually);
//	return g_deliver_dose_value;
//}
//
//TE_BOOLEAN IsUptoDose()
//{
//	/*if(g_deli_expected > g_deli_actually)
//	{
//		g_is_delivery_dose_matched = E_FALSE;
//		if ((g_deli_expected - g_deli_actually) > OCCLUSION_BONUS)
//			CtlEventPost(E_EVENT_ALARM_OCCLUSION, 0);
//	}
//	else
//		g_is_delivery_dose_matched = E_TRUE;*/
//	return g_is_delivery_dose_matched;
//}
//
//void TermDelivery()
//{
//	//clear the delivery start state
//    g_deliver_is_running = 0;
//	SaveSysPara(E_SYSTEM_PARA_DELI_IS_RUNNING, 0);
//    
//    //CtlTimerClear(E_TIMER_CHECK_BONUS);
//    CtlDeliveryStop();
//	
//    //g_is_delivery_stopped = E_TRUE;
//    SetNextDeliTimePoint();
//	
//	DeliveryOnEncoderStop(0);
//	if(g_is_in_aging_mode == E_TRUE)
//	{
//		aging_deliver_counter++;
//		M_CommonPrint("Aging deliver counter: %u\n", aging_deliver_counter);
//	}
//
//    if(g_is_need_sleep_after_delivery_stopped)
//    {
//        TE_ALARM_ID alarm_id = 1;
//        
//        g_is_need_sleep_after_delivery_stopped = E_FALSE;
//        if(alarm_id == E_ALARM_COUNT)
//        {            
//            //if no alarm, auto sleep
//            CtlTimerBindHandle(E_TIMER_AUTO_SLEEP, AppTimerHanldeAutoSleep);
//		    CtlTimerSet(E_TIMER_AUTO_SLEEP, AUTO_SLEEP_TIME_MS);
//        }
//    }
//
//}
//
//void SetNextDeliTimePoint()
//{
//	if(g_is_in_aging_mode == E_FALSE)
//	{
//        T_S16 deli_week;
//		T_S16 next_deli_point = GetNextDeliverPoint(&g_deliver_dose_value, &deli_week);
//
//        if(next_deli_point != -1)
//        {
//    		g_next_deliver_time.second = 0;
//    		g_next_deliver_time.hour = next_deli_point / 4;
//
//    		switch(next_deli_point % 4)
//    		{
//    			case 0:
//    				g_next_deliver_time.minute = 0;
//    				break;
//    			case 1:
//    				g_next_deliver_time.minute = 15;
//    				break;
//    			case 2:
//    				g_next_deliver_time.minute = 30;
//    				break;
//    			case 3:
//    				g_next_deliver_time.minute = 45;
//    				break;
//    			default:
//    				g_next_deliver_time.minute = 0;
//    				break;			
//    		}
//
//            g_next_deliver_time.week = deli_week;
//        }
//        else
//        {
//            memset(&g_next_deliver_time, 0, sizeof(g_next_deliver_time));
//        }
//	}
//	else
//	{
//		Ds1302TimeRead();
//		g_next_deliver_time = g_current_time;
//		g_deliver_dose_value = AGING_MODE_DELIVER_DOSE;
//	  	g_next_deliver_time.minute ++;
//		if(g_next_deliver_time.minute > 59)
//		{
//			g_next_deliver_time.minute %= 60;
//			g_next_deliver_time.hour ++;
//			if (g_next_deliver_time.hour > 23)
//			{
//				g_next_deliver_time.hour %= 24;
//				g_next_deliver_time.week++;
//                if(g_next_deliver_time.week>7)
//                {
//                   g_next_deliver_time.week = 1; 
//                }
//			}		
//		}
//		
//	}
//
//    M_CommonPrint("set next delivery time: week:%ld, %02lu:%02lu:%02lu\n", 
//        g_next_deliver_time.week-1, 
//        g_next_deliver_time.hour, g_next_deliver_time.minute, g_next_deliver_time.second);
//}

//calc the diff seconds by the week, hour, minute, seconds
T_S32 DiffDateTimeInOneDay(TS_TIME_INFO *new_time, TS_TIME_INFO *old_time)
{
    T_S32 new_secs, old_secs, diff_secs;
    T_SINT week_diff;

    new_secs = ConvertTimeToSecond(new_time);
    old_secs = ConvertTimeToSecond(old_time);

    week_diff = (T_SINT)new_time->week - (T_SINT)old_time->week;
    if(week_diff<0)
    {
        week_diff += 7;
    }

    //pass one day, add one day
    if(week_diff == 1)
    {
        diff_secs = new_secs + (T_S32)MAX_MINUTE_IN_ONE_DAY * 60 - old_secs;
    }
    else if(week_diff==0)//all in one day
    {
        diff_secs = new_secs - old_secs;
    }
    else if(week_diff == 6)//less one day
    {
        diff_secs = new_secs - (T_S32)MAX_MINUTE_IN_ONE_DAY * 60 - old_secs;
    }
    else
    {
    	//do not know less or pass, set -1
        diff_secs = -1;
        M_Assert(0);
    }    

    return diff_secs;
}

//reduce the dest time by secs, only change week, hour, minute, seconds
void ReduceSecsInOneDay(TS_TIME_INFO *dest, T_S32 secs)
{
    T_S32 dest_secs;

    dest_secs = ConvertTimeToSecond(dest);
    dest_secs -= secs;
    if(dest_secs<0)
    {
        dest_secs += MAX_SECS_IN_ONE_DAY;
        dest->week--;
        if(dest->week<WEEK_START)
        {
            dest->week = WEEK_END;
        }
    }
    ConvertSecsToTimeInOneDay(dest, dest_secs);   
}
void ConvertSecsToTimeInOneDay(TS_TIME_INFO *dest, T_S32 secs)
{
    M_Assert(secs<=MAX_SECS_IN_ONE_DAY);
    dest->hour = (secs/3600)%24;
    secs = secs%3600;
    dest->minute = secs/60;
    dest->second = secs%60;

}

