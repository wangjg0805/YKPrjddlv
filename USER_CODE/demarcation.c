//**********************************************************************
//文件名:demarcation.c
//**********************************************************************
#include "demarcation.h"
#include "FontLib.h"

//#include "lcd2478_graph.h"
#include "lcd2478.h"
#include "disp.h"
#include "graph.h"
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

#define INC_DEC_LONG    10
//修改参数
static void Demar_Change_Paw_AD(uint8 dir, uint8 step,sint8 point);
static void Demar_Change_O2_AD(uint8 dir, uint8 step,sint8 point);
static void Demar_Change_VT_Lps(uint8 dir, uint8 step,sint8 point);

static void Demar_Put_Ad_Req_Frame(void);
static void Put_Paw_AD(uint8 disp_mode);
static void Put_Paw_Filt_Len(uint8 disp_mode);
static void Put_Paw_Filt_Band(uint8 disp_mode);
static void Put_O2_AD(uint8 disp_mode);
static void Put_O2_Filt_Len(uint8 disp_mode);
static void Put_O2_Filt_Band(uint8 disp_mode);
static void Put_Batt_V_AD(uint8 disp_mode);
static void Put_Batt_V_Filt_Len(uint8 disp_mode);
static void Put_Batt_V_Filt_Band(uint8 disp_mode);
static void Put_Batt_I_AD(uint8 disp_mode);
static void Put_Batt_I_Filt_Len(uint8 disp_mode);
static void Put_Batt_I_Filt_Band(uint8 disp_mode);
static void Put_Vref_AD(uint8 disp_mode);
static void Put_Vref_Filt_Len(uint8 disp_mode);
static void Put_Vref_Filt_Band(uint8 disp_mode);
static void Put_Vt_Loops(uint8 disp_mode);
static void Put_Vt_Filt_Len(uint8 disp_mode);
static void Put_Vt_Filt_Band(uint8 disp_mode);
static uint16 Demar_Ad_Req_Pro(uint16 menu_option);

//"选项/按钮/窗口"显示函数
static void Put_Paw_Dem_Butt( uint8 disp_mode);
static void Put_O2_Dem_Butt( uint8 disp_mode);
static void Put_Vt_Dem_Butt( uint8 disp_mode);
static void Put_Pre_Butt(uint8 disp_mode);
static void Put_Next_Butt(uint8 disp_mode);
static void Put_Check_Cur_Butt(uint8 disp_mode);
static void Put_Save_Butt(uint8 disp_mode);
static void Put_Ret_Butt(uint8 disp_mode);
static void Put_Paw_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_O2_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_Vt_Dem_Point_Box(uint8 disp_mode,sint32 para);
static void Put_Paw_Ad_In_Box(uint8 disp_mode,uint16 para);
static void Put_O2_Ad_In_Box(uint8 disp_mode,uint16 para);
static void Put_Vt_Lps_In_Box(uint8 disp_mode,sint32 para);
static void Put_Paw_Ad_Val( uint8 disp_mode,uint16 ad);
static void Put_O2_Ad_Val( uint8 disp_mode,uint16 ad);
static void Put_Cur_Pressur_Dem_Paw( uint8 disp_mode,sint32 para);
static void Put_Cur_O2_Dem_O2( uint8 disp_mode,sint32 para);
static void Put_Zero_Val_Dem_Paw( uint8 disp_mode,sint16 para);
static void Put_O2_Cell_Volt_Dem_O2( uint8 disp_mode,sint32 para);
static void Put_Vt_Lps_Val( uint8 disp_mode,sint32 para);
static void Put_Cur_Vt( uint8 disp_mode,sint32 para);
static void Put_Vol_Axis_Dem_Paw( uint8 disp_mode);
static void Put_Press_Axis_Dem_Paw( uint8 disp_mode);
static void Put_Paw_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_Paw_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Vol_Axis_Dem_O2( uint8 disp_mode);
static void Put_O2_Axis_Dem_O2( uint8 disp_mode);
static void Put_O2_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_O2_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Lps_Axis_Dem_Vt( uint8 disp_mode);
static void Put_Vt_Axis_Dem_Vt( uint8 disp_mode);
static void Put_Vt_Sen_Cur(sint8 curve, uint8 disp_mode);
static void Put_Vt_Dem_Dot(sint8 point, uint8 disp_mode);
static void Put_Br_Icon_Dem(uint8 disp_mode);

//菜单界面初始化函数
static void Init_Dem_Main_Menu(sint32 op);
static void Init_Paw_Dem_Menu(sint32 op);
static void Init_O2_Dem_Menu(sint32 op);
static void Init_Vt_Dem_Menu(sint32 op);
//界面处理函数
static sint32 Menu_Lg_Sel_Op_Dem_Pro(uint8 key_c,sint32 op);
static sint32 Dem_Sel_Menu_Pro(uint8 key_c,sint32 op);
static sint32 Paw_Dem_Menu_Pro(uint8 key_c,sint32 op);
static sint32 O2_Dem_Menu_Pro(uint8 key_c,sint32 op);
static sint32 Vt_Dem_Menu_Pro(uint8 key_c,sint32 op);
//选项处理函数
static sint32 Dem_En_Butt_Pro(uint8 key_c);
static sint32 Dem_Ch_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Butt_Pro(uint8 key_c);
static sint32 O2_Sen_Dem_Butt_Pro(uint8 key_c);
static sint32 Vt_Sen_Dem_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 Paw_Dem_Ad_In_Box_Pro(uint8 key_c);
static sint32 O2_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 O2_Dem_Ad_In_Box_Pro(uint8 key_c);
static sint32 Vt_Dem_Ret_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Save_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Check_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Next_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Pre_Butt_Pro(uint8 key_c);
static sint32 Vt_Dem_Lps_In_Box_Pro(uint8 key_c);
//显示处理函数
static void Dem_Lg_Sel_Disp_Pro(void);
static void Dem_Sel_Disp_Pro(void);
static void Paw_Dem_Disp_Pro(void);
static void O2_Dem_Disp_Pro(void);
static void Vt_Dem_Disp_Pro(void);

//提示信息处理函数
#define CURVE_ERR_TIPS_DEM            0x0001    //传感器曲线错误提示
#define CURVE_OK_TIPS_DEM            0x0002    //传感器曲线正确
#define SAVE_ERR_TIPS_DEM            0x0003    //保存曲线时出错
#define SAVE_OK_TIPS_DEM            0x0004    //保存成功
#define AD_ERR_TIPS_DEM                0x0005    //AD值超出范围
#define LPS_ERR_TIPS_DEM            0x0006    //风轮圈数超出范围
#define TIPS_DISP_TIME_DEM        150    //在标定时提示信息的显示时间
Tips_Struct*  cur_tips_ptr_dem = &blank_tips;    //当前提示信息的指针
static sint16 tips_disp_time_dem;    //提示信息显示计时器
static uint8  flag_tips_saved;    //已经保存被覆盖区域的标志
static uint8  tips_disp_trigger_dem;    //触发提示信息的显示
static uint32 save_buf_dem[LINE_SIZE*SCR_YSIZE/4];//被提示信息覆盖的显示数据临时保存的缓冲区
//标定模式下的提示信息
static Tips_Struct    curve_err_tips_dem = {CURVE_ERR_TIPS_DEM,"传感器曲线错误,请重新标定!","Sensor Curve Error!\nPlease Demarcate Again!",NULL};
static Tips_Struct    curve_ok_tips_dem = {CURVE_OK_TIPS_DEM,"传感器曲线在正常范围内!","Sensor Curve In Proper Range!",NULL};
static Tips_Struct    save_err_tips_dem = {SAVE_ERR_TIPS_DEM,"保存数据时出错!请重新保存!","Save Error!\nPlease Save Again!",NULL};
static Tips_Struct    save_ok_tips_dem = {SAVE_OK_TIPS_DEM,"保存成功!","Save Successful!",NULL};
static Tips_Struct    ad_err_tips_dem = {AD_ERR_TIPS_DEM,"AD值超出范围!\n请重新检查后再保存!","AD Value Exceed!\nPlease Check And Save Again!",NULL};
static Tips_Struct    lps_err_tips_dem = {LPS_ERR_TIPS_DEM,"风轮圈数超出范围!\n请重新检查后再保存!","Fan Laps Exceed!\nPlease Check And Save Again!",NULL};
static void Log_Tips_Dem(Tips_Struct*tips_ptr);
static void Cancel_Tips_Dem(Tips_Struct*tips_ptr);
static void Put_Tips_Mess_Dem(void);
static uint8 Check_Tips_Disp_Dem(void);

static uint8 dem_op_st;
static uint8 cur_demar_point; 
//**********************************************************************
//函数  名:Demar_Manage
//函数功能:标定模式下的菜单切换处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Demar_Manage(void)
{
    struct menu_str menu[DEMAR_MENU_NUM] =
    {{MENU_DEMAR_LG_SEL,Init_LG_Sel_Menu,    Menu_Lg_Sel_Op_Dem_Pro,    Dem_Lg_Sel_Disp_Pro,NULL},//语言选择界面
    {DEMAR_MAIN_MENU,    Init_Dem_Main_Menu,    Dem_Sel_Menu_Pro,        Dem_Sel_Disp_Pro,    NULL},//主界面
    {DEMAR_NEW_PAW_CUR,    Init_Paw_Dem_Menu,    Paw_Dem_Menu_Pro,        Paw_Dem_Disp_Pro,    NULL},//压力传感器标定
    {DEMAR_O2_SEN,        Init_O2_Dem_Menu,    O2_Dem_Menu_Pro,        O2_Dem_Disp_Pro,    NULL},//氧气传感器标定
    {DEMAR_VT,            Init_Vt_Dem_Menu,    Vt_Dem_Menu_Pro,        Vt_Dem_Disp_Pro,    NULL}};// 潮气量标定
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
    uint8 key_c;            //按键号
    uint8 lcd_ref_interval = 0;
    sint16 lcd_line=0;    //LCD 分时刷新的起始刷新行数
        
    Set_Key_Sound_Switch(ON);    //标定模式下按键音开
    cur_op =0xffffffff ;  
    next_op = MENU_DEMAR_LG_SEL_CH_OP;
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            cur_demar_point = 0;    //第一个标定点
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        
        if (flag_10ms_ok)
        {
            flag_10ms_ok = FALSE;
            key_c = Get_Key_Code();        //键盘扫描
            next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_c,cur_op);    //选项处理        
            Put_Tips_Mess_Dem();    //显示提示信息
            //Check_Extend_Port();    //检查外部扩展口
            Check_Batt_Supply_Status();    //检查电池的状态                
        }    
        
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;
            (*(menu[(cur_op>>8)&0xff].disp_pro))();    //显示处理    
            //显示机动/手动呼吸灯
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
//函数  名:Demar_Change_Paw_AD
//函数功能:标定时修改压力AD值
//入口参数:dir--方向(1:增加;0:减少)
//           step--(LONG_STEP:增减10;其它:增减1)
//           point--修改点 
//出口参数:无
//**********************************************************************
static void Demar_Change_Paw_AD(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > PAW_DEMAR_POINTS-1) return;
    if (dir)    //增加
    {
        if (step == INC_DEC_LONG)
        {
            paw_ad_tab[point] += 10;
        }
        else
        {
            paw_ad_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            paw_ad_tab[point] -= 10;
        }
        else
        {
            paw_ad_tab[point]--;
        }        
    }
    di = Check_Pipeline_P_AD(point,paw_ad_tab[point]);    //检查修改范围
    if (!(di&0xff000000))    //范围正确,发正常按键音
    {
        Key_Valid_Sound();    //发按键有效音
    }
    else//范围错误,发按键异常音并修改AD值
    {
        paw_ad_tab[point] = di&0xffff;
        Key_No_Valid_Sound();    //发按键无效音
    }
}

