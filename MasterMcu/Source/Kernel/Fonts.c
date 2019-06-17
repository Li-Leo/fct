/************************************************
* COPYRIGHT 2016 .
*
* NAME: Fonts.c
*
* DESCRIPTION:
*	define the fonts library.
*
* REVISION HISTORY:
*	Created by Kevin Shi at 04/28/2016
*
* END OF FILE HEADER
************************************************/
#include "..\TypeDefine.h"
#include "Fonts.h"
#include "string.h"
#include "common.h"
 
const unsigned char nAsciiDot[] =              // ASCII code
{
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,  //92
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // - -
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
        0x00,0x00,0x38,0xFC,0xFC,0x38,0x00,0x00,  // -!-
        0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,        
        
        0x20,0xF8,0xF8,0x20,0xF8,0xF8,0x20,0x00,  // -#-
        0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x02,0x00,
        
        0x38,0x7C,0x44,0x47,0x47,0xCC,0x98,0x00,  // -$-
        0x03,0x06,0x04,0x1C,0x1C,0x07,0x03,0x00,
        
        0x30,0x30,0x00,0x80,0xC0,0x60,0x30,0x00,  // -%-
        0x0C,0x06,0x03,0x01,0x00,0x0C,0x0C,0x00,
        
        0x80,0xD8,0x7C,0xE4,0xBC,0xD8,0x40,0x00,  // -&-
        0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00,
        
        0x00,0x10,0x1E,0x0E,0x00,0x00,0x00,0x00,  // -'-
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
        0x00,0x00,0xF0,0xF8,0x0C,0x04,0x00,0x00,  // -(-
        0x00,0x00,0x03,0x07,0x0C,0x08,0x00,0x00,
        
        0x00,0x00,0x04,0x0C,0xF8,0xF0,0x00,0x00,  // -)-
        0x00,0x00,0x08,0x0C,0x07,0x03,0x00,0x00,
        
        0x80,0xA0,0xE0,0xC0,0xC0,0xE0,0xA0,0x80,  // -*-
        0x00,0x02,0x03,0x01,0x01,0x03,0x02,0x00,
        
        0x00,0x80,0x80,0xE0,0xE0,0x80,0x80,0x00,  // -+-
        0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,
        
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -,-
        0x00,0x00,0x10,0x1E,0x0E,0x00,0x00,0x00,
        
        0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,  // ---
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -.-
        0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,
        
        0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x00,  // -/-
        0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,
        
        0xF8,0xFC,0x04,0xC4,0x24,0xFC,0xF8,0x00,  // -0-
        0x07,0x0F,0x09,0x08,0x08,0x0F,0x07,0x00,
        
        0x00,0x10,0x18,0xFC,0xFC,0x00,0x00,0x00,  // -1-
        0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00,
        
        0x08,0x0C,0x84,0xC4,0x64,0x3C,0x18,0x00,  // -2-
        0x0E,0x0F,0x09,0x08,0x08,0x0C,0x0C,0x00,
        
        0x08,0x0C,0x44,0x44,0x44,0xFC,0xB8,0x00,  // -3-
        0x04,0x0C,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0xC0,0xE0,0xB0,0x98,0xFC,0xFC,0x80,0x00,  // -4-
        0x00,0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,
        
        0x7C,0x7C,0x44,0x44,0xC4,0xC4,0x84,0x00,  // -5-
        0x04,0x0C,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0xF0,0xF8,0x4C,0x44,0x44,0xC0,0x80,0x00,  // -6-
        0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0x0C,0x0C,0x04,0x84,0xC4,0x7C,0x3C,0x00,  // -7-
        0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,
        
        0xB8,0xFC,0x44,0x44,0x44,0xFC,0xB8,0x00,  // -8-
        0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0x38,0x7C,0x44,0x44,0x44,0xFC,0xF8,0x00,  // -9-
        0x00,0x08,0x08,0x08,0x0C,0x07,0x03,0x00,
        
        0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,  // -:-
        0x00,0x00,0x00,0x06,0x06,0x00,0x00,0x00,
        
        0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,  // -;-
        0x00,0x00,0x08,0x0E,0x06,0x00,0x00,0x00,
        
        0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00,  // -<-
        0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00,
        
        0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,  // -=-
        0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,
        
        0x00,0x08,0x18,0x30,0x60,0xC0,0x80,0x00,  // ->-
        0x00,0x08,0x0C,0x06,0x03,0x01,0x00,0x00,
        
        0x18,0x1C,0x04,0xC4,0xE4,0x3C,0x18,0x00,  // -?-
        0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,
        
        0xF0,0xF8,0x08,0xC8,0xC8,0xF8,0xF0,0x00,  // -@-
        0x07,0x0F,0x08,0x0B,0x0B,0x0B,0x01,0x00,
        
        0xE0,0xF0,0x98,0x8C,0x98,0xF0,0xE0,0x00,  // -A-
        0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00,
        
        0x04,0xFC,0xFC,0x44,0x44,0xFC,0xB8,0x00,  // -B-
        0x08,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00,
        
        0xF0,0xF8,0x0C,0x04,0x04,0x0C,0x18,0x00,  // -C-
        0x03,0x07,0x0C,0x08,0x08,0x0C,0x06,0x00,
        
        0x04,0xFC,0xFC,0x04,0x0C,0xF8,0xF0,0x00,  // -D-
        0x08,0x0F,0x0F,0x08,0x0C,0x07,0x03,0x00,
        
        0x04,0xFC,0xFC,0x44,0xE4,0x0C,0x1C,0x00,  // -E-
        0x08,0x0F,0x0F,0x08,0x08,0x0C,0x0E,0x00,
        
        0x04,0xFC,0xFC,0x44,0xE4,0x0C,0x1C,0x00,  // -F-
        0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00,
        
        0xF0,0xF8,0x0C,0x84,0x84,0x8C,0x98,0x00,  // -G-
        0x03,0x07,0x0C,0x08,0x08,0x07,0x0F,0x00,
        
        0xFC,0xFC,0x40,0x40,0x40,0xFC,0xFC,0x00,  // -H-
        0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00,
        
        0x00,0x00,0x04,0xFC,0xFC,0x04,0x00,0x00,  // -I-
        0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,
        
        0x00,0x00,0x00,0x04,0xFC,0xFC,0x04,0x00,  // -J-
        0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,0x00,
        
        0x04,0xFC,0xFC,0xC0,0xF0,0x3C,0x0C,0x00,  // -K-
        0x08,0x0F,0x0F,0x00,0x01,0x0F,0x0E,0x00,
        
        0x04,0xFC,0xFC,0x04,0x00,0x00,0x00,0x00,  // -L-
        0x08,0x0F,0x0F,0x08,0x08,0x0C,0x0E,0x00,
        
        0xFC,0xFC,0x38,0x70,0x38,0xFC,0xFC,0x00,  // -M-
        0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00,
        
        0xFC,0xFC,0x38,0x70,0xE0,0xFC,0xFC,0x00,  // -N-
        0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00,
        
        0xF0,0xF8,0x0C,0x04,0x0C,0xF8,0xF0,0x00,  // -O-
        0x03,0x07,0x0C,0x08,0x0C,0x07,0x03,0x00,
        
        0x04,0xFC,0xFC,0x44,0x44,0x7C,0x38,0x00,  // -P-
        0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00,
        
        0xF8,0xFC,0x04,0x04,0x04,0xFC,0xF8,0x00,  // -Q-
        0x07,0x0F,0x08,0x0E,0x3C,0x3F,0x27,0x00,
        
        0x04,0xFC,0xFC,0x44,0xC4,0xFC,0x38,0x00,  // -R-
        0x08,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,
        
        0x18,0x3C,0x64,0x44,0xC4,0x9C,0x18,0x00,  // -S-
        0x06,0x0E,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0x00,0x1C,0x0C,0xFC,0xFC,0x0C,0x1C,0x00,  // -T-
        0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,
        
        0xFC,0xFC,0x00,0x00,0x00,0xFC,0xFC,0x00,  // -U-
        0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0xFC,0xFC,0x00,0x00,0x00,0xFC,0xFC,0x00,  // -V-
        0x01,0x03,0x06,0x0C,0x06,0x03,0x01,0x00,
        
        0xFC,0xFC,0x00,0x80,0x00,0xFC,0xFC,0x00,  // -W-
        0x03,0x0F,0x0E,0x03,0x0E,0x0F,0x03,0x00,
        
        0x0C,0x3C,0xF0,0xC0,0xF0,0x3C,0x0C,0x00,  // -X-
        0x0C,0x0F,0x03,0x00,0x03,0x0F,0x0C,0x00,
        
        0x00,0x3C,0x7C,0xC0,0xC0,0x7C,0x3C,0x00,  // -Y-
        0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,
        
        0x1C,0x0C,0x84,0xC4,0x64,0x3C,0x1C,0x00,  // -Z-
        0x0E,0x0F,0x09,0x08,0x08,0x0C,0x0E,0x00,
        
        0x00,0x00,0xFC,0xFC,0x04,0x04,0x00,0x00,  // -[-
        0x00,0x00,0x0F,0x0F,0x08,0x08,0x00,0x00,
        
        0x00,0x00,0x04,0x04,0xFC,0xFC,0x00,0x00,  // -]-
        0x00,0x00,0x08,0x08,0x0F,0x0F,0x00,0x00,
        
        0x08,0x0C,0x06,0x03,0x06,0x0C,0x08,0x00,  // -^-
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -_-
        0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
        
        0x00,0xA0,0xA0,0xA0,0xE0,0xC0,0x00,0x00,  // -a-
        0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00,
        
        0x04,0xFC,0xFC,0x20,0x60,0xC0,0x80,0x00,  // -b-
        0x08,0x0F,0x07,0x08,0x08,0x0F,0x07,0x00,
        
        0xC0,0xE0,0x20,0x20,0x20,0x60,0x40,0x00,  // -c-
        0x07,0x0F,0x08,0x08,0x08,0x0C,0x04,0x00,
        
        0x80,0xC0,0x60,0x24,0xFC,0xFC,0x00,0x00,  // -d-
        0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00,
        
        0xC0,0xE0,0xA0,0xA0,0xA0,0xE0,0xC0,0x00,  // -e-
        0x07,0x0F,0x08,0x08,0x08,0x0C,0x04,0x00,
        
        0x40,0xF8,0xFC,0x44,0x0C,0x18,0x00,0x00,  // -f-
        0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00,
        
        0xC0,0xE0,0x20,0x20,0xC0,0xE0,0x20,0x00,  // -g-
        0x27,0x6F,0x48,0x48,0x7F,0x3F,0x00,0x00,
        
        0x04,0xFC,0xFC,0x40,0x20,0xE0,0xC0,0x00,  // -h-
        0x08,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,
        
        0x00,0x00,0x20,0xEC,0xEC,0x00,0x00,0x00,  // -i-
        0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,
        
        0x00,0x00,0x00,0x00,0x20,0xEC,0xEC,0x00,  // -j-
        0x00,0x30,0x70,0x40,0x40,0x7F,0x3F,0x00,
        
        0x04,0xFC,0xFC,0x80,0xC0,0x60,0x20,0x00,  // -k-
        0x08,0x0F,0x0F,0x01,0x03,0x0E,0x0C,0x00,
        
        0x00,0x00,0x04,0xFC,0xFC,0x00,0x00,0x00,  // -l-
        0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,
        
        0xE0,0xE0,0x60,0xC0,0x60,0xE0,0xC0,0x00,  // -m-
        0x0F,0x0F,0x00,0x0F,0x00,0x0F,0x0F,0x00,
        
        0x20,0xE0,0xC0,0x20,0x20,0xE0,0xC0,0x00,  // -n-
        0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,
        
        0xC0,0xE0,0x20,0x20,0x20,0xE0,0xC0,0x00,  // -o-
        0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00,
        
        0x20,0xE0,0xC0,0x20,0x20,0xE0,0xC0,0x00,  // -p-
        0x40,0x7F,0x7F,0x48,0x08,0x0F,0x07,0x00,
        
        0xC0,0xE0,0x20,0x20,0xC0,0xE0,0x20,0x00,  // -q-
        0x07,0x0F,0x08,0x48,0x7F,0x7F,0x40,0x00,
        
        0x20,0xE0,0xC0,0x60,0x20,0x60,0xC0,0x00,  // -r-
        0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00,
        
        0x40,0xE0,0xA0,0x20,0x20,0x60,0x40,0x00,  // -s-
        0x04,0x0C,0x09,0x09,0x0B,0x0E,0x04,0x00,
        
        0x20,0x20,0xF8,0xFC,0x20,0x20,0x00,0x00,  // -t-
        0x00,0x00,0x07,0x0F,0x08,0x0C,0x04,0x00,
        
        0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,0x00,  // -u-
        0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00,
        
        0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,  // -v-
        0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00,
        
        0xE0,0xE0,0x00,0x00,0x00,0xE0,0xE0,0x00,  // -w-
        0x07,0x0F,0x0C,0x07,0x0C,0x0F,0x07,0x00,
        
        0x20,0x60,0xC0,0x80,0xC0,0x60,0x20,0x00,  // -x-
        0x08,0x0C,0x07,0x03,0x07,0x0C,0x08,0x00,
        
        0xE0,0xE0,0x00,0x00,0x00,0xE0,0xE0,0x00,  // -y-
        0x47,0x4F,0x48,0x48,0x68,0x3F,0x1F,0x00,
        
        0x60,0x60,0x20,0xA0,0xE0,0x60,0x20,0x00,  // -z-
        0x0C,0x0E,0x0B,0x09,0x08,0x0C,0x0C,0x00,
        
        0x00,0x40,0x40,0xF8,0xBC,0x04,0x04,0x00,  // -{-
        0x00,0x00,0x00,0x07,0x0F,0x08,0x08,0x00,
        
        0x00,0x04,0x04,0xBC,0xF8,0x40,0x40,0x00,  // -}-
        0x00,0x08,0x08,0x0F,0x07,0x00,0x00,0x00,
        
        0x08,0x0C,0x04,0x0C,0x08,0x0C,0x04,0x00,  // -~-
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const T_CHAR g_AsciiDot_code[][2] = 
{
    " ","!","#","$","%","&","'","(",")","*","+",",","-",".","/",	
    "0","1","2","3","4","5","6","7","8","9",
    ":",";","<","=",">","?","@",
    "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O",
    "P","Q","R","S","T","U","V","W","X","Y","Z",
	"[","]","^","_",
	"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o",
	"p","q","r","s","t","u","v","w","x","y","z",
    "{","}","~"
};
const TS_FONT_INFO gg_ASCII_font = 
{
    //height, height_byte, char_count,                    widths_pos, 	font_data_pos,
    16,       2,           sizeof(g_AsciiDot_code)/2,         0,          92,
        (const T_U16*)g_AsciiDot_code, nAsciiDot
};

void StringCopy(T_U8 *dst, const T_U8 *src, T_U16 len)
{
    T_U8 idx;
    for(idx = 0; idx < len; idx++)
	{
        *dst++ = *src++;
    }
}

T_S16 FontFindCharIndex(const TS_FONT_INFO* font_info, T_CHAR ch[2])
{
    T_SINT i;
    //T_U16 ch_code = (ch[0])|(ch[1]<<8);
    T_CHAR *p;
    for(i=0; i<font_info->char_count; i++)
    {
        p = (T_CHAR*)(&font_info->char_codes[i]);
        //printf("%u %u\n",font_info->char_codes[i], ch_code);
        if(p[0]==ch[0] && p[1] == ch[1])
        {
            break;
        }
    }

    if(i>=font_info->char_count)
    {
        return -1;
    }
    else
    {
        return i;
    }
}

T_S16 FontFindCharPos(const TS_FONT_INFO* font_info, T_S16 index)
{
    T_SINT i;
    T_SINT width;
    T_SINT ch_pos = 0;
    for(i=0; i<index; i++)
    {
        width = font_info->font_lib[font_info->widths_start_pos+i];
        ch_pos += width*font_info->height_byte;
    }

    return ch_pos;
}


//if find the char, return true
TE_BOOLEAN FontGetCharInfo(const TS_FONT_INFO* font_info, T_CHAR ch[2], TS_CHAR_INFOR* ch_info)
{
    T_SINT char_index;

    memset(ch_info, 0 ,sizeof(*ch_info));
    char_index = FontFindCharIndex(font_info, ch);
    if(char_index != -1)
    {
        ch_info->height = font_info->height;
        ch_info->width = font_info->font_lib[font_info->widths_start_pos + char_index];
        ch_info->data_size = ch_info->width*font_info->height_byte;        
        ch_info->height_byte = font_info->height_byte;
        ch_info->data = font_info->font_lib + 
            font_info->font_data_start_pos + FontFindCharPos(font_info, char_index);
        return E_TRUE;
    }
    else
    {
        return E_FALSE;
    }
}

