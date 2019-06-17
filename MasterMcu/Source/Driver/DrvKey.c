//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************
#include "DrvKey.h"
	
//************全局变量**************************************************
T_U16 g_key_hold_time[E_KEY_MAX]={0};
static T_U8 g_key_pre_status[E_KEY_MAX];
static T_U8 g_key_status[E_KEY_MAX];
static TE_BOOLEAN g_key_status_changed[E_KEY_MAX]={E_FALSE};
static T_U8 g_key_count[E_KEY_MAX]={0};

/****************************************************************
* COPYRIGHT 2016 
*
* Description:
* This function get the key value
*
* PROTOTYPE:	
* static void DrvGetKeyValue(void)
*
* ARG:	void 
*	
* RETURN:  N/A
*
* END OF FUNCTION HEADER
*****************************************************************/
static void DrvKeyGetValue(void)
{
  g_key_status[E_KEY_MAIN] = (SCAN_KEY_MAIN_STATUS & BIT1) ? E_FALSE : E_TRUE;      
  g_key_status[E_KEY_PLUS] = (SCAN_KEY_PLUS_STATUS & BIT6) ? E_FALSE : E_TRUE;  
  g_key_status[E_KEY_MINUS] = (SCAN_KEY_MINUS_STATUS & BIT7) ? E_FALSE : E_TRUE;    
}

/****************************************************************
* COPYRIGHT 2016 
*
* Description:
* This function initialize the key value
*
* PROTOTYPE:	
* void DrvKeyInitial(void)
*
* ARG:	N/A 
*	
* RETURN:  N/A
*
* END OF FUNCTION HEADER
*****************************************************************/
void DrvKeyInitial(void)
{
  T_U8 i;
  for(i=0;i<E_KEY_MAX;i++)
  {
	g_key_hold_time[i]=0;
	g_key_pre_status[i] = 0;
	g_key_status[i] = 0;
	g_key_status_changed[i] = E_FALSE;
	g_key_count[i] = 0;
  }
}

/****************************************************************
* COPYRIGHT 2016 
*
* Description:
* This function get the key hold time
*
* PROTOTYPE:	
* void DrvKeyGeyState(void)
*
* ARG:	N/A 
*	
* RETURN:  N/A
*
* END OF FUNCTION HEADER
*****************************************************************/
void DrvKeyGetState(void)
{
  T_U8 key;
  
  DrvKeyGetValue();
  
  for(key=0;key<E_KEY_MAX;key++)
  {
    if(g_key_pre_status[key] != g_key_status[key])
    {
      if(!g_key_status_changed[key])
      {
        g_key_count[key] = 0;
        g_key_status_changed[key] = E_TRUE;
      }
    }
    
    if(!g_key_pre_status[key])
    {
      if(g_key_status[key])
      {
        g_key_count[key]++;
        
        if(g_key_count[key]>3)
        {
          //key press will be detected between 40ms and 60ms,
          //so use average value: 50ms
          g_key_hold_time[key] = KEY_PRESS_TIME;
          
          g_key_count[key] = 0;
          g_key_pre_status[key] = g_key_status[key];
          g_key_status_changed[key] = E_TRUE;
        }
      }
      else
      {
        g_key_count[key] = 0;
        g_key_hold_time[key] = 0;
      }
    }
    else
    {
      if(g_key_status[key])
      {
        g_key_count[key]++;
        g_key_hold_time[key]+=KEY_SCAN_MILLI_SECOND;
        
        if(g_key_count[key]>3)
        {
          g_key_count[key] = 0;
        }
        
        if(g_key_hold_time[key] >= 65000)
        {
           g_key_hold_time[key] = 65000;
        }
      }
      else
      {
        g_key_count[key]++;
        
        if(g_key_count[key]>3)
        {
          //set as first released
          g_key_hold_time[key] = KEY_PRESS_FIRST_RELEASE_TIME;
          g_key_count[key] = 0;
          g_key_pre_status[key] = g_key_status[key];
          g_key_status_changed[key] = E_TRUE;
        }
      } 
    }
  }
}

void DrvKeyPostClearState(void)
{
  T_U8 i;
  for(i=0;i<E_KEY_MAX;i++)
  {
	g_key_hold_time[i]=0;
	g_key_pre_status[i] = 0;
	g_key_status[i] = 0;
	g_key_status_changed[i] = E_FALSE;
	g_key_count[i] = 0;
  }
}