//**********************************************************************
//函数  名:Demar_Change_O2_AD
//函数功能:标定时修改氧气AD值
//入口参数:dir--方向(1:增加;0:减少)
//           step--(LONG_STEP:增减10;其它:增减1)
//           point--修改点 
//出口参数:无
//**********************************************************************
static void Demar_Change_O2_AD(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > 1) return;    
    if (dir)    //增加
    {
        if (step == INC_DEC_LONG)
        {
            o2_ad_tab[point] += 10;
        }
        else
        {
            o2_ad_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            o2_ad_tab[point] -= 10;
        }
        else
        {
            o2_ad_tab[point]--;
        }        
    }
    di = Check_O2_AD(point,o2_ad_tab[point]);    //检查修改范围
    if (!(di&0xff000000))    //范围正确,发正常按键音
    {
        Key_Valid_Sound();    //发按键有效音
    }
    else//范围错误,发按键异常音并修改AD值
    {
        Key_No_Valid_Sound();    //发按键无效音
        o2_ad_tab[point] = di&0xffff;
    }
}

//**********************************************************************
//函数  名:Demar_Change_VT_Lps
//函数功能:标定时修改潮气量圈数
//入口参数:dir--方向(1:增加;0:减少)
//           step--(LONG_STEP:增减10;其它:增减1)
//           point--修改点 
//出口参数:无
//**********************************************************************
static void Demar_Change_VT_Lps(uint8 dir, uint8 step,sint8 point)
{
    sint32 di;
    if (point < 0) return;
    if (point > VT_DEMAR_POINTS-1) return;    
    if (dir)    //增加
    {
        if (step == INC_DEC_LONG)
        {
            vt_lap_tab[point] += 10;
        }
        else
        {
            vt_lap_tab[point]++;
        }
    }
    else
    {
        if (step == INC_DEC_LONG)
        {
            vt_lap_tab[point] -= 10;
        }
        else
        {
            vt_lap_tab[point]--;
        }        
    }
    di = Check_Vt_Laps(point,vt_lap_tab[point]);    //检查修改范围
    if (!(di&0xff000000))    //范围正确,发正常按键音
    {
        Key_Valid_Sound();    //发按键有效音
    }
    else//范围错误,发按键异常音并修改圈数
    {
        Key_No_Valid_Sound();    //发按键无效音
        vt_lap_tab[point] = di&0xffff;
    }
}

//**********************************************************************
//函数  名:Demar_Put_Ad_Req_Frame
//函数功能:在标定模式下显示AD查询界面的框架
//入口参数:无
//出口参数:无
//**********************************************************************
static void Demar_Put_Ad_Req_Frame(void)
{
    sint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Chr_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    Rectangle(5,5,314,234);    //主外框
                                //查询表绘制
    Rectangle(8,40,312,201);
    for (di=0; di<6; di++)
    {
        Hor_Line(9,311,63+di*23);
    }
    Ver_Line(41,200,83);
    Ver_Line(41,200,122);        
    Ver_Line(41,200,202);    
    
    Hor_Line(6,313,31);        //标题
    Pattern_Bar(6,6,313,30,0xaa);
    Bar(95,9,221,27);
    Set_Write_Mode(WRITE_EXOR);//异或模式
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial10B_Font);
    Put_Line_Str(126,11,"AD Require");
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Line_Str(15,46,"Channel");                
    Put_Line_Str(94,46,"AD");    
    Put_Line_Str(134,46,"Filt Len");
    Put_Line_Str(220,46,"Filt Band");            
    Put_Line_Str(22,68,"Paw");        
    Put_Line_Str(22,92,"O2");        
    Put_Line_Str(22,114,"Batt_V");    
    Put_Line_Str(22,138,"Batt_I");    
    Put_Line_Str(22,159,"Vref");
    Put_Line_Str(22,184,"Vt");
    
    Put_Paw_AD(DISP_RT);        
    Put_O2_AD(DISP_RT);
    Put_Batt_V_AD(DISP_RT);        
    Put_Batt_I_AD(DISP_RT);
    Put_Vref_AD(DISP_RT);
    Put_Vt_Loops(DISP_RT);
    
    Put_Paw_Filt_Len(DISP_RT);        
    Put_O2_Filt_Len(DISP_RT);
    Put_Batt_V_Filt_Len(DISP_RT);        
    Put_Batt_I_Filt_Len(DISP_RT);
    Put_Vref_Filt_Len(DISP_RT);
    Put_Vt_Filt_Len(DISP_RT);        
    
    Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);        
    Put_O2_Filt_Band(DISP_RT);
    Put_Batt_V_Filt_Band(DISP_RT);        
    Put_Batt_I_Filt_Band(DISP_RT);
    Put_Vref_Filt_Band(DISP_RT);
    Put_Vt_Filt_Band(DISP_RT);
                                
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数名:Put_Paw_AD
//函数功能:显示PAW的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Paw_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,64,121,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,68,paw_ad,1);
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,64,121,85);
    }    
}

//**********************************************************************
//函数名:Put_Paw_Filt_Len
//函数功能:显示PAW的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Paw_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,64,201,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(135,68,PAW_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,64,201,85);
    }
}

//**********************************************************************
//函数名:Put_Paw_Filt_Band
//函数功能:显示PAW的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Paw_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,64,311,85);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,65,310,84);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,68,paw_ad_filt_band,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,64,311,85);
    }
}

//**********************************************************************
//函数名:Put_O2_AD
//函数功能:显示O2的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_O2_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,64,121,85);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,92,o2_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,87,121,108);
    }
}

//**********************************************************************
//函数名:Put_O2_Filt_Len
//函数功能:显示O2的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_O2_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,87,201,108);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(135,92,O2_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,87,201,108);
    }
}

//**********************************************************************
//函数名:Put_O2_Filt_Band
//函数功能:显示O2的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_O2_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,87,311,108);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,88,310,107);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,92,VALID_O2_AD_SCALE,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,87,311,108);
    }
}

//**********************************************************************
//函数名:Put_Batt_V_AD
//函数功能:显示电池电压的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_V_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,110,121,131);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,114,batt_v_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,110,121,131);
    }
}

//**********************************************************************
//函数名:Put_Batt_V_Filt_Len
//函数功能:显示电池电压的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_V_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,110,201,131);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(135,114,BATT_V_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,110,201,131);
    }
}

//**********************************************************************
//函数名:Put_Batt_V_Filt_Band
//函数功能:显示电池电压的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_V_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,110,311,131);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,111,310,130);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,114,BATT_V_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,110,311,131);
    }
}

//**********************************************************************
//函数名:Put_Batt_I_AD
//函数功能:显示电池电流的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_I_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,133,121,154);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,138,batt_i_ad,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,133,121,154);
    }
}

//**********************************************************************
//函数名:Put_Batt_I_Filt_Len
//函数功能:显示电池电流的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_I_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,133,201,154);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(135,138,BATT_I_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,133,201,154);
    }
}

//**********************************************************************
//函数名:Put_Batt_I_Filt_Band
//函数功能:显示电池电流的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Batt_I_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,133,311,154);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,134,310,153);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,138,BATT_I_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,133,311,154);
    }
}

//**********************************************************************
//函数名:Put_Vref_AD
//函数功能:显示参考电压的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vref_AD(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,156,121,177);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,159,vref_ad,1);    
    if (disp_mode&DISP_RT)
    {        
        LCD_Refresh_Area(84,156,121,177);
    }
}

//**********************************************************************
//函数名:Put_Vref_Filt_Len
//函数功能:显示参考电压的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vref_Filt_Len(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,156,201,177);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(135,159,VREF_AD_FILT_LEN,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,156,201,177);
    }
}

//**********************************************************************
//函数名:Put_Vref_Filt_Band
//函数功能:显示参考电压的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vref_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,156,311,177);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,157,310,176);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,159,VREF_AD_FILT_BAND,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,156,311,177);
    }
}

//**********************************************************************
//函数名:Put_Vt_Loops
//函数功能:显示潮气量的圈数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vt_Loops(uint8 disp_mode)
{
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(84,179,121,200);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Write_Mode(WRITE_OR);//或模式
    Put_Numeral(88,184,demar_laps,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(84,179,121,200);
    }
}

//**********************************************************************
//函数名:Put_Vt_Filt_Len
//函数功能:显示潮气量的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vt_Filt_Len(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(123,179,201,200);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(124,180,200,199);
    }    
    Set_ASC_Lib(Arial10B_Font);    
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(135,184,vte_filter_const,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(123,179,201,200);
    }
}

//**********************************************************************
//函数名:Put_Vt_Filt_Band
//函数功能:显示参潮气量的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vt_Filt_Band(uint8 disp_mode)
{
    if (disp_mode&DISP_INVERSE)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
    }else
    {
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(203,179,311,200);
    if (disp_mode&DISP_INPUT)
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Rectangle(204,180,310,199);
    }
    Set_ASC_Lib(Arial10B_Font);
    if (disp_mode&DISP_INVERSE)
    {
        Set_Write_Mode(WRITE_EXOR);//异或模式
    }else
    {    
        Set_Write_Mode(WRITE_OR);//或模式
    }
    Put_Numeral(220,184,5,1);    
    if (disp_mode&DISP_RT)
    {    
        LCD_Refresh_Area(203,179,311,200);
    }
}

//**********************************************************************
//函数  名:Demar_Ad_Req_Pro
//函数功能:查询所以通道AD值
//入口参数:menu_option--菜单号(高8位)及默认选项(低8位)
//出口参数:所切换到的界面号(高8位)及进入后的焦点选项号(低八位)
//**********************************************************************
static uint16 Demar_Ad_Req_Pro(uint16 menu_option)
{
     uint8 key_code;
    uint16 key_pressed;
    uint8 oper_st = 0;
    
    Demar_Put_Ad_Req_Frame();
    Get_Key_Code();    //清除原来的键码
    phase_vent_ctrl = PHASE_START_VENT;//启动机动呼吸
    while (1)
    {
        if (flag_10ms_ok)    //10ms
        {
            flag_10ms_ok =  FALSE;
            key_code = Get_Key_Code();
            key_pressed = Get_Key_Status();
            switch (key_code)
            {
                case KEY_INC|KEY_PRESSED://"增长"键
                    if (oper_st == 1)    //正在输入参数
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //修改PAW滤波带宽
                        {
                            paw_ad_filt_band++;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            vte_filter_const++;
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);
                        }
                    }
                    else
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //修改PAW滤波带宽
                        {
                            menu_option = MENU_AD_REQ_VT_LEN;
                            Put_Paw_Filt_Band(DISP_RT);
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            menu_option = MENU_AD_REQ_PAW_BAND;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);
                            Put_Vt_Filt_Len(DISP_RT);                            
                        }                        
                    }
                    Key_Valid_Sound();    //发按键有效音
                    break;
                case KEY_DEC|KEY_PRESSED://"减小"键
                    if (oper_st == 1)    //正在输入参数
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //修改PAW滤波带宽
                        {
                            paw_ad_filt_band--;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            vte_filter_const--;
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);
                        }
                    }
                    else
                    {
                        if (menu_option==MENU_AD_REQ_PAW_BAND)    //修改PAW滤波带宽
                        {
                            menu_option = MENU_AD_REQ_VT_LEN;
                            Put_Paw_Filt_Band(DISP_RT);
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);                            
                        }
                        else if (menu_option==MENU_AD_REQ_VT_LEN)
                        {
                            menu_option = MENU_AD_REQ_PAW_BAND;
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);
                            Put_Vt_Filt_Len(DISP_RT);                                
                        }                        
                    }
                    Key_Valid_Sound();    //发按键有效音
                    break;                    
                case KEY_OK|KEY_PRESSED:    //"确认"键
                    Key_Valid_Sound();    //发按键有效音                
                    oper_st ^= 0x01;        //修改选项状态
                    if (menu_option==MENU_AD_REQ_PAW_BAND)    //修改PAW滤波带宽
                    {
                        if (oper_st)
                        {
                            Put_Paw_Filt_Band(DISP_RT|DISP_INPUT);    
                        }else
                        {
                            Put_Paw_Filt_Band(DISP_RT|DISP_INVERSE);                
                        }
                    }
                    else if (menu_option==MENU_AD_REQ_VT_LEN)
                    {
                        if (oper_st)
                        {
                            Put_Vt_Filt_Len(DISP_RT|DISP_INPUT);    
                        }else
                        {
                            Put_Vt_Filt_Len(DISP_RT|DISP_INVERSE);                
                        }                        
                    }                        
                    break;    
                case KEY_MENU|KEY_PRESSED_2S://"菜单键"键
                    Key_Valid_Sound();    //发按键有效音
                    phase_vent_ctrl = PHASE_STOP_VENT;//停止机动呼吸
                    return (DEMAR_MAIN_MENU_VT);
                    break;                
                default:
                    if (key_code&(KEY_PRESSED_5S|KEY_PRESSED_2S|KEY_PRESSED))//有键按下,但是无效
                    {
                        Key_No_Valid_Sound();    //发按键无效音
                    }
                    break;
            }
        }
            
        if (flag_100ms_ok)
        {
            flag_100ms_ok = FALSE;    
            Put_Paw_AD(DISP_RT);        
            Put_O2_AD(DISP_RT);
            Put_Batt_V_AD(DISP_RT);        
            Put_Batt_I_AD(DISP_RT);
            Put_Vref_AD(DISP_RT);
            Put_Vt_Loops(DISP_RT);
            //显示机动/手动呼吸灯
            if(phase_vent_ctrl == PHASE_NO_VENT)
            {
                LED_Indicate_Off(EO2_LED_VENT);
                LED_Indicate_On(EO2_LED_BAG);
            }
            else{
                LED_Indicate_Off(EO2_LED_BAG);
                LED_Indicate_On(EO2_LED_VENT);
            }
            LED_Indicate_Show();    //刷新显示LED                                                
        }
    }    
}

