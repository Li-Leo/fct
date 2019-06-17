/************************************************
* COPYRIGHT 2012 
*
* NAME: ctl_display_main.c
*
* REVISION HISTORY:
 * 
* END OF FILE HEADER
************************************************/
#include "CtlDisplay.h"
#include "CtlTimer.h"
#include "Fonts.h"
#include "..\Driver\DrvLcd.h"
#include <string.h>
#include <assert.h>
#include "common.h"



//************global variables******************
TE_BOOLEAN g_is_screen_display = E_FALSE;

//************global variables******************
T_U8 text_pos_x = 0;
T_U8 text_buffer[48] = {0};
T_U8 reversed_buffer[48] = {0};
TE_DRAW_LCD_TYPE text_type;
TE_BOOLEAN is_orignal_text;
static T_U8 g_display_cache[4][128];//32 row,128 column
static TS_RECT g_display_dirty_rect;
static TE_BOOLEAN g_display_is_dirty = E_FALSE;
static const TS_FONT_INFO* g_display_font_lib = 0;

/***********************************************
* Description:
*   Get the offset of the display page.
*	This is a local function which only be called in this file.
*
* Argument:
*   T_U8 row,
*
* Return:
*  	Offset data.
************************************************/
T_U8 GetOffsetOfPage(T_U8 row, TE_PAGE_OFFSET_TYPE type)
{	
	assert(row <= ROW_NUMBER_MAX && row >= ROW_NUMBER_MIN);
	assert(type < E_PAGE_OFFSET_TYPE_MAX);
	T_U8 offset = row % 8;
	T_U8 data = 0x00;
	switch(offset)
	{
		case 1:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xFE : 0x03;
			break;
		case 2:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xFC : 0x07;
			break;
		case 3:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xF8 : 0x0F;
			break;
		case 4:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xF0 : 0x1F;
			break;
		case 5:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xE0 : 0x3F;
			break;
		case 6:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xC0 : 0x7F;
			break;
		case 7:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0x80 : 0xFF;
			break;
		case 0:
			data = (type == E_PAGE_OFFSET_TYPE_TOP) ? 0xFF : 0x01;
			break;
		default:
			data = 0x00;
			break;
	}
	return data;
}


//flush the display cache to lcd, x_start:0~127, y_start:0~31
void DisplayFlush(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height)
{
    T_SINT start_page;
    T_SINT end_page;
    T_SINT i,j;

    M_Assert(x_start<DISPLAY_WIDTH&&y_start<DISPLAY_HEIGHT);
    if(x_start+width>DISPLAY_WIDTH)
    {
        width = DISPLAY_WIDTH - x_start;
    }
    if(y_start+height>DISPLAY_HEIGHT)
    {
        height = DISPLAY_HEIGHT - y_start;
    }
    start_page = y_start/8;
    end_page = (y_start+height-1)/8;
    for(i=start_page; i<=end_page; i++)
    {
        DrvLcdSetPos((TE_PAGE_NUM)i, x_start);
        for(j=x_start; j<x_start+width; j++)
        {
            DrvLcdWriteByte(E_LCD_DATA, g_display_cache[i][j]);
        }
    }
    
}
//flush the dirty area to lcd
void DisplayFlushDirty()
{
    if(g_display_is_dirty)
    {
        M_DisplayPrint("flash dirty: x_start:%d, y_start:%d, width:%d, height:%d\n",
            g_display_dirty_rect.x, g_display_dirty_rect.y, g_display_dirty_rect.x_size,
            g_display_dirty_rect.y_size);

        g_display_is_dirty = E_FALSE;
        DisplayFlush(g_display_dirty_rect.x, g_display_dirty_rect.y, 
            g_display_dirty_rect.x_size, g_display_dirty_rect.y_size);
    }
}
//is exist dirty area
TE_BOOLEAN DisplayIsDirty()
{
    return g_display_is_dirty;
}
//set dirty area
void DisplaySetDirty(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height)
{
    M_DisplayPrint("set dirty: x_start:%d, y_start:%d, width:%d, height:%d\n",
        x_start, y_start, width, height);
    if(!g_display_is_dirty)
    {
        g_display_is_dirty = E_TRUE;
        g_display_dirty_rect.x = x_start;
        g_display_dirty_rect.y = y_start;
        g_display_dirty_rect.x_size = width;
        g_display_dirty_rect.y_size = height;
    }
    else
    {
        T_SINT x_end = g_display_dirty_rect.x+g_display_dirty_rect.x_size;
        T_SINT y_end = g_display_dirty_rect.y+g_display_dirty_rect.y_size;

        //use the maxed area
        if(x_start<g_display_dirty_rect.x)
        {
            g_display_dirty_rect.x = x_start; 
        }
        if(y_start<g_display_dirty_rect.y)
        {
            g_display_dirty_rect.y = y_start; 
        }
        
        if(x_end<x_start+width)
        {
            x_end = x_start+width;
        }
        g_display_dirty_rect.x_size = x_end - g_display_dirty_rect.x; 
        
        if(y_end<y_start+height)
        {
            y_end = y_start+height;
        }        
        g_display_dirty_rect.y_size = y_end - g_display_dirty_rect.y; 
    }
}


