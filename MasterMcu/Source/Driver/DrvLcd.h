/*******************************************************************************
**�ļ���:DrvKey.h
**
**����:
**
**����:HJH
**
**����:2016-04-28
**
**�޸�����:
**
**����:
**
**����:
**
*******************************************************************************/
#ifndef DRV_LCD_
#define DRV_LCD_

#include <msp430fr5989.h>
#include "..\TypeDefine.h"

#define LCD_BACK_LIGHT_ON       (P3OUT |= BIT0)
#define LCD_BACK_LIGHT_OFF      (P3OUT &= ~BIT0)

#define LCD_RD          (P3OUT |= BIT6)// Һ�������źţ���ӦҺ����дѡ������R/W 
#define LCD_WR          (P3OUT &= ~BIT6)//Һ����д�źţ���ӦҺ����дѡ������R/W

#define LCD_RST_ENA     (P3OUT &= ~BIT7)// Һ����λ����ӦҺ����λ����RES         
#define LCD_RST_DIS     (P3OUT |= BIT7)// ��ֹҺ����λ����ӦҺ����λ����RES

#define LCD_DATA_S      (P2OUT |= BIT2)//LCD��д�����������ݣ���ӦҺ��������A0
#define LCD_CMD_S       (P2OUT &= ~BIT2)//LCD��д�������������ӦҺ��������A0

#define LCD_E_ENA       (P2OUT &= ~ BIT3)// Һ������ʹ���ź�,��ӦҺ����д��������E
#define LCD_E_DIS       (P2OUT |= BIT3)//Һ����дʹ���źţ���ӦҺ����д��������E

#define LCD_DATA        P9OUT//Һ�����������

#define  PAGE_MIN       0xb0//ҳ��ʼ��ַ(��Сҳ��ַ)��Ϊ��ҳ����0ҳ 
#define  PAGE_MAX       0xb8//ҳ������ַ(���ҳ��ַ)��Ϊĩҳ����8ҳ����ҳֻ��һ�У�
                            //ʵ�ʲ����в�ʹ�ø�ҳ 

#define ROW_MIN         0x40//����ʼ��ַ(��С�е�ַ)��Ϊ���м���0��
#define ROW_MAX         0x7f//�н�����ַ(����е�ַ)��Ϊĩ�м���63��

#define COLUMN_MIN      0x00//����ʼ��ַ(��С�е�ַ)��Ϊ���м���0��
#define COLUMN_MAX      0x80//�н�����ַ(����е�ַ)��Ϊĩ�м���128��




typedef enum //LCDд���������Ͷ���
{
  E_LCD_INSTRUCTION,//����
  E_LCD_DATA,//����
  E_LCD_MAX,
} TE_LCD_CMD;

typedef enum//�����״̬����
{
  E_BACK_LIGHT_ON,//����ƿ���
  E_BACK_LIGHT_OFF,//����ƹر�
  E_BACK_LIGHT_MAX,
} TE_BACK_LIGHT;

typedef enum//����ҳ���ţ� 
{
  E_PAGE_ONE = 0,//��0ҳ
  E_PAGE_TWO,//��1ҳ
  E_PAGE_THREE,//��2ҳ
  E_PAGE_FOUR,//��3ҳ
  E_PAGE_MAX,
} TE_PAGE_NUM;

void DrvLcdInit(void);
void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data);
void DrvLcdDisp(void);
void DrvLcdClr(void);
void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add);
void DrvLcdSetBackLight(TE_BACK_LIGHT bl_state);
void DrvLcdLowPower(void);

#endif
