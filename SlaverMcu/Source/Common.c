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
    "mcu_monitor",
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


void CommonInit()
{
#ifdef _DEBUG
    g_is_module_enable_output[E_MODULE_COMMON] = E_TRUE;
    g_is_module_enable_output[E_MODULE_MOTOR] = E_TRUE;
    g_is_module_enable_output[E_MODULE_MCU_MONITOR] = E_TRUE;
    //SetModuleOutputByName("all", E_TRUE);
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