//**********************************************************************
//函数  名:Put_Pre_Butt
//函数功能:显示"前一点"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Pre_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,97,309,115);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("前一点","PRE");
        di = 282-Get_Str_Width(str)/2; //计算显示的横坐标
        Put_Line_Str(di,99,str);
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,97,309,115);
    }
}

//**********************************************************************
//函数  名:Put_Next_Butt
//函数功能:显示"下一点"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Next_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,120,309,138);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("下一点","NEXT");
        di = 282-Get_Str_Width(str)/2; //计算显示的横坐标
        Put_Line_Str(di,123,str);
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,120,309,138);
    }
}

//**********************************************************************
//函数  名:Put_Check_Cur_Butt
//函数功能:显示"检查"曲线按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Check_Cur_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,157,309,175);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("检查","CHECK");
        di = 282-Get_Str_Width(str)/2; //计算显示的横坐标
        Put_Line_Str(di,159,str);
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,157,309,175);
    }
}

//**********************************************************************
//函数  名:Put_Save_Butt
//函数功能:显示"保存"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Save_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,181,309,199);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        str = Language_Str("保存","SAVE");
        di = 282-Get_Str_Width(str)/2; //计算显示的横坐标
        Put_Line_Str(di,183,str);
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,181,309,199);
    }
}

//**********************************************************************
//函数  名:Put_Ret_Butt
//函数功能:显示"退出"按钮
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Ret_Butt(uint8 disp_mode)
{
    sint16 di;
    uint8* str;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,204,309,222);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
        //Set_Write_Mode(WRITE_EXOR);
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_THINRED,COL_BLACK);
        str = Language_Str("退出","EXIT");
        di = 282-Get_Str_Width(str)/2; //计算显示的横坐标
        Put_Line_Str(di,206,str);
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,204,309,222);
    }
}

//**********************************************************************
//函数  名:Put_Paw_Dem_Point_Box
//函数功能:显示压力标定点的值
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_Paw_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("cmH2O")+Get_Num_Width(para,1))/2; //计算显示的横坐标
    dj = Put_Numeral(di,80,para,1);    //显示标定点的值
    Put_Line_Str(dj,80,"cmH2O"); //显示单位
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//函数  名:Put_O2_Dem_Point_Box
//函数功能:显示氧气传感器标定点的值
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_O2_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("%")+Get_Num_Width(para,1))/2; //计算显示的横坐标
    dj = Put_Numeral(di,79,para,1);    //显示标定点的值
    Put_Line_Str(dj,79,"%"); //显示单位
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//函数  名:Put_Vt_Dem_Point_Box
//函数功能:显示潮气量标定点的值
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_Vt_Dem_Point_Box(uint8 disp_mode,sint32 para)
{
    sint16 di,dj;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(254,76,309,93);
    
    //Set_Write_Mode(WRITE_EXOR);
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = 282-(Get_Str_Width("mL")+Get_Num_Width(para,1))/2; //计算显示的横坐标
    dj = Put_Numeral(di,79,para,1);    //显示标定点的值
     Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(dj,79,"mL"); //显示单位
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(254,76,309,93);
    }
}

//**********************************************************************
//函数  名:Put_Paw_Ad_In_Box
//函数功能:显示管道压力传感器的AD输入窗口
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_Paw_Ad_In_Box(uint8 disp_mode,uint16 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//正在输入
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
       /* if (disp_mode&DISP_INPUT)//正在输入
        {
            Set_Write_Mode(WRITE_OR);
        }
        else 
        Set_Write_Mode(WRITE_EXOR);*/
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //计算显示的横坐标
        Put_Numeral(di,55,para,1);    //显示标定点的值
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//函数  名:Put_O2_Ad_In_Box
//函数功能:显示氧气传感器的AD输入窗口
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_O2_Ad_In_Box(uint8 disp_mode,uint16 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//正在输入
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE); 
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_WHITE);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
       /* if (disp_mode&DISP_INPUT)//正在输入
        {
            Set_Write_Mode(WRITE_OR);
        }
        else 
        Set_Write_Mode(WRITE_EXOR);*/
        
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //计算显示的横坐标
        Put_Numeral(di,55,para,1);    //显示标定点的值
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//函数  名:Put_Vt_Lps_In_Box
//函数功能:显示潮气量传感器圈数输入窗口
//入口参数:disp_mode--显示方式
//            para--显示的参数
//出口参数:无
//**********************************************************************
static void Put_Vt_Lps_In_Box(uint8 disp_mode,sint32 para)
{
    sint16 di;
    
    if (disp_mode&DISP_INPUT)//正在输入
    {
        Set_Graphic_Color(COL_WHITE,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_BLACK);
         Set_Chr_Color(COL_WHITE,COL_BLACK);
    }
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式    
    Bar(255,52,307,69);
    
    if (!(disp_mode&DISP_ER))    //不擦除文字
    {
       /* if (disp_mode&DISP_INPUT)//正在输入
        {
            Set_Write_Mode(WRITE_OVERLAY);
        }
        else Set_Write_Mode(WRITE_OVERLAY);*/
        
        Set_ASC_Lib(Arial10B_Font);
        di = 282-Get_Num_Width(para,1)/2; //计算显示的横坐标
        Put_Numeral(di,55,para,1);    //显示标定点的值
    }
    if (disp_mode&DISP_RT)    //实时刷新
    {
        LCD_Refresh_Area(255,52,307,69);
    }
}

//**********************************************************************
//函数  名:Put_Paw_Ad_Val
//函数功能:在标定压力传感器时显示测量的AD值
//入口参数:disp_mode
//            ad--显示的AD值
//出口参数:无
//**********************************************************************
static void Put_Paw_Ad_Val( uint8 disp_mode,uint16 ad)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,ad,1);
    di = Put_Line_Str(di,36,"(");    
    di = Put_Numeral(di,36,Cal_Vol(ad),100);//显示电压值
    di = Put_Line_Str(di,36,"V)");
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//函数  名:Put_O2_Ad_Val
//函数功能:在标定氧气传感器时显示测量的AD值
//入口参数:disp_mode
//            ad--显示的AD值
//出口参数:无
//**********************************************************************
static void Put_O2_Ad_Val( uint8 disp_mode,uint16 ad)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,ad,1);
    di = Put_Line_Str(di,36,"(");    
    di = Put_Numeral(di,36,Cal_Vol(ad),100);//显示电压值
    di = Put_Line_Str(di,36,"V)");
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//函数  名:Put_Cur_Pressur_Dem_Paw
//函数功能:在标定压力传感器时显示测量的压力值
//入口参数:disp_mode
//            para--压力值(0.01cmH2O)
//出口参数:无
//**********************************************************************
static void Put_Cur_Pressur_Dem_Paw( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para*10/PAW_MUL,10);    //显示压力测量值
    di = Put_Line_Str(di,55,"cmH2O");    //显示单位
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//函数  名:Put_Cur_O2_Dem_O2
//函数功能:在标定氧气传感器时显示测量的氧气浓度
//入口参数:disp_mode
//            para--压力值(0.1cmH2O)
//出口参数:无
//**********************************************************************
static void Put_Cur_O2_Dem_O2( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para,10);    //显示氧气浓度测量值
    di = Put_Line_Str(di,55,"%");    //显示单位
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//函数  名:Put_Zero_Val_Dem_Paw
//函数功能:在标定压力传感器时显示压力校零值
//入口参数:disp_mode
//            para--校零值
//出口参数:无
//**********************************************************************
static void Put_Zero_Val_Dem_Paw( uint8 disp_mode,sint16 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,71,246,88);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,74,para,1);    //显示校零值
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,71,246,88);
    }    
}

//**********************************************************************
//函数  名:Put_O2_Cell_Volt_Dem_O2
//函数功能:在标定氧气传感器时显示氧电池电压
//入口参数:disp_mode
//            para--电压值
//出口参数:无
//**********************************************************************
static void Put_O2_Cell_Volt_Dem_O2( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,71,246,88);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,74,para,10);    //显示电压值
    Put_Str(di,74,"mV");        //显示单位
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,71,246,88);
    }    
}

//**********************************************************************
//函数  名:Put_Vt_Lps_Val
//函数功能:在标定潮气量传感器的时候显示圈数测量值
//入口参数:disp_mode--显示方式
//            para--圈数
//出口参数:无
//**********************************************************************
static void Put_Vt_Lps_Val( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,33,246,50);
    
    Set_ASC_Lib(Arial10B_Font);
    //Set_Write_Mode(WRITE_OR);//或模式
     Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,36,para,1);
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,33,246,50);
    }    
}

//**********************************************************************
//函数  名:Put_Cur_Vt
//函数功能:在标定潮气量传感器时显示潮气量测量值
//入口参数:disp_mode
//            para--压力值(0.1cmH2O)
//出口参数:无
//**********************************************************************
static void Put_Cur_Vt( uint8 disp_mode,sint32 para)
{
    uint16 di;
    
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(124,52,246,69);
    
    Set_ASC_Lib(Arial10B_Font);
   // Set_Write_Mode(WRITE_OR);//或模式
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    di = Put_Numeral(132,55,para,1);    //显示压力测量值
    di = Put_Line_Str(di,55,"mL");    //显示单位
    
    if (disp_mode&DISP_RT)
    {
        LCD_Refresh_Area(124,52,246,69);
    }    
}

//**********************************************************************
//函数  名:Put_Vol_Axis_Dem_Paw
//函数功能:在标定压力传感器的时候,画AD(VOL)轴
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vol_Axis_Dem_Paw( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //与模式
    Hor_Line(40,218,206);    //画x轴
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<9; di++)//画刻度
    {
        //Set_Write_Mode(WRITE_AND);    //与模式
        x1 = di*20+40;
        Ver_Line(204,205,x1);
        num = di*5;
        x2 = x1-Get_Num_Width(num,10)/2;
        //Set_Write_Mode(WRITE_NOT);    //反显覆盖
        Put_Numeral(x2,210,num,10);
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(213,203,218,206);//画箭头
    Line(213,209,218,206);
    //Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Char(223,201,'V');
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(40,203,232,220);
    }
}

//**********************************************************************
//函数  名:Put_Press_Axis_Dem_Paw
//函数功能:在标定压力传感器的时候,画A压力轴
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Press_Axis_Dem_Paw( uint8 disp_mode)
{
    uint16 y;
    uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //与模式
    Ver_Line(107,206,40);    //画y轴
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<5; di++)//画刻度
    {
        y = 206-di*20;
        //Set_Write_Mode(WRITE_AND);    //与模式
        Hor_Line(41,42,y);
        num = di*20;
        //Set_Write_Mode(WRITE_NOT);    //反显覆盖
        Put_Numeral(25,y-5,num,1);    
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(37,112,40,107);    //画箭头
    Line(43,112,40,107);
    Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Line_Str(13,95,"cmH2O");
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(13,95,55,209);
    }        
}

