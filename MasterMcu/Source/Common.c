/************************************************
* COPYRIGHT 2016.
*
* NAME: Common.c
*
* DESCRIPTION:
*   define some common function interfaces
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/03/2016
*
* END OF FILE HEADER
************************************************/
#include "Common.h"
#include "io430fr5989.h"
#include <assert.h>
#include "string.h"

TE_BOOLEAN g_is_module_enable_output[E_MODULE_MAX];
const T_CHAR* g_module_names[] =
{
    "common",
    "comm",
    "motor",
    "delivery_detail",
    "mcu_monitor",   
    "display",
};

/************************************************
* Functions:
************************************************/

/***********************************************
* Description: double link init
*
* Argument:
*   p_double_link:point to double link initi struct
*   size:capacity of the node array
*   p_node:node array address
*
* Return:
*
************************************************/
void DoubleLinkInit(TS_DOUBLE_LINK * p_double_link,T_U8 size,TS_DOUBLE_LINK_NODE * p_node)
{
    T_U8 i;
    
    assert(p_double_link);
    assert(p_node);

    for (i=0;i<size;i++){
        p_node[i].data = MAX_T_U32;
        p_node[i].previous = MAX_T_U8;
        p_node[i].next = MAX_T_U8;
    }

    p_double_link->count = 0;
    p_double_link->size = size;
    p_double_link->head_index = 0xFF;/* means null link */
    p_double_link->p_node = p_node;  /* static node array address */
}

/***********************************************
* Description:insert a node as a new first node
*
* Argument:
*   p_double_link:point to double link struct
*   data:node's data
*
* Return:if success will return data, if failure will return -1
*
************************************************/
T_U32 DoubleLinkInsert(TS_DOUBLE_LINK * p_double_link,T_U32 data)
{
    T_U8 index;
    T_U8 head_index;
    
    assert(p_double_link);   

    if (p_double_link->count < p_double_link->size){
        /* find a node unused in node array */
        index = DoubleLinkNotUse(p_double_link);
        head_index = p_double_link->head_index;
        /* init new node */
        p_double_link->p_node[index].data = data;
        p_double_link->p_node[index].previous = MAX_T_U8; /* means first node */        
        p_double_link->p_node[index].next = head_index;
        /* if before have node in the link */
        if (head_index < p_double_link->size){
            p_double_link->p_node[head_index].previous = index;
        }
        p_double_link->head_index = index;

        /* node count ++ */
        p_double_link->count ++;

        return data;
    }

    return MAX_T_U32;
    
}

/***********************************************
* Description:delete a node
*
* Argument:
*   p_double_link:point to a double link struct
*   data:node's data
*
* Return:if success will return the node's data, if failure will return -1
*
************************************************/
T_U32 DoubleLinkDel(TS_DOUBLE_LINK * p_double_link,T_U32 data)
{
    T_U8 index;
    T_U8 temp_index;
    T_U32 return_data = MAX_T_U32;
    
    assert(p_double_link);

    index = DoubleLinkFind(p_double_link,data);

    /* can't find the node you want to delete */
    if (0xFF == index){
        return MAX_T_U32;
    }

    /* this is a only node */
    if (p_double_link->p_node[index].previous == 0xFF &&
        p_double_link->p_node[index].next == 0xFF){
        return_data = p_double_link->p_node[index].data;
        p_double_link->p_node[index].data = MAX_T_U32;

        
    }
    /* this is the first node */
    else if (p_double_link->p_node[index].previous == 0xFF &&
             p_double_link->p_node[index].next != 0xFF){

        p_double_link->head_index = p_double_link->p_node[index].next;
        temp_index = p_double_link->p_node[index].next;
        p_double_link->head_index = temp_index;
        /* delete the node */
        return_data = p_double_link->p_node[index].data;
        p_double_link->p_node[index].data = MAX_T_U32;
        p_double_link->p_node[index].previous = MAX_T_U8;
        p_double_link->p_node[index].next = MAX_T_U8;

        
    }
    /* this is the tail node */
    else if (p_double_link->p_node[index].next == 0xFF &&
             p_double_link->p_node[index].previous < p_double_link->size){
        temp_index = p_double_link->p_node[index].previous;
        return_data = p_double_link->p_node[index].data;
        /* delete the node */    
        p_double_link->p_node[index].previous = MAX_T_U8;
        p_double_link->p_node[index].next = MAX_T_U8;
        p_double_link->p_node[index].data = MAX_T_U32;

        p_double_link->p_node[temp_index].next = MAX_T_U8;
    }
    /* normal node */
    else if (p_double_link->p_node[index].previous < p_double_link->size &&
             p_double_link->p_node[index].next < p_double_link->size &&
             p_double_link->p_node[index].data != MAX_T_U32){
        return_data = p_double_link->p_node[index].data;
        /* relink */
        temp_index = p_double_link->p_node[index].previous;
        p_double_link->p_node[temp_index].next = p_double_link->p_node[index].next;
        temp_index = p_double_link->p_node[index].next;
        p_double_link->p_node[temp_index].previous = p_double_link->p_node[index].previous;
        /* delete the node */
        p_double_link->p_node[index].previous = MAX_T_U8;
        p_double_link->p_node[index].next = MAX_T_U8;
        p_double_link->p_node[index].data = MAX_T_U32;
    }

    p_double_link->count -- ;
    return return_data;    
    
}

