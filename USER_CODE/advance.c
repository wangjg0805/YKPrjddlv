//**********************************************************************
//文件名:advance.c
//**********************************************************************
#include "FontLib.h"
#include "disp.h"
//#include "graph.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "key_scan.h"
#include "timer.h"
#include "global.h"
#include "menu.h"
#include "data_pro.h"
#include "i2c_com.h"
#include "speaker.h"
#include "eeprom.h"
#include "ad_acquire.h"
#include "type_def.h"
#include "port.h"
#include "stdio.h"
#include "data_pro.h"
#include "alarm_pro.h"
#include "sys_para.h"
#include "advance.h"

//选项显示
static void Put_Vte_Filt_Const_Box(uint8 disp_mode,uint8 para,uint8 st);
static void Put_O2_Flash_Cyc_Box(uint8 disp_mode,uint8 para,uint8 st);
static void Put_Pl_Comp_Box(uint8 disp_mode,sint16 para,uint8 st);
static void Put_Paw_No_Raise_Box(uint8 disp_mode,sint32 para,uint8 st);
//菜单界面初始化函数
static void Init_Advc_Para_Menu(sint32 op);
//界面处理函数
static sint32 Menu_Adv_Para_Op_Pro(uint8 key_c,sint32 op);
static sint32 Menu_Lg_Sel_Op_Pro(uint8 key_c,sint32 op);
//选项处理函数
static sint32 Ch_Butt_Pro(uint8 key_c);
static sint32 En_Butt_Pro(uint8 key_c);
static sint32 Vt_Filt_Const_Box_Pro(uint8 key_c);
static sint32 O2_Flash_Cyc_Box_Pro(uint8 key_c);
static sint32 Pl_Comp_Box_Pro(uint8 key_c);
static sint32 Paw_No_R_Box_Pro(uint8 key_c);

static uint8 ad_op_st;

//**********************************************************************
//函数  名:Advance_Manage
//函数功能:高级参数模式下的菜单切换处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Advance_Manage(void)
{
    struct menu_str menu[2] = 
    {{MENU_AD_LG_SEL,Init_LG_Sel_Menu,        Menu_Lg_Sel_Op_Pro,        NULL,NULL},    //语言选择界面
     {MENU_AD_PARA,     Init_Advc_Para_Menu,    Menu_Adv_Para_Op_Pro,    NULL,NULL}};//主界面
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
    uint8 key_c;            //按键号
    uint8 lcd_ref_interval = 0;
    sint16 lcd_line=0;    //LCD 分时刷新的起始刷新行数
        
    Set_Key_Sound_Switch(ON);    //高级参数模式下按键音开
    cur_op =0xffffffff ;  
    next_op = MENU_AD_LG_SEL_CH_OP;
    ad_op_st |= OP_ST_SEL;
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_c = Get_Key_Code();        //键盘扫描
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_c,cur_op);    //选项处理
           // Check_Extend_Port();    //检查外部扩展口
            Check_Batt_Supply_Status();    //检查电池的状态
        }    
        
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;
            //显示机动/手动呼吸灯 modify
            if(phase_vent_ctrl == PHASE_NO_VENT)
            {
                LED_Indicate_Off(EO2_LED_VENT);
                LED_Indicate_On(EO2_LED_BAG);
            }
            else
            {
                LED_Indicate_Off(EO2_LED_BAG);
                LED_Indicate_On(EO2_LED_VENT);
            }
            LED_Indicate_Show();
            Batt_Alarm_Pro();    
            lcd_ref_interval++;
            lcd_ref_interval &= 0x01;
            if (!lcd_ref_interval)
            {        
                LCD_Refresh_Area(0,lcd_line,319,lcd_line+23);//分时刷新LCD
                lcd_line += 24;
                if (lcd_line > 216)    //从头刷新
                {
                    lcd_line = 0; 
                }
            }                        
        }
        
        if (flag_500ms_ok)
        {//500ms时间到
            flag_500ms_ok = FALSE;
            LED_Indicate_Cpl(EO2_LED_CPU_S); //CPU工作指示灯闪烁频率1Hz
            LED_Indicate_Show();
        }        
    }
}