//**********************************************************************
//函数  名:Put_Paw_Sen_Cur
//函数功能:画压力传感器特征曲线
//入口参数:disp_mode--显示方式
//           curve--显示第几段
//出口参数:无
//**********************************************************************
static void Put_Paw_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //曲线段的投影长度和高度
    uint16 x1,y1,x2,y2;
    
    if (curve >= PAW_DEMAR_POINTS-1)    return;//没有该曲线段则返回
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//实时刷新
    {
        Set_Refresh_Mode(1);
    }     
    //计算第一点的投影宽度和高度
    w1 = paw_ad_tab[curve]*120/1024;    //满度AD为120pixels
    h1 = paw_demar_tab[curve];            //1cmH2O/pixel
    w2 = paw_ad_tab[curve+1]*120/1024;    //满度AD为120pixels    
    h2 = paw_demar_tab[curve+1];        //1cmH2O/pixel
    if (disp_mode&DISP_ER)    //擦除旧曲线
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
       // Set_Write_Mode(WRITE_OR);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //与模式
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 206-h1;
    y2 = 206-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//标定点突出显示
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//擦除曲线后都要重画坐标轴
    {
        Put_Vol_Axis_Dem_Paw(DISP_RT);
        Put_Press_Axis_Dem_Paw(DISP_RT);
    }    
}

//**********************************************************************
//函数  名:Put_Paw_Dem_Dot
//函数功能:显示压力标定点的图标
//入口参数:disp_mode--显示方式
//          point--标定点的序号
//出口参数:无
//**********************************************************************
static void Put_Paw_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > PAW_DEMAR_POINTS-1)    return;    //没有该标定点
    if (point < 0) return;
    
    x = 40+paw_ad_tab[point]*120/1024;    //满度AD为120pixels
    y = 206- paw_demar_tab[point];            //1cmH2O/pixel
    if (disp_mode&DISP_ER)    //擦除显示
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //与模式
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//函数  名:Put_Vol_Axis_Dem_O2
//函数功能:在标定氧气传感器的时候,画AD(VOL)轴
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vol_Axis_Dem_O2( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_Write_Mode(WRITE_AND);    //与模式
    Hor_Line(40,218,206);    //画x轴
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<9; di++)//画刻度
    {
        //Set_Write_Mode(WRITE_AND);    //与模式
        x1 = di*20+40;
        Ver_Line(204,205,x1);
        num = di*5;
        x2 = x1-Get_Num_Width(num,10)/2;
        //Set_Write_Mode(WRITE_NOT);    //反显覆盖
        Put_Numeral(x2,210,num,10);
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(213,203,218,206);//画箭头
    Line(213,209,218,206);
    //Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Char(223,201,'V');
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(40,203,232,220);
    }
}

//**********************************************************************
//函数  名:Put_O2_Axis_Dem_O2
//函数功能:在标定氧气传感器的时候,画氧气浓度轴(Y轴)
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_O2_Axis_Dem_O2( uint8 disp_mode)
{
    uint16 y;
    uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //与模式
    Ver_Line(107,206,40);    //画y轴
    Set_ASC_Lib(Arial8B_Font);
    for (di=0; di<5; di++)//画刻度
    {
        y = 206-di*20;
        //Set_Write_Mode(WRITE_AND);    //与模式
        Hor_Line(41,42,y);
        num = di*25;
        //Set_Write_Mode(WRITE_NOT);    //反显覆盖
        Put_Numeral(20,y-5,num,1);    //画刻度值    
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(37,112,40,107);    //画箭头
    Line(43,112,40,107);
    //Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Line_Str(33,95,"%");
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(13,95,55,209);
    }        
}

//**********************************************************************
//函数  名:Put_O2_Sen_Cur
//函数功能:画氧气浓度传感器特征曲线
//入口参数:disp_mode--显示方式
//           curve--显示第几段
//出口参数:无
//**********************************************************************
static void Put_O2_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //曲线段的投影长度和高度
    uint16 x1,y1,x2,y2;
    
    if (curve >= O2_SEN_DEM_POINTS-1)    return;//没有该曲线段则返回
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//实时刷新
    {
        Set_Refresh_Mode(1);
    }     
    //计算第一点的投影宽度和高度
    w1 = o2_ad_tab[curve]*120/1024;    //满度AD为120pixels
    h1 = o2_demar_tab[curve]*4/5;    //100%/80dots
    w2 = o2_ad_tab[curve+1]*120/1024;    //满度AD为120pixels    
    h2 = o2_demar_tab[curve+1]*4/5;        //100%/80dots
    if (disp_mode&DISP_ER)    //擦除旧曲线
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
       // Set_Write_Mode(WRITE_AND);    //与模式
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 206-h1;
    y2 = 206-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//标定点突出显示
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//擦除曲线后都要重画坐标轴
    {
        Put_Vol_Axis_Dem_O2(DISP_RT);
        Put_O2_Axis_Dem_O2(DISP_RT);
    }    
}

//**********************************************************************
//函数  名:Put_O2_Dem_Dot
//函数功能:显示氧气浓度标定点的图标
//入口参数:disp_mode--显示方式
//          point--标定点的序号
//出口参数:无
//**********************************************************************
static void Put_O2_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > O2_SEN_DEM_POINTS-1)    return;    //没有该标定点
    if (point < 0) return;
    
    x = 40+o2_ad_tab[point]*120/1024;    //满度AD为120pixels
    y = 206- o2_demar_tab[point]*4/5;   //100%/80dots
    if (disp_mode&DISP_ER)    //擦除显示
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //与模式
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//函数  名:Put_Lps_Axis_Dem_Vt
//函数功能:在标定潮气量的时候画X轴(风轮圈数)
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Lps_Axis_Dem_Vt( uint8 disp_mode)
{
    uint16 di;
    uint16 x1,x2;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //与模式
    Hor_Line(40,218,191);    //画x轴
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    for (di=0; di<9; di++)//画刻度
    {
        //Set_Write_Mode(WRITE_AND);    //与模式
        x1 = di*20+40;
        Ver_Line(189,190,x1);
        num = di*100;
        if(!(di%2))    //每两个刻度显示一个刻度值
        {
            x2 = x1-Get_Num_Width(num,1)/2;
            //Set_Write_Mode(WRITE_NOT);    //反显覆盖
            Put_Numeral(x2,196,num,1);
        }    
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(213,188,218,191);//画箭头
    Line(213,194,218,191);
    //Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Line_Str(215,197,"loops");
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(34,189,246,209);
    }
}

//**********************************************************************
//函数  名:Put_Vt_Axis_Dem_Vt
//函数功能:在标定潮气量的时候,显示Y轴(Vt轴)
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vt_Axis_Dem_Vt( uint8 disp_mode)
{
    uint16 y;
     uint8 di;
    uint16 num;
    
    Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
    //Set_Write_Mode(WRITE_AND);    //与模式
    Ver_Line(92,191,40);    //画y轴
    Set_ASC_Lib(Arial8B_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    for (di=0; di<5; di++)//画刻度
    {
        y = 191-di*20;
        //Set_Write_Mode(WRITE_AND);    //与模式
        Hor_Line(41,42,y);
        num = di*200;
        //Set_Write_Mode(WRITE_NOT);    //反显覆盖
        Put_Numeral(16,y-5,num,1);    //画刻度值
    }
    //Set_Write_Mode(WRITE_AND);    //与模式
    Line(37,97,40,92);    //画箭头
    Line(43,97,40,92);
    //Set_Write_Mode(WRITE_NOT);    //反显覆盖
    Put_Line_Str(29,77,"mL");
    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(12,77,46,195);
    }        
}

//**********************************************************************
//函数  名:Put_Vt_Sen_Cur
//函数功能:画潮气量特征曲线
//入口参数:disp_mode--显示方式
//           curve--显示第几段
//出口参数:无
//**********************************************************************
static void Put_Vt_Sen_Cur(sint8 curve, uint8 disp_mode)
{
    uint16 w1,w2,h1,h2;    //曲线段的投影长度和高度
    uint16 x1,y1,x2,y2;
    
    if (curve >= VT_DEMAR_POINTS-1)    return;//没有该曲线段则返回
    if (curve < 0)    return;

    if (disp_mode&DISP_RT)//实时刷新
    {
        Set_Refresh_Mode(1);
    }     
    //计算第一点的投影宽度和高度
    w1 = vt_lap_tab[curve]/5;    //满度1000圈为200pixels(200/1000=1/5)
    h1 = vt_demar_tab[curve]/10;            //10mL/pixel(80/800)
    w2 = vt_lap_tab[curve+1]/5;    //满度1000圈为200pixels(200/1000=1/5)
    h2 = vt_demar_tab[curve+1]/10;        //10mL/pixel(80/800)
    if (disp_mode&DISP_ER)    //擦除旧曲线
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OVERLAY);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OVERLAY);    //与模式
    }
    x1 = 40+w1;
    x2 = 40+w2;
    y1 = 191-h1;
    y2 = 191-h2;
    Line(x1,y1,x2,y2);
    Fill_Circle(x1,y1,2);//标定掉突出显示
    Fill_Circle(x2,y2,2);
    Set_Refresh_Mode(0);
    
    if (disp_mode&DISP_ER)//擦除曲线后都要重画坐标轴
    {
        Put_Lps_Axis_Dem_Vt(DISP_RT);
        Put_Vt_Axis_Dem_Vt(DISP_RT);
    }    
}

//**********************************************************************
//函数  名:Put_Vt_Dem_Dot
//函数功能:标定潮气量的时候显示标定点的图标
//入口参数:disp_mode--显示方式
//          point--标定点的序号
//出口参数:无
//**********************************************************************
static void Put_Vt_Dem_Dot(sint8 point, uint8 disp_mode)
{
    uint16 x,y;
    if (point > VT_DEMAR_POINTS-1)    return;    //没有该标定点
    if (point < 0) return;
    
    x = 40+vt_lap_tab[point]/5;    //满度1000圈为200pixels(200/1000=1/5)
    y = 191- vt_demar_tab[point]/10;//10mL/pixel(80/800)
    if (disp_mode&DISP_ER)    //擦除显示
    {
        Set_Graphic_Color(COL_THINGRAY,COL_THINGRAY);
        //Set_Write_Mode(WRITE_OR);    //或模式    
    }
    else
    {
        Set_Graphic_Color(COL_BLACK,COL_THINGRAY);
        //Set_Write_Mode(WRITE_AND);    //与模式
    }
    Fill_Circle(x,y,2);    
    if (disp_mode&DISP_RT)    //即时刷新
    {
        LCD_Refresh_Area(x-2,y-2,x+2,y+2);
    }
}

//**********************************************************************
//函数  名:Put_Br_Icon_Dem
//函数功能:显示呼吸图标
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Br_Icon_Dem(uint8 disp_mode)
{
    static uint8 disp_st = 0;//(=0:显示吸气图标;=1:显示呼气图标)
    Set_Graphic_Color(COL_WHITE,COL_DEEPGREEN);
    switch(phase_vent_ctrl)
    {
        case PHASE_VCV_INSP:    //吸气或者屏气则显示吸气图标
        case PHASE_VCV_HOLD:
            if(disp_st)    //还没显示吸气图标
            {
                disp_st =0;
                //Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                LCD_Refresh_Area(270,60,301,89);
            }    
            break;
        default:
            if (!(disp_st))//还没显示呼气图标
            {
                disp_st = 1;    //显示呼气图标
                Put_Image(212,80,Vent_Icon,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);                    
            }
            break;                    
    }
}

