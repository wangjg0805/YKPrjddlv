//**********************************************************************
//文件名:sys_para.c
//说  明:全局变量/系统参数的操作
//函数列表:
//1.改变窒息报警的设置:Change_Asph_Alm_Set
//2.获取当前的窒息报警设置值:Get_Asph_Alm_Set
//3.把窒息报警设置保存到EEPROM中:Wr_Asph_EEPROM
//4.从EEPROM中读取窒息报警的设置值:Rd_Asph_EEPROM
//5.重置报警静音时间:Reset_Alm_Silence_T
//6.获取报警静音的剩余时间:Get_Alm_Silence_T
//7.保存用户设置的呼吸频率值:Save_Rate_Usr_Set
//8.从EEPROM中读取用户设置的呼吸频率值:Rd_Rate_Usr_Set_EEP
//9.获取频率设置窗口的输入值:Get_Rate_Usr_Set
//10.检查呼吸频率的设置值:Check_Rate_Usr_Set
//11.修改呼吸频率的设置值:Inc_Dec_Rate_Usr_Set
//12.获取吸呼比输入窗口的设置值:Get_IE_Usr_Set
//13.修改吸呼比的设置值:Inc_Dec_IE_Usr_Set
//14.检查吸呼比的设置值是否超出范围:Check_IE_Usr_Set
//15.保存吸呼比的设置值:Save_IE_Usr_Set
//16.从EEPROM中读取吸呼比的设置值:Rd_IE_Usr_Set_EEP
//17.获取压力限制窗口的输入值:Get_Pl_Usr_Set
//18.修改压力限制的设置值:Inc_Dec_Pl_Usr_Set
//19.检查压力限制的设置值是否超出范围:Check_Pl_Usr_Set
//20.保存压力限制的设置值:Save_Pl_Usr_Set
//21.从EEPROM中读取压力限制的设置值:Rd_Pl_Usr_Set_EEP
//22.获取吸气暂停窗口的输入值:Get_Asph_Usr_Set
//23.修改吸气暂停的设置值:Inc_Dec_Asph_Usr_Set
//24.检查吸气暂停设置值是否超出范围:Check_Asph_Usr_Set
//25.保存吸气暂停的设置值:Save_Asph_Usr_Set
//26.从EEPROM中读取吸气暂停的设置值:Rd_Asph_Usr_Set_EEP
//27.获取触发压力的设置值:Get_P_Trig_Usr_Set
//28.修改触发压力的设置值:Inc_Dec_P_Trig_Usr_Set
//29.检查触发压力的设置值:Check_P_Trig_Usr_Set
//30.保存触发压力的设置值:Save_P_Trig_Usr_Set
//31.获取触发压力的设置值:Rd_P_Trig_Usr_Set_EEP
//32.获取触发窗口的设置值:Get_Trig_Win_Usr_Set
//33.修改触发窗口的设置值:Inc_Dec_Trig_Win_Usr_Set
//34.检查触发窗口的设置值:Check_Trig_Win_Usr_Set
//35.保存触发窗口的设置值:Save_Trig_Win_Usr_Set
//36.获取触发窗口的设置值:Rd_Trig_Win_Usr_Set_EEP
//37.获取显示对比度的设置值:Get_Scr_Bri
//38.修改显示对比度:Inc_Dec_Scr_Bri
//39.检查对比度的设置是否超出范围:Check_Scr_Bri
//40.保存对比的设置:Save_Scr_Bri
//41.从EEPROM中读取对比度的设置:Rd_Scr_BRi_EEP
//42.获取报警音量的设置:Get_Alm_Vol
//43.修改报警音量:Inc_Dec_Alm_Vol
//44.检查报警音量的设置是否超出范围:Check_Alm_Vol
//45.保存报警音量:Save_Alm_Vol
//46.从EEPROM中读取报警音量:Rd_Alm_Vol_EEP
//47.获取“主界面全中文”的设置:Get_All_Ch
//48.修改"主界面全中文"的设置:Change_All_Ch_Set
//49.保存"主界面全中文"的设置:Save_All_Ch_Set
//50.从EEPROM中读取"主界面全中文"的设置:Rd_All_Ch_Set_EEP
//51.获取当前的呼吸模式设置:Get_Br_Mode_Sel
//52.设置呼吸模式:Set_Br_Mode_Sel
//53.保存呼吸模式的设置:Save_Br_Mode_Sel
//54.从EEPROM中读取呼吸模式的设置:Rd_Br_M_Sel_EEP
//55.获取氧气浓度报警的下限值:Get_O2_Alm_L
//56.修改氧气浓度报警的下限值:Inc_Dec_O2_Alm_L
//57.检查氧气浓度报警的下限值是否超出范围:Check_O2_Alm_L
//58.保存氧气浓度报警的下限值:Save_O2_Alm_L
//59.从EEPROM中读取氧气浓度报警的下限值:Rd_O2_Alm_L_EEP
//60.获取氧气浓度报警的上限值:Get_O2_Alm_H
//61.修改氧气浓度报警的上限值:Inc_Dec_O2_Alm_H
//62.检查氧气浓度报警的上限值是否超出范围:Check_O2_Alm_H
//63.保存氧气浓度报警的上限值:Save_O2_Alm_H
//64.从EEPROM中读取氧气浓度报警的上限值:Rd_O2_Alm_H_EEP
//66.获取分钟通气报警的下限值:Get_VE_Alm_L
//67.修改分钟通气报警的下限值:Inc_Dec_Ve_Alm_L
//68.检查分钟通气报警的下限值是否超出范围:Check_Ve_Alm_L
//69.保存分钟通气报警的下限值:Save_Ve_Alm_L
//70.从EEPROM中读取分钟通气报警的下限值:Rd_Ve_Alm_L_EEP
//71.读取分钟通气报警的上限值:Get_VE_Alm_H
//72.修改分钟通气报警的上限值:Inc_Dec_Ve_Alm_H
//73.检查分钟通气报警的上限值是否超出范围:Check_Ve_Alm_H
//74.保存分钟通气报警的上限值:Save_Ve_Alm_H
//75.从EEPROM中读取分钟通气报警的上限值:Rd_Ve_Alm_H_EEP
//76.获取潮气量报警的下限值:Get_Vt_Alm_L
//77.修改潮气量报警的下限值:Inc_Dec_Vt_Alm_L
//78.检查潮气量报警的下限值:Check_Vt_Alm_L
//79.保存潮气量报警的下限值:Save_Vt_Alm_L
//80.读取潮气量报警的下限值:Rd_Vt_Alm_L_EEP
//81.获取潮气量报警的上限值:Get_Vt_Alm_H
//82.修改潮气量报警的上限值:Inc_Dec_Vt_Alm_H
//83.检查潮气报警的上限值是否超出范围:Check_Vt_Alm_H
//84.保存潮气报警的上限值:Save_Vt_Alm_H
//85.从EEPROM中读取潮气量报警的上限值:Rd_Vt_Alm_H_EEP
//86.获取潮气量报警"开/关"的设置:Get_Vt_Alm_Switch
//87.修改潮气量报警的"开/关"设置:Change_Vt_Alm_Switch
//88.获取海拔高度:Get_Alt
//89.修改海拔高度:Inc_Dec_Alt
//90.检查海拔高度的设置是否超出范围:Check_Alt
//91.保存海拔高度;Save_Alt
//92.从EEPROM中读取海拔高度:Rd_Alt_EEP
//93.读取驱动气体:Get_Drv_Gas
//94.设置驱动气体:Set_Drv_Gas
//95.保存驱动气体:Save_Drv_Gas
//96.从EEPROM中读取驱动气体:Rd_Drv_Gas_EEP
//97.读取氧浓度监测"开/关":Get_O2_Inspect
//98.设置氧浓度监测"开/关":Set_O2_Inspect
//99.保存氧浓度监测"开/关":Save_O2_Inspect
//100.从EEPROM中读取氧浓度监测"开/关":Rd_O2_Inspect_EEP
//101.读取报警信息显示的"开/关":Get_Alm_Mess_D
//102.设置报警信息显示的"开/关"设置:Set_Alm_Mess_D
//103.保存报警信息显示的"开/关"设置:Save_Alm_Mess_D
//104.从EEPROM中读取报警信息显示的"开/关"设置:Rd_Alm_Mess_D_EEP
//105.读取最大压力测量值:Get_Pmax
//106.读取最大压力测量值的单位:Get_Pmax_Units
//107.设置最大压力测量值:Set_Pmax
//108.读取平均压力测量值:Get_Pavr
//109.读取平均压力测量值的单位:Get_Pavr_Units
//110.设置平均压力测量值:Set_Pavr
//111.读取压力的实时测量值:Get_Paw
//112.读取压力测量值的单位:Get_Paw_Units
//113.设置压力测量值:Set_Paw
//114.获取最小压力测量值:Get_Pmin
//115.获取最小压力测量值的单位:Get_Pmin_Units
//116.设置最小压力测量值:Set_Pmin
//117.获取潮气量测量值:Get_Vt
//118.获取潮气量测量值的单位:Get_Vt_Units
//119.设置潮气量测量值:Set_Vt
//120.获取呼吸频率的测量值:Get_Rate_Meas
//121.设置呼吸频率的测量值:Set_Rate_Meas
//122.获取分钟通气测量值:Get_Ve
//123.获取分钟通气测量值的单位:Get_Ve_Units
//124.设置分钟通气测量值:Set_Ve
//125.读取氧气浓度:Get_O2
//126.读取氧气浓度的单位:Get_O2_Units
//127.设置氧气浓度的测量值:Set_O2
//128.获取顺应性测量值:Get_Humor
//129.获取顺应性测量值的单位:Get_Humor_Units
//130.设置顺应性测量值:Set_Humor
//131.获取蓄电池电压的测量值:Get_Batt_Vol
//132.记录蓄电池电压的测量值:Log_Batt_Vol
//133.获取充电电流的测量值:Get_Batt_Charge_I
//134.记录充电电流的测量值:Log_Batt_Charge_I
//135.获取按键音"开/关"的设置:Get_Key_Sound_Switch
//136.设置按键音"开/关"的设置:Set_Key_Sound_Switch
//137.保存按键音"开/关"的设置:Save_Key_Sound_Switch
//138.从EEPROM中读取按键音"开/关"的设置:Rd_Key_Sound_Switch_EEP
//139.获取潮气量滤波常数的设置:Get_Vt_Filter_Const
//140.修改潮气量滤波常数的设置:Inc_Dec_Vt_Filt_C
//141.检查潮气量滤波常数的设置:Check_Vte_Filt_C
//142.保存潮气量滤波常数的设置:Save_Vt_Filt_C
//143.从EEPROM中读取潮气量滤波常数的设置:Rd_Vt_Filt_C_EEP
//144.获取氧浓度刷新周期的设置:Get_O2_Flash_Cyc
//145.修改氧浓度刷新周期的设置:Inc_Dec_O2_Flash_Cyc
//146.检查氧浓度刷新周期的设置:Check_O2_Flash_Cyc
//147.保存氧浓度刷新周期:Save_O2_Flash_Cyc
//148.从EEPROM中读取氧浓度刷新周期:Rd_O2_Flash_Cyc_EEP
//149.获取压力限制补偿的设置:Get_Pl_Comp
//150.修改压力限制补偿的设置:Inc_Dec_Pl_Comp
//151.检查压力限制补偿的设置:Check_Pl_Comp
//152.保存压力限制补偿的设置:Save_Pl_Comp
//153.从EEPROM中读取压力限制补偿的设置:Rd_Pl_Comp_EEP
//154.获取管压无提升的判断门槛:Get_Paw_No_Raise
//155.修改管压无提升的判断门槛:Inc_Dec_Paw_No_R
//156.检查管压无提升的判断门槛:Check_Paw_No_R
//157.保存管压无提升的判断门槛:Save_Paw_No_R
//158.从EEPROM中读取管压无提升的判断门槛:Rd_Paw_No_R_EEP