//**********************************************************************
//函数  名:Put_Vte_Filt_Const_Box
//函数功能:显示"潮气量滤波常数"输入框
//入口参数:disp_mode--显示方式
//           para--显示参数
//           st--输入框的状态(选中/输入)
//出口参数:无
//**********************************************************************
static void Put_Vte_Filt_Const_Box(uint8 disp_mode,uint8 para,uint8 st)
{
      Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    /*if (st&OP_ST_SEL)    //选中状态
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //正在输入参数
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,33,313,49);   //清除显示
        Set_Chr_Color(COL_BLACK,COL_WHITE);
       Rectangle(157,34,312,48);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,33,313,49);    //清除显示
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
   
   if (!(disp_mode&DISP_ER))//不擦除标题文字则显示
    {
        Set_ASC_Lib(Arial10B_Font);    //设置字库
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//处在选中状态
        {
            Set_Write_Mode(WRITE_EXOR);    //异或显示    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //或显示            
        }*/
        Put_Numeral(162,35,para,1);    //显示设置值        
    }
    if (disp_mode&DISP_RT)//实时刷新
    {
        LCD_Refresh_Area(156,33,313,49);    
    }    
}

//**********************************************************************
//函数  名:Put_O2_Flash_Cyc_Box
//函数功能:显示"氧浓度刷新周期"输入框
//入口参数:disp_mode--显示方式
//           para--显示参数
//           st--输入框的状态(选中/输入)
//出口参数:无
//**********************************************************************
static void Put_O2_Flash_Cyc_Box(uint8 disp_mode,uint8 para,uint8 st)
{
      Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    /*if (st&OP_ST_SEL)    //选中状态
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //正在输入参数
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,51,313,67);    //清除显示
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,52,312,66);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
       Bar(156,51,313,67);    //清除显示
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
   
   
    if (!(disp_mode&DISP_ER))//不擦除标题文字则显示
    {
        Set_ASC_Lib(Arial10B_Font);    //设置字库
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//处在选中状态
        {
            Set_Write_Mode(WRITE_EXOR);    //异或显示    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //或显示            
        }*/
        Put_Numeral(162,53,para,10);    //显示设置值        
    }
    if (disp_mode&DISP_RT)//实时刷新
    {
        LCD_Refresh_Area(156,51,313,67);    
    }    
}

//**********************************************************************
//函数  名:Put_Pl_Comp_Box
//函数功能:显示"压力限制补偿"输入框
//入口参数:disp_mode--显示方式
//           para--显示参数
//           st--输入框的状态(选中/输入)
//出口参数:无
//**********************************************************************
static void Put_Pl_Comp_Box(uint8 disp_mode,sint16 para,uint8 st)
{
   Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    /*if (st&OP_ST_SEL)    //选中状态
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //正在输入参数
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,69,313,85);   //清除显示
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,70,312,84);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,69,313,85);    //清除显示
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
  
    if (!(disp_mode&DISP_ER))//不擦除标题文字则显示
    {
        Set_ASC_Lib(Arial10B_Font);    //设置字库
        //Set_Chr_Color(COL_WHITE,COL_BLACK);
        /*if (st&OP_ST_SEL)//处在选中状态
        {
            Set_Write_Mode(WRITE_EXOR);    //异或显示    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //或显示            
        }*/
        Put_Numeral(162,71,para,10);    //显示设置值        
    }
    if (disp_mode&DISP_RT)//实时刷新
    {
        LCD_Refresh_Area(156,69,313,85);    
    }    
}