/***********************************************
* Description:find a node 
*
* Argument:
*   p_double_link:point to a double link struct
*   data:node's data
*
* Return:node 's index in the node array
*
************************************************/
T_U8 DoubleLinkFind(TS_DOUBLE_LINK * p_double_link,T_U32 data)
{
    T_U8 head_index;
    T_U8 index;
    T_U8 i;
    
    assert(p_double_link);
	assert(p_double_link->count <= p_double_link->size);    /* assure link logic is ok */
    assert(data != MAX_T_U32);  /* -1 is reset status data,can't be used */

    if (p_double_link->count > 0){
        head_index = p_double_link->head_index;

        index = head_index;        

        for (i= 0;i<p_double_link->size;i++){
            if (p_double_link->p_node[index].data == data){
                return index;
            }
            /* already in the tail node ,so can't finde the node you want. */
            else if (p_double_link->p_node[index].next == 0xFF){
                return MAX_T_U8;
            }

            index = p_double_link->p_node[index].next;
        }        
    }
    
    return MAX_T_U8;
}


/***********************************************
* Description:find a index not be used in node array
*
* Argument:
*   p_double_link:point to double link struct
*
* Return:if failure will return 0xFF,if success will return index that you want.
*
************************************************/
T_U8 DoubleLinkNotUse(TS_DOUBLE_LINK * p_double_link)
{
    T_U8 i;

    assert(p_double_link);

    if (p_double_link->count < p_double_link->size){
        for (i=0;i<p_double_link->size;i++){
            if (MAX_T_U32 == p_double_link->p_node[i].data){
                return i;
            }
        }
    }

    return MAX_T_U8;
}


/***********************************************
* Description:get the first node's data
*
* Argument:
*   p_double_link:point to double link struct
*
* Return:first node's data,if have no node will return -1
*
************************************************/
T_U32 DoubleLinkFirstData(TS_DOUBLE_LINK * p_double_link)
{
    T_U8 index;
    
    assert(p_double_link);

    if (p_double_link->count > 0){
        index = p_double_link->head_index;
        return (p_double_link->p_node[index].data);
    }

    return MAX_T_U32;
}

/***********************************************
* Description:
*   calc the elapsed num from old_value to new_value
* Argument:
*   new_value:
*   old_value:
*
* Return:
*
************************************************/
T_U32 DiffForU32( T_U32 new_value, T_U32 old_value )
{
    if(new_value >= old_value)
    {
        return new_value-old_value;
    }
    else
    {
        return MAX_T_U32-old_value+new_value+1;
    }
}

T_U16 DiffForU16( T_U16 new_value, T_U16 old_value )
{
    if(new_value >= old_value)
    {
        return new_value-old_value;
    }
    else
    {
        return MAX_T_U16-old_value+new_value+1;
    }    
}