//**********************************************************************
//函数  名:Put_Paw_Dem_Butt
//函数功能:显示"管道压力传感器标定"选项
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Paw_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(35,49,276,71);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,60,5);    //用前景色画圆
   
    if (!(disp_mode&DISP_ER))    //显示文字
    {
        //Set_Write_Mode(WRITE_EXOR);//异或模式
        Set_HZ_Lib(&ST_11);        //设置字库
        Set_ASC_Lib(Arial12_Font);   
        Set_Chr_Color(COL_WHITE,COL_BLACK); 
        Put_Line_Str(43,52,Language_Str("管道压力传感器标定","PAW Sensor Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//即时刷新
    {
        LCD_Refresh_Area(18,55,28,65);    //刷新园
        LCD_Refresh_Area(35,49,276,71);//刷新标题
    }
}

//**********************************************************************
//函数  名:Put_O2_Dem_Butt
//函数功能:显示"氧气传感器标定"选项
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_O2_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_BLACK);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
     Bar(35,78,276,100);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,89,5);    //用前景色画圆
    
    if (!(disp_mode&DISP_ER))    //显示文字
    {
        //Set_Write_Mode(WRITE_EXOR);//异或模式
        Set_HZ_Lib(&ST_11);        //设置字库
        Set_ASC_Lib(Arial12_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        Put_Line_Str(43,81,Language_Str("氧气浓度传感器标定","O2 Sensor Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//即时刷新
    {
        LCD_Refresh_Area(18,84,28,94);    //刷新圆
        LCD_Refresh_Area(35,78,276,100);//刷新标题
    }
}        

//**********************************************************************
//函数  名:Put_Vt_Dem_Butt
//函数功能:显示"潮气量标定"选项
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
static void Put_Vt_Dem_Butt( uint8 disp_mode)
{
    Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
     Bar(35,109,276,130);
    Set_Graphic_Color(COL_THINGREEN,COL_BLACK);
    Fill_Circle(23,119,5);    //用前景色画圆
      
    if (!(disp_mode&DISP_ER))    //显示文字
    {
        //Set_Write_Mode(WRITE_EXOR);//异或模式
        Set_HZ_Lib(&ST_11);        //设置字库
        Set_ASC_Lib(Arial12_Font);
        Set_Chr_Color(COL_WHITE,COL_BLACK);
        Put_Line_Str(43,111,Language_Str("潮气量标定","VT Demarcation"));
    }
    
    if (disp_mode&DISP_RT)//即时刷新
    {
        LCD_Refresh_Area(18,114,28,124);    //刷新园
        LCD_Refresh_Area(35,109,276,130);//刷新标题
    }
}

//**********************************************************************
//函数  名:Init_Dem_Main_Menu
//函数功能:在标定模式下显示主界面框架
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
static void Init_Dem_Main_Menu(sint32 op)
{
    uint8 *str;
    sint32 di;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //主外框
    Hor_Line(6,313,31);        //标题
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Pattern_Bar(6,6,313,30,0xFF);

    //Set_Write_Mode(WRITE_EXOR);//异或模式
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_THINGREEN,COL_DEEPGREEN);
    str = Language_Str("标定主界面","Demarcation Main Interface");
    di = 159-Get_Str_Width(str)/2;
    Put_Line_Str(di,11,str);
    //Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(Language_Pos(213,193),208,Language_Str("退出请关机","Turnoff To Exit"));
    
    Put_Paw_Dem_Butt(DISP_NOR);    
    Put_O2_Dem_Butt(DISP_NOR);
    Put_Vt_Dem_Butt(DISP_NOR);
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数  名:Init_Paw_Dem_Menu
//函数功能:标定压力传感器时显示界面框架
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
static void Init_Paw_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //主外框
    Hor_Line(6,313,27);        //标题
    Pattern_Bar(6,6,313,26,0xFF);
   
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("管道压力传感器标定","PAW Sensor Demarcation");
    
    di = 159-Get_Str_Width(str)/2;    
        Put_Line_Str(di,9,str);
    
    //画表头
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Hor_Line(12,247,89);
    Ver_Line(33,88,12);
    Ver_Line(33,88,123);
    Ver_Line(33,88,247);
   
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Put_Line_Str(16,36,"AD Value");
    Put_Line_Str(16,54,"Pressure");
    Put_Line_Str(16,73,"Zero Value");
    
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(12,93,247,223);            
    Put_Vol_Axis_Dem_Paw(DISP_NOT_RT);    //画x轴
    Put_Press_Axis_Dem_Paw(DISP_NOT_RT);    //画Y轴
    for (di=0; di<PAW_DEMAR_POINTS-1; di++)
    {
        Put_Paw_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    //Set_Write_Mode(WRITE_EXOR);//异或模式
    Bar(253,32,309,70);
    
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);  
    Put_Line_Str(282-Get_Str_Width("AD In")/2,35,"AD In");
    
    Put_Paw_Ad_In_Box(DISP_NOR,paw_ad_tab[0]);//显示标定点AD值    
    Put_Paw_Dem_Point_Box(DISP_NOR,paw_demar_tab[0]);//显示标定点
    
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    Put_Paw_Ad_Val(DISP_NOR,paw_ad);
    Put_Cur_Pressur_Dem_Paw(DISP_NOR,airway_p);
    Put_Zero_Val_Dem_Paw(DISP_NOR,paw_zero_ad);    //显示校零值
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数  名:Init_O2_Dem_Menu
//函数功能:标定氧气传感器时显示界面框架
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
static void Init_O2_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //主外框
    Hor_Line(6,313,27);        //标题
    Pattern_Bar(6,6,313,26,0xFF);
    
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial12_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("氧气浓度传感器标定","O2 Sensor Demarcation");
    di = 159-Get_Str_Width(str)/2;    
    Put_Line_Str(di,9,str);
    
    //画表头
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Hor_Line(12,247,89);
    Ver_Line(33,88,12);
    Ver_Line(33,88,123);
    Ver_Line(33,88,247);
    //Set_Write_Mode(WRITE_OR);
     Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Set_ASC_Lib(Arial10B_Font);    
    Put_Line_Str(16,36,"AD Value");
    Put_Line_Str(16,54,"O2 Thickness");
    Put_Line_Str(16,73,"O2 Cell Volt");
    
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Bar(12,93,247,223); 
               
    Put_Vol_Axis_Dem_O2(DISP_NOT_RT);    //画x轴
    Put_O2_Axis_Dem_O2(DISP_NOT_RT);    //画Y轴
    for (di=0; di<O2_SEN_DEM_POINTS-1; di++)
    {
        Put_O2_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);    
    Bar(253,32,309,70);
    
    Set_ASC_Lib(Arial10B_Font);
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN); 
    Put_Line_Str(282-Get_Str_Width("AD In")/2,35,"AD In");
    
    Put_O2_Ad_In_Box(DISP_NOR,o2_ad_tab[0]);//显示标定点AD值    
    Put_O2_Dem_Point_Box(DISP_NOR,o2_demar_tab[0]);//显示标定点
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    //显示测量值
    Put_O2_Ad_Val(DISP_NOR,o2_ad);
    Put_Cur_O2_Dem_O2(DISP_NOR,cur_o2); 
    Put_O2_Cell_Volt_Dem_O2(DISP_NOR,o2_vol);
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数  名:Init_Vt_Dem_Menu
//函数功能:初始化潮气量标定菜单
//入口参数:op--当前选项
//出口参数:无
//**********************************************************************
static void Init_Vt_Dem_Menu(sint32 op)
{
     sint32 di;
     uint8 *str;
    
    Set_Graphic_Color(COL_DEEPGREEN,COL_DEEPGREEN);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Clr_Screen(DISP_NOT_RT);    //用背景色清屏
    
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Rectangle(5,5,314,234);    //主外框
    Hor_Line(6,313,27);        //标题
    Pattern_Bar(6,6,313,26,0xFF);
   
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial12_Font);
     Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    str = Language_Str("潮气量标定","Vt Demarcation");
    di = 159-Get_Str_Width(str)/2;
    Put_Line_Str(di,9,str);
    
    //画表头
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Hor_Line(12,247,32);    
    Hor_Line(12,247,51);    
    Hor_Line(12,247,70);
    Ver_Line(33,69,12);
    Ver_Line(33,69,123);
    Ver_Line(33,69,247);
    //Set_Write_Mode(WRITE_OR);
    Set_ASC_Lib(Arial10B_Font); 
     Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);   
    Put_Line_Str(16,36,"Loops Value");
    Put_Line_Str(16,54,"Current Vt");
    
    //Set_Graphic_Color(COL_BLACK,COL_WHITE);
    Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
    Set_Graphic_Color(COL_THINGRAY,COL_DEEPGREEN);
    Bar(12,76,247,223);            
    Put_Lps_Axis_Dem_Vt(DISP_NOT_RT);    //画x轴
    Put_Vt_Axis_Dem_Vt(DISP_NOT_RT);    //画Y轴
    for (di=0; di<VT_DEMAR_POINTS-1; di++)
    {
        Put_Vt_Sen_Cur(di,DISP_NOT_RT|DISP_NOT_ER);
    }
    Put_Br_Icon_Dem(DISP_NOR);
    
    Set_Graphic_Color(COL_WHITE,COL_BLACK);
    //Set_Write_Mode(WRITE_EXOR);//异或模式
    Set_Graphic_Color(COL_THINCYAN,COL_DEEPGREEN);
    Bar(253,32,309,70);
    Set_ASC_Lib(Arial10B_Font);    
    Set_Chr_Color(COL_DEEPBLUE,COL_THINCYAN);
    Put_Line_Str(282-Get_Str_Width("LPS In")/2,35,"LPS In");
    Put_Vt_Lps_In_Box(DISP_NOR,vt_lap_tab[0]);//显示标定点圈数
    Put_Vt_Dem_Point_Box(DISP_NOR,vt_demar_tab[0]);//显示标定点
    Put_Pre_Butt(DISP_NOR);
    Put_Next_Butt(DISP_NOR);    
    Put_Check_Cur_Butt(DISP_NOR);
    Put_Save_Butt(DISP_NOR);
    Put_Ret_Butt(DISP_NOR);
    
    Put_Vt_Lps_Val(DISP_NOR,demar_laps);    //显示圈数
    Put_Cur_Vt(DISP_NOR,demarc_vte);
    LCD_Refresh_Area(0,0,319,239);    //全屏刷新    
}

//**********************************************************************
//函数  名:Dem_Sel_Menu_Pro
//函数功能:标定项目选择菜单的处理(10ms处理一次)
//入口参数:key_c--按键码
//            op--当前选项
//出口参数:切换到的选项
//**********************************************************************
static sint32 Dem_Sel_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_MAIN_MENU_OPS]=
    {{DEMAR_MAIN_MENU_VT,Vt_Sen_Dem_Butt_Pro},    //潮气量标定
    {DEMAR_MAIN_MENU_O2,O2_Sen_Dem_Butt_Pro},    //氧气传感器标定
    {DEMAR_MAIN_MENU_PAW,Paw_Dem_Butt_Pro}};    //压力传感器标定
    sint32 op_id;
    static uint8 time=0;
    
    if (!time)    //mod32(320ms周期闪烁显示)
    {
        switch (op)
        {
            case DEMAR_MAIN_MENU_VT://潮气量标定
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Vt_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Vt_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_MAIN_MENU_O2://氧气传感器标定            
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_O2_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_O2_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_MAIN_MENU_PAW://压力传感器标定                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Paw_Dem_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Paw_Dem_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
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
//函数  名:Paw_Dem_Butt_Pro
//函数功能:"压力传感器标定"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音        
            dem_op_st = 0;
            phase_vent_ctrl = PHASE_PAW_DEM; //压力传感器标定模式下的阀门处理
            return(DEMAR_NEW_PAW_EXIT);//切换到压力传感器标定菜单
            break;
        case KEY_PRESSED|KEY_DEC://减少键
            Key_Valid_Sound();    //按键有效音            
            if (dem_op_st&OP_ST_ER)
            {
                Put_Paw_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_O2);//切换到"氧气传感器标定"选项
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_MAIN_MENU_PAW);//仍然处在"压力传感器标定"    
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Sen_Dem_Butt_Pro
//函数功能:"氧气传感器标定"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Sen_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音        
            dem_op_st = 0;    
            //return (DEMAR_MAIN_MENU_O2);    //暂时不起作用
            return(DEMAR_O2_SEN_EXIT);//切换到氧气传感器标定菜单
            break;
        case KEY_PRESSED|KEY_INC:    //增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//擦除则恢复显示
            {
                Put_O2_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_PAW);//切换到"压力传感器标定"选项        
            break;    
        case KEY_PRESSED|KEY_DEC://减少键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//擦除则恢复显示
            {
                Put_O2_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_VT);//切换到"潮气量标定"选项
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_MAIN_MENU_O2);//仍然处在"氧气传感器标定"    
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Sen_Dem_Butt_Pro
//函数功能:"潮气量标定"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Sen_Dem_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音        
            dem_op_st = 0;
            phase_vent_ctrl = PHASE_START_VENT;    //启动机动呼吸    
            return(DEMAR_VT_EXIT);//切换到潮气量标定菜单
            break;
        case KEY_PRESSED|KEY_INC:    //增长键
            Key_Valid_Sound();    //按键有效音    
            if (dem_op_st&OP_ST_ER)//擦除则恢复显示
            {
                Put_Vt_Dem_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_MAIN_MENU_O2);//切换到"氧气传感器标定"选项        
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_MAIN_MENU_VT);//仍然处在"潮气量标定"    
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Menu_Pro
//函数功能:管道压力传感器标定菜单的处理(10ms处理一次)
//入口参数:key_c--按键码
//            op--当前选项
//出口参数:切换到的选项
//**********************************************************************
static sint32 Paw_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_NEW_PAW_OPS]=
    {{DEMAR_NEW_PAW_EXIT,Paw_Dem_Ret_Butt_Pro},//退出
    {DEMAR_NEW_PAW_SAVE,Paw_Dem_Save_Butt_Pro},    //保存
    {DEMAR_NEW_PAW_CHECK,Paw_Dem_Check_Butt_Pro},    //检查    
    {DEMAR_NEW_PAW_NEXT,Paw_Dem_Next_Butt_Pro},    //下一点
    {DEMAR_NEW_PAW_PRE,Paw_Dem_Pre_Butt_Pro},    //前一点
    {DEMAR_NEW_PAW_AD_IN,Paw_Dem_Ad_In_Box_Pro}};//AD输入
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp = FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms周期闪烁显示)
    {
        if (FALSE == flag_dot_disp)//擦除显示标定点
        {
            flag_dot_disp = TRUE;
            Put_Paw_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);
        }
        switch (op)
        {
            case DEMAR_NEW_PAW_EXIT://退出
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_SAVE://保存            
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_NEXT://下一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_NEW_PAW_PRE://前一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_NEW_PAW_CHECK://检查                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_NEW_PAW_AD_IN://AD输入                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_Paw_Ad_In_Box(di|DISP_RT,paw_ad_tab[cur_demar_point]);
                }
                else//原来显示则擦除
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_Paw_Ad_In_Box(di|DISP_RT|DISP_ER,paw_ad_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
                break;                                            
            default:break;
        }    
    }
    time++;
    time &= 31;    
    op_id = (*((op_tab[op&0xff]).op_pro))(key_c); //选项处理子程序
    if ((op_id^op)&0x0000ffff)    //切换了菜单界面则清除闪烁计时
    {
        time = 0;
    }
    return (op_id);    
}