//**********************************************************************
//函数  名:Put_Paw_No_Raise_Box
//函数功能:显示"管压无提升"输入框
//入口参数:disp_mode--显示方式
//           para--显示参数
//           st--输入框的状态(选中/输入)
//出口参数:无
//**********************************************************************
static void Put_Paw_No_Raise_Box(uint8 disp_mode,sint32 para,uint8 st)
{
    Set_Write_Mode(WRITE_OVERLAY);    //覆盖显示
    /*if (st&OP_ST_SEL)    //选中状态
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);        
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
    }*/
    if (st&OP_ST_IN)    //正在输入参数
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE); 
        Bar(156,87,313,103);    //清除显示
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Rectangle(157,88,312,102);
    }
    else
     {Set_Graphic_Color(COL_BLACK,COL_BLACK); 
      Bar(156,87,313,103);    //清除显示
      Set_Chr_Color(COL_WHITE,COL_BLACK);
     }
    if (!(disp_mode&DISP_ER))//不擦除标题文字则显示
    {
        Set_ASC_Lib(Arial10B_Font);    //设置字库
        /*if (st&OP_ST_SEL)//处在选中状态
        {
            Set_Write_Mode(WRITE_EXOR);    //异或显示    
        }
        else
        {
            Set_Write_Mode(WRITE_OR);    //或显示            
        }*/
        Put_Numeral(162,89,para,10);    //显示设置值        
    }
    if (disp_mode&DISP_RT)//实时刷新
    {
        LCD_Refresh_Area(156,87,313,103);    
    }    
}