void DisplayWriteData(T_U8 page, T_U8 colume, T_U8 data)
{
    M_Assert(page<=PAGE_NUMBER_MAX&&colume<=COL_NUMBER_MAX);
    g_display_cache[page][colume]= data;
}

void DisplayWriteDataByPos(T_U8 row, T_U8 colume, T_U8 data)
{
    T_U8 page;

    page = (TE_PAGE_NUM)(row>>3);
    M_Assert(row<=ROW_NUMBER_MAX&&colume<=COL_NUMBER_MAX);
    g_display_cache[page][colume]= data;
}

/***********************************************
* Description:
*   draw a vertical line on the Lcd screen.
*
* Argument:
*   T_U8 col, T_U8 row_start, T_U8 row_end
*
* Return:
*	N/A
************************************************/
void DrawLineColumn(T_U8 col, T_U8 row_start, T_U8 row_end)
{
	assert(col <= COL_NUMBER_MAX && col >= COL_NUMBER_MIN);
	assert(row_start <= row_end);
	
	TE_PAGE_NUM page_start = (TE_PAGE_NUM)(row_start>>3);
	TE_PAGE_NUM page_end = (TE_PAGE_NUM)(row_end>>3);
	T_U8 data = 0x00;

	if(page_start < page_end)
	{
		data = GetOffsetOfPage(row_start, E_PAGE_OFFSET_TYPE_TOP);
		//DrvLcdSetPos(page_start, col);
		//DrvLcdWriteByte(E_LCD_DATA, data);
        DisplayWriteData(page_start, col, data);
		while(page_start < page_end)
		{
			page_start++;
			if(page_start < page_end)
			{
				//DrvLcdSetPos(page_start, col);
				//DrvLcdWriteByte(E_LCD_DATA, 0xFF);
                DisplayWriteData(page_start, col, 0xFF);
			}
			else
			{
				data = GetOffsetOfPage(row_end, E_PAGE_OFFSET_TYPE_BOTTOM);
				//DrvLcdSetPos(page_start, col);
				//DrvLcdWriteByte(E_LCD_DATA, data);
                DisplayWriteData(page_start, col, data);
			}
		}
	}
	else	//page_start == page_end
	{
		data = GetOffsetOfPage(row_start, E_PAGE_OFFSET_TYPE_TOP);
		data &= GetOffsetOfPage(row_end, E_PAGE_OFFSET_TYPE_BOTTOM);
		//DrvLcdSetPos(page_start, col);
		//DrvLcdWriteByte(E_LCD_DATA, data);
        DisplayWriteData(page_start, col, data);
	}
    
    DisplaySetDirty(col, row_start, 1, row_end-row_start+1);
}