//**********************************************************************
//函数  名:Paw_Dem_Ret_Butt_Pro
//函数功能:压力传感器标定"退出"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音
            if (TRUE == Check_Tips_Disp_Dem())    //有提示信息则删除提示信息
                Cancel_Tips_Dem(&save_err_tips_dem);
            dem_op_st = 0;   
            phase_vent_ctrl = PHASE_STOP_VENT;//停止机动呼吸 
            return(DEMAR_MAIN_MENU_PAW);//切换到主界面"压力传感器标定"选项
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_SAVE);//切换到"保存"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_EXIT);//仍然处在"退出"    
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Save_Butt_Pro
//函数功能:压力传感器标定"保存"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Save_Butt_Pro(uint8 key_c)
{
    uint16 ad;
    
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            ad = paw_ad;
            if (!(Check_Pipeline_P_AD(cur_demar_point,ad)&0xff000000))    //正确
            {
                if (Write_EEPROM(EEP_PIPELINE_P+cur_demar_point*(sizeof(paw_ad_tab[cur_demar_point])), ( uint8 *)(&ad), sizeof(ad)))
                {//写入EEPROM错误
                    Key_No_Valid_Sound();    //按键无效音    
                    Log_Tips_Dem(&save_err_tips_dem);    //显示保存错误信息            
                }
                else
                {
                    Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                    Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //擦除旧曲线
                    Update_Paw_Curve(ad,cur_demar_point);    //更新曲线
                    Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                    Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线
                    Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值    
                    if (paw_zero_ad) //校零值不为0则清零校零值
                    {
                        paw_zero_ad = 0;
                        Put_Zero_Val_Dem_Paw(DISP_RT,paw_zero_ad);//显示校零值
                        Write_EEPROM(EEP_ZERO_P, ( uint8 *)(&(paw_zero_ad)), sizeof(paw_zero_ad));
                    }                        
                    Key_Valid_Sound();    //按键有效音    
                    Log_Tips_Dem(&save_ok_tips_dem);    //显示保存成功信息                
                }
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音    
                Log_Tips_Dem(&ad_err_tips_dem);    //显示AD超出范围的提示信息
            }    
            return(DEMAR_NEW_PAW_SAVE);//仍然处在"保存"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_CHECK);//切换到"检查"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_EXIT);//切换到"退出"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_SAVE);//仍然处在"保存"
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Check_Butt_Pro
//函数功能:压力传感器标定"检查"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if(Check_EEP_Pipeline_P())//曲线检查不合格
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&curve_err_tips_dem);    //显示不合格信息    
            }
            else
            {
                Key_Valid_Sound();    //按键有效音
                Log_Tips_Dem(&curve_ok_tips_dem);    //显示合格信息    
            }
            return(DEMAR_NEW_PAW_CHECK);//仍然处在"检查"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_NEXT);//切换到"下一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_SAVE);//切换到"保存"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_CHECK);//仍然处在"检查"
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Next_Butt_Pro
//函数功能:压力传感器标定"下一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (PAW_DEMAR_POINTS-1 > cur_demar_point)//还有下一点
            {
                Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point++;
                Put_Paw_Dem_Point_Box(DISP_RT,paw_demar_tab[cur_demar_point]);    //显示标定点的压力值
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_NEXT);//仍然处在"下一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_PRE);//切换到"前一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_CHECK);//切换到"检查"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_NEXT);//仍然处在"下一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Pre_Butt_Pro
//函数功能:压力传感器标定"前一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (cur_demar_point > 0)//还有前一点
            {
                Put_Paw_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point--;
                Put_Paw_Dem_Point_Box(DISP_RT,paw_demar_tab[cur_demar_point]);    //显示标定点的压力值
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_PRE);//仍然处在"前一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_AD_IN);//切换到"AD输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_NEW_PAW_NEXT);//切换到"下一点"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_PRE);//仍然处在"前一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:Paw_Dem_Ad_In_Box_Pro
//函数功能:压力传感器标定"AD输入"窗口处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Paw_Dem_Ad_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (dem_op_st&OP_ST_IN)    //处在输入状态
            {
                dem_op_st &= ~OP_ST_IN;    //变为选中状态
                Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                if (dem_op_st&OP_ST_CHG)//被修改则保存
                {
                    if (Write_EEPROM(EEP_PIPELINE_P+cur_demar_point*(sizeof(paw_ad_tab[cur_demar_point])), ( uint8 *)(&(paw_ad_tab[cur_demar_point])), sizeof(paw_ad_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                        Log_Tips_Dem(&save_err_tips_dem);    //提示保存错误
                    }
                    else
                    {
                        Key_Valid_Sound();    //按键有效音    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //按键有效音    
                }
                dem_op_st &= ~OP_ST_CHG;    //去掉修改标记
            }
            else//进入修改状态
            {
                dem_op_st |= OP_ST_IN;
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音    
            }
            return(DEMAR_NEW_PAW_AD_IN);//仍然处在"AD输入"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//擦除旧曲线                        
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_Paw_AD(1,INC_DEC_LONG,cur_demar_point);                                
                }
                else
                {
                    Demar_Change_Paw_AD(1,0,cur_demar_point);                    
                }
                Update_Paw_Curve(paw_ad_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值    
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_NEW_PAW_AD_IN);//仍然处在"AD输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            if (dem_op_st&OP_ST_IN)    //正在修改
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//擦除旧曲线                        
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_Paw_AD(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_Paw_AD(0,0,cur_demar_point);
                }
                Update_Paw_Curve(paw_ad_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_Paw_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                Put_Paw_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线                            
                Put_Paw_Ad_In_Box(DISP_RT|DISP_INPUT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值    
                return(DEMAR_NEW_PAW_AD_IN);//仍然处在"AD输入"
            }
            else
            {    
                Key_Valid_Sound();    //按键有效音    
                if (dem_op_st&OP_ST_ER)//恢复显示
                {
                    Put_Paw_Ad_In_Box(DISP_RT,paw_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                }
                dem_op_st = 0;            
                return(DEMAR_NEW_PAW_PRE);//切换到"前一点"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_NEW_PAW_AD_IN);//仍然处在"AD输入"
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Menu_Pro
//函数功能:氧气浓度传感器标定菜单的处理(10ms处理一次)
//入口参数:key_c--按键码
//            op--当前选项
//出口参数:切换到的选项
//**********************************************************************
static sint32 O2_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_NEW_PAW_OPS]=
    {{DEMAR_O2_SEN_EXIT,O2_Dem_Ret_Butt_Pro},//退出
    {DEMAR_O2_SEN_SAVE,O2_Dem_Save_Butt_Pro},    //保存
    {DEMAR_O2_SEN_CHCEK,O2_Dem_Check_Butt_Pro},    //检查    
    {DEMAR_O2_SEN_NEXT,O2_Dem_Next_Butt_Pro},    //下一点
    {DEMAR_O2_SEN_PRE, O2_Dem_Pre_Butt_Pro},    //前一点
    {DEMAR_O2_SEN_AD_IN,O2_Dem_Ad_In_Box_Pro}};//AD输入
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp = FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms周期闪烁显示)
    {
        if (FALSE == flag_dot_disp)//擦除显示标定点
        {
            flag_dot_disp = TRUE;
            Put_O2_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_O2_Dem_Dot(cur_demar_point,DISP_RT);
        }
        switch (op)
        {
            case DEMAR_O2_SEN_EXIT://退出
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_SAVE://保存            
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_NEXT://下一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_O2_SEN_PRE://前一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_O2_SEN_CHCEK://检查                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_O2_SEN_AD_IN://AD输入                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_O2_Ad_In_Box(di|DISP_RT,o2_ad_tab[cur_demar_point]);
                }
                else//原来显示则擦除
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_O2_Ad_In_Box(di|DISP_RT|DISP_ER,o2_ad_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
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
//函数  名:O2_Dem_Ret_Butt_Pro
//函数功能:氧气传感器标定"退出"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音
            if (TRUE == Check_Tips_Disp_Dem())    //有提示信息则删除提示信息
                Cancel_Tips_Dem(&save_err_tips_dem);
            dem_op_st = 0;    
            return(DEMAR_MAIN_MENU_O2);//切换到主界面"氧气传感器标定"选项
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_SAVE);//切换到"保存"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_EXIT);//仍然处在"退出"    
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Save_Butt_Pro
//函数功能:氧气传感器标定"保存"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Save_Butt_Pro(uint8 key_c)
{
    uint16 ad;
    
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            ad = o2_ad;
            if (!(Check_O2_AD(cur_demar_point,ad)&0xff000000))    //正确
            {
                if (Write_EEPROM(EEP_O2+cur_demar_point*(sizeof(o2_ad_tab[cur_demar_point])), ( uint8 *)(&ad), sizeof(ad)))
                {//写入EEPROM错误
                    Key_No_Valid_Sound();    //按键无效音    
                    Log_Tips_Dem(&save_err_tips_dem);    //显示保存错误信息            
                }
                else
                {
                    Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                    Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //擦除旧曲线
                    Update_O2_Sen_Curve(ad,cur_demar_point);    //更新曲线
                    Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                    Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线
                    Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值                        
                    Key_Valid_Sound();    //按键有效音    
                    Log_Tips_Dem(&save_ok_tips_dem);    //显示保存成功信息                
                }
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音    
                Log_Tips_Dem(&ad_err_tips_dem);    //显示AD超出范围的提示信息
            }    
            return(DEMAR_O2_SEN_SAVE);//仍然处在"保存"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_CHCEK);//切换到"检查"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_EXIT);//切换到"退出"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_SAVE);//仍然处在"保存"
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Check_Butt_Pro
//函数功能:氧气传感器标定"检查"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if(Check_O2_Curve())//曲线检查不合格
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&curve_err_tips_dem);    //显示不合格信息    
            }
            else
            {
                Key_Valid_Sound();    //按键有效音
                Log_Tips_Dem(&curve_ok_tips_dem);    //显示合格信息    
            }
            return(DEMAR_O2_SEN_CHCEK);//仍然处在"检查"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_NEXT);//切换到"下一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_SAVE);//切换到"保存"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_CHCEK);//仍然处在"检查"
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Next_Butt_Pro
//函数功能:氧气传感器标定"下一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (O2_SEN_DEM_POINTS-1 > cur_demar_point)//还有下一点
            {
                Put_O2_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point++;
                Put_O2_Dem_Point_Box(DISP_RT,o2_demar_tab[cur_demar_point]);    //显示标定点的压力值
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_NEXT);//仍然处在"下一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_PRE);//切换到"前一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_CHCEK);//切换到"检查"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_NEXT);//仍然处在"下一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Pre_Butt_Pro
//函数功能:氧气传感器标定"前一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (cur_demar_point > 0)//还有前一点
            {
                Put_O2_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point--;
                Put_O2_Dem_Point_Box(DISP_RT,o2_demar_tab[cur_demar_point]);    //显示标定点的压力值
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_PRE);//仍然处在"前一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_AD_IN);//切换到"AD输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_O2_SEN_NEXT);//切换到"下一点"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_PRE);//仍然处在"前一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:O2_Dem_Ad_In_Box_Pro
//函数功能:氧气传感器标定"AD输入"窗口处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 O2_Dem_Ad_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (dem_op_st&OP_ST_IN)    //处在输入状态
            {
                dem_op_st &= ~OP_ST_IN;    //变为选中状态
                Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                if (dem_op_st&OP_ST_CHG)//被修改则保存
                {
                    if (Write_EEPROM(EEP_O2+cur_demar_point*(sizeof(o2_ad_tab[cur_demar_point])), ( uint8 *)(&(o2_ad_tab[cur_demar_point])), sizeof(o2_ad_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                        Log_Tips_Dem(&save_err_tips_dem);    //提示保存错误
                    }
                    else
                    {
                        Key_Valid_Sound();    //按键有效音    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //按键有效音    
                }
                dem_op_st &= ~OP_ST_CHG;    //去掉修改标记
            }
            else//进入修改状态
            {
                dem_op_st |= OP_ST_IN;
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                Key_Valid_Sound();    //按键有效音    
            }
            return(DEMAR_O2_SEN_AD_IN);//仍然处在"AD输入"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//擦除旧曲线                        
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_O2_AD(1,INC_DEC_LONG,cur_demar_point);                                
                }
                else
                {
                    Demar_Change_O2_AD(1,0,cur_demar_point);                    
                }
                Update_O2_Sen_Curve(o2_ad_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值    
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_O2_SEN_AD_IN);//仍然处在"AD输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            if (dem_op_st&OP_ST_IN)    //正在修改
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);//擦除旧曲线                        
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_O2_AD(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_O2_AD(0,0,cur_demar_point);
                }
                Update_O2_Sen_Curve(o2_ad_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_O2_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                Put_O2_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线                            
                Put_O2_Ad_In_Box(DISP_RT|DISP_INPUT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值    
                return(DEMAR_O2_SEN_AD_IN);//仍然处在"AD输入"
            }
            else
            {    
                Key_Valid_Sound();    //按键有效音    
                if (dem_op_st&OP_ST_ER)//恢复显示
                {
                    Put_O2_Ad_In_Box(DISP_RT,o2_ad_tab[cur_demar_point]);//显示标定点的当前AD值
                }
                dem_op_st = 0;            
                return(DEMAR_O2_SEN_PRE);//切换到"前一点"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_O2_SEN_AD_IN);//仍然处在"AD输入"
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Menu_Pro
//函数功能:潮气量标定菜单的处理(10ms处理一次)
//入口参数:key_c--按键码
//            op--当前选项
//出口参数:切换到的选项
//**********************************************************************
static sint32 Vt_Dem_Menu_Pro(uint8 key_c,sint32 op)
{
    struct op_str op_tab[DEMAR_VT_OPS]=
    {{DEMAR_VT_EXIT,Vt_Dem_Ret_Butt_Pro},//退出
    {DEMAR_VT_SAVE,Vt_Dem_Save_Butt_Pro},    //保存
    {DEMAR_VT_CHECK,Vt_Dem_Check_Butt_Pro},    //检查    
    {DEMAR_VT_NEXT,Vt_Dem_Next_Butt_Pro},    //下一点
    {DEMAR_VT_PRE,Vt_Dem_Pre_Butt_Pro},    //前一点
    {DEMAR_VT_LPS_IN,Vt_Dem_Lps_In_Box_Pro}};//圈数输入
    
    sint32 op_id;
    static uint8 time=0;
    static uint8 flag_dot_disp=FALSE;
    uint8 di;
    
    if (!time)    //mod32(320ms周期闪烁显示)
    {
        if (FALSE == flag_dot_disp)//擦除显示标定点
        {
            flag_dot_disp = TRUE;
            Put_Vt_Dem_Dot(cur_demar_point,DISP_RT|DISP_ER);
        }
        else
        {
            flag_dot_disp = FALSE;
            Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);
        }    
        switch (op)
        {
            case DEMAR_VT_EXIT://退出
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Ret_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Ret_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_SAVE://保存            
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Save_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Save_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_NEXT://下一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Next_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Next_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;
            case DEMAR_VT_PRE://前一点                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Pre_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Pre_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case DEMAR_VT_CHECK://检查                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_Check_Cur_Butt(DISP_RT|DISP_NOT_ER);
                }
                else//原来显示则擦除
                {
                    Put_Check_Cur_Butt(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;    
            case DEMAR_VT_LPS_IN://圈数输入                
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;
                    Put_Vt_Lps_In_Box(di|DISP_RT,vt_lap_tab[cur_demar_point]);
                }
                else//原来显示则擦除
                {
                    if (dem_op_st&OP_ST_IN) di = DISP_INPUT;
                    else di = 0;                
                    Put_Vt_Lps_In_Box(di|DISP_RT|DISP_ER,vt_lap_tab[cur_demar_point]);
                }
                dem_op_st ^= OP_ST_ER;
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
//函数  名:Vt_Dem_Ret_Butt_Pro
//函数功能:潮气量标定"退出"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Ret_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            Key_Valid_Sound();    //按键有效音
            if (TRUE == Check_Tips_Disp_Dem())    //有提示信息则删除
            {
                Cancel_Tips_Dem(&save_err_tips_dem);
            }
            dem_op_st = 0;    
            phase_vent_ctrl = PHASE_STOP_VENT;//停止机动呼吸
            return(DEMAR_MAIN_MENU_VT);//切换到主界面"潮气量标定"选项
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Ret_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_SAVE);//切换到"保存"
            break;                                                                                        
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_EXIT);//仍然处在"退出"    
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Save_Butt_Pro
//函数功能:潮气量标定"保存"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Save_Butt_Pro(uint8 key_c)
{
    sint32 lps;
    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            lps = demar_laps;
            if (Check_Vt_Laps(cur_demar_point,lps)&0xff000000)    //圈数不在正常范围内
            {
                Key_No_Valid_Sound();
                Log_Tips_Dem(&lps_err_tips_dem);    //显示提示信息        
            }
            else
            {
                if(Write_EEPROM(EEP_VTE+cur_demar_point*(sizeof(vt_lap_tab[cur_demar_point])), ( uint8 *)(&(lps)), sizeof(lps)))
                {//保存出错
                    Key_No_Valid_Sound();    //按键无效音
                    Log_Tips_Dem(&save_err_tips_dem);    //显示提示信息                
                }
                else
                {
                    Key_Valid_Sound();    //按键有效音
                    Put_Vt_Sen_Cur(cur_demar_point-1,DISP_ER|DISP_RT);    //擦除旧曲线
                    Put_Vt_Sen_Cur(cur_demar_point,DISP_ER|DISP_RT);    //擦除旧曲线
                    Update_Vt_Cur(lps,cur_demar_point);    //更新曲线
                    Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT);    //显示新曲线
                    Put_Vt_Sen_Cur(cur_demar_point,DISP_RT);    //显示新曲线    
                    Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);    //显示当前圈数    
                    Log_Tips_Dem(&save_ok_tips_dem);    //显示提示信息            
                }
            }    
            return(DEMAR_VT_SAVE);//仍然处在"保存"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_CHECK);//切换到"检查"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Save_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_EXIT);//切换到"退出"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_SAVE);//仍然处在"保存"
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Check_Butt_Pro
//函数功能:潮气量标定"检查"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Check_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (Check_EEP_Vte())//曲线检查不合格
            {
                Key_No_Valid_Sound();    //按键无效音
                Log_Tips_Dem(&curve_err_tips_dem);    //显示提示信息
            }
            else
            {
                Key_Valid_Sound();    //按键有效音
                Log_Tips_Dem(&curve_ok_tips_dem);    //显示提示信息
            }
            return(DEMAR_VT_CHECK);//仍然处在"检查"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_NEXT);//切换到"下一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Check_Cur_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_SAVE);//切换到"保存"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_CHECK);//仍然处在"检查"
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Next_Butt_Pro
//函数功能:潮气量标定"下一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Next_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (VT_DEMAR_POINTS-1 > cur_demar_point)//还有下一点
            {
                Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point++;
                Put_Vt_Dem_Point_Box(DISP_RT,vt_demar_tab[cur_demar_point]);    //显示标定点的潮气量
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_NEXT);//仍然处在"下一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_PRE);//切换到"前一点"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Next_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_CHECK);//切换到"检查"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_NEXT);//仍然处在"下一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Pre_Butt_Pro
//函数功能:潮气量标定"前一点"按钮处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Pre_Butt_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (cur_demar_point > 0)//还有前一点
            {
                Put_Vt_Dem_Dot(cur_demar_point,DISP_RT);//恢复显示以前标定点的图标            
                cur_demar_point--;
                Put_Vt_Dem_Point_Box(DISP_RT,vt_demar_tab[cur_demar_point]);    //显示标定点潮气量
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                Key_Valid_Sound();    //按键有效音
            }
            else
            {
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_PRE);//仍然处在"前一点"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_LPS_IN);//切换到"圈数输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            Key_Valid_Sound();    //按键有效音        
            if (dem_op_st&OP_ST_ER)//恢复显示
            {
                Put_Pre_Butt(DISP_RT);
            }
            dem_op_st = 0;            
            return(DEMAR_VT_NEXT);//切换到"下一点"
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_PRE);//仍然处在"前一点"
            break;            
    }    
}

//**********************************************************************
//函数  名:Vt_Dem_Lps_In_Box_Pro
//函数功能:潮气量标定"圈数输入"窗口处理
//入口参数:key_c--按键代号
//出口参数:切换到的选项ID
//**********************************************************************
static sint32 Vt_Dem_Lps_In_Box_Pro(uint8 key_c)
{
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            if (dem_op_st&OP_ST_IN)    //处在输入状态
            {
                dem_op_st &= ~OP_ST_IN;    //变为选中状态
                Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                if (dem_op_st&OP_ST_CHG)//被修改则保存
                {
                    if(Write_EEPROM(EEP_VTE+cur_demar_point*(sizeof(vt_lap_tab[cur_demar_point])), ( uint8 *)(&(vt_lap_tab[cur_demar_point])), sizeof(vt_lap_tab[cur_demar_point])))
                    {
                        Key_No_Valid_Sound();
                    }
                    else
                    {
                        Key_Valid_Sound();    //按键有效音    
                    }
                }
                else
                {
                    Key_Valid_Sound();    //按键有效音    
                }
                dem_op_st &= ~OP_ST_CHG;    //去掉修改标记
            }
            else//进入修改状态
            {
                dem_op_st |= OP_ST_IN;
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                Key_Valid_Sound();    //按键有效音    
            }
            return(DEMAR_VT_LPS_IN);//仍然处在"圈数输入"
            break;
        case KEY_PRESSED|KEY_INC://增长键
            if (dem_op_st&OP_ST_IN)
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //擦除旧曲线                        
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_VT_Lps(1,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_VT_Lps(1,0,cur_demar_point);
                }
                Update_Vt_Cur(vt_lap_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_NOR);    //显示新曲线
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_NOR);    //显示新曲线                    
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
            }
            else
            {
                Key_No_Valid_Sound();
            }        
            return(DEMAR_VT_LPS_IN);//仍然处在"圈数输入"
            break;    
        case KEY_PRESSED|KEY_DEC://减小键
            if (dem_op_st&OP_ST_IN)    //正在修改
            {
                dem_op_st |= OP_ST_CHG;    //作修改标记
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_ER);    //擦除旧曲线
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_ER);    //擦除旧曲线                    
                if (Get_Key_Status()&(1<<KEY_MENU))//同时按下菜单键则长步调整
                {
                    Demar_Change_VT_Lps(0,INC_DEC_LONG,cur_demar_point);
                }
                else
                {
                    Demar_Change_VT_Lps(0,0,cur_demar_point);
                }
                Update_Vt_Cur(vt_lap_tab[cur_demar_point],cur_demar_point);    //更新曲线
                Put_Vt_Sen_Cur(cur_demar_point-1,DISP_RT|DISP_NOR);    //显示新曲线
                Put_Vt_Sen_Cur(cur_demar_point,DISP_RT|DISP_NOR);    //显示新曲线                        
                Put_Vt_Lps_In_Box(DISP_RT|DISP_INPUT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                return(DEMAR_VT_LPS_IN);//仍然处在"圈数输入"
            }
            else
            {        
                Key_Valid_Sound();    //按键有效音    
                if (dem_op_st&OP_ST_ER)//恢复显示
                {
                    Put_Vt_Lps_In_Box(DISP_RT,vt_lap_tab[cur_demar_point]);//显示标定点的当前圈数
                }
                dem_op_st = 0;            
                return(DEMAR_VT_PRE);//切换到"前一点"
            }
            break;                                                                                                    
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(DEMAR_VT_LPS_IN);//仍然处在"圈数输入"
            break;            
    }    
}

