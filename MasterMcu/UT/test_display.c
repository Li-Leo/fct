#ifdef UNIT_TEST
#include "TypeDefine.h"
#include "DrvPower.h"
#include "DrvBeep.h"
#include "CtlTimer.h"
//#include "DrvAdc.h"
#include "stdio.h"
#include "DrvTimer.h"
#include "CtlTime.h"
#include "CtlDisplay.h"
#include "fonts.h"

void test_display_new()
{
    //T_U16 ch_code;
    T_CHAR sz[3] = {0};

    DisplaySetFontLib(&gg_songti16_font);

    //show string
    DisplayTextAtXCenter(MIDDLE_LINE_Y, "Ò©Á¿: 0.5 g");
    DisplayFlushDirty();
    DelayMs(20000);    
    
    //show hanzi
    #if 1
    printf("show hanzi\n");
    for(int i=0; i<144; i++)
    {
        sz[0] = gg_song_typeface_16_char_codes[i][0];
        sz[1] = gg_song_typeface_16_char_codes[i][1];
        printf("show %d char [%s]\n", i+1, sz);
        DisplayTextAtXCenter(MIDDLE_LINE_Y, sz);
        DisplayFlushDirty();
        DelayMs(2000);
    }
    #endif
    //show english
    printf("show english\n");
    for(int i=144; i<176; i++)
    {
        sz[0] = gg_song_typeface_16_char_codes[i][0];
        sz[1] = gg_song_typeface_16_char_codes[i][1];
        printf("show %d char [%s]\n", i+1, sz);
        DisplayTextAtXCenter(MIDDLE_LINE_Y, sz);
        DisplayFlushDirty();
        DelayMs(2000);
    }
}

void test_display()
{
    test_display_new();
    while(1)
    {
        //DisplayClearAll();
        DrawLineColumn(0, 0 ,ROW_NUMBER_MAX);
        DisplayFlushDirty();
        DelayMs(2000);
        //DisplayClearAll();
        DrawLineColumn(COL_NUMBER_MAX, 0 ,ROW_NUMBER_MAX);
        DisplayFlushDirty();
        DelayMs(2000);
        //DisplayClearAll();
        DrawLineRow(0, 0, COL_NUMBER_MAX);
        DisplayFlushDirty();
        DelayMs(2000);
        //DisplayClearAll();
        DrawLineRow(ROW_NUMBER_MAX, 0, COL_NUMBER_MAX);
        DisplayFlushDirty();
        DelayMs(2000);


        
        DisplayClearAll();
        DrawTextAtLineOne(0 + 16, &gg_song_typeface_16[178+32*0x3f], E_DRAW_LCD_TYPE_CHINESE_WORD);
        DisplayFlushDirty();
        DelayMs(2000);
        DisplayClearAll();
        DrawTextAtLineOne(0 + 32, &gg_song_typeface_16[178+32*0x44], E_DRAW_LCD_TYPE_CHINESE_WORD);
        DisplayFlushDirty();
        DelayMs(2000);
        DisplayClearAll();
        DrawTextAtLineOne(0 + 48, &gg_song_typeface_16[178+32*0x14], E_DRAW_LCD_TYPE_CHINESE_WORD);
        DisplayFlushDirty();
        DelayMs(2000);
        DisplayClearAll();
        DrawTextAtLineOne(0 + 64, &gg_song_typeface_16[178+32*0x26], E_DRAW_LCD_TYPE_CHINESE_WORD);
        DisplayFlushDirty();
        DelayMs(2000);
        DisplayClearAll();
        DrawTextAtLineOne(0 + 80, &gg_song_typeface_16[178+32*0x88+16*0x0e], E_DRAW_LCD_TYPE_ASCII_CHAR); 
        DisplayFlushDirty();
        DelayMs(2000);
        DisplayClearAll();
        DrawTextAtLineTwo(0 + 80, &gg_song_typeface_16[178+32*0x88+16*0x0e], E_DRAW_LCD_TYPE_ASCII_CHAR); 
        DisplayFlushDirty();
        DelayMs(2000);
    }
}

#endif