/***********************************************
* Description:
*   get the absolute value for two value's difference
* Argument:
*   a:
*   b:
*
* Return:
*
************************************************/
T_S32 AbcDiff( T_S32 a, T_S32 b )
{
    if(a>b)
    {
        return a-b;
    }
    else
    {
        return b-a;
    }
}

/********************************************************************************************************* 
*	 函 数 名: CalcWeek 
*	 功能说明: 根据日期计算星期几 
*	 形    参: _year _mon _day	年月日	(年是2字节整数，月和日是字节整数） 
*	 返 回 值: 周几 （0-6） 0表示周日 
*********************************************************************************************************/	
T_S32 CalculateWeekDay(T_S32 year, T_S32 month, T_S32 date)
{  
	/*	
	蔡勒（Zeller）公式	
		历史上的某一天是星期几？未来的某一天是星期几？关于这个问题，有很多计算公式（两个通用计算公式和 
	一些分段计算公式），其中最著名的是蔡勒（Zeller）公式。 
		即w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1  
		 
		公式中的符号含义如下， 
		 w：星期； 
		 c：年的高2位，即世纪-1 
		 y：年（两位数）； 
		 m：月（m大于等于3，小于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算， 
			  比如2003年1月1日要看作2002年的13月1日来计算）； 
		 d：日； 
		 [ ]代表取整，即只要整数部分。 
		 
		算出来的W除以7，余数是几就是星期几。如果余数是0，则为星期日。  
		如果结果是负数，负数求余数则需要特殊处理： 
			负数不能按习惯的余数的概念求余数，只能按数论中的余数的定义求余。为了方便  
		计算，我们可以给它加上一个7的整数倍，使它变为一个正数，然后再求余数 	
		 
		以2049年10月1日（100周年国庆）为例，用蔡勒（Zeller）公式进行计算，过程如下：  
		蔡勒（Zeller）公式：w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1  
		=49+[49/4]+[20/4]-2×20+[26× (10+1)/10]+1-1  
		=49+[12.25]+5-40+[28.6]  
		=49+12+5-40+28	
		=54 (除以7余5)	
		即2049年10月1日（100周年国庆）是星期5。 
	*/	
	T_S32 y, c, m, d, week;
	T_S32 zeller;
  
	if (month >= 3)
	{
		m = month;
		y = year % 100;
		c = year / 100;
		d = date;
	}
	else	/* 某年的1、2月要看作上一年的13、14月来计算 */
	{
		m = month + 12;
		y = (year - 1) % 100;
		c = (year - 1) / 100;
		d = date;
	}
  
	zeller = y + y / 4 +  c / 4 - 2 * c + (26 * (m + 1))/10 + d - 1;	   
	if (zeller < 0)    /* 如果w是负数，则计算余数方式不同 */  
	{
		week = 7 - (-zeller) % 7;
	}
	else
	{
		week = zeller % 7;
	}
	return week;
}  

void CommonInit()
{
#ifdef _DEBUG
    //g_is_module_enable_output[E_MODULE_COMMON] = E_TRUE;
    //g_is_module_enable_output[E_MODULE_MOTOR] = E_TRUE;
    //g_is_module_enable_output[E_MODULE_MCU_MONITOR] = E_TRUE;
    //g_is_module_enable_output[E_MODULE_DISPLAY] = E_TRUE;
#endif
}

void SetModuleOutput(TE_MODULE module,TE_BOOLEAN is_enable)
{
    g_is_module_enable_output[module] = is_enable;
}

TE_BOOLEAN SetModuleOutputByName(const T_CHAR* module_name, TE_BOOLEAN is_enable)
{
    T_SINT i;
    TE_BOOLEAN ret = E_FALSE;

    for(i=0; i<M_NumOfArr(g_module_names); i++)
    {
        if(strcmp(module_name,g_module_names[i]) == 0 )
        {
            SetModuleOutput((TE_MODULE)i, is_enable);
            ret = E_TRUE;
            break;
        }
        else if(strcmp(module_name,"all") == 0)
        {
            SetModuleOutput((TE_MODULE)i, is_enable);
            ret = E_TRUE;
        }
            
    }

    return ret;
}

TE_BOOLEAN IsEnableOutput(TE_MODULE module)
{
    return g_is_module_enable_output[module];
}