//**********************************************************************
//函数  名:Dem_Lg_Sel_Disp_Pro
//函数功能:语言选择界面下的显示处理(100ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void Dem_Lg_Sel_Disp_Pro(void)
{}

//**********************************************************************
//函数  名:Dem_Sel_Disp_Pro
//函数功能:选择要标定的选项界面下的显示处理(100ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void Dem_Sel_Disp_Pro(void)
{}

//**********************************************************************
//函数  名:Paw_Dem_Disp_Pro
//函数功能:标定压力传感器时的显示处理函数(100ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void Paw_Dem_Disp_Pro(void)
{
    static sint32 ad_disp = 0;    //AD的显示值
    static sint32 p_disp = 0;    //压力量显示值
    
    if (FALSE == Check_Tips_Disp_Dem()) //有提示信息覆盖则不显示
    {
        if (paw_ad != ad_disp)
        {
            ad_disp = paw_ad;
            Put_Paw_Ad_Val(DISP_RT,ad_disp); 
        }
        if (airway_p != p_disp)
        {
            p_disp = airway_p;
            Put_Cur_Pressur_Dem_Paw(DISP_RT,p_disp);
        }
    }
}

//**********************************************************************
//函数  名:O2_Dem_Disp_Pro
//函数功能:标定氧气传感器时的显示处理函数(100ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void O2_Dem_Disp_Pro(void)
{
    static sint32 ad_disp = 0;    //AD的显示值
    static sint32 o2_disp = 0;    //氧气浓度显示值
    static sint32 o2_volt_disp = 0;
    if (FALSE == Check_Tips_Disp_Dem()) //有提示信息覆盖则不显示
    {
       if (o2_ad != ad_disp)
        {
            ad_disp = o2_ad;
            Put_O2_Ad_Val(DISP_RT,ad_disp); 
        }
        if (cur_o2 != o2_disp)
        {
            o2_disp = cur_o2;
            Put_Cur_O2_Dem_O2(DISP_RT,o2_disp);
        }
    }
    if (o2_vol != o2_volt_disp)    //显示氧电池电压
    {
        o2_volt_disp = o2_vol;
        Put_O2_Cell_Volt_Dem_O2(DISP_RT,o2_volt_disp);
    }
}