/***********************************************
* Description:
*   draw a horizontal line on the Lcd screen.
*
* Argument:
*   T_U8 row, T_U8 col_start, T_U8 col_end
*
* Return:
*	N/A
************************************************/
void DrawLineRow(T_U8 row, T_U8 col_start, T_U8 col_end)
{
	assert(col_start <= COL_NUMBER_MAX && col_start >= COL_NUMBER_MIN);
	assert(col_end <= COL_NUMBER_MAX && col_end >= COL_NUMBER_MIN);
	assert(col_start <= col_end);
	
	TE_PAGE_NUM page = (TE_PAGE_NUM)(row>>3);
	T_U8 data = 0x00;
	T_U8 index = 0;
	
	data = GetOffsetOfPage(row, E_PAGE_OFFSET_TYPE_TOP);
	data &= GetOffsetOfPage(row, E_PAGE_OFFSET_TYPE_BOTTOM);

	for(index = col_start; index <= col_end; index++)
	{
		//DrvLcdSetPos(page, index);
		//DrvLcdWriteByte(E_LCD_DATA, data);
		DisplayWriteData(page, index, data);
	}
    
    DisplaySetDirty(col_start, row, col_end-col_start+1, 1);
}

/***********************************************
* Description:
*	draw a rectangle on the Lcd screen.
*
* Argument:
*	T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end
*
* Return:
*	N/A
************************************************/
void DrawRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end)
{
  assert(x_start <= COL_NUMBER_MAX && x_start >= COL_NUMBER_MIN);
  assert(x_end <= COL_NUMBER_MAX && x_end >= COL_NUMBER_MIN);
  assert(x_start <= x_end);
  assert(y_start <= y_end);	

  DrawLineColumn(x_start,y_start,y_end);
  DrawLineColumn(x_end,y_start,y_end);
  DrawLineRow(y_start,x_start,x_end);
  DrawLineRow(y_end,x_start,x_end);
}

/***********************************************
* Description:
*	draw a filled rectangle on the Lcd screen.
*
* Argument:
*	T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end
*
* Return:
*	N/A
************************************************/
void FillRectangle(T_U8 x_start, T_U8 y_start, T_U8 x_end, T_U8 y_end)
{
	assert(x_start <= COL_NUMBER_MAX && x_start >= COL_NUMBER_MIN);
	assert(x_end <= COL_NUMBER_MAX && x_end >= COL_NUMBER_MIN);
	assert(x_start <= x_end);
	assert(y_start <= y_end);

	T_U8 index = 0;
	for(index = x_start; index <= x_end; index++)
	{
		DrawLineColumn(index, y_start, y_end);
	}
}

/***********************************************
* Description:
*	draw text at line one, the top half of the Lcd screen.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type
*
* Return:
*	N/A
************************************************/
void DrawTextAtLineOne(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	assert(grid_x <= COL_NUMBER_MAX && grid_x >= COL_NUMBER_MIN);
	assert(type < E_DRAW_LCD_TYPE_MAX);
	T_U8 buff_len = 0;
	T_U8 index = 0;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			buff_len = CHINESE_WORD_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			buff_len = ASCII_CHAR_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			buff_len = ICON_ARRAY_LENGTH;
			break;
		default:
			buff_len = 0;
			break;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_ONE, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
		DisplayWriteData(E_PAGE_ONE, grid_x + index, *buffer);
		buffer++;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_TWO, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
        DisplayWriteData(E_PAGE_TWO, grid_x + index, *buffer);
		buffer++;
	}
    DisplaySetDirty(grid_x, 0, (buff_len>>1), DISPLAY_HEIGHT/2);
}

