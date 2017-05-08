//**********************************************************************
//文件名:menu.c
//**********************************************************************
#include "FontLib.h"
#include "disp.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "graph.h"
#include "menu.h"
#include "type_def.h"
#include "stdio.h"
#include "global.h"

//**********************************************************************
//函数  名:Put_CH_Butt_Lg_Sel
//函数功能:在语言选择对话框中显示"中文"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_CH_Butt_Lg_Sel(uint8 disp_mode)
{
    uint16 di;
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);  
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);   
    Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    Bar(99,129,153,147);
    
    if (!(disp_mode&DISP_ER))//不擦除则显示标题
    {
       // Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_Chr_Color(COL_THINCYAN,COL_DEEPGREEN); 
        di = Get_Str_Width("中文")/2;
        Put_Line_Str(126-di,131,"中文");
    }    
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(99,129,153,147);
    }
}

//**********************************************************************
//函数  名:Put_EN_Butt_Lg_Sel
//函数功能:在语言选择对话框中显示"English"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_EN_Butt_Lg_Sel(uint8 disp_mode)
{
    uint16 di;
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN); 
    Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    Bar(170,129,224,147);
    if (!(disp_mode&DISP_ER))//不擦除则显示标题
    {
        Set_ASC_Lib(Arial10B_Font);
        //Set_Write_Mode(WRITE_EXOR);
        Set_Chr_Color(COL_THINCYAN,COL_DEEPGREEN); 
        di = Get_Str_Width("English")/2;
        Put_Line_Str(197-di,131,"English");
    }    
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(170,129,224,147);
    }
}

//**********************************************************************
//函数  名:Init_LG_Sel_Menu
//函数功能:初始化语言选择界面
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
void Init_LG_Sel_Menu(sint32 op)
{

    //Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Put_Image(78,65,Language_Sel_Box,WRITE_OVERLAY);
    
    Put_CH_Butt_Lg_Sel(DISP_NOT_RT);//"中文"
    Put_EN_Butt_Lg_Sel(DISP_NOT_RT);//"English"
    
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}