//**********************************************************************
//函数  名:Init_Advc_Para_Menu
//函数功能:初始化高级参数界面
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
static void Init_Advc_Para_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //主外框
    Hor_Line(6,313,27);        //标题
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Pattern_Bar(6,6,313,26,0xFF);
    //Bar(36,7,283,25);
        
    //Set_Write_Mode(WRITE_EXOR);//异或模式
    
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_THINGREEN,COL_DEEPGREEN);
    str = Language_Str("高级参数","Advanced Parameter");
    di = 159-Get_Str_Width(str)/2;    
    Put_Line_Str(di,9,str);
    
    //画表头
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    for (di=0; di<5; di++)
    {
        Hor_Line(6,313,32+di*18);
    }
    Ver_Line(33,103,155);
    //显示标题
    //Set_Write_Mode(WRITE_OR);
    Set_ASC_Lib(Arial10B_Font);
    Set_HZ_Lib(&ST_11);    
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(7,34,Language_Str("潮气量滤波常数","Vt Filt Const"));
    Put_Line_Str(7,52,Language_Str("氧浓度刷新周期(S)","O2 Flash Cycle(S)"));
    Put_Line_Str(7,70,Language_Str("压力限制补偿(cmH2O)","Plimit Comp(cmH2O)"));
    Put_Line_Str(7,88,Language_Str("管压无提升(cmH2O)","Paw No Raise(cmH2O)"));

    //显示设置值
    Put_Vte_Filt_Const_Box(DISP_NOT_ER,Get_Vt_Filter_Const(),OP_ST_NONE);
    Put_O2_Flash_Cyc_Box(DISP_NOT_ER,Get_O2_Flash_Cyc(),OP_ST_NONE);
    Put_Pl_Comp_Box(DISP_NOT_ER,Get_Pl_Comp(),OP_ST_NONE);
    Put_Paw_No_Raise_Box(DISP_NOT_ER,Get_Paw_No_Raise(),OP_ST_NONE);

    //显示表格标题
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数  名:Menu_Adv_Para_Op_Pro
//函数功能:高级参数菜单的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static sint32 Menu_Adv_Para_Op_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_AD_PARA_OP_NUM]=
    {{MENU_AD_VT_FILT_CONST_OP, Vt_Filt_Const_Box_Pro},//潮气量滤波常数
    {MENU_AD_O2_FLASH_CYC_OP,    O2_Flash_Cyc_Box_Pro},//氧浓度刷新周期
    {MENU_AD_PL_COMP_OP,        Pl_Comp_Box_Pro},    //压力限制补偿
    {MENU_AD_PAW_NO_R_OP,        Paw_No_R_Box_Pro}};//管压无提升

    static uint8 time; //闪烁时间

    if (!time)    //mod32(320ms周期闪烁显示)
    {
        switch (op)
        {
            case MENU_AD_VT_FILT_CONST_OP://潮气量滤波常数    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),ad_op_st);
                }
                else//原来显示则擦除
                {
                    Put_Vte_Filt_Const_Box(DISP_RT|DISP_ER,Get_Vt_Filter_Const(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_O2_FLASH_CYC_OP://氧浓度刷新周期    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),ad_op_st);
                }
                else//原来显示则擦除
                {
                    Put_O2_Flash_Cyc_Box(DISP_RT|DISP_ER,Get_O2_Flash_Cyc(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;    
            case MENU_AD_PL_COMP_OP://压力限制补偿    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),ad_op_st);
                }
                else//原来显示则擦除
                {
                    Put_Pl_Comp_Box(DISP_RT|DISP_ER,Get_Pl_Comp(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_PAW_NO_R_OP://管压无提升门限    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),ad_op_st);
                }
                else//原来显示则擦除
                {
                    Put_Paw_No_Raise_Box(DISP_RT|DISP_ER,Get_Paw_No_Raise(),ad_op_st);
                }
                ad_op_st ^= OP_ST_ER;
                break;                                    
            default:break;    
        }
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //切换了菜单界面则清除闪烁计时
    {
        time = 0;
    }
    return (op_id);
}

//**********************************************************************
//函数  名:Vt_Filt_Const_Box_Pro
//函数功能:潮气量滤波常数输入窗口处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Vt_Filt_Const_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//保存设置值
                ad_op_st &= ~OP_ST_IN;        //去掉输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st |= OP_ST_SEL;        //选中状态
                if (ERROR == Save_Vt_Filt_C())
                {//保存失败
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //发按键有效音        
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_SEL);    //显示窗口
            }
            else//非输入状态
            {//状态修改为输入状态
                ad_op_st |= OP_ST_IN;    //进入输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st &= ~OP_ST_SEL;    //去掉选中状态
                Key_Valid_Sound();        //发按键有效音        
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //显示窗口
            }                
            return(MENU_AD_VT_FILT_CONST_OP);//仍然处在"潮气量滤波常数"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Vt_Filt_C(INC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                        
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_VT_FILT_CONST_OP);//仍然处在"潮气量滤波常数"
            }
            else//非输入状态
            {
                Key_No_Valid_Sound();        //发按键有效音                    
                return(MENU_AD_VT_FILT_CONST_OP);//仍然处在"潮气量滤波常数"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://减少键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Vt_Filt_C(DEC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_VT_FILT_CONST_OP);//仍然处在"潮气量滤波常数"
            }
            else//非输入状态
            {//切换选项
                Key_Valid_Sound();        //发按键有效音                
                Put_Vte_Filt_Const_Box(DISP_RT,Get_Vt_Filter_Const(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_O2_FLASH_CYC_OP);//切换到"氧气浓度刷新周期"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_VT_FILT_CONST_OP);//仍然处在"潮气量滤波常数"
            break;            
    }        
}

//**********************************************************************
//函数  名:O2_Flash_Cyc_Box_Pro
//函数功能:氧气浓度刷新周期输入窗口处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 O2_Flash_Cyc_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//保存设置值
                ad_op_st &= ~OP_ST_IN;        //去掉输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st |= OP_ST_SEL;        //选中状态
                if (ERROR == Save_O2_Flash_Cyc())
                {//保存失败
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //发按键有效音                    
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_SEL);    //显示窗口
            }
            else//非输入状态
            {//状态修改为输入状态
                ad_op_st |= OP_ST_IN;    //进入输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st &= ~OP_ST_SEL;    //去掉选中状态
                Key_Valid_Sound();        //发按键有效音            
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //显示窗口
            }                
            return(MENU_AD_O2_FLASH_CYC_OP);//仍然处在"氧气浓度刷新周期"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_O2_Flash_Cyc(INC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_O2_FLASH_CYC_OP);//仍然处在"氧气浓度刷新周期"
            }
            else//非输入状态
            {
                Key_Valid_Sound();        //发按键有效音                    
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_VT_FILT_CONST_OP);//切换到"潮气量滤波常数"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://减少键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_O2_Flash_Cyc(DEC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                    
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_O2_FLASH_CYC_OP);//仍然处在"氧气浓度刷新周期"
            }
            else//非输入状态
            {//切换选项
                Key_Valid_Sound();        //发按键有效音            
                Put_O2_Flash_Cyc_Box(DISP_RT,Get_O2_Flash_Cyc(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_PL_COMP_OP);//切换到"压力限制补偿"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_O2_FLASH_CYC_OP);//仍然处在"氧气浓度刷新周期"
            break;            
    }        
}

