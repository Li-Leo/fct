/************************************************
* COPYRIGHT 2016.
*
* NAME: CtlInterrupt.c
*
* DESCRIPTION:
*   interrupt control module
*
* REVISION HISTORY:
*	Created by Kevin Shi at 05/13/2016
*
* END OF FILE HEADER
************************************************/
#include"CtlInterrupt.h"

/***********************************************
*   description:
*       isp initialization
*   parameters:
*       N/A
*   return:       
*       N/A
***********************************************/
void InterruptCtlInit(void)
{
	
}

/***********************************************
*   description:
*       isp enable
*   parameters:
*       N/A
*   return:       
*       N/A
***********************************************/
void InterruptCtlEnable(void)
{
	_EINT();
}

/***********************************************
*   description:
*       isp disable
*   parameters:
*       N/A
*   return:       
*       N/A
***********************************************/
void InterruptCtlDisable(void)
{
	_DINT();
}