//**********************************************************************
#include "global.h"
#include "sys_para.h"
#include "eeprom.h"
#include "Timer.h"
#include "alarm_pro.h"
#include "type_def.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

#define SCR_BRI_MAX        0x07    //LCD对比度的最大值
#define SCR_BRI_MIN        0x00    //LCD对比度最小值
#define SCR_BRI_DEF        0x03    //LCD对比度默认值
static sint8 scr_brightness;        //LCD对比度的设置值
#define ALM_VOL_MAX        0x07    //报警音量最大值
#define ALM_VOL_MIN        0x00    //报警音量最小值
#define ALM_VOL_DEF        0x03    //报警音量默认值
static sint8 alm_vol;            //报警音量设置值
static uint8 main_inter_ch;        //主界面全中文
static uint8 breath_mode_sel;    //选择的呼吸模式
static uint8 drv_gas;            //驱动气体
static uint8 o2_inspect;            //氧浓度监测
static uint8 alm_mess_disp;        //报警信息显示
static uint8 key_sound_switch;    //按键音开关


//**********************************************************************
//函数  名:Change_Asph_Alm_Set
//函数功能:改变窒息报警的设置
//入口参数:无
//出口参数:无
//**********************************************************************
void Change_Asph_Alm_Set(void)
{
    asph_al_set = (asph_al_set==ON)?OFF:ON;
    asph_al_set_chg_flag = TRUE;    //触发闪烁显示
}

//**********************************************************************
//函数  名:Get_Asph_Alm_Set
//函数功能:获取当前窒息报警的设置
//入口参数:无
//出口参数:窒息报警的设置
//**********************************************************************
uint8 Get_Asph_Alm_Set(void)
{
    return (asph_al_set);
}