//**********************************************************************
//函数  名:Pl_Comp_Box_Pro
//函数功能:压力限制补偿输入窗口处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Pl_Comp_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//保存设置值
                ad_op_st &= ~OP_ST_IN;        //去掉输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st |= OP_ST_SEL;        //选中状态
                if (ERROR == Save_Pl_Comp())
                {//保存失败
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //发按键有效音                
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_SEL);    //显示窗口
            }
            else//非输入状态
            {//状态修改为输入状态
                ad_op_st |= OP_ST_IN;    //进入输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st &= ~OP_ST_SEL;    //去掉选中状态
                Key_Valid_Sound();        //发按键有效音            
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //显示窗口
            }                
            return(MENU_AD_PL_COMP_OP);//仍然处在"压力限制补偿"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Pl_Comp(INC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                    
                }
                else
                {
                    Key_No_Valid_Sound();                    
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_PL_COMP_OP);//仍然处在"压力限制补偿"    
            }
            else//非输入状态
            {
                Key_Valid_Sound();        //发按键有效音                
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_O2_FLASH_CYC_OP);//切换到"氧浓度刷新周期"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://减少键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Pl_Comp(DEC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                    
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_PL_COMP_OP);//仍然处在"压力限制补偿"
            }
            else//非输入状态
            {//切换选项
                Key_Valid_Sound();        //发按键有效音                
                Put_Pl_Comp_Box(DISP_RT,Get_Pl_Comp(),OP_ST_NONE);
                ad_op_st = OP_ST_SEL;
                return(MENU_AD_PAW_NO_R_OP);//切换到"管压无提升"
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_PL_COMP_OP);//仍然处在"压力限制补偿"
            break;            
    }        
}

//**********************************************************************
//函数  名:Paw_No_R_Box_Pro
//函数功能:管压无提升输入窗口处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Paw_No_R_Box_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//保存设置值
                ad_op_st &= ~OP_ST_IN;        //去掉输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st |= OP_ST_SEL;        //选中状态
                if (ERROR == Save_Paw_No_R())
                {//保存失败
                    Key_No_Valid_Sound();
                }
                else
                {
                    Key_Valid_Sound();        //发按键有效音                
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_SEL);    //显示窗口
            }
            else//非输入状态
            {//状态修改为输入状态
                ad_op_st |= OP_ST_IN;    //进入输入状态
                ad_op_st &= ~OP_ST_CHG;    //去掉修改标记
                ad_op_st &= ~OP_ST_SEL;    //去掉选中状态
                Key_Valid_Sound();        //发按键有效音            
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //显示窗口
            }                
            return(MENU_AD_PAW_NO_R_OP);//仍然处在"管压无提升"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Paw_No_R(INC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                        
                }
                else
                {
                    Key_No_Valid_Sound();                
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_PAW_NO_R_OP);//仍然处在"管压无提升"        
            }
            else//非输入状态
            {
                Key_Valid_Sound();        //发按键有效音                
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_NONE);    
                ad_op_st = OP_ST_SEL;                        
                return(MENU_AD_PL_COMP_OP);//切换到"压力限制补偿"
            }        
            break;    
        case KEY_PRESSED|KEY_DEC://减少键
            if (ad_op_st&OP_ST_IN)//正在输入状态
            {//修改设置值
                ad_op_st |= OP_ST_CHG;        //作修改标记
                if (Inc_Dec_Paw_No_R(DEC,1) == OK)
                {//修改成功
                    Key_Valid_Sound();        //发按键有效音                
                }
                else
                {
                    Key_No_Valid_Sound();    
        
                }
                Put_Paw_No_Raise_Box(DISP_RT,Get_Paw_No_Raise(),OP_ST_IN);    //显示修改后的值
                return(MENU_AD_PAW_NO_R_OP);//仍然处在"管压无提升"        
            }
            else//非输入状态
            {//切换选项
                Key_No_Valid_Sound();        //发按键无 效音
                return(MENU_AD_PAW_NO_R_OP);//仍然处在"管压无提升"        s
            }                        
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_PAW_NO_R_OP);//仍然处在"管压无提升"        
            break;            
    }        
}

