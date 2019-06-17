/*******************************************************************************
**�ļ���:DrvFram.h
**
**����:��Ӱ�ڡ�DrvFram.c���ļ��Լ�����һЩ�ļ���ʹ�õĺꡢ������
**
**����:
**
**����:2016-03-01
**
**�޸����ݣ�
**
**���ߣ�
**
**���ڣ�
*******************************************************************************/
#ifndef DRV_FRAM_H
#define DRV_FRAM_H

#include <msp430fr5989.h>
#include "..\TypeDefine.h"
/*******************************************************************************
**�����û���Ҫ���ݴ洢��FRAM����Ϊ0.5K, ����information memory,���س���ʱ����ѡ���Ƿ����
**�����û����ݴ洢��FRAM����Ϊ33K(���в���: 1K,��ע��¼:4K, ��������¼:4K, ͣ���¼:4K, ������¼:20K),��ʼ��ַ��0x4400��������ַ��0xC7FF
*******************************************************************************/
//512byte to save main param
#define FRAM_MAIN_PARAM_START_ADDRESS      0x1800//�û�FRAM��ʼ��ַ
#define FRAM_MAIN_PARAM_END_ADDRESS        0x19FF//�û�FRAM������ַ

//33K to save other param and log
#define FRAM_START_ADDRESS      (T_U16)0x4400//�û�FRAM��ʼ��ַ
#define FRAM_END_ADDRESS        (T_U16)0xC7FF//�û�FRAM������ַ

void FramReadData(T_U32 *data_point, T_U32 *fram_point, T_U16 data_num);
void FramWriteData(T_U32 *data_point,T_U32 *fram_point, T_U16 data_num);

#endif /* DRV_FRAM_H */