//**********************************************************************
//函数  名:Wr_Asph_EEPROM
//函数功能:把窒息报警的设置保存到EEPROM中
//入口参数:无
//出口参数:OK--保存正确
//           ERROR--保存错误
//**********************************************************************
uint8 Wr_Asph_EEPROM(void)
{
    if(Write_EEPROM_1B(EEP_ASPH_ALM_SET,asph_al_set))
    {
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Asph_EEPROM
//函数功能:从EEPROM中读取窒息报警的设置值
//入口参数:设置值的指针
//出口参数:OK--读取正确
//           ERROR--读取错误
//**********************************************************************
uint8 Rd_Asph_EEPROM(void)
{
    if(Read_EEPROM_1B(EEP_ASPH_ALM_SET,( uint8*)&asph_al_set))
    {
        asph_al_set = ON;
        return(ERROR);
    }
    else
    {
        if ((asph_al_set != OFF)&&(asph_al_set != ON))
        {
            asph_al_set = ON;
            Wr_Asph_EEPROM();
        }
        return(OK);
    }
}


//**********************************************************************
//函数  名:Reset_Alm_Silence_T
//函数功能:重置报警静音时间
//入口参数:无
//出口参数:无
//**********************************************************************
void Reset_Alm_Silence_T(uint8 type)
{
    if (RST_ALM_SILENCE == type)
    {//静音当前报警
        al_silence_time = TIME_ALARM_MUTE;
    }
    else if (RST_ALM_FORE_SILENCE == type)//预静音
    {
        al_silence_time = TIME_NORMAL_MUTE;
    }
    else//清零报警静音时间
    {
        al_silence_time = 0;
    }    
    flag_alm_silence_t_ok = TRUE;
}

//**********************************************************************
//函数  名:Get_Alm_Silence_T
//函数功能:获取报警静音剩余时间(单位1s)
//入口参数:无
//出口参数:无
//**********************************************************************
uint8 Get_Alm_Silence_T(void)
{
    if (al_silence_time > TIME_ALARM_MUTE)
        al_silence_time = TIME_ALARM_MUTE;
        
    if (al_silence_time >= 0)
    {
        return(al_silence_time/1000);
    }
    else
    {
        return(0);
    }
}

//**********************************************************************
//函数  名:Save_Rate_Usr_Set
//函数功能:保存用户设置的呼吸频率
//入口参数:无
//出口参数:OK--保存成功
//           ERROR--保存失败
//**********************************************************************
uint8 Save_Rate_Usr_Set(void)
{
    if(Write_EEPROM(EEP_RATE_SET,( uint8*)(&br_rate_set_disp),sizeof(br_rate_set_disp)) )
    {//保存错误
        br_rate_set_disp = br_rate_set;
        return (ERROR);
    }
    else
    {
        br_rate_set = br_rate_set_disp;    //输入值有效
        return (OK);
    }
}

//**********************************************************************
//函数  名:Rd_Rate_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的呼吸频率值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Rate_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_RATE_SET,(uint8*)(&br_rate_set_disp),sizeof(br_rate_set_disp)))
    {
        br_rate_set_disp = br_rate_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        br_rate_set = br_rate_set_disp;    
        if (Check_Rate_Usr_Set() == ERROR)
        {
            Save_Rate_Usr_Set();//保存默认值
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Rate_Usr_Set
//函数功能:获取呼吸频率设置窗口的输入值
//入口参数:无
//出口参数:呼吸频率
//**********************************************************************
sint32 Get_Rate_Usr_Set(void)
{
    if (br_rate_set_disp > RATE_SET_MAX) br_rate_set_disp=RATE_SET_MAX;
    if (br_rate_set_disp < RATE_SET_MIN) br_rate_set_disp=RATE_SET_MIN;
    return(br_rate_set_disp);
}

//**********************************************************************
//函数  名:Check_Rate_Usr_Set
//函数功能:检查用户设置的呼吸频率值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Rate_Usr_Set(void)
{
    if (br_rate_set_disp > RATE_SET_MAX)//太大
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);
    }
    else if (br_rate_set > RATE_SET_MAX)
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);        
    }
    else if (br_rate_set_disp < RATE_SET_MIN)//太小
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);            
    }
    else if (br_rate_set < RATE_SET_MIN)
    {
        br_rate_set_disp = br_rate_set = RATE_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Inc_Dec_Rate_Usr_Set
//函数功能:加/减呼吸频率的设置值,并检查修改值是否正确
//入口参数:dir--方向
//            step--步长
//出口参数:OK--修改正确
//           ERROR--修改错误
//**********************************************************************
uint8 Inc_Dec_Rate_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((br_rate_set_disp+step)<=RATE_SET_MAX)
        {
            br_rate_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//小于最小吸气时间
                br_rate_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//小于最小呼气时间
                br_rate_set_disp -= step;
                return(ERROR);
            }        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((br_rate_set_disp-step)>=RATE_SET_MIN)
        {
            br_rate_set_disp -= step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set] > TIME_MAX_INSP * 256)
            {//大于最大吸气时间
                br_rate_set_disp += step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//函数  名:Get_IE_Usr_Set
//函数功能:获取吸呼比设置窗口的输入值
//入口参数:无
//出口参数:吸呼比
//**********************************************************************
uint8 Get_IE_Usr_Set(void)
{
    if (ie_set_disp > IE_IND_MAX) ie_set_disp = IE_IND_MAX;
    if (ie_set_disp <= IE_IND_MIN) ie_set_disp = IE_IND_MIN;
    return(ie_set_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_IE_Usr_Set
//函数功能:加/减吸呼比的设置值,并检查修改值是否正确
//入口参数:dir--方向
//            step--步长
//出口参数:OK--修改正确
//           ERROR--修改错误
//**********************************************************************
uint8 Inc_Dec_IE_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((ie_set_disp+step)<=IE_IND_MAX)
        {
            ie_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//小于最小吸气时间
                ie_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//小于最小呼气时间
                ie_set_disp -= step;
                return(ERROR);
            }                        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((ie_set_disp-step)>=IE_IND_MIN)
        {
            ie_set_disp -= step;
            if(rate_cycle_tab[br_rate_set] * ie_i_tab[ie_set_disp] > TIME_MAX_INSP * 256)
            {//大于最大吸气时间
                ie_set_disp += step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//函数  名:Check_IE_Usr_Set
//函数功能:检查用户设置的吸呼比值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_IE_Usr_Set(void)
{
    if (ie_set_disp > IE_IND_MAX)//太大
    {
        ie_set_disp = ie_set =IE_IND_DEF;
        return(ERROR);
    }
    else if (ie_set > IE_IND_MAX)
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);        
    }
    else if (ie_set_disp < IE_IND_MIN)//太小
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);            
    }
    else if (ie_set_disp < IE_IND_MIN)
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}
//**********************************************************************
//函数  名:Save_IE_Usr_Set
//函数功能:保存用户输入的吸呼比值,如果保存成功则输入值有效,否则回复原来的设置值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_IE_Usr_Set(void)
{
    if (Write_EEPROM(EEP_IE_SET,( uint8*)(&ie_set_disp),sizeof(ie_set_disp)) )
    {
        ie_set_disp = ie_set;
        return(ERROR);
    }
    else
    {
        ie_set = ie_set_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_IE_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的吸呼比值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_IE_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_IE_SET,( uint8*)(&ie_set_disp),sizeof(ie_set_disp)))
    {
        ie_set_disp = ie_set = IE_IND_DEF;
        return(ERROR);
    }
    else
    {
        ie_set = ie_set_disp;    
        if (Check_IE_Usr_Set() == ERROR)
        {
            Save_IE_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Pl_Usr_Set
//函数功能:获取压力限制窗口的输入值
//入口参数:无
//出口参数:压力限制
//**********************************************************************
sint16 Get_Pl_Usr_Set(void)
{
    if (paw_limit_set_disp>PL_USR_SET_MAX) paw_limit_set_disp=PL_USR_SET_MAX;
    if (paw_limit_set_disp<PL_USR_SET_MIN) paw_limit_set_disp=PL_USR_SET_MIN;
    return(paw_limit_set_disp);    
}

//**********************************************************************
//函数  名:Inc_Dec_Pl_Usr_Set
//函数功能:加/减压力限制的设置值,并检查修改值是否正确
//入口参数:dir--方向
//            step--步长
//出口参数:OK--修改正确
//           ERROR--修改错误
//**********************************************************************
uint8 Inc_Dec_Pl_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((paw_limit_set_disp+step)<=PL_USR_SET_MAX)
        {
            paw_limit_set_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((paw_limit_set_disp-step)>=PL_USR_SET_MIN)
        {
            paw_limit_set_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
}

//**********************************************************************
//函数  名:Check_Pl_Usr_Set
//函数功能:检查用户设置的压力限制值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Pl_Usr_Set(void)
{
    if (paw_limit_set_disp > PL_USR_SET_MAX)//太大
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else if (paw_limit_set > PL_USR_SET_MAX)
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);        
    }
    else if (paw_limit_set_disp < PL_USR_SET_MIN)//太小
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);            
    }
    else if (paw_limit_set < PL_USR_SET_MIN)
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Pl_Usr_Set
//函数功能:保存用户输入的压力限制值,如果保存成功则输入值有效,否则回复原来的设置值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Pl_Usr_Set(void)
{
    if (Write_EEPROM(EEP_PL_SET,( uint8*)(&paw_limit_set_disp),sizeof(paw_limit_set_disp)) )
    {
        paw_limit_set_disp = paw_limit_set;
        return(ERROR);
    }
    else
    {
        paw_limit_set = paw_limit_set_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Pl_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的压力限制值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Pl_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_PL_SET,( uint8*)(&paw_limit_set_disp),sizeof(paw_limit_set_disp)))
    {
        paw_limit_set_disp = paw_limit_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        paw_limit_set = paw_limit_set_disp;    
        if (Check_Pl_Usr_Set() == ERROR)
        {
            Save_Pl_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Asph_Usr_Set
//函数功能:获取吸气暂停窗口的输入值
//入口参数:无
//出口参数:吸气暂停
//**********************************************************************
uint8 Get_Asph_Usr_Set(void)
{
    if (asph_set_disp>ASPH_USR_SET_MAX) asph_set_disp=ASPH_USR_SET_MAX;
    if (asph_set_disp<=ASPH_USR_SET_MIN) asph_set_disp=ASPH_USR_SET_MIN;
    return(asph_set_disp);    
}

//**********************************************************************
//函数  名:Inc_Dec_Asph_Usr_Set
//函数功能:增加/减少屏气时间的设置值,并检查修改是否正确
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Asph_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((asph_set_disp+step)<=ASPH_USR_SET_MAX)
        {
            asph_set_disp += step;
            if(rate_cycle_tab[br_rate_set_disp] * ie_i_tab[ie_set]*(100-asph_set) < TIME_MIN_INSP * 256*100)
            {//小于最小吸气时间
                asph_set_disp -= step;
                return(ERROR);
            }
            if(rate_cycle_tab[br_rate_set_disp]*(256-ie_i_tab[ie_set]) < TIME_MIN_INSP * 256)
            {//小于最小呼气时间
                asph_set_disp -= step;
                return(ERROR);
            }            
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((asph_set_disp-step)>=ASPH_USR_SET_MIN)
        {
            asph_set_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Asph_Usr_Set
//函数功能:检查用户设置的吸气暂停值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Asph_Usr_Set(void)
{
    if (asph_set_disp > ASPH_USR_SET_MAX)//太大
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);
    }
    else if (asph_set > ASPH_USR_SET_MAX)
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);        
    }
    else if (asph_set_disp < ASPH_USR_SET_MIN)//太小
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);            
    }
    else if (asph_set < ASPH_USR_SET_MIN)
    {
        asph_set_disp = asph_set = ASPH_USR_SET_DEF;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Asph_Usr_Set
//函数功能:保存用户输入的吸气暂停值,如果保存成功则输入值有效,否则回复原来的设置值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Asph_Usr_Set(void)
{
    if (Write_EEPROM(EEP_ASPH_SET,( uint8*)(&asph_set_disp),sizeof(asph_set_disp)) )
    {
        asph_set_disp = asph_set;
        return(ERROR);
    }
    else
    {
        asph_set = asph_set_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Asph_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的吸气暂停值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Asph_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_ASPH_SET,( uint8*)(&asph_set_disp),sizeof(asph_set_disp)))
    {
        asph_set_disp = asph_set = PL_USR_SET_DEF;
        return(ERROR);
    }
    else
    {
        asph_set = asph_set_disp;    
        if (Check_Asph_Usr_Set() == ERROR)
        {
            Save_Asph_Usr_Set();
        }

        return(OK);
    }
}


//**********************************************************************
//函数  名:Get_P_Trig_Usr_Set
//函数功能:获取触发压力的输入值
//入口参数:无
//出口参数:触发压力
//**********************************************************************
uint8 Get_P_Trig_Usr_Set(void)
{
    if (trigger_p_disp>TRIGGER_P_SET_MAX) trigger_p_disp=TRIGGER_P_SET_MAX;
    if (trigger_p_disp<=TRIGGER_P_SET_MIN) trigger_p_disp=TRIGGER_P_SET_MIN;
    return(trigger_p_disp);    
}

