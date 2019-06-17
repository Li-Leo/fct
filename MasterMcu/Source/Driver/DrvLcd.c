/*******************************************************************************
**�ļ���:DrvLcd.c
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
#include "DrvLcd.h"

TE_BACK_LIGHT g_back_light_status = E_BACK_LIGHT_OFF;

/*******************************************************************************
**������:void DrvLcdInit(void)
**
**����:Һ������ʼ������Һ����������ѹ��ɨ�跽��Ƚ�������
**
**���ߣ�HJH
**
**���ڣ�2016-04-28
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdInit(void)
{
  T_U16 i;
  LCD_RST_ENA;//Һ������λ
  for(i = 0;i < 10;i++)//����һ��ʱ����ʱ
  {
    ;
  }
  LCD_RST_DIS;//��ֹҺ������λ
  for(i = 0;i < 5000;i++)
  {
    ;
  }
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa3);//Һ��������ѹƫѹ��(bias ratio)ѡ��1/7 0xa3
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa0);//ɨ�跽�����ã�normal(0xa0),reverse(0xa1)
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa0);//��ֱ��ʾ�������ã�normal(0xc0),reverse(0xc8)
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x24);//����Regulation Ratio(V5)Ϊ5.0  adjust range:0x20-0x27
  
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x81);//����EV���� adjust brightness of LCD
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x15);//range: 0x00-0x3f
  
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x2f);//����Power Control����
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xaf);//��ʾ����
  DrvLcdClr();//����
  }

/*******************************************************************************
**������:void DrvLcdDisp(void)
**
**����:��������������ʾ�������������ص�
**
**���ߣ�HJH
**
**���ڣ�2016-04-28
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdDisp(void)
{
  T_U16 j,k,m,n;
   for(j = PAGE_MIN; j < PAGE_MAX;j++)
  {
    DrvLcdWriteByte(E_LCD_INSTRUCTION,j);
    for(k = COLUMN_MIN;k < COLUMN_MAX;k++)
    {
      m = k&0x0f;
      n = k&0xf0;
      n= (n>>4);
      n = n|0x10;
      DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//������ʼ��Ϊ��һ��
      DrvLcdWriteByte(E_LCD_INSTRUCTION,m);//
      DrvLcdWriteByte(E_LCD_DATA,0xff);
    }
  }  
}

/*******************************************************************************
**������:void DrvLcdClr(void)
**
**����:Һ�������������������������ص�ر�
**
**���ߣ�HJH
**
**���ڣ�2016-04-28
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdClr(void)
{
  T_U16 j,k,m,n;
   for(j = PAGE_MIN; j < PAGE_MAX;j++)
  {
    DrvLcdWriteByte(E_LCD_INSTRUCTION,j);//д��ҳ��ַ
    for(k = COLUMN_MIN;k < COLUMN_MAX;k++)
    {
      m = k&0x0f;
      n = k&0xf0;
      n= (n>>4);
      n = n|0x10;
      DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//д���е�ַ
      DrvLcdWriteByte(E_LCD_INSTRUCTION,m);
      DrvLcdWriteByte(E_LCD_DATA,0x00);
    }
  } 
}

/*******************************************************************************
**������:void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add)
**
**����:����Һ������ʾ����,����pos_x��ʾҳ��ַ��pos_y��ʾ�е�ַ
**
**���ߣ�HJH
**
**���ڣ�2016-04-28
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdSetPos(TE_PAGE_NUM page_num,T_U8 colum_add)
{
  T_U16 k,m,n;//k�����ҳ��ַ��m������е�ַ��4λ��n������е�ַ����λ
/******************************************************************************/  
  k = page_num&0x0f;//������datasheet��page_num��4λ����
  k = k|0xb0;//���������ֱ��д��Ĵ�����ҳ��ַ
/******************************************************************************/  
  m = colum_add&0x0f;//����ST7565��datasheet��ȡ�е�ַ����λ
  n = colum_add&0xf0;//����ST7565��datasheet��ȡ�е�ַ����λ
  n= (n>>4);
  n = n|0x10;
/******************************************************************************/    
  DrvLcdWriteByte(E_LCD_INSTRUCTION,k);//д��ҳ��ַ
  DrvLcdWriteByte(E_LCD_INSTRUCTION,n);//д���е�ַ
  DrvLcdWriteByte(E_LCD_INSTRUCTION,m);
}

/*******************************************************************************
**������:void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data)
**
**����:��Һ����д��һ���ֽ����ݣ������ݿ���Ϊ���ݻ�����������
**
**���ߣ�HJH
**
**���ڣ�2016-04-28
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdWriteByte(TE_LCD_CMD cmd,T_U8 data)
{
  T_U8 i;
  LCD_DATA = data;//��Ҫд������д������˿ڼĴ���
  switch(cmd)
  {
    case E_LCD_INSTRUCTION:
      {
        LCD_CMD_S;//ָ��д�������Ϊ����
      }
      break;
  case E_LCD_DATA:
    {
      LCD_DATA_S;//ָ��д�������Ϊ����
    }
    break;
  default:
    ;
    break;
  }
  LCD_WR;//LCDд����
  LCD_E_DIS;//��ֹ��������
  for(i = 0;i < 10;i++)
  {
    ;
  }
  LCD_E_ENA;//ʹ����������
  for(i = 0;i < 10;i++)
  {
    ;
  }
  LCD_E_DIS;//��ֹ��������
}

/*******************************************************************************
**������:void DrvLcdSetBackLight(T_U8 bl_state)
**
**����:Һ��������ƴ򿪺͹رտ���
**
**���ߣ�HJH
**
**���ڣ�2016-04-11
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdSetBackLight(TE_BACK_LIGHT bl_state)
{
  if(bl_state == E_BACK_LIGHT_ON)
  {
    LCD_BACK_LIGHT_ON;
  }
  else
  {
    LCD_BACK_LIGHT_OFF;
  }
  g_back_light_status = bl_state;
}

/*******************************************************************************
**������:void DrvLcdLowPower()
**
**����:ͨ���رձ�����Լ�Һ�����ڲ���Դ��ʹҺ��������͹���״̬
**
**���ߣ�HJH
**
**���ڣ�2016-05-20
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
void DrvLcdLowPower(void)
{
  LCD_BACK_LIGHT_OFF; 
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xae);//�ر���ʾ
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0xa5);//������ʾ���
  _NOP();
  _NOP();
  _NOP();
  DrvLcdWriteByte(E_LCD_INSTRUCTION,0x28);//�رյ�Դ
}