/***********************************************
* Description:
*	draw text at line two, the bottom half of the Lcd screen.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type
*
* Return:
*	N/A
************************************************/
void DrawTextAtLineTwo(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	assert(grid_x <= COL_NUMBER_MAX && grid_x >= COL_NUMBER_MIN);
	assert(type < E_DRAW_LCD_TYPE_MAX);
	T_U8 buff_len = 0;
	T_U8 index = 0;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			buff_len = CHINESE_WORD_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			buff_len = ASCII_CHAR_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			buff_len = ICON_ARRAY_LENGTH;
			break;
		default:
			buff_len = 0;
			break;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_THREE, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
        DisplayWriteData(E_PAGE_THREE, grid_x + index, *buffer);
		buffer++;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_FOUR, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
        DisplayWriteData(E_PAGE_FOUR, grid_x + index, *buffer);
		buffer++;
	}
    DisplaySetDirty(grid_x, DISPLAY_HEIGHT/2, (buff_len>>1), DISPLAY_HEIGHT/2);
}

/***********************************************
* Description:
*	draw text at middle line of the lcd screen.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type
*
* Return:
*	N/A
************************************************/
void DrawTextAtMiddleLine(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	assert(grid_x <= COL_NUMBER_MAX && grid_x >= COL_NUMBER_MIN);
	assert(type < E_DRAW_LCD_TYPE_MAX);
	T_U8 buff_len = 0;
	T_U8 index = 0;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			buff_len = CHINESE_WORD_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			buff_len = ASCII_CHAR_LENGTH;
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			buff_len = ICON_ARRAY_LENGTH;
			break;
		default:
			buff_len = 0;
			break;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_TWO, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
        DisplayWriteData(E_PAGE_TWO, grid_x + index, *buffer);
		buffer++;
	}
	for(index = 0; index < (buff_len>>1); index++)
	{
		//DrvLcdSetPos(E_PAGE_THREE, grid_x + index);
		//DrvLcdWriteByte(E_LCD_DATA, *buffer);
		DisplayWriteData(E_PAGE_THREE, grid_x + index, *buffer);
		buffer++;
	}
    DisplaySetDirty(grid_x, 8, (buff_len>>1), DISPLAY_HEIGHT/2);
}

static void TextFlashAtLineOne()
{
	is_orignal_text = (is_orignal_text == E_TRUE) ? E_FALSE : E_TRUE;
	if(is_orignal_text)
		DrawTextAtLineOne(text_pos_x, text_buffer, text_type);
	else
		DrawTextAtLineOne(text_pos_x, reversed_buffer, text_type);
}

static void TextFlashAtLineTwo()
{
	is_orignal_text = (is_orignal_text == E_TRUE) ? E_FALSE : E_TRUE;
	if(is_orignal_text)
		DrawTextAtLineTwo(text_pos_x, text_buffer, text_type);
	else
		DrawTextAtLineTwo(text_pos_x, reversed_buffer, text_type);
}

static void TextFlashAtMiddleLine()
{
	is_orignal_text = (is_orignal_text == E_TRUE) ? E_FALSE : E_TRUE;
	if(is_orignal_text)
		DrawTextAtMiddleLine(text_pos_x, text_buffer, text_type);
	else
		DrawTextAtMiddleLine(text_pos_x, reversed_buffer, text_type);
}

