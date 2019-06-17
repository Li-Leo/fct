/************************************************
* COPYRIGHT 2016 .
*
* NAME: Fonts.h
*
* DESCRIPTION:
*   Modify the Array fonts.
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
*
* END OF FILE HEADER
************************************************/
#ifndef FONTS_H
#define FONTS_H

#include "..\TypeDefine.h"


typedef struct
{    
    T_U8 height; //字体像素高度
    T_U8 height_byte; //字体字节高度
    T_S16 char_count; //字总数
    T_S16 widths_start_pos; //字宽度在字库中的开始位置
    T_S16 font_data_start_pos; //字数据在字库中的开始位置
    const T_U16* char_codes; //字与index映射
    const T_U8* font_lib;
}TS_FONT_INFO;


typedef struct
{
    T_U8 height;
    T_U8 width;
    T_U8 height_byte; //字体字节高度
    T_U8 data_size; //the size of bytes    
    const T_U8 *data; //char lib
}TS_CHAR_INFOR;


extern const unsigned char nAsciiDot[];
extern const T_CHAR g_AsciiDot_code[][2];


extern const TS_FONT_INFO gg_ASCII_font;

void StringCopy(T_U8 *dst, const T_U8 *src, T_U16 len);

//if find the char, return true
TE_BOOLEAN FontGetCharInfo(const TS_FONT_INFO* font_info, T_CHAR ch[2], TS_CHAR_INFOR* ch_info);

#endif	/* FONTS_H */

