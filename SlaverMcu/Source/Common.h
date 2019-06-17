/************************************************
* COPYRIGHT 2016.
*
* NAME: Common.h
*
* DESCRIPTION:
*   define some common function interfaces
*
* REVISION HISTORY:
*	Created by Kevin Shi at 06/20/2016
*
* END OF FILE HEADER
************************************************/
#ifndef COMMON_H
#define COMMON_H

#include "TypeDefine.h"
#include <stdio.h>
#include <assert.h>
/************************************************
* Declaration
************************************************/

//trace message, valid ony when debug
#ifdef _DEBUG
#define M_Trace printf
#else
#define M_Trace(...)
#endif //_DEBUG

#define M_Str2(x)    M_Str(x)
#define M_Str(x)       #x


//assert if it is true, valid ony when debug
#ifdef _DEBUG

#define M_Assert(test) {if(!(test))puts(__FILE__ ":" M_Str2(__LINE__) " " #test " -- Assert Fail\n");}; \
    assert(test)
#else
#define M_Assert(test) ((void)0)
#endif
#define M_NumOfArr(arr) (sizeof(arr)/sizeof(arr[0]))
#define M_CommonPrint if(IsEnableOutput(E_MODULE_COMMON))printf



/* static double link table */
typedef struct
{
	T_U32 data;
	T_U8 previous;
	T_U8 next;
} TS_DOUBLE_LINK_NODE;

typedef struct
{
	T_U8 count;						/* current node count */
	T_U8 size;						/* correspond alarm count */
	T_U8 head_index;				/* first node index in node array */
	TS_DOUBLE_LINK_NODE * p_node;	/* node array */
}TS_DOUBLE_LINK;

typedef enum
{
    E_MODULE_COMMON,    
    E_MODULE_COMM,
    E_MODULE_MOTOR,
    E_MODULE_MCU_MONITOR,
    E_MODULE_MAX
}TE_MODULE;

void DoubleLinkInit(TS_DOUBLE_LINK * p_double_link,T_U8 size,TS_DOUBLE_LINK_NODE * p_node); 
T_U32 DoubleLinkInsert(TS_DOUBLE_LINK * p_double_link,T_U32 data);
T_U32 DoubleLinkDel(TS_DOUBLE_LINK * p_double_link,T_U32 data);
T_U8 DoubleLinkFind(TS_DOUBLE_LINK * p_double_link,T_U32 data);
T_U8 DoubleLinkNotUse(TS_DOUBLE_LINK * p_double_link);
T_U32 DoubleLinkFirstData(TS_DOUBLE_LINK * p_double_link);

//calc the elapsed num from old_value to new_value
T_U32 DiffForU32( T_U32 new_value, T_U32 old_value );

//get the absolute value for two value's difference
T_S32 AbcDiff(T_S32 a, T_S32 b);

void CommonInit();
void SetModuleOutput(TE_MODULE module,TE_BOOLEAN is_enable);
TE_BOOLEAN SetModuleOutputByName(const T_CHAR* module_name, TE_BOOLEAN is_enable);
TE_BOOLEAN IsEnableOutput(TE_MODULE module);

#endif /* COMMON_H */