//**********************************************************************
//函数  名:Vt_Dem_Disp_Pro
//函数功能:标定潮气量时的显示处理函数(100ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void Vt_Dem_Disp_Pro(void)
{
    static sint32 vt_lps_disp = 0;    //风轮圈数的显示值
    static sint32 vt_disp = 0;    //潮气量显示值
    static uint8 flag_br_icon_disp = 0;//呼吸图标显示
    
    
    if (FALSE == Check_Tips_Disp_Dem()) //有提示信息覆盖则不显示
    {
        if (demar_laps != vt_lps_disp)
        {
            vt_lps_disp = demar_laps;
            Put_Vt_Lps_Val(DISP_RT,vt_lps_disp); 
        }
        if (demarc_vte != vt_disp)
        {
            vt_disp = demarc_vte;
            Put_Cur_Vt(DISP_RT,vt_disp);
        }
    }
     Set_Graphic_Color(COL_WHITE,COL_DEEPGREEN);
    switch(phase_vent_ctrl)
    {
        case PHASE_VCV_INSP:    //吸气或者屏气则显示吸气图标
        case PHASE_VCV_HOLD:
            if(0x00 == (flag_br_icon_disp&0x01))    //还没显示吸气图标
            {
                flag_br_icon_disp = 0x01;    //显示吸气图标
                Put_Image(212,80,Vent_Icon_Press,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);
            }    
            break;
        default:
            if(0x00 == (flag_br_icon_disp&0x02))//还没显示呼气图标
            {
                flag_br_icon_disp = 0x02;    //显示呼气图标
                Put_Image(212,80,Vent_Icon,WRITE_OVERLAY);
                LCD_Refresh_Area(212,80,243,109);                    
            }
            break;                    
    }    
}

//**********************************************************************
//函数  名:Menu_Lg_Sel_Op_Dem_Pro
//函数功能:标定时语言设置菜单的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static sint32 Menu_Lg_Sel_Op_Dem_Pro(uint8 key_c,sint32 op)
{ 
    sint32 op_id;    
    struct op_str op_tab[MENU_DEMAR_LG_OP_NUM]=
    {{MENU_DEMAR_LG_SEL_CH_OP,Dem_Ch_Butt_Pro},//"中文"
    {MENU_DEMAR_LG_SEL_EN_OP,Dem_En_Butt_Pro}};//"English"
    static uint8 time; //闪烁时间

    if (!time)    //mod32(320ms周期闪烁显示)
    {
        switch (op)
        {
            case MENU_DEMAR_LG_SEL_CH_OP://中文    
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_CH_Butt_Lg_Sel(DISP_RT);
                }
                else//原来显示则擦除
                {
                    Put_CH_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
                break;        
            case MENU_DEMAR_LG_SEL_EN_OP://English    
                if (dem_op_st&OP_ST_ER)    //擦除了显示
                {//恢复显示
                    Put_EN_Butt_Lg_Sel(DISP_RT);
                }
                else//原来显示则擦除
                {
                    Put_EN_Butt_Lg_Sel(DISP_RT|DISP_ER);
                }
                dem_op_st ^= OP_ST_ER;
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
//函数  名:Dem_Ch_Butt_Pro
//函数功能:"中文"按钮的处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Dem_Ch_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            dem_op_st |= OP_ST_SEL;        //选中状态
            Set_Language_Type(CHINESE);    //设置语言类型
            Key_Valid_Sound();        //发按键有效音        
            return(DEMAR_MAIN_MENU_PAW);//切换到"压力传感器标定"        
            break;
        case KEY_PRESSED|KEY_DEC://减少键
            Key_Valid_Sound();        //发按键有效音                
            if (dem_op_st&OP_ST_ER)    //恢复显示
            {
                Put_CH_Butt_Lg_Sel(DISP_RT);
            }    
            dem_op_st = OP_ST_SEL;
            return(MENU_DEMAR_LG_SEL_EN_OP);//切换到"English"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_DEMAR_LG_SEL_CH_OP);//仍然处在"中文"
            break;            
    }        
}

//**********************************************************************
//函数  名:Dem_En_Butt_Pro
//函数功能:"English"按钮的处理
//入口参数:key_c--按键代码
//出口参数:切换到的选项ID 
//**********************************************************************
static sint32 Dem_En_Butt_Pro(uint8 key_c)
{    
    switch(key_c)
    {
        case KEY_PRESSED|KEY_OK:    //"确认"键
            dem_op_st |= OP_ST_SEL;        //选中状态
            Key_Valid_Sound();        //发按键有效音
            Set_Language_Type(ENGLISH);    //设置语言类型        
            return(DEMAR_MAIN_MENU_PAW);//切换到"压力传感器标定"        
            break;
        case KEY_PRESSED|KEY_INC://增长键
            Key_Valid_Sound();        //发按键有效音                
            if (dem_op_st&OP_ST_ER)    //恢复显示
            {
                Put_EN_Butt_Lg_Sel(DISP_RT);
            }
            dem_op_st = OP_ST_SEL;    
            return(MENU_DEMAR_LG_SEL_CH_OP);//切换到"中文"                    
            break;                            
        default:
            if (key_c&(KEY_PRESSED|KEY_PRESSED_2S|KEY_PRESSED_5S))
            {        
                Key_No_Valid_Sound();    //按键无效音
            }
            return(MENU_DEMAR_LG_SEL_EN_OP);//仍然处在"English"
            break;            
    }        
}



//**********************************************************************
//函数  名:Log_Tips_Dem
//函数功能:把提示信息加入显示链表
//入口参数:tips_ptr--提示信息的指针
//出口参数:无
//**********************************************************************
static void Log_Tips_Dem(Tips_Struct*tips_ptr)
{
    if (NULL == tips_ptr) return;
    if (!flag_tips_saved)    //尚未保存被覆盖的区域则保存
    {
        flag_tips_saved = TRUE;
        Save_Area(12,32,241,69,(uint32 *)save_buf_dem);                //保存覆盖区域
    }
    cur_tips_ptr_dem = tips_ptr;
    tips_disp_time_dem = TIPS_DISP_TIME_DEM;
    tips_disp_trigger_dem = TRIGGER_ON;    //触发显示提示信息
}

//*********************************************************************
//函数  名:Cancel_Tips_Dem
//函数功能:把提示信息从显示链表中删除
//入口参数:tips_ptr--提示信息的指针
//出口参数:无
//*********************************************************************
static void Cancel_Tips_Dem(Tips_Struct*tips_ptr)
{
    if (NULL == tips_ptr) return;                //检查入口参数的合法性
    if (NO_TIPS == tips_ptr->tips_id) return;
    if (FALSE == Check_Tips_Disp_Dem()) return;        //当前无报警信息要取消则返回
    
    Recover_Area(12,32,241,69,(uint32 *)save_buf_dem);//恢复被覆盖的区域
    LCD_Refresh_Area(12,32,241,69);
    cur_tips_ptr_dem = &blank_tips;            //清空提示信息
    flag_tips_saved = FALSE;
    tips_disp_trigger_dem = TRIGGER_OFF;    //触发关闭提示信息
}

//**********************************************************************
//函数  名:Put_Tips_Mess_Dem
//函数功能:显示提示消息(10ms执行一次)
//入口参数:无
//出口参数:无
//**********************************************************************
static void Put_Tips_Mess_Dem(void)
{
    if (TRIGGER_ON == tips_disp_trigger_dem)
    {
        tips_disp_trigger_dem = TRIGGER_NONE;
        Set_Graphic_Color(COL_WHITE,COL_BLACK);
        Set_Write_Mode(WRITE_OVERLAY);//覆盖
        Put_Image(12,32,Tip_Box,WRITE_OVERLAY);

        //Set_Write_Mode(WRITE_EXOR);    //异或
        Set_HZ_Lib(&ST_11);
        Set_ASC_Lib(Arial10B_Font);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Put_Str(15,35,Language_Str((uint8*)cur_tips_ptr_dem->ch_tips,(uint8*)cur_tips_ptr_dem->en_tips));
        LCD_Refresh_Area(12,32,241,69);
    }
    if (Check_Tips_Disp_Dem()) //正在显示提示信息则倒计时
    {
        if(0 == --tips_disp_time_dem)    //倒计时时间到
        Cancel_Tips_Dem(&save_err_tips_dem);
    }
}

//**********************************************************************
//函数  名:Check_Tips_Disp_Dem
//函数功能:检查当前是否显示提示信息
//入口参数:无
//出口参数:TRUE--显示
//**********************************************************************
static uint8 Check_Tips_Disp_Dem(void)
{
    if (NO_TIPS ==cur_tips_ptr_dem->tips_id) return(FALSE);
    return(TRUE);
}