//**********************************************************************
//函数  名:Menu_Lg_Sel_Op_Pro
//函数功能:语言设置菜单的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static sint32 Menu_Lg_Sel_Op_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_AD_LG_OP_NUM]=
    {{MENU_AD_LG_SEL_CH_OP,    Ch_Butt_Pro},//"中文"
    {MENU_AD_LG_SEL_EN_OP,    En_Butt_Pro}};//"English"
    static uint8 time; //闪烁时间

    if (!time)    //mod32(320ms周期闪烁显示)
    {
        switch (op)
        {
            case MENU_AD_LG_SEL_CH_OP://中文    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_CH_Butt_Lg_Sel(DISP_RT);
                }
                else//原来显示则擦除
                {
                    Put_CH_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                ad_op_st ^= OP_ST_ER;
                break;        
            case MENU_AD_LG_SEL_EN_OP://English    
                if (ad_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_EN_Butt_Lg_Sel(DISP_RT);
                }
                else//原来显示则擦除
                {
                    Put_EN_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                ad_op_st ^= OP_ST_ER;
                break;                                    
            default:break;    
        }
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c);
    if ((op_id^op)&0x0000ffff)    //切换了菜单界面则清除闪烁计时
    {
        time = 0;
    }
    return (op_id);
}

//**********************************************************************
//函数  名:Ch_Butt_Pro
//函数功能:"中文"按钮的处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Ch_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            ad_op_st |= OP_ST_SEL;        //选中状态
            Set_Language_Type(CHINESE);    //设置语言类型
            Key_Valid_Sound();        //发按键有效音        
            return(MENU_AD_VT_FILT_CONST_OP);//切换到"潮气量滤波常数"        
            break;
        case KEY_PRESSED|KEY_DEC://减少键
            Key_Valid_Sound();        //发按键有效音                
            if (ad_op_st&OP_ST_ER)    //恢复显示
            {
                Put_CH_Butt_Lg_Sel(DISP_RT);
            }    
            ad_op_st = OP_ST_SEL;
            return(MENU_AD_LG_SEL_EN_OP);//切换到"English"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_LG_SEL_CH_OP);//仍然处在"中文"
            break;            
    }        
}

//**********************************************************************
//函数  名:En_Butt_Pro
//函数功能:"English"按钮的处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 En_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            ad_op_st |= OP_ST_SEL;        //选中状态
            Key_Valid_Sound();        //发按键有效音
            Set_Language_Type(ENGLISH);    //设置语言类型        
            return(MENU_AD_VT_FILT_CONST_OP);//切换到"潮气量滤波常数"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();        //发按键有效音                
            if (ad_op_st&OP_ST_ER)    //恢复显示
            {
                Put_EN_Butt_Lg_Sel(DISP_RT);
            }
            ad_op_st = OP_ST_SEL;    
            return(MENU_AD_LG_SEL_CH_OP);//切换到"中文"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_AD_LG_SEL_EN_OP);//仍然处在"English"
            break;            
    }        
}

