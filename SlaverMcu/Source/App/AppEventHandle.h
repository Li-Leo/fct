/************************************************
* COPYRIGHT 2016.
*
* NAME: AppEventHandle.h
*
* DESCRIPTION:
*   handle all events processing
*
* REVISION HISTORY:
*	Created by Kevin Shi at 07/29/2016
*
* END OF FILE HEADER
************************************************/
#ifndef APP_EVENT_HANDLE_H
#define APP_EVENT_HANDLE_H

#include "..\TypeDefine.h"

/******************************************************************************/  
#define MCU_RESET_RECOVER	(P1OUT &= ~BIT3)		//���͵�ƽ���ָ��ȴ�״̬
#define MCU_RESET_EXCUTE	(P1OUT |= BIT3)	//���ߵ�ƽ����λ
/******************************************************************************/

/************************************************
* Declaration
************************************************/
void AppEventHandleInit(void);
void AppTimerHandleAfterMasterMcuReset(void);


#endif /* APP_EVENT_HANDLE_H */