//**********************************************************************
//函数  名:Inc_Dec_P_Trig_Usr_Set
//函数功能:增加/减少触发压力的设置值,并检查修改是否正确
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_P_Trig_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((trigger_p_disp+step)<=TRIGGER_P_SET_MAX)
        {
            trigger_p_disp += step;        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((trigger_p_disp-step)>=TRIGGER_P_SET_MIN)
        {
            trigger_p_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_P_Trig_Usr_Set
//函数功能:检查用户设置的触发压力值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_P_Trig_Usr_Set(void)
{
    if (trigger_p_disp > TRIGGER_P_SET_MAX)//太大
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);
    }
    else if (trigger_p > TRIGGER_P_SET_MAX)
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);        
    }
    else if (trigger_p_disp < TRIGGER_P_SET_MIN)//太小
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);            
    }
    else if (trigger_p < TRIGGER_P_SET_MIN)
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_P_Trig_Usr_Set
//函数功能:保存用户输入的触发压力值,如果保存成功则输入值有效,否则回复原来的设置值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_P_Trig_Usr_Set(void)
{
    if (Write_EEPROM(EEP_TRIGGER_P,( uint8*)(&trigger_p_disp),sizeof(trigger_p_disp)) )
    {
        trigger_p_disp = trigger_p;
        return(ERROR);
    }
    else
    {
        trigger_p = trigger_p_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_P_Trig_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的触发压力值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_P_Trig_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_TRIGGER_P,( uint8*)(&trigger_p_disp),sizeof(trigger_p_disp)))
    {
        trigger_p_disp = trigger_p = TRIGGER_P_SET_DEFAULT;
        return(ERROR);
    }
    else
    {
        trigger_p = trigger_p_disp;    
        if (Check_P_Trig_Usr_Set() == ERROR)
        {
            Save_P_Trig_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Trig_Win_Usr_Set
//函数功能:获取触发窗口的输入值
//入口参数:无
//出口参数:吸气暂停
//**********************************************************************
uint8 Get_Trig_Win_Usr_Set(void)
{
    if (trigger_window_disp>TRIGGER_WIN_SET_MAX) trigger_window_disp=TRIGGER_WIN_SET_MAX;
    if (trigger_window_disp<=TRIGGER_WIN_SET_MIN) trigger_window_disp=TRIGGER_WIN_SET_MIN;
    return(trigger_window_disp);    
}

//**********************************************************************
//函数  名:Inc_Dec_Trig_Win_Usr_Set
//函数功能:增加/减少触发窗口的设置值,并检查修改是否正确
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Trig_Win_Usr_Set(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((trigger_window_disp+step)<=TRIGGER_WIN_SET_MAX)
        {
            trigger_window_disp += step;        
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((trigger_window_disp-step)>=TRIGGER_WIN_SET_MIN)
        {
            trigger_window_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Trig_Win_Usr_Set
//函数功能:检查用户设置的触发窗口值是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Trig_Win_Usr_Set(void)
{
    if (trigger_window_disp > TRIGGER_WIN_SET_MAX)//太大
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);
    }
    else if (trigger_window > TRIGGER_WIN_SET_MAX)
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);        
    }
    else if (trigger_window_disp < TRIGGER_WIN_SET_MIN)//太小
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);            
    }
    else if (trigger_window < TRIGGER_WIN_SET_MIN)
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);        
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Trig_Win_Usr_Set
//函数功能:保存用户输入的触发窗口值,如果保存成功则输入值有效,否则回复原来的设置值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Trig_Win_Usr_Set(void)
{
    if (Write_EEPROM(EEP_TRIGGER_WIN,( uint8*)(&trigger_window_disp),sizeof(trigger_window_disp)) )
    {
        trigger_window_disp = trigger_window;
        return(ERROR);
    }
    else
    {
        trigger_window = trigger_window_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Trig_Win_Usr_Set_EEP
//函数功能:从EEPROM中读出用户设置的触发窗口值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Trig_Win_Usr_Set_EEP(void)
{
    if (Read_EEPROM(EEP_TRIGGER_WIN,( uint8*)(&trigger_window_disp),sizeof(trigger_window_disp)))
    {
        trigger_window_disp = trigger_window = TRIGGER_WIN_SET_DEFAULT;
        return(ERROR);
    }
    else
    {
        trigger_window = trigger_window_disp;    
        if (Check_Trig_Win_Usr_Set() == ERROR)
        {
            Save_Trig_Win_Usr_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Scr_Bri
//函数功能:获取当前的显示器对比度设置值
//入口参数:无
//出口参数:对比度
//**********************************************************************
uint8 Get_Scr_Bri(void)
{
    if (scr_brightness > SCR_BRI_MAX) scr_brightness = SCR_BRI_MAX;
    if (scr_brightness < SCR_BRI_MIN) scr_brightness = SCR_BRI_MIN;
    return(scr_brightness);    
}

//**********************************************************************
//函数  名:Inc_Dec_Scr_Bri
//函数功能:增加/减少显示器对比度
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Scr_Bri(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((scr_brightness+step)<=SCR_BRI_MAX)
        {
            scr_brightness += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((scr_brightness-step)>=SCR_BRI_MIN)
        {
            scr_brightness -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Scr_Bri
//函数功能:检查用户设置的显示器对比度是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Scr_Bri(void)
{
    if (scr_brightness > SCR_BRI_MAX)//太大
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else if (scr_brightness < SCR_BRI_MIN)//太小
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Scr_Bri
//函数功能:保存用户输入的对比度值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Scr_Bri(void)
{
    if (Write_EEPROM(EEP_SCR_BR,( uint8*)(&scr_brightness),sizeof(scr_brightness)) )
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Scr_BRi_EEP
//函数功能:从EEPROM中读出用户设置的LCD对比度,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Scr_BRi_EEP(void)
{
    if (Read_EEPROM(EEP_SCR_BR,( uint8*)(&scr_brightness),sizeof(scr_brightness)))
    {
        scr_brightness = SCR_BRI_DEF;
        return(ERROR);
    }
    else
    {
        if (Check_Scr_Bri() == ERROR)
        {
            Save_Scr_Bri();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Alm_Vol
//函数功能:获取当前的报警音量设置值
//入口参数:无
//出口参数:报警音量
//**********************************************************************
uint8 Get_Alm_Vol(void)
{
    if (alm_vol > ALM_VOL_MAX) alm_vol = ALM_VOL_MAX;
    if (alm_vol < ALM_VOL_MIN) alm_vol = ALM_VOL_MIN;
    return(alm_vol);    
}

//**********************************************************************
//函数  名:Inc_Dec_Alm_Vol
//函数功能:增加/减少报警音量
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Alm_Vol(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((alm_vol+step)<=ALM_VOL_MAX)
        {
            alm_vol += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((alm_vol-step)>=ALM_VOL_MIN)
        {
            alm_vol -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Alm_Vol
//函数功能:检查报警音量设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Alm_Vol(void)
{
    if (alm_vol > ALM_VOL_MAX)//太大
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else if (alm_vol < ALM_VOL_MIN)//太小
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Alm_Vol
//函数功能:保存用户输入的报警音量,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Alm_Vol(void)
{
    if (Write_EEPROM(EEP_ALM_VOL,( uint8*)(&alm_vol),sizeof(alm_vol)) )
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Alm_Vol_EEP
//函数功能:从EEPROM中读出用报警音量,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Alm_Vol_EEP(void)
{
    if (Read_EEPROM(EEP_ALM_VOL,( uint8*)(&alm_vol),sizeof(alm_vol)) )
    {
        alm_vol = ALM_VOL_DEF;
        return(ERROR);
    }
    else
    {
        if (Check_Alm_Vol() == ERROR)
        {
            Save_Alm_Vol();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_All_Ch
//函数功能:获取当前的主界面全中文设置
//入口参数:无
//出口参数:主界面全中文设置
//**********************************************************************
uint8 Get_All_Ch(void)
{
    return(main_inter_ch);    
}

//**********************************************************************
//函数  名:Change_All_Ch_Set
//函数功能:修改主界面全中文设置
//入口参数:无
//出口参数:无
//**********************************************************************
uint8 Change_All_Ch_Set(void)
{
    main_inter_ch = (main_inter_ch)?FALSE:TRUE;
    return (main_inter_ch);
}


//**********************************************************************
//函数  名:Save_All_Ch_Set
//函数功能:保存主界面全中文设置,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_All_Ch_Set(void)
{
    if (Write_EEPROM(EEP_ALL_CH,( uint8*)(&main_inter_ch),sizeof(main_inter_ch)) )
    {
        main_inter_ch = TRUE;
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_All_Ch_Set_EEP
//函数功能:从EEPROM中读出用报警音量,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_All_Ch_Set_EEP(void)
{
    if (Read_EEPROM(EEP_ALL_CH,( uint8*)(&main_inter_ch),sizeof(main_inter_ch)) )
    {
        main_inter_ch = TRUE;
        return(ERROR);
    }
    else
    {
        if ((main_inter_ch != TRUE)&&(main_inter_ch != FALSE))
        {
            main_inter_ch = TRUE;
            Save_All_Ch_Set();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Br_Mode_Sel
//函数功能:获取当前选择的呼吸模式
//入口参数:无
//出口参数:呼吸模式
//**********************************************************************
uint8 Get_Br_Mode_Sel(void)
{
    return(breath_mode_sel);
}

//**********************************************************************
//函数  名:Set_Br_Mode_Sel
//函数功能:设置呼吸模式
//入口参数:para--呼吸模式
//出口参数:无
//**********************************************************************
void Set_Br_Mode_Sel(uint8 para)
{
    breath_mode_sel = para;
}

//**********************************************************************
//函数  名:Save_Br_Mode_Sel
//函数功能:保存呼吸模式的设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Br_Mode_Sel(void)
{
    if (Write_EEPROM(EEP_BR_M_SEL,( uint8*)(&breath_mode_sel),sizeof(breath_mode_sel)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Br_M_Sel_EEP
//函数功能:从EEPROM中读出呼吸模式设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Br_M_Sel_EEP(void)
{
    if (Read_EEPROM(EEP_BR_M_SEL,( uint8*)(&breath_mode_sel),sizeof(breath_mode_sel)) )
    {
        breath_mode_sel = BRM_DEF;
        return(ERROR);
    }
    else
    {
        if ((breath_mode_sel != BRM_VCV)&&(breath_mode_sel != BRM_SIPPV)&&(breath_mode_sel != BRM_SIMV))
        {
            breath_mode_sel = BRM_DEF;
            Save_Br_Mode_Sel();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_O2_Alm_L
//函数功能:获取当前的氧气浓度报警设置值的下限值
//入口参数:无
//出口参数:设置值
//**********************************************************************
uint8 Get_O2_Alm_L(void)
{
    if (o2_alm_l_disp > O2_ALM_L_MAX)    o2_alm_l_disp = O2_ALM_L_MAX;
    if (o2_alm_l_disp < O2_ALM_L_MIN)    o2_alm_l_disp = O2_ALM_L_MIN;
    return(o2_alm_l_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_O2_Alm_L
//函数功能:增加/减少氧气浓度报警下限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_O2_Alm_L(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        o2_alm_l_disp += step;
        if ((o2_alm_l_disp<=O2_ALM_L_MAX)&&(o2_alm_l_disp < o2_alm_h))
        {    
            return(OK);
        }
        else
        {
            o2_alm_l_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        o2_alm_l_disp -= step;
        if ((o2_alm_l_disp>=O2_ALM_L_MIN)&&(o2_alm_l_disp < o2_alm_h))
        {
            return(OK);
        }
        else
        {
            o2_alm_l_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_O2_Alm_L
//函数功能:检查氧气浓度报警下限设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_O2_Alm_L(void)
{
    if (o2_alm_l > O2_ALM_L_MAX)//太大
    {
        o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);
    }
    else if (o2_alm_l < O2_ALM_L_MIN)//太小
    {
        o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_O2_Alm_L
//函数功能:保存用户输入的氧气浓度报警下限,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_O2_Alm_L(void)
{
    if (Write_EEPROM(EEP_O2_ALM_L,( uint8*)(&o2_alm_l_disp),sizeof(o2_alm_l_disp)) )
    {
        o2_alm_l = o2_alm_l_disp =O2_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_l = o2_alm_l_disp ;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_O2_Alm_L_EEP
//函数功能:从EEPROM中读出氧气浓度报警下限设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_O2_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_O2_ALM_L,( uint8*)(&o2_alm_l),sizeof(o2_alm_l)) )
    {
        o2_alm_l_disp = o2_alm_l = O2_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_l_disp = o2_alm_l;    
        if (Check_O2_Alm_L() == ERROR)
        {
            Save_O2_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_O2_Alm_H
//函数功能:获取当前的氧气浓度报警设置值的上限值
//入口参数:无
//出口参数:设置值
//**********************************************************************
uint8 Get_O2_Alm_H(void)
{
    if (o2_alm_h_disp > O2_ALM_H_MAX)    o2_alm_h_disp = O2_ALM_H_MAX;
    if (o2_alm_h_disp < O2_ALM_H_MIN)    o2_alm_h_disp = O2_ALM_H_MIN;
    return(o2_alm_h_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_O2_Alm_H
//函数功能:增加/减少氧气浓度报警上限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_O2_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        o2_alm_h_disp += step;
        if ((o2_alm_h_disp<=O2_ALM_H_MAX)&&(o2_alm_h_disp > o2_alm_l))
        {
            return(OK);
        }
        else
        {
            o2_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        o2_alm_h_disp -= step;
        if ((o2_alm_h_disp>=O2_ALM_H_MIN)&&(o2_alm_h_disp > o2_alm_l))
        {
            return(OK);
        }
        else
        {
            o2_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_O2_Alm_H
//函数功能:检查氧气浓度报警上限值设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_O2_Alm_H(void)
{
    if (o2_alm_h > O2_ALM_H_MAX)//太大
    {
        o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);
    }
    else if (o2_alm_h < O2_ALM_H_MIN)//太小
    {
        o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_O2_Alm_H
//函数功能:保存用户输入的氧气浓度报警上限值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_O2_Alm_H(void)
{
    if (Write_EEPROM(EEP_O2_ALM_H,( uint8*)(&o2_alm_h_disp),sizeof(o2_alm_h_disp)) )
    {
        o2_alm_h = o2_alm_h_disp =O2_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_h = o2_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_O2_Alm_H_EEP
//函数功能:从EEPROM中读出氧气浓度报警上限值设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_O2_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_O2_ALM_H,( uint8*)(&o2_alm_h),sizeof(o2_alm_h)) )
    {
        o2_alm_h_disp = o2_alm_h = O2_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        o2_alm_h_disp = o2_alm_h;    
        if (Check_O2_Alm_H() == ERROR)
        {
            Save_O2_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_VE_Alm_L
//函数功能:获取当前的分钟通气报警下限值
//入口参数:无
//出口参数:设置值(单位:0.1L/min)
//**********************************************************************
sint32 Get_VE_Alm_L(void)
{
    if (ve_alm_l_disp > VE_ALM_L_MAX)    ve_alm_l_disp = VE_ALM_L_MAX;
    if (ve_alm_l_disp < VE_ALM_L_MIN)    ve_alm_l_disp = VE_ALM_L_MIN;
    return(ve_alm_l_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_Ve_Alm_L
//函数功能:增加/减少分钟通气报警下限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Ve_Alm_L(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((ve_alm_l_disp+step)<=VE_ALM_L_MAX)
        {
            ve_alm_l_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((ve_alm_l_disp-step)>=VE_ALM_L_MIN)
        {
            ve_alm_l_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Ve_Alm_L
//函数功能:检查分钟通气报警下限值设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Ve_Alm_L(void)
{
    if (ve_alm_l > VE_ALM_L_MAX)//太大
    {
        ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);
    }
    else if (ve_alm_l < VE_ALM_L_MIN)//太小
    {
        ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Ve_Alm_L
//函数功能:保存用户输入的分钟通气报警下限值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Ve_Alm_L(void)
{
    if (Write_EEPROM(EEP_VE_ALM_L,( uint8*)(&ve_alm_l_disp),sizeof(ve_alm_l_disp)) )
    {
        ve_alm_l = ve_alm_l_disp = VE_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_l = ve_alm_l_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Ve_Alm_L_EEP
//函数功能:从EEPROM中读出分钟通气报警下限设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Ve_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_VE_ALM_L,( uint8*)(&ve_alm_l),sizeof(ve_alm_l)) )
    {
        ve_alm_l_disp = ve_alm_l = VE_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_l_disp = ve_alm_l;    
        if (Check_Ve_Alm_L() == ERROR)
        {
            Save_Ve_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_VE_Alm_H
//函数功能:获取当前的分钟通气报警设置上限值
//入口参数:无
//出口参数:设置值(单位:0.1L/min)
//**********************************************************************
sint32 Get_VE_Alm_H(void)
{
    if (ve_alm_h_disp > VE_ALM_H_MAX)    ve_alm_h_disp = VE_ALM_H_MAX;
    if (ve_alm_h_disp < VE_ALM_H_MIN)    ve_alm_h_disp = VE_ALM_H_MIN;
    return(ve_alm_h_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_Ve_Alm_H
//函数功能:增加/减少分钟通气报警上限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Ve_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        ve_alm_h_disp += step;
        if ((ve_alm_h_disp<=VE_ALM_H_MAX)&&(ve_alm_h_disp>ve_alm_l))
        {
            return(OK);
        }
        else
        {
            ve_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        ve_alm_h_disp -= step;
        if ((ve_alm_h_disp>=VE_ALM_H_MIN)&&(ve_alm_h_disp>ve_alm_l))
        {
            return(OK);
        }
        else
        {
            ve_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Ve_Alm_H
//函数功能:检查分钟通气报警上限值设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Ve_Alm_H(void)
{
    if (ve_alm_h > VE_ALM_H_MAX)//太大
    {
        ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < VE_ALM_H_MIN)//太小
    {
        ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Ve_Alm_H
//函数功能:保存用户输入的分钟通气报警上限值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Ve_Alm_H(void)
{
    if (Write_EEPROM(EEP_VE_ALM_H,( uint8*)(&ve_alm_h_disp),sizeof(ve_alm_h_disp)) )
    {
        ve_alm_h = ve_alm_h_disp = VE_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_h = ve_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Ve_Alm_H_EEP
//函数功能:从EEPROM中读出分钟通气报警上限设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Ve_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_VE_ALM_H,( uint8*)(&ve_alm_h),sizeof(ve_alm_h)) )
    {
        ve_alm_h_disp = ve_alm_h = VE_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        ve_alm_h_disp = ve_alm_h;    
        if (Check_Ve_Alm_H() == ERROR)
        {
            Save_Ve_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Vt_Alm_L
//函数功能:获取当前的潮气量报警下限值
//入口参数:无
//出口参数:设置值(1ml)
//**********************************************************************
sint16 Get_Vt_Alm_L(void)
{
    if (vt_alm_l_disp > VT_ALM_L_MAX)    vt_alm_l_disp = VT_ALM_L_MAX;
    if (vt_alm_l_disp < VT_ALM_L_MIN)    vt_alm_l_disp = VT_ALM_L_MIN;
    return(vt_alm_l_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_Vt_Alm_L
//函数功能:增加/减少潮气量报警下限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Vt_Alm_L(uint8 dir,uint8 step)
{
    if (vt_alm_l_disp>20) step = 20;
    if (dir == INC)
    {
        vt_alm_l_disp += step;
        if ((vt_alm_l_disp<=VT_ALM_L_MAX)&&(vt_alm_l_disp < vt_alm_h))
        {
            
            return(OK);
        }
        else
        {
            vt_alm_l_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        vt_alm_l_disp -= step;
        if ((vt_alm_l_disp>=VT_ALM_L_MIN)&&(vt_alm_l_disp < vt_alm_h))
        {
            return(OK);
        }
        else
        {
            vt_alm_l_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Vt_Alm_L
//函数功能:检查潮气量报警下限值设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Vt_Alm_L(void)
{
    if (vt_alm_l > VT_ALM_L_MAX)//太大
    {
        vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);
    }
    else if (ve_alm_l < VE_ALM_L_MIN)//太小
    {
        vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Vt_Alm_L
//函数功能:保存用户输入的潮气量报警下限值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Vt_Alm_L(void)
{
    if (Write_EEPROM(EEP_VT_ALM_L,( uint8*)(&vt_alm_l_disp),sizeof(vt_alm_l_disp)) )
    {
        vt_alm_l = vt_alm_l_disp = VT_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_l = vt_alm_l_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Vt_Alm_L_EEP
//函数功能:从EEPROM中读出潮气量报警下限设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Vt_Alm_L_EEP(void)
{
    if (Read_EEPROM(EEP_VT_ALM_L,( uint8*)(&vt_alm_l),sizeof(vt_alm_l)) )
    {
        vt_alm_l_disp = vt_alm_l = VT_ALM_L_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_l_disp = vt_alm_l;    
        if (Check_Vt_Alm_L() == ERROR)
        {
            Save_Vt_Alm_L();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Vt_Alm_H
//函数功能:获取当前的潮气量报警上限值
//入口参数:无
//出口参数:设置值(1ml)
//**********************************************************************
sint16 Get_Vt_Alm_H(void)
{
    if (vt_alm_h_disp > VT_ALM_H_MAX)    vt_alm_h_disp = VT_ALM_H_MAX;
    if (vt_alm_h_disp < VT_ALM_H_MIN)    vt_alm_h_disp = VT_ALM_H_MIN;
    return(vt_alm_h_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_Vt_Alm_H
//函数功能:增加/减少潮气量报警上限值
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Vt_Alm_H(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        vt_alm_h_disp += step;
        if ((vt_alm_h_disp<=VT_ALM_H_MAX)&&(vt_alm_h_disp > vt_alm_l))
        {    
            return(OK);
        }
        else
        {
            vt_alm_h_disp -= step;
            return(ERROR);
        }
    }
    else
    {
        vt_alm_h_disp -= step;
        if ((vt_alm_h_disp>=VT_ALM_H_MIN)&&(vt_alm_h_disp > vt_alm_l))
        {
            return(OK);
        }
        else
        {
            vt_alm_h_disp += step;
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Vt_Alm_H
//函数功能:检查潮气量报警上限值设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Vt_Alm_H(void)
{
    if (vt_alm_h > VT_ALM_H_MAX)//太大
    {
        vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < VE_ALM_H_MIN)//太小
    {
        vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);            
    }    
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Vt_Alm_H
//函数功能:保存用户输入的潮气量报警上限值,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Vt_Alm_H(void)
{
    if (Write_EEPROM(EEP_VT_ALM_H,(uint8*)(&vt_alm_h_disp),sizeof(vt_alm_h_disp)) )
    {
        vt_alm_h = vt_alm_h_disp = VT_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_h = vt_alm_h_disp;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Vt_Alm_H_EEP
//函数功能:从EEPROM中读出潮气量报警上限设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Vt_Alm_H_EEP(void)
{
    if (Read_EEPROM(EEP_VT_ALM_H,( uint8*)(&vt_alm_h),sizeof(vt_alm_h)) )
    {
        vt_alm_h_disp = vt_alm_h = VT_ALM_H_DEF;
        return(ERROR);
    }
    else
    {
        vt_alm_h_disp = vt_alm_h;    
        if (Check_Vt_Alm_H() == ERROR)
        {
            Save_Vt_Alm_H();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Vt_Alm_Switch
//函数功能:读取潮气量报警开关设置
//入口参数:无
//出口参数:TRUE--关闭潮气量报警
//            FASLE--打开潮气量报警
//**********************************************************************
uint8 Get_Vt_Alm_Switch(void)
{
    return(vt_alarm_off);
}

//**********************************************************************
//函数  名:Change_Vt_Alm_Switch
//函数功能:设置潮气量报警开关设置
//入口参数:无
//出口参数:无
//**********************************************************************
void Change_Vt_Alm_Switch(void)
{
    vt_alarm_off = (vt_alarm_off)?FALSE:TRUE;
}

//**********************************************************************
//函数  名:Get_Alt
//函数功能:获取当前的海拔高度设置值
//入口参数:无
//出口参数:设置值(1ml)
//**********************************************************************
sint16 Get_Alt(void)
{
    if (altitude_disp > ALT_MAX)    altitude_disp = ALT_MAX;
    if (altitude_disp < ALT_MIN)    altitude_disp = ALT_MIN;
    return(altitude_disp);
}

//**********************************************************************
//函数  名:Inc_Dec_Alt
//函数功能:增加/减少海拔高度
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Alt(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((altitude_disp+step)<=ALT_MAX)
        {
            altitude_disp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((altitude_disp-step)>=ALT_MIN)
        {
            altitude_disp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Alt
//函数功能:检查海拔高度设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Alt(void)
{
    if (abs(altitude)%ALT_STEP)
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }    
    else if (altitude > ALT_MAX)//太大
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }
    else if (ve_alm_h < ALT_MIN)//太小
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Alt
//函数功能:保存用户输入的海拔高度,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Alt(void)
{
    if (Write_EEPROM(EEP_ALT,( uint8*)(&altitude_disp),sizeof(altitude_disp)) )
    {
        altitude = altitude_disp = ALT_DEF;
        return(ERROR);
    }
    else
    {
        altitude = altitude_disp ;
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Alt_EEP
//函数功能:从EEPROM中读出海拔高度,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Alt_EEP(void)
{
    if (Read_EEPROM(EEP_ALT,( uint8*)(&altitude),sizeof(altitude)) )
    {
        altitude_disp = altitude = ALT_DEF;
        return(ERROR);
    }
    else
    {
        altitude_disp = altitude;    
        if (Check_Alt() == ERROR)
        {
            Save_Alt();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Drv_Gas
//函数功能:获取当前选择驱动气体
//入口参数:无
//出口参数:驱动气体
//**********************************************************************
uint8 Get_Drv_Gas(void)
{
    return(drv_gas);
}

//**********************************************************************
//函数  名:Set_Drv_Gas
//函数功能:设置驱动气体
//入口参数:para--驱动气体
//出口参数:无
//**********************************************************************
void Set_Drv_Gas(uint8 para)
{
    drv_gas = para;
}

//**********************************************************************
//函数  名:Save_Drv_Gas
//函数功能:保存驱动气体的设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Drv_Gas(void)
{
    if (Write_EEPROM(EEP_DRV_GAS,( uint8*)(&drv_gas),sizeof(drv_gas)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Drv_Gas_EEP
//函数功能:从EEPROM中读出驱动气体设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Drv_Gas_EEP(void)
{
    if (Read_EEPROM(EEP_DRV_GAS,( uint8*)(&drv_gas),sizeof(drv_gas)) )
    {
        drv_gas = DRV_GAS_DEF;
        return(ERROR);
    }
    else
    {
        if ((drv_gas != DRV_O2)&&(drv_gas != DRV_AIR))
        {
            drv_gas = DRV_GAS_DEF;
            Save_Drv_Gas();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_O2_Inspect
//函数功能:获取当前选择氧浓度监测
//入口参数:无
//出口参数:氧浓度监测
//**********************************************************************
uint8 Get_O2_Inspect(void)
{
    return(o2_inspect);
}

//**********************************************************************
//函数  名:Set_O2_Inspect
//函数功能:设置氧浓度监测
//入口参数:para--氧浓度监测
//出口参数:无
//**********************************************************************
void Set_O2_Inspect(uint8 para)
{
    o2_inspect = para;
}

//**********************************************************************
//函数  名:Save_O2_Inspect
//函数功能:保存氧浓度监测的设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_O2_Inspect(void)
{
    if (Write_EEPROM(EEP_O2_INSPECT,( uint8*)(&o2_inspect),sizeof(o2_inspect)) )
    {
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_O2_Inspect_EEP
//函数功能:从EEPROM中读出氧浓度监测设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_O2_Inspect_EEP(void)
{
    if (Read_EEPROM(EEP_O2_INSPECT,( uint8*)(&o2_inspect),sizeof(o2_inspect)) )
    {
        o2_inspect = O2_INSPECT_DEF;
        return(ERROR);
    }
    else
    {
        if ((o2_inspect != ON)&&(o2_inspect != OFF))
        {
            o2_inspect = O2_INSPECT_DEF;
            Save_O2_Inspect();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Alm_Mess_D
//函数功能:获取当前报警信息显示设置
//入口参数:无
//出口参数:报警信息显示设置
//**********************************************************************
uint8 Get_Alm_Mess_D(void)
{
    return(alm_mess_disp);
}

//**********************************************************************
//函数  名:Set_O2_Inspect
//函数功能:设置报警信息显示设置
//入口参数:para--报警信息显示设置
//出口参数:无
//**********************************************************************
void Set_Alm_Mess_D(uint8 para)
{
    alm_mess_disp = para;
}

//**********************************************************************
//函数  名:Save_Alm_Mess_D
//函数功能:保存报警信息显示设置的设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Alm_Mess_D(void)
{
    if (Write_EEPROM(EEP_ALM_MS_DISP,( uint8*)(&alm_mess_disp),sizeof(alm_mess_disp)) )
    {
        return(ERROR); 
    } 
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Alm_Mess_D_EEP
//函数功能:从EEPROM中读出报警信息显示设置设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Alm_Mess_D_EEP(void)
{
    if (Read_EEPROM(EEP_ALM_MS_DISP,( uint8*)(&alm_mess_disp),sizeof(alm_mess_disp)) )
    {
        alm_mess_disp = ALM_MS_DISP_DEF;
        return(ERROR);
    }
    else
    {
        if ((alm_mess_disp != ON)&&(alm_mess_disp != OFF))
        {
            alm_mess_disp = ALM_MS_DISP_DEF;
            Save_Alm_Mess_D();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Pmax
//函数功能:获取最大压力测量值
//入口参数:无
//出口参数:最大压力测量值
//**********************************************************************
sint32 Get_Pmax(void)
{    
    if (paw_max > PAW_MAX*PAW_MAX_MUL) return (PAW_NOT_VALID);    //压力太大,返回无效值
    else if (paw_max < PAW_MIN*PAW_MAX_MUL) return (PAW_NOT_VALID);    //压力太小,返回无效值
    else return (paw_max);
}

//**********************************************************************
//函数  名:Get_Pmax_Units
//函数功能:获取最大压力测量值的单位
//入口参数:无
//出口参数:压力测量值(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pmax_Units(void)
{
    return (PAW_MAX_MUL);
}

//**********************************************************************
//函数  名:Set_Pmax
//函数功能:设置最大压力的测量值
//入口参数:p--压力值
//            units--单位
//出口参数:OK--压力范围正常
//           ERROR--压力范围超限
//**********************************************************************
uint8 Set_Pmax(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//太大
    {
        paw_max = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//太小
    {
        paw_max = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_max = p;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Pavr
//函数功能:获取平均压力测量值
//入口参数:无
//出口参数:平均压力测量值
//**********************************************************************
sint32 Get_Pavr(void)
{
    if (paw_avr_meas > PAW_MAX*PAW_AVR_MEAS_MUL) return (PAW_NOT_VALID);    //压力太大,返回无效值
    else if (paw_avr_meas < PAW_MIN*PAW_AVR_MEAS_MUL) return (PAW_NOT_VALID);    //压力太小,返回无效值
    else return (paw_avr_meas);
}

//**********************************************************************
//函数  名:Get_Pavr_Units
//函数功能:获取平均压力测量值的单位
//入口参数:无
//出口参数:压力测量值(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pavr_Units(void)
{
    return (PAW_AVR_MEAS_MUL);
}

//**********************************************************************
//函数  名:Set_Pavr
//函数功能:设置平均压力的测量值
//入口参数:p--压力值
//            units--单位
//出口参数:OK--压力范围正常
//           ERROR--压力范围超限
//**********************************************************************
uint8 Set_Pavr(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//太大
    {
        paw_avr_meas = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//太小
    {
        paw_avr_meas = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_avr_meas = p;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Paw
//函数功能:获取压力测量值
//入口参数:无
//出口参数:压力测量值
//**********************************************************************
sint32 Get_Paw(void)
{
    if (airway_p > PAW_MAX*PAW_MUL) return (PAW_NOT_VALID);    //压力太大,返回无效值
    else if (airway_p < PAW_MIN*PAW_MUL) return (PAW_NOT_VALID);    //压力太小,返回无效值
    else return (airway_p);
}

//**********************************************************************
//函数  名:Get_Paw_Units
//函数功能:获取压力测量值的单位
//入口参数:无
//出口参数:压力测量值(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Paw_Units(void)
{
    return (PAW_MUL);
}

//**********************************************************************
//函数  名:Set_Pmax
//函数功能:设置压力的测量值
//入口参数:p--压力值
//            units--单位
//出口参数:OK--压力范围正常
//           ERROR--压力范围超限
//**********************************************************************
uint8 Set_Paw(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//太大
    {
        airway_p = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//太小
    {
        airway_p = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        airway_p = p;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Pmin
//函数功能:获取最小压力测量值
//入口参数:无
//出口参数:最小压力测量值
//**********************************************************************
sint32 Get_Pmin(void)
{
    if (paw_min > PAW_MAX*PAW_MIN_MUL) return (PAW_NOT_VALID);    //最小压力太大,返回无效值
    else if (paw_min < PAW_MIN*PAW_MIN_MUL) return (PAW_NOT_VALID);    //最小压力太小,返回无效值
    else return (paw_min);
}

//**********************************************************************
//函数  名:Get_Pmin_Units
//函数功能:获取最小压力测量值的单位
//入口参数:无
//出口参数:最小压力测量值(1--1cmH2O;10--0.1cmH2O)
//**********************************************************************
uint8 Get_Pmin_Units(void)
{
    return (PAW_MIN_MUL);
}

//**********************************************************************
//函数  名:Set_Pmin
//函数功能:设置最小压力的测量值
//入口参数:p--最小压力值
//            units--单位
//出口参数:OK--最小压力范围正常
//           ERROR--最小压力范围超限
//**********************************************************************
uint8 Set_Pmin(sint32 p,uint8 units)
{
    if (p > PAW_MAX*units)//太大
    {
        paw_min = PAW_NOT_VALID;
        return (ERROR);
    }
    else if (p < PAW_MIN*units)//太小
    {
        paw_min = PAW_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        paw_min = p;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Vt
//函数功能:获取潮气量测量值
//入口参数:无
//出口参数:潮气量测量值
//**********************************************************************
sint32 Get_Vt(void)
{
    if (vt_meas > VT_MEAS_MAX*vt_meas_units) return (VT_MEAS_NOT_VALID);    //潮气量太大,返回无效值
    else if (vt_meas < VT_MEAS_MIN*vt_meas_units) return (VT_MEAS_NOT_VALID);    //潮气量太小,返回无效值
    else return (vt_meas);
}

//**********************************************************************
//函数  名:Get_Vt_Units
//函数功能:获取潮气量测量值的单位
//入口参数:无
//出口参数:潮气量测量值(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Vt_Units(void)
{
    return (vt_meas_units);
}

//**********************************************************************
//函数  名:Set_Vt
//函数功能:设置潮气量的测量值
//入口参数:p--潮气量值
//            units--单位
//出口参数:OK--潮气量范围正常
//           ERROR--潮气量范围超限
//**********************************************************************
uint8 Set_Vt(sint32 p,uint8 units)
{
    if (p > VT_MEAS_MAX*units)//太大
    {
        vt_meas = VT_MEAS_NOT_VALID;
        vt_meas_units = 1;
        return (ERROR);
    }
    else if (p < VT_MEAS_MIN*units)//太小
    {
        vt_meas = VT_MEAS_NOT_VALID;
        vt_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        vt_meas = p;
        vt_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Rate_Meas
//函数功能:获取呼吸频率测量值
//入口参数:无
//出口参数:呼吸频率测量值
//**********************************************************************
sint32 Get_Rate_Meas(void)
{
    if (br_rate_meas > RATE_MEAS_MAX) return (RATE_NOT_VALID);    //呼吸频率太大,返回无效值
    else if (br_rate_meas < RATE_MEAS_MIN) return (RATE_NOT_VALID);    //呼吸频率太小,返回无效值
    else return (br_rate_meas);
}


//**********************************************************************
//函数  名:Set_Rate_Meas
//函数功能:设置呼吸频率的测量值
//入口参数:p--呼吸频率值
//出口参数:OK--呼吸频率范围正常
//           ERROR--呼吸频率范围超限
//**********************************************************************
uint8 Set_Rate_Meas(sint32 p)
{
    if (p > RATE_MEAS_MAX)//太大
    {
        br_rate_meas = RATE_NOT_VALID;
        return (ERROR);
    }
    else if (p < RATE_MEAS_MIN)//太小
    {
        br_rate_meas = RATE_NOT_VALID;
        return (ERROR);        
    }
    else
    {
        br_rate_meas = p;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Ve
//函数功能:获取分钟通气测量值
//入口参数:无
//出口参数:分钟通气测量值
//**********************************************************************
sint32 Get_Ve(void)
{
    if (insp_flux_meas > VE_MEAS_MAX*insp_flux_meas_unit) return (VE_MEAS_NOT_VALID);    //分钟通气太大,返回无效值
    else if (insp_flux_meas < VE_MEAS_MIN*insp_flux_meas_unit) return (VE_MEAS_NOT_VALID);    //分钟通气太小,返回无效值
    else return (insp_flux_meas);
}

//**********************************************************************
//函数  名:Get_Ve_Units
//函数功能:获取分钟通气测量值的单位
//入口参数:无
//出口参数:分钟通气测量值(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Ve_Units(void)
{
    return (insp_flux_meas_unit);
}

//**********************************************************************
//函数  名:Set_Ve
//函数功能:设置分钟通气的测量值
//入口参数:p--分钟通气值
//            units--单位
//出口参数:OK--分钟通气范围正常
//           ERROR--分钟通气范围超限
//**********************************************************************
uint8 Set_Ve(sint32 p,uint8 units)
{
    if (p > VE_MEAS_MAX*units)//太大
    {
        insp_flux_meas = VE_MEAS_NOT_VALID;
        insp_flux_meas_unit = 1;
        return (ERROR);
    }
    else if (p < VE_MEAS_MIN*units)//太小
    {
        insp_flux_meas = VE_MEAS_NOT_VALID;
        insp_flux_meas_unit = 1;
        return (ERROR);        
    }
    else
    {
        insp_flux_meas = p;
        insp_flux_meas_unit = units;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_O2
//函数功能:获取氧气浓度测量值
//入口参数:无
//出口参数:氧气浓度测量值
//**********************************************************************
sint32 Get_O2(void)
{
    if (o2_meas > O2_MEAS_MAX*o2_meas_units) return (O2_MEAS_NOT_VALID);    //氧气浓度太大,返回无效值
    else if (o2_meas < O2_MEAS_MIN*o2_meas_units) return (O2_MEAS_NOT_VALID);    //氧气浓度太小,返回无效值
    else return (o2_meas);
}

//**********************************************************************
//函数  名:Get_O2_Units
//函数功能:获取氧气浓度测量值的单位
//入口参数:无
//出口参数:氧气浓度测量值(1--1%;10--0.1%)
//**********************************************************************
uint8 Get_O2_Units(void)
{
    return (o2_meas_units);
}

//**********************************************************************
//函数  名:Set_O2
//函数功能:设置氧气浓度的测量值
//入口参数:p--氧气浓度值
//            units--单位
//出口参数:OK--氧气浓度范围正常
//           ERROR--氧气浓度范围超限
//**********************************************************************
uint8 Set_O2(sint32 p,uint8 units)
{
    if (p > O2_MEAS_MAX*units)//太大
    {
        o2_meas = O2_MEAS_NOT_VALID;
        o2_meas_units = 1;
        return (ERROR);
    }
    else if (p < O2_MEAS_MIN*units)//太小
    {
        o2_meas = O2_MEAS_NOT_VALID;
        o2_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        o2_meas = p;
        o2_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Humor
//函数功能:获取顺应性测量值
//入口参数:无
//出口参数:顺应性测量值
//**********************************************************************
sint32 Get_Humor(void)
{
    if (humor_meas > HUMOR_MAX*humor_meas_units) return (HUMOR_NOT_VALID);    //顺应性太大,返回无效值
    else if (humor_meas < HUMOR_MIN*humor_meas_units) return (HUMOR_NOT_VALID);    //顺应性太小,返回无效值
    else return (humor_meas);
}

//**********************************************************************
//函数  名:Get_Humor_Units
//函数功能:获取顺应性测量值的单位
//入口参数:无
//出口参数:顺应性测量值(1--1mL;10--0.1mL)
//**********************************************************************
uint8 Get_Humor_Units(void)
{
    return (humor_meas_units);
}

//**********************************************************************
//函数  名:Set_Humor
//函数功能:设置顺应性的测量值
//入口参数:p--顺应性值
//            units--单位
//出口参数:OK--顺应性范围正常
//           ERROR--顺应性范围超限
//**********************************************************************
uint8 Set_Humor(sint32 p,uint8 units)
{
    if (p > HUMOR_MAX*units)//太大
    {
        humor_meas = HUMOR_NOT_VALID;
        humor_meas_units = 1;
        return (ERROR);
    }
    else if (p < HUMOR_MIN*units)//太小
    {
        humor_meas = HUMOR_NOT_VALID;
        humor_meas_units = 1;
        return (ERROR);        
    }
    else
    {
        humor_meas = p;
        humor_meas_units = units;
        return (OK);            
    }
}

//**********************************************************************
//函数  名:Get_Batt_Vol
//函数功能:获取蓄电池的电压
//入口参数:无
//出口参数:电压(单位:0.01V)
//**********************************************************************
sint32 Get_Batt_Vol(void)
{
    return(batt_v);
}

//**********************************************************************
//函数  名:Log_Batt_Vol
//函数功能:记录蓄电池的电压
//入口参数:电压(单位:0.01V)
//出口参数:无
//***********************************************************************
void Log_Batt_Vol(sint32 vol)
{
    batt_v = vol;
}

//**********************************************************************
//函数  名:Get_Batt_Charge_I
//函数功能:获取蓄电池充电电流
//入口参数:无
//出口参数:电流(单位:ma)
//**********************************************************************
sint32 Get_Batt_Charge_I(void)
{
    return(batt_charge_i);
}

//**********************************************************************
//函数  名:Log_Batt_Charge_I
//函数功能:记录蓄电池的电压
//入口参数:电流(单位:ma)
//出口参数:无
//***********************************************************************
void Log_Batt_Charge_I(sint32 i)
{
    batt_v = i;
}

//**********************************************************************
//函数  名:Get_Key_Sound_Switch
//函数功能:当前的按键音是否打开
//入口参数:无
//出口参数:ON--打开;OFF--关闭
//**********************************************************************
uint8 Get_Key_Sound_Switch(void)
{
    return(key_sound_switch);
}

//**********************************************************************
//函数  名:Set_Key_Sound_Switch
//函数功能:设置按键音的开/关
//入口参数:para--按键音开/关
//出口参数:无
//**********************************************************************
void Set_Key_Sound_Switch(uint8 para)
{
    key_sound_switch = para;
}

//**********************************************************************
//函数  名:Save_Key_Sound_Switch
//函数功能:保存按键音开/关的设置
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Key_Sound_Switch(void)
{
    if (Write_EEPROM(EEP_KEY_SOUND_SW,( uint8*)(&key_sound_switch),sizeof(key_sound_switch)) )
    {
        key_sound_switch = ON;
        return(ERROR); 
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Key_Sound_Switch_EEP
//函数功能:从EEPROM中读出按键音开关设置值,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Key_Sound_Switch_EEP(void)
{
    if (Read_EEPROM(EEP_KEY_SOUND_SW,( uint8*)(&key_sound_switch),sizeof(key_sound_switch)) )
    {
        key_sound_switch = ON;
        return(ERROR);
    }
    else
    {
        if ((ON != key_sound_switch)&&(OFF != key_sound_switch))
        {//设置值不一致则重新设置
            key_sound_switch = ON;
            Save_Key_Sound_Switch();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Vt_Filter_Const
//函数功能:获取潮气量滤波常数设置值
//入口参数:无
//出口参数:设置值
//**********************************************************************
uint8 Get_Vt_Filter_Const(void)
{
    if (vte_filter_const > VTE_FC_MAX)    vte_filter_const = VTE_FC_DEFAULT;
    if (vte_filter_const < VTE_FC_MIN)    vte_filter_const = VTE_FC_DEFAULT;
    return(vte_filter_const);
}

//**********************************************************************
//函数  名:Inc_Dec_Vt_Filt_C
//函数功能:增加/减少潮气量滤波常数
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Vt_Filt_C(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((vte_filter_const+step)<=VTE_FC_MAX)
        {
            vte_filter_const += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((vte_filter_const-step)>=VTE_FC_MIN)
        {
            vte_filter_const -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Vte_Filt_C
//函数功能:检查潮气量滤波常数设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Vte_Filt_C(void)
{
    if (vte_filter_const > VTE_FC_MAX)//太大
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else if (vte_filter_const < VTE_FC_MIN)//太小
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Vt_Filt_C
//函数功能:保存潮气量滤波常数,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Vt_Filt_C(void)
{
    if (Write_EEPROM(EEP_VT_FILT_CONST,( uint8*)(&vte_filter_const),sizeof(vte_filter_const)) )
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Vt_Filt_C_EEP
//函数功能:从EEPROM中读出潮气量滤波常数,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Vt_Filt_C_EEP(void)
{
    if (Read_EEPROM(EEP_VT_FILT_CONST,( uint8*)(&vte_filter_const),sizeof(vte_filter_const)) )
    {
        vte_filter_const = VTE_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Vte_Filt_C() == ERROR)
        {
            Save_Vt_Filt_C();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_O2_Flash_Cyc
//函数功能:获取氧浓度刷新周期设置值
//入口参数:无
//出口参数:设置值
//**********************************************************************
uint8 Get_O2_Flash_Cyc(void)
{
    if (o2_flash_cycle > O2_FC_MAX)    o2_flash_cycle = O2_FC_DEFAULT;
    if (o2_flash_cycle < O2_FC_MIN)    o2_flash_cycle = O2_FC_DEFAULT;
    return(o2_flash_cycle);
}

//**********************************************************************
//函数  名:Inc_Dec_O2_Flash_Cyc
//函数功能:增加/减少氧浓度刷新周期
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_O2_Flash_Cyc(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((o2_flash_cycle+step)<=O2_FC_MAX)
        {
            o2_flash_cycle += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((o2_flash_cycle-step)>=O2_FC_MIN)
        {
            o2_flash_cycle -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_O2_Flash_Cyc
//函数功能:检查氧浓度刷新周期设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_O2_Flash_Cyc(void)
{
    if (o2_flash_cycle > O2_FC_MAX)//太大
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else if (o2_flash_cycle < O2_FC_MIN)//太小
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_O2_Flash_Cyc
//函数功能:保存氧浓度刷新周期,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_O2_Flash_Cyc(void)
{
    if (Write_EEPROM(EEP_O2_FLASH_CYC,( uint8*)(&o2_flash_cycle),sizeof(o2_flash_cycle)) )
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_O2_Flash_Cyc_EEP
//函数功能:从EEPROM中读出氧浓度刷新周期,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_O2_Flash_Cyc_EEP(void)
{
    if (Read_EEPROM(EEP_O2_FLASH_CYC,( uint8*)(&o2_flash_cycle),sizeof(o2_flash_cycle)) )
    {
        o2_flash_cycle = O2_FC_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_O2_Flash_Cyc() == ERROR)
        {
            Save_O2_Flash_Cyc();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Pl_Comp
//函数功能:获取压力限制补偿设置值
//入口参数:无
//出口参数:设置值(0.1cmH2O)
//**********************************************************************
sint16 Get_Pl_Comp(void)
{
    if (plimit_comp > PLIMIT_COMP_MAX)    plimit_comp = PLIMIT_COMP_DEFAULT;
    if (plimit_comp < PLIMIT_COMP_MIN)    plimit_comp = PLIMIT_COMP_DEFAULT;
    return(plimit_comp);
}

//**********************************************************************
//函数  名:Inc_Dec_Pl_Comp
//函数功能:增加/减少压力限制补偿
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Pl_Comp(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((plimit_comp+step)<=PLIMIT_COMP_MAX)
        {
            plimit_comp += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((plimit_comp-step)>=PLIMIT_COMP_MIN)
        {
            plimit_comp -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Pl_Comp
//函数功能:检查压力限制补偿设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Pl_Comp(void)
{
    if (plimit_comp > PLIMIT_COMP_MAX)//太大
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else if (plimit_comp < PLIMIT_COMP_MIN)//太小
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Pl_Comp
//函数功能:保存压力限制补偿,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Pl_Comp(void)
{
    if (Write_EEPROM(EEP_PL_COMP,( uint8*)(&plimit_comp),sizeof(plimit_comp)) )
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Pl_Comp_EEP
//函数功能:从EEPROM中读出压力限制补偿,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Pl_Comp_EEP(void)
{
    if (Read_EEPROM(EEP_PL_COMP,( uint8*)(&plimit_comp),sizeof(plimit_comp)) )
    {
        plimit_comp = PLIMIT_COMP_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Pl_Comp() == ERROR)//范围不正确则写入默认值
        {
            Save_Pl_Comp();
        }
        return(OK);
    }
}

//**********************************************************************
//函数  名:Get_Paw_No_Raise
//函数功能:获取压力管压无提升设置值
//入口参数:无
//出口参数:设置值(0.1cmH2O)
//**********************************************************************
sint32 Get_Paw_No_Raise(void)
{
    if (paw_no_raise_threshold > PAW_NR_MAX)    paw_no_raise_threshold = PAW_NR_DEFAULT;
    if (paw_no_raise_threshold < PAW_NR_MIN)    paw_no_raise_threshold = PAW_NR_DEFAULT;
    return(paw_no_raise_threshold);
}

//**********************************************************************
//函数  名:Inc_Dec_Paw_No_R
//函数功能:增加/减少管压无提升
//入口参数:dir--修改方向
///            step--步长
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Inc_Dec_Paw_No_R(uint8 dir,uint8 step)
{
    if (dir == INC)
    {
        if ((paw_no_raise_threshold+step)<=PAW_NR_MAX)
        {
            paw_no_raise_threshold += step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }
    else
    {
        if ((paw_no_raise_threshold-step)>=PAW_NR_MIN)
        {
            paw_no_raise_threshold -= step;
            return(OK);
        }
        else
        {
            return(ERROR);
        }
    }    
}

//**********************************************************************
//函数  名:Check_Paw_No_R
//函数功能:检查管压无提升设置是否正确,如果不正确,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Check_Paw_No_R(void)
{
    if (paw_no_raise_threshold > PAW_NR_MAX)//太大
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else if (paw_no_raise_threshold < PAW_NR_MIN)//太小
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);            
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Save_Paw_No_R
//函数功能:保存管压无提升,如果保存成功则输入值有效,否则使用默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--错误
//**********************************************************************
uint8 Save_Paw_No_R(void)
{
    if (Write_EEPROM(EEP_PAW_NO_R,( uint8*)(&paw_no_raise_threshold),sizeof(paw_no_raise_threshold)) )
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else
    {
        return(OK);
    }
}

//**********************************************************************
//函数  名:Rd_Pl_Comp_EEP
//函数功能:从EEPROM中读出管压无提升,如果读出不成功,则赋默认值
//入口参数:无
//出口参数:OK--正确
//            ERROR--读出错误
//**********************************************************************
uint8 Rd_Paw_No_R_EEP(void)
{
    if (Read_EEPROM(EEP_PAW_NO_R,( uint8*)(&paw_no_raise_threshold),sizeof(paw_no_raise_threshold)) )
    {
        paw_no_raise_threshold = PAW_NR_DEFAULT;
        return(ERROR);
    }
    else
    {
        if (Check_Paw_No_R() == ERROR)
        {
            Save_Paw_No_R();
        }
        return(OK);
    }
}