/***********************************************
* Description:
*	draw flashed text at line 1.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type, TE_TEXT_POSITION pos
*
* Return:
*	N/A
************************************************/
void DrawTextAtLineOneWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	text_pos_x = grid_x;
	text_type = type;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			for(int i = 0; i < 32; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			for(int i = 0; i < 16; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			for(int i = 0; i < 48; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		default:
			break;
	}
    //ReverseFontBuffer(reversed_buffer,buffer, type);
    is_orignal_text = E_TRUE;
	DrawTextAtLineOne(grid_x, buffer, type);
	CtlTimerBindScreenHandle(E_TIMER_DISPLAY_FLASH, TextFlashAtLineOne);
	CtlTimerSetRepeat(E_TIMER_DISPLAY_FLASH, TEXT_DISPLAY_FLASH_MS);
}

/***********************************************
* Description:
*	draw flashed text at line 2.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type, TE_TEXT_POSITION pos
*
* Return:
*	N/A
************************************************/
void DrawTextAtLineTwoWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	text_pos_x = grid_x;
	text_type = type;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			for(int i = 0; i < 32; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			for(int i = 0; i < 16; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			for(int i = 0; i < 48; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		default:
			break;
	}
    //ReverseFontBuffer(reversed_buffer,buffer, type);
    is_orignal_text = E_TRUE;
	DrawTextAtLineTwo(grid_x, buffer, type);
	CtlTimerBindScreenHandle(E_TIMER_DISPLAY_FLASH, TextFlashAtLineTwo);
	CtlTimerSetRepeat(E_TIMER_DISPLAY_FLASH, TEXT_DISPLAY_FLASH_MS);
}

/***********************************************
* Description:
*	draw flashed text at middle line.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type, TE_TEXT_POSITION pos
*
* Return:
*	N/A
************************************************/
void DrawTextAtMiddleLineWithFlash(T_U8 grid_x, T_U8 const* buffer, TE_DRAW_LCD_TYPE type)
{
	text_pos_x = grid_x;
	text_type = type;
	switch(type)
	{
		case E_DRAW_LCD_TYPE_CHINESE_WORD:
			for(int i = 0; i < 32; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ASCII_CHAR:
			for(int i = 0; i < 16; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		case E_DRAW_LCD_TYPE_ICON_ARRAY:
			for(int i = 0; i < 48; i++)
			{
				text_buffer[i] = buffer[i];
				reversed_buffer[i] = ~ (T_UINT)buffer[i];
			}
			break;
		default:
			break;
	}
    //ReverseFontBuffer(reversed_buffer,buffer, type);
    is_orignal_text = E_TRUE;
	DrawTextAtMiddleLine(grid_x, buffer, type);
	CtlTimerBindScreenHandle(E_TIMER_DISPLAY_FLASH, TextFlashAtMiddleLine);
	CtlTimerSetRepeat(E_TIMER_DISPLAY_FLASH, TEXT_DISPLAY_FLASH_MS);
}

/***********************************************
* Description:
*	draw flashed icon at line two.
*
* Argument:
*	T_U8 grid_x, T_U8* buffer, TE_DRAW_LCD_TYPE type
*
* Return:
*	N/A
************************************************/
void DrawFlashIconAtLineTwo(T_U8 grid_x, T_U8 const* buffer)
{
	text_pos_x = grid_x;
	text_type = E_DRAW_LCD_TYPE_ICON_ARRAY;
	
	for(int i = 0; i < 48; i++)
	{
		text_buffer[i] = buffer[i];
		reversed_buffer[i] = 0x00;
	}
    //ReverseFontBuffer(reversed_buffer,buffer, type);
    is_orignal_text = E_TRUE;
	DrawTextAtLineTwo(grid_x, buffer, E_DRAW_LCD_TYPE_ICON_ARRAY);
	CtlTimerBindScreenHandle(E_TIMER_DISPLAY_FLASH, TextFlashAtLineTwo);
	CtlTimerSetRepeat(E_TIMER_DISPLAY_FLASH, TEXT_DISPLAY_FLASH_MS);
}

// void ClearBottomLine()
// {
// 	//if(CtlTimerIsExist(E_TIMER_UPDATE_MOTOR_FREQUENCY) == E_TRUE)
// 		//CtlTimerClear(E_TIMER_UPDATE_MOTOR_FREQUENCY);
// 	for(int idx = 0; idx < 16; idx ++)
// 		DrawTextAtLineTwo(idx*8,&gg_song_typeface_16[178+32*0x88+16*0x0a], E_DRAW_LCD_TYPE_ASCII_CHAR);
// }


//clear all display
void DisplayClearAll()
{
    memset(g_display_cache, 0, sizeof(g_display_cache));
    M_DisplaySetAllDirty();
}
//clear the display, x_start:0~127, y_start:0~31
void DisplayClear(T_U8 x_start, T_U8 y_start, T_U8 width, T_U8 height)
{
    T_SINT start_page;
    T_SINT end_page;
    T_SINT i,j;

    M_Assert(x_start<DISPLAY_WIDTH&&y_start<DISPLAY_HEIGHT);
    if(x_start+width>DISPLAY_WIDTH)
    {
        width = DISPLAY_WIDTH - x_start;
    }
    if(y_start+height>DISPLAY_HEIGHT)
    {
        height = DISPLAY_HEIGHT - y_start;
    }
    start_page = y_start/8;
    end_page = (y_start+height-1)/8;
    for(i=start_page; i<=end_page; i++)
    {
        for(j=x_start; j<x_start+width; j++)
        {           
           g_display_cache[i][j]= 0;
        }
    }    
    DisplaySetDirty(x_start, y_start, width, height);
}


void DisplaySetFontLib(const TS_FONT_INFO* font_lib)
{
    g_display_font_lib = font_lib;
}

//get the string's width
T_S16 DisplayGetXSize(const T_CHAR* str)
{
    //T_CHAR a;
    T_CHAR ch[2];
    T_S16 xsize = 0;
    TS_CHAR_INFOR ch_info;

    M_Assert(g_display_font_lib);
    M_Assert(str);
    for(;*str!=0;str++)
    {
        ch[0] = *str;
        if(*str>=0x81 && *str<=0xFE)//GBK encoder
        {
            str++;
            ch[1] = *str;
        }
        else
        {
            ch[1] = 0;
        }
        if(FontGetCharInfo(g_display_font_lib, ch, &ch_info))
        {
            xsize += ch_info.width;
        }
        else
        {
            M_Assert(0);
        }
    }

    return xsize;
}

T_SINT DisplayOneCharAtPos(T_U8 x_start, T_U8 y_start, T_CHAR ch[2])
{
    TS_CHAR_INFOR ch_info;

    M_Assert(g_display_font_lib);
    if(FontGetCharInfo(g_display_font_lib, ch, &ch_info))
    {
        T_SINT i,j;
        T_U8 page = (TE_PAGE_NUM)(y_start>>3);;
        const T_U8* p = ch_info.data;

        for(j=0; j<ch_info.height_byte; j++)
        {
            for(i=0; i<ch_info.width; i++)
            {
                M_Assert( (p-ch_info.data)<=ch_info.data_size);
                DisplayWriteData(page, x_start+i,*p);
                p++;
            }
            page++;
        }
        
        DisplaySetDirty(x_start, y_start, ch_info.width, ch_info.height);
        return ch_info.width;
    }
    else
    {
        M_Assert(0);
        return 0;
    }
    
}

/*ÏÔÊ¾ÎÄ×Ö*/
void DisplayTextAtPos(T_U8 x_start, T_U8 y_start, const T_CHAR* str)
{
    T_CHAR ch[2];

    for(;*str!=0;str++)
    {
        ch[0] = *str;
        if(*str>=0x81 && *str<=0xFE)//GBK encoder
        {
            str++;
            ch[1] = *str;
        }
        else
        {
            ch[1] = 0;
        }
        x_start += DisplayOneCharAtPos(x_start, y_start, ch);
    }
    
}

//display at center of x
void DisplayTextAtXCenter(T_U8 y_start, const T_CHAR* str)
{
    T_SINT xsize = DisplayGetXSize(str);

    DisplayTextAtPos((DISPLAY_WIDTH-xsize)/2, y_start, str);
}


void DisplayInit()
{
    g_display_font_lib = &gg_ASCII_font;
}

