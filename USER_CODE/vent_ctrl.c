//**********************************************************************
//文件名:vent_ctrl.c
//说    明:
//1.机动呼吸的控制和检测,手动呼吸的检测
//2.检测并记录呼吸过程中的异常状态,以供其它模块查询使用
//函数列表:
//1.初始化机动呼吸的控制参数:Init_Vent_Para
//2.初始化压力统计值:Init_Paw_Para
//3.复位呼吸参数:Reset_Br_Para
//4.计算呼吸测量值:Cal_Br_Meas_Val
//5.给呼吸测量值赋无效值:Invalid_Br_Meas_Val
//6.计算压力相关的测量值:Cal_Pressure_Value
//7.检查进气阀和呼气阀:Check_Val_Status
//8.呼吸控制:Vent_Control_Detct
//9.查询呼吸状态:Get_Br_Status
//10.强制停止机动呼吸:Force_Stop_Vent
//11.标定潮气量时的呼吸控制:Demarc_Vent_Control_Pro
//12.计算管道压力限制参数:Calc_Plimit_Para
//13.获取最大压力的统计值:Get_P_Max_Statistic
//14.获取最小压力的统计值:Get_P_Min_Statistic
//15.获取压力限制值:Get_Pl_Use
//16.获取压力提升值:Get_P_Raise
//17.获取呼吸计时器的时间:Get_Br_Timer
//18.获取机动呼气结束的时间:Get_Exp_End_Time
//19.获取机动屏气结束的时间:Get_Ppl_End_Time
//20.获取机动吸气结束的时间:Get_Insp_End_Time
//21.获取当前手工通气的状态:Get_Manual_Vent_Phase
//**********************************************************************
#include "global.h"
#include "port.h"
#include "vent_ctrl.h"
#include "ad_acquire.h"
#include "data_pro.h"
//#include "debug.h"
#include "type_def.h"
#include "speaker.h"
#include "sys_para.h"

#if (LCDTYPE == TFTLCD)
#include "lpc2400.h"
#else
#include "lpc2294.h"
#endif 

#define FLOW_DETECT_NO_EXP        (0xFFFF/4)    //无呼气检测流量(1/4秒未测到叶轮转)
#define INSP_END_COMP_TIME        50        //吸气提前结束补偿时间(单位:1ms)
#define INSP_VAL_CLS_TIME         80        //吸气阀提前关闭的时间(即最小屏气时间)(单位:1ms)
static sint32 vent_ctrl_timer;            // 机动呼吸计时器,用于统计每次呼吸的时间(单位:1ms)
static sint32 time_rate_count;            //呼吸频率计时器
static sint32 vent_exp_end_time;        //机动呼吸的呼气结束时间(单位:1ms)(相对于vent_ctrl_timer)
static sint32 vent_ppl_end_time;        //机动呼吸的屏气结束时间(单位:1ms)(相对于vent_ctrl_timer)
static sint32 vent_insp_end_time;        //机动呼吸的吸气结束时间(单位:1ms)(相对于vent_ctrl_timer)
static sint32 vent_trigger_end_time;    //SIMV模式下的触发窗口的结束时间(单位:1ms)
static sint32 vent_no_trigger_end_time;    //SIPPV模式下的呼吸不应期时间(单位:1ms)
static sint32 vent_ppl_timer;            //屏气时间累计(单位:1ms)
uint8 trigger_judge_phase = TRIGGER_PHASE_START;
uint16 fan_prd_min_for_trig;
static sint32 paw_start;                //呼吸起始时刻的压力
static sint32 exp_end_p;                //呼气结束时刻的压力值,单位0.1cmH2O
static sint32 paw_max_statistic;        //最大压力统计值
static sint32 paw_min_statistic;        //最大压力统计值
#define PAW_STATISTIC_MUL    10        //最大压力和最小压力统计值的单位
static sint32 paw_sum;                    //压力累加值
#define PAW_SUM_MUL            100            //压力累加值的放大倍数
static sint32 paw_sum_count;            //压力累加次数(用来计算压力平均值)
static uint8 flag_paw_over;                //压力超限标志
static uint8 flag_paw_negative;            //负压标志
static sint32 paw_sus_higher_time;        //压力持续过高时间(单位:1ms) 
static sint32 paw_sus_high_time;        //压力持续较高时间(单位:1ms)
#define TIME_PAW_SUS_HIGHER    10000        //管压持续过高判断时间10秒
#define TIME_PAW_SUS_HIGH    15000        //管压持续较高判断时间15秒
static uint8 paw_no_raise_count;        //管道压力无提升次数(指启动机动呼吸后统计的管道压力无提升次数)

#define TIME_TEST_VALVE        150            //阀电平稳定150ms才判其报警标志
static sint32 time_no_vent;                //机动呼吸期间,没有检测到机动呼吸的时间
static uint8 flag_insp_val_err;                                //进气阀状态错误标记
static uint8 flag_exp_val_err;                                //出气阀状态错误标记
static uint8 flag_insp_peep_val_err;                        //吸气PEEP恒压阀状态错误标记
static uint8 flag_exp_peep_val_err;                            //呼气PEEP调节压阀状态错误标记
static uint8 flag_mpl501_status;                            //压力安全阀的状态
static uint8 flag_br_ceasing;                                //吸气被终止中止(因在呼吸期间压力安全阀打开而被中止)

static sint32 time_manual_detect_dis;        //机动呼吸结束后,禁止手动呼吸检测的计时器(倒计时)
#define TIME_MANUAL_DETECT_DIS        5000    //机动呼吸停止后5s内禁止检测手动呼吸
static uint8 flag_manual_insp;                //处于手动呼吸的吸气状态标记
static uint8 flag_apnea_30s;                //手动呼吸30s没有检测到呼吸
static uint8 count_apnea_30s;                //无呼吸30s次数
static uint8 flag_apnea_120s;                //手动呼吸120s没有检测到呼吸

#define DEMA_HOLD_END_TIME    1000    //标定模式下屏气结束点(1秒)
#define DEMA_VENT_CYCLE        3000    //标定模式下呼吸周期(3秒)
#define DEMA_INSP_END_TIME    (1000-INSP_VAL_CLS_TIME)    //标定模式下吸气结束点(0.9秒)

static sint32 plimit_pressure;//压力限制值(单位:0.1cmH2O)
#define PLMT_MUL    10
static uint8 flag_force_exp=FALSE;    //强制呼气的标志

//**********************************************************************
//函数  名:Init_Vent_Para
//函数功能:初始化机动呼吸的控制参数
//入口参数:无
//出口参数:无
//**********************************************************************
static void Init_Vent_Para(void)
{
    Clr_Force_Exp_Flag();
    vent_exp_end_time = rate_cycle_tab[br_rate_set];    //呼吸周期(即呼气结束的时间点)(单位:1ms)
    vent_ppl_end_time = vent_exp_end_time*ie_i_tab[ie_set]/256-INSP_END_COMP_TIME;    //屏气结束的时间点
    vent_insp_end_time = vent_ppl_end_time*(100-asph_set)*41/4096;
    if(vent_insp_end_time>vent_ppl_end_time-INSP_VAL_CLS_TIME)
    {//至少屏气100ms
        vent_insp_end_time = vent_ppl_end_time - INSP_VAL_CLS_TIME;
    }
    vent_ctrl_timer = 0;    //清零机动呼吸计时器
    time_rate_count = 0;
            
    paw_start = airway_p;        //记录呼吸起始时刻的压力值(用于计算压力提升)    
}

//**********************************************************************
//函数  名:Init_Paw_Para
//函数功能:初始化压力相关参数
//入口参数:无
//出口参数:无
//**********************************************************************
static void Init_Paw_Para(void)
{
    paw_max_statistic = 0xffffffff;    //压力最大值统计,赋值最小值
    paw_min_statistic = 0xffff;    //压力最小值统计,赋值最大
    paw_sum = 0;                    //清零压力累加值
    paw_sum_count = 0;                
    flag_paw_over = FALSE;            //清除压力超限和负压标志
    flag_paw_negative = FALSE;
}


//**********************************************************************
//函数  名:Reset_Br_Para
//函数功能: 复位呼吸参数
//入口参数:无
//出口参数:无
//**********************************************************************
static void Reset_Br_Para(void)
{
    Init_Paw_Para();    //初始化压力相关值

    vent_ctrl_timer = 0;    //清零机动呼吸计时器
    time_rate_count = 0;
    vent_ppl_timer = 0;    
    time_no_vent = 0;        //清除无机动呼吸时间    
    //Init_Vt_Ad();        //初始化潮气量测量值
    //Init_Fan_Period_Statistic();//初始化风扇周期的统计值
    paw_no_raise_count = 0; //清除管道压力无提升次数
    flag_br_ceasing = FALSE;    //清除吸气被中止标志
    flag_manual_insp = TRUE;    //处于手动呼吸的吸气状态
}

//**********************************************************************
//函数  名:Cal_Br_Meas_Val
//函数功能:计算呼吸测量值
//入口参数:无
//出口参数:无
//**********************************************************************
static void Cal_Br_Meas_Val(void)
{
    sint32 di;
    
    if (time_rate_count)
    {
        br_rate_meas = (60000+time_rate_count/2)/time_rate_count;    //呼吸频率测量值(四舍五入)(次/min)
    }
    br_rate_meas = br_rate_set; //呼吸频率直接使用设置值(2012/2/8)
    
    if (flag_eep_vte_error)//潮气量标定数据错误
    {
        vt_meas = VT_MEAS_NOT_VALID;
        insp_flux_meas = VE_MEAS_NOT_VALID;
    }
    else{
        //di = Calc_Filt_Fan_Lap();
        vt_meas = Cal_Vt(di,(uint8*)(&vt_meas_units));    //计算潮气量
        insp_flux_meas = (vt_meas*br_rate_meas+50)/100;    //计算分钟通气量(0.1L/min)
        insp_flux_meas_unit = 10;
    }
    
    if (flag_eep_vte_error||flag_eep_p_error)
    {
        humor_meas = HUMOR_NOT_VALID;
    }
    else{
        //vt_meas_units为0吗?
        if (paw_max_statistic != paw_min_statistic)
        {
            humor_meas = vt_meas*PAW_STATISTIC_MUL/((paw_max_statistic-paw_min_statistic)*vt_meas_units);
        }
        else
        {
            humor_meas = HUMOR_NOT_VALID;
        }
        humor_meas_units = 1;//顺应性单位:1ml/cmH2O
    }
}

//*********************************************************************
//函数  名:Invalid_Br_Meas_Val
//函数功能:赋值给呼吸测量量一个无效值
//入口参数:无
//出口参数:无
//*********************************************************************
static void Invalid_Br_Meas_Val(void)
{
    br_rate_meas = RATE_NOT_VALID;//呼吸频率
    vt_meas = VT_MEAS_NOT_VALID;    //潮气量
    insp_flux_meas = VE_MEAS_NOT_VALID;        //分钟通气量
    humor_meas = HUMOR_NOT_VALID;        //顺应性
}

//**********************************************************************
//函数  名:Cal_Pressure_Value
//函数功能:在呼吸期间,计算压力相关的数据
//入口参数:无
//出口参数:无
//**********************************************************************
static void Cal_Pressure_Value(void)
{
    //压力测量值相关
    if (flag_eep_p_error)//压力标定数据错误
    {
        paw_max = PAW_NOT_VALID;
        paw_avr_meas = PAW_NOT_VALID;
        peep_meas = PAW_NOT_VALID;
    }
    else{
        paw_max = paw_max_statistic*PAW_MAX_MUL/PAW_STATISTIC_MUL;    //一个呼吸周期内的最大压力测量值
        //paw_sum_count为0吗?
        paw_avr_meas = paw_sum*PAW_AVR_MEAS_MUL/(paw_sum_count*PAW_SUM_MUL);    //平均压力
        peep_meas = airway_p*PEEP_MEAS_MUL/PAW_MUL;
    }
}

/****************************************************************************
* 名称：Goto_SIPPV_Exp_Phase()
* 功能：进入SIPPV呼气相
* 入口参数：无
* 出口参数：无
****************************************************************************/
static void Goto_SIPPV_Exp_Phase(void)
{
    vent_no_trigger_end_time = vent_ctrl_timer + TIME_MIN_EXP;    //计算不应期结束时间
    if(vent_no_trigger_end_time > vent_exp_end_time)
    {//不应期结束时间不能超过整个呼吸周期
        vent_no_trigger_end_time = vent_exp_end_time;
    }
    phase_vent_ctrl = PHASE_SIPPV_EXP;    //进入SIPPV呼气相
}

/****************************************************************************
* 名称：Init_SIMV_Trigger_Para()
* 功能：初始化SIMV模式下的触发参数
* 入口参数：无
* 出口参数：无
****************************************************************************/
static void Init_SIMV_Trigger_Para(void)
{
    vent_ctrl_timer = 0;                //机动控制时间清0
    vent_exp_end_time = rate_cycle_tab[br_rate_set];
    vent_trigger_end_time = (vent_exp_end_time * trigger_window) / 100;
    exp_end_p = airway_p;        //呼气末初始压力
}

/****************************************************************************
* 名称：Init_SIMV_Control_Para()
* 功能：初始化SIMV模式下的控制参数
* 入口参数：无
* 出口参数：无
****************************************************************************/
static void Init_SIMV_Control_Para(void)
{
    Clr_Force_Exp_Flag();
    vent_ppl_end_time =    (vent_exp_end_time * ie_i_tab[ie_set]) / 256+vent_ctrl_timer - INSP_END_COMP_TIME;
    if(vent_ppl_end_time > vent_exp_end_time - TIME_MIN_EXP)
    {
        vent_ppl_end_time = vent_exp_end_time - TIME_MIN_EXP;
    }
    vent_insp_end_time = ((vent_ppl_end_time - vent_ctrl_timer) * (100 - asph_set))
                         / 100 + vent_ctrl_timer;
    if(vent_insp_end_time > vent_ppl_end_time - INSP_VAL_CLS_TIME)
    {//至少屏气80ms
        vent_insp_end_time = vent_ppl_end_time - INSP_VAL_CLS_TIME;
    }
    paw_start = airway_p;    //吸气初始压力 
}

//**********************************************************************
//函数  名:Check_Val_Status
//函数功能:检查进气阀和出气阀的状态是否正常
//入口参数:inlet_sta--进气阀的正确状态
//            oulet_sta--出气阀的正确状态(未使用)
//            insp_peep_sta--吸气PEEP恒压阀的正确状态
//            exp_peep_sta--呼气PEEP调节阀的正确状态
//出口参数:无
//**********************************************************************
static void Check_Val_Status( uint8 inlet_sta, uint8 outlet_sta,uint8 insp_peep_sta,uint8 exp_peep_sta)
{
    static sint32 time_inlet_val_ok;
    static sint32 time_inlet_val_err;
    static sint32 time_outlet_val_ok;
    static sint32 time_outlet_val_err;
    static sint32 time_insp_peep_val_ok;
    static sint32 time_insp_peep_val_err;
    static sint32 time_exp_peep_val_ok;
    static sint32 time_exp_peep_val_err;

    if (Get_Insp_Val_Status()==inlet_sta)    //进气阀状态正常
    {
        time_inlet_val_err = 0;
        if (time_inlet_val_ok < TIME_TEST_VALVE)//累加正常时间
        {
            time_inlet_val_ok += VENT_CTRL_PER;
        }    
        if (time_inlet_val_ok >= TIME_TEST_VALVE)
        {//累计正常超过一定时间
            flag_insp_val_err = FALSE;        //清除进气阀状态错误的标记
        }
    }
    else
    {
        time_inlet_val_ok = 0;
        if (time_inlet_val_err < TIME_TEST_VALVE)    //累加异常时间
        {
            time_inlet_val_err += VENT_CTRL_PER;
        }
        if (time_inlet_val_err >= TIME_TEST_VALVE)//异常超过一定时间
        {
            flag_insp_val_err = TRUE;            //标记进气阀状态错误
        }            
    }
    
    if (Get_Insp_Peep_Val_Status()==insp_peep_sta)    //吸气PEEP恒压阀状态正常
    {
        time_insp_peep_val_err = 0;
        if (time_insp_peep_val_ok < TIME_TEST_VALVE)//累加正常时间
        {
            time_insp_peep_val_ok += VENT_CTRL_PER;
        }    
        if (time_insp_peep_val_ok >= TIME_TEST_VALVE)
        {//累计正常超过一定时间
            flag_insp_peep_val_err = FALSE;        //清除吸气PEEP恒压阀状态错误的标记
        }
    }
    else
    {
        time_insp_peep_val_ok = 0;
        if (time_insp_peep_val_err < TIME_TEST_VALVE)    //累加异常时间
        {
            time_insp_peep_val_err += VENT_CTRL_PER;
        }
        if (time_insp_peep_val_err >= TIME_TEST_VALVE)//异常超过一定时间
        {
            flag_insp_peep_val_err = TRUE;            //标记吸气PEEP恒压阀状态错误
        }            
    }    
    
    if (Get_Exp_Peep_Val_Status()==exp_peep_sta)    //呼气PEEP调节阀状态正常
    {
        time_exp_peep_val_err = 0;
        if (time_exp_peep_val_ok < TIME_TEST_VALVE)//累加正常时间
        {
            time_exp_peep_val_ok += VENT_CTRL_PER;
        }    
        if (time_exp_peep_val_ok >= TIME_TEST_VALVE)
        {//累计正常超过一定时间
            flag_exp_peep_val_err = FALSE;        //清除呼气PEEP调节阀状态错误的标记
        }
    }
    else
    {
        time_exp_peep_val_ok = 0;
        if (time_exp_peep_val_err < TIME_TEST_VALVE)    //累加异常时间
        {
            time_exp_peep_val_err += VENT_CTRL_PER;
        }
        if (time_exp_peep_val_err >= TIME_TEST_VALVE)//异常超过一定时间
        {
            flag_exp_peep_val_err = TRUE;            //标记呼气PEEP调节阀状态错误
        }            
    }    
    
    //现在不需要使用呼气阀，因此这部分无效(预留)
    if (Get_Exp_Val_Status()==outlet_sta)    //出气阀状态正常
    {
        time_outlet_val_err = 0;
        if (time_outlet_val_ok < TIME_TEST_VALVE)//累加正常时间
        {
            time_outlet_val_ok += VENT_CTRL_PER;
        }
        if (time_outlet_val_ok >= TIME_TEST_VALVE)//持续正常超过一段时间
        {
            flag_exp_val_err = FALSE;        //清除出气阀状态错误标记
        }
    }
    else
    {
        time_outlet_val_ok = 0;
        if (time_outlet_val_err < TIME_TEST_VALVE)//累加异常时间
        {
            time_outlet_val_err += VENT_CTRL_PER;
        }
        if (time_outlet_val_err >= TIME_TEST_VALVE)//持续异常超过一段时间
        {
            flag_exp_val_err = TRUE;    //标记出气阀状态错误
        }        
    }
    flag_exp_val_err = FALSE;    //现在不需要使用呼气阀，因此这部分无效
}


/****************************************************************************
* 名称: Init_No_Raised_Paw_Para()
* 功能: 初始化压力无提升的相关参数
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
static void Init_No_Raised_Paw_Para(void)
{
    paw_no_raise_count = 0;        //清管压无提升发生次数
    time_no_vent = 0;                //清无机动呼吸时间
}

/****************************************************************************
* 名称：No_Raised_Paw_Detect_Pro()
* 功能：压力无提升检测处理
* 入口参数：无
* 出口参数：无
****************************************************************************/
static void No_Raised_Paw_Detect_Pro(void)
{
    if((airway_p - paw_start)*PAW_NO_RAISE_THR_MUL <= paw_no_raise_threshold*PAW_MUL)
    {//管压提升未超过阈值
        if(paw_no_raise_count < PAW_NO_RAISE_COUNT)
        {
            paw_no_raise_count++;//管压无提升发生次数加1
        }
    }
    else{//管压提升超过了
        Init_No_Raised_Paw_Para();        //初始化压力无提升判别参数
    }
}

//**********************************************************************
//函数  名:Check_Paw_Stable
//函数功能:检查管道压力是否稳定
//入口参数:paw_buf--记录管道压力的缓冲区
//            num --记录管道压力的个数
//            paw_avr_ptr--返回压力平均值的地址
//出口参数:TRUE--压力已经稳定
//            FALSE--压力尚未稳定
//**********************************************************************
static uint8 Check_Paw_Stable(sint32* paw_buf,uint8 num,sint32 *paw_avr_ptr)
{
    /*uint8 i;
    sint32 paw;
    //求压力平均值
    paw = 0;
    for (i=0; i<num; i++)
    {
        paw += paw_buf[i];
    }
    paw /= num;
    
    *paw_avr_ptr = paw;    //返回压力平均值
    //检查压力是否稳定
    for (i=0; i<num; i++)
    {
        if (abs(paw-paw_buf[i])*100 >PAW_MUL*8 )    //压力偏差大于0.08cmH2O
        {//压力没有稳定
            return(FALSE);
        }
    }*/
    
    if ((paw_buf[3]-paw_buf[2]<2)&&
        (paw_buf[2]-paw_buf[1]<2)&&
        (paw_buf[1]-paw_buf[0]<2))    //连续变化率小于0.02cmH2O/50ms
    {
        *paw_avr_ptr =     (paw_buf[0]+paw_buf[1]+paw_buf[2]+paw_buf[3])/4;
        return(TRUE);    
    }    
    return (FALSE);
}

//**********************************************************************
//函数  名:Pressure_Trig_Det
//函数功能:压力触发检测
//入口参数:phase--当前所在检测相位
//            fan_prd_buf--风轮周期的最近几次记录
//            fan_min --进入触发相后检测到的最小风轮周期
//            paw_rcd_buf--最近几次的管道压力记录,采集周期:120ms,单位:0.1cmH2O
//            cur_paw--当前压力值(单位:0.1cmH2O)
//            trig_p--触发压力(单位:0.1cmH2O)
//            pre_avr_p--前一次呼吸的平均压力(单位:0.1cmH2O)
//出口参数:触发检测的相位
//**********************************************************************
uint8 Pressure_Trig_Det(uint8 phase,uint16* fan_prd_buf,uint16 fan_min,sint32* paw_rcd_buf,sint32 cur_paw,uint8 trig_p,
                        sint32 pre_avr_p)
{
    static sint32 paw_trig_cmp; //触发比较点的压力值(单位:0.1cmH2O)
    static sint32 trig_phase_time;    //处在触发相的剩余时间(单位1ms)
    static sint32 fan_acc_phase_time;//处在风轮加速判断相的最大时间(单位1ms)
    switch(phase)
    {
        case TRIGGER_PHASE_START://开始启动检测
            fan_acc_phase_time = 1000;    //处在风轮加速判断相最多1000ms
            phase = TRIGGER_PHASE_FAN_ACC;
            break;
        case TRIGGER_PHASE_FAN_ACC://加速判断
            fan_acc_phase_time -= VENT_CTRL_PER;
            if (((fan_prd_buf[FAN_PERIOD0_IND]+100<fan_prd_buf[FAN_PERIOD1_IND])&&
                 (fan_prd_buf[FAN_PERIOD1_IND]+100<fan_prd_buf[FAN_PERIOD2_IND]))||
                 (fan_acc_phase_time <= 0))
            {//风轮已经加速转起来或者1000ms还未转动则进入风轮拐点判断相
                phase = TRIGGER_PHASE_FAN_INFLEXION;    
            }
            break;
        case TRIGGER_PHASE_FAN_INFLEXION://加速到减速的拐点判断
            if ((fan_min < fan_prd_buf[FAN_PERIOD0_IND])&&
               (fan_min < fan_prd_buf[FAN_PERIOD1_IND])&&
               (fan_min < fan_prd_buf[FAN_PERIOD2_IND]))
             {//已经检测到最大速度，进入减速判断阶段
                 phase = TRIGGER_PHASE_FAN_DEC;    
             }  
            break;    
        case TRIGGER_PHASE_FAN_DEC://风轮减速
            if (((fan_min > 1500)//转速很低的时候根据趋势来判断是否呼气结束 
                &&(fan_prd_buf[FAN_PERIOD0_IND] > fan_prd_buf[FAN_PERIOD1_IND]+50)
                &&(fan_prd_buf[FAN_PERIOD1_IND] > fan_prd_buf[FAN_PERIOD2_IND]+50))||
                 ((fan_min <= 1500
                 &&(fan_prd_buf[FAN_PERIOD0_IND]>2*fan_min)
                 &&(fan_prd_buf[FAN_PERIOD1_IND]>2*fan_min)
                 &&(fan_prd_buf[FAN_PERIOD2_IND]>2*fan_min)))||
                    (T1TC > FLOW_DETECT_NO_EXP)||
                     (fan_prd_buf[FAN_PERIOD0_IND] > FLOW_DETECT_NO_EXP))
            {//减速到一定程度,进入压力判断阶段
                phase = TRIGGER_PHASE_PAW_STABLE;
            }                        
            break;
        case TRIGGER_PHASE_PAW_STABLE:    //判断管道压力是否稳定
            if (((paw_rcd_buf[0]-paw_rcd_buf[1] <= 3*(paw_rcd_buf[1]-paw_rcd_buf[2]))&&
                (paw_rcd_buf[1]<paw_rcd_buf[2])&&(paw_rcd_buf[2]<paw_rcd_buf[3]))||
                ((paw_rcd_buf[0]-paw_rcd_buf[1] <= -4)&&
                (paw_rcd_buf[1]==paw_rcd_buf[2])&&(paw_rcd_buf[2]==paw_rcd_buf[3])))                        
            {//斜率突然变化较大,可能有病人吸气
                if (cur_paw < pre_avr_p)
                {
                    paw_trig_cmp = paw_rcd_buf[1];
                    trig_phase_time = TRIGGER_TIME;        //进入触发相TRIGGER_TIME个ms
                    phase = TRIGGER_PHASE_TRIG;
                }    
            }                    
            break;
        case TRIGGER_PHASE_TRIG://判断病人是否触发吸气
            if(paw_trig_cmp - cur_paw >= trig_p)
            {//已到触发压力
                phase = TRIGGER_PHASE_SUC;    //触发成功
            }
            else
            {//没到触发压力,判断触发相时间是否结束    
                trig_phase_time -= VENT_CTRL_PER;    //计算处在触发期的时间
                if (trig_phase_time <= 0)
                {//规定时间内压力下降值没有达到触发压力
                    phase = TRIGGER_PHASE_PAW_STABLE;//回到压力斜率变化判断
                }
            }            
            break;                    
    }
    return (phase);        
}

//**********************************************************************
//函数  名:Vent_Control_Detct
//函数功能:机动呼吸控制及呼吸监测(放在中断中,每个VENT_CTRL_PER ms进入一次)
//入口参数:无
//出口参数:无
//**********************************************************************
void Vent_Control_Detct(void)
{
    uint16* fan_per_ptr;
    static uint8 flag_no_manual_br = TRUE;    //无手工通气标志
    static uint8 flag_in_manual = TRUE;        //处于手工通气的标志
    #ifdef _DEBUG
    static  sint16 fan_c; //惯性圈数
    #endif
    static uint8 flag_in_simv_first = TRUE;    //第一次进入SIMV模式的标志
    uint8 br_mode;
    static sint32 paw_trend_buf[16],paw_trend_prd = 0;
    
    vent_ctrl_timer += VENT_CTRL_PER;        //机动呼吸控制计时
    time_rate_count += VENT_CTRL_PER;
    //压力监测
    if (airway_p*PAW_STATISTIC_MUL > paw_max_statistic*PAW_MUL)    //统计最大压力
    {
        paw_max_statistic = airway_p*PAW_STATISTIC_MUL/PAW_MUL;
    }
    if (airway_p*PAW_STATISTIC_MUL < paw_min_statistic*PAW_MUL)//统计最小压力
    {
        paw_min_statistic = airway_p*PAW_STATISTIC_MUL/PAW_MUL;
    }
    paw_sum += airway_p*PAW_SUM_MUL/PAW_MUL;        //累加压力值
    paw_sum_count++;            //压力累加次数统计        
    if (airway_p*PLMT_MUL > plimit_pressure*PAW_MUL)    //压力超限检测
    {
        flag_paw_over = TRUE;
    }
    if (airway_p < PAW_NEG_THRESHOLD*PAW_MUL)    //负压检测
    {
        flag_paw_negative = TRUE;
    }
    if (airway_p > PAW_HIGHER_THRESHOLD*PAW_MUL)//管道压力持续过高(压力大于100cmH2O)时间统计
    {
        if (paw_sus_higher_time<TIME_PAW_SUS_HIGHER)
        {
            paw_sus_higher_time += VENT_CTRL_PER;
        }    
    }
    else{
        paw_sus_higher_time = 0;
    }
    if (phase_vent_ctrl == PHASE_NO_VENT)
    {
        if (airway_p*PAW_SUB_HI_THR_MUL > paw_sub_high_thr_manual*PAW_MUL)    //手动呼吸时管道压力持续较高判断
        {
            if (paw_sus_high_time<TIME_PAW_SUS_HIGH)
            {
                paw_sus_high_time += VENT_CTRL_PER;
            }
        }
        else{
            paw_sus_high_time = 0;
        }
    
    }
    else{
        if (airway_p*PAW_SUB_HI_THR_MUL > paw_sub_high_thr_vent*PAW_MUL)    //机动呼吸时管道压力持续较高判断
        {
            if (paw_sus_high_time<TIME_PAW_SUS_HIGH)
            {
                paw_sus_high_time += VENT_CTRL_PER;
            }
        }
        else
        {
            paw_sus_high_time = 0;
        }
    }    
    flag_mpl501_status = Get_MPL501_Status(); //检查压力安全阀
    
    if (++paw_trend_prd>60)
    {//120ms记录一次压力趋势
        paw_trend_prd = 0;
        paw_trend_buf[15] = paw_trend_buf[14];
        paw_trend_buf[14] = paw_trend_buf[13];
        paw_trend_buf[13] = paw_trend_buf[12];
        paw_trend_buf[12] = paw_trend_buf[11];
        paw_trend_buf[10] = paw_trend_buf[9];
        paw_trend_buf[9] = paw_trend_buf[8];
        paw_trend_buf[8] = paw_trend_buf[7];
        paw_trend_buf[7] = paw_trend_buf[6];
        paw_trend_buf[6] = paw_trend_buf[5];
        paw_trend_buf[5] = paw_trend_buf[4];
        paw_trend_buf[4] = paw_trend_buf[3];
        paw_trend_buf[3] = paw_trend_buf[2];
        paw_trend_buf[2] = paw_trend_buf[1];
        paw_trend_buf[1] = paw_trend_buf[0];
        paw_trend_buf[0] = airway_p*10/PAW_MUL;    
    }
                
    switch (phase_vent_ctrl)
    {
        case PHASE_NO_VENT:    //无机动呼吸
            flag_in_manual = TRUE;
            flag_in_simv_first = TRUE;
            Valve_Action_in_Manual();        //保持呼气状态(无PEEP)
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_CLOSE);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:关)
            if (time_manual_detect_dis >0)        //手动呼吸检测屏蔽倒计时
            {
                time_manual_detect_dis -= VENT_CTRL_PER;
                if (time_manual_detect_dis <= 0)    //手动呼吸屏蔽时间到
                {
                    flag_no_manual_br = TRUE;
                    Cal_Pressure_Value();    //计算压力数据
                    Invalid_Br_Meas_Val();    //使呼吸测量值无效
                   // Calc_Filt_Fan_Lap();    //清除之前的风轮圈数统计
                    Reset_Br_Para();    //复位呼吸参数
                    flag_apnea_30s = 0;
                    count_apnea_30s = 0;
                    flag_apnea_120s = 0;
                }
                return;
            }
            //fan_per_ptr = Get_Fan_Prd_Ptr();    //获取最近3次的结果
            if (flag_manual_insp == TRUE)    //处于吸气状态
            {
                if ( (fan_per_ptr[FAN_PERIOD0_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND])
                     &&(fan_per_ptr[FAN_PERIOD2_IND]<2*fan_per_ptr[FAN_PERIOD_MAX_IND]) 
                     &&(fan_per_ptr[FAN_PERIOD0_IND]<fan_per_ptr[FAN_PERIOD1_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]<fan_per_ptr[FAN_PERIOD2_IND]))//检测到进入呼气相
                {
                    flag_manual_insp = FALSE;
                    //Init_Fan_Period_Statistic();    //初始化风扇周期的检测值
                }
                else if (time_rate_count >= 30000)//30s没有呼气
                {
                    Cal_Pressure_Value();    //计算压力数据
                    Cal_Br_Meas_Val();    //计算呼吸测量值
                    br_rate_meas = 0;    //30s没有呼气则呼吸频率为0
                    Reset_Br_Para();    //复位呼吸参数
                    flag_apnea_30s = TRUE;        //标记窒息30s
                    if (++count_apnea_30s>=4) flag_apnea_120s=TRUE;                    
                }
                else    //吸气状态下每3s计算一次呼吸测量值
                {
                    switch(time_rate_count)
                    {
                        case 3000://已到3秒未检测到呼气
                        case 6000://已到6秒未检测到呼气
                        case 9000://已到9秒未检测到呼气
                            if(!flag_no_manual_br) break;        //有手工通气则退出
                        case 12000://已到12秒未检测到呼气
                        case 15000://已到15秒未检测到呼气
                        case 18000://已到18秒未检测到呼气
                        case 21000://已到21秒未检测到呼气
                        case 24000://已到24秒未检测到呼气
                        case 27000://已到27秒未检测到呼气
                            flag_no_manual_br = TRUE;            //无手动通气
                            Cal_Pressure_Value();    
                            if (br_rate_meas == 0)
                            {//当前无呼吸
                                Cal_Br_Meas_Val();                //计算一下全部结果
                                br_rate_meas = 0;
                            }
                            else{
                                Invalid_Br_Meas_Val();                //计算一下全部结果        
                            }                        
                            Init_Paw_Para();                //初始化压力相关的参数
                            break;
                    }    
                }     
            }
            else //处于呼气状态
            {
                if (((fan_per_ptr[FAN_PERIOD_MIN_IND]>600)//转速很低的时候根据趋势来判断是否进入吸气相
                    &&(fan_per_ptr[FAN_PERIOD0_IND] > fan_per_ptr[FAN_PERIOD1_IND]+50)
                    &&(fan_per_ptr[FAN_PERIOD1_IND] > fan_per_ptr[FAN_PERIOD2_IND]+50))||
                     ((fan_per_ptr[FAN_PERIOD_MIN_IND]<=600
                     &&(fan_per_ptr[FAN_PERIOD0_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND])
                     &&(fan_per_ptr[FAN_PERIOD1_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND])
                     &&(fan_per_ptr[FAN_PERIOD2_IND]>3*fan_per_ptr[FAN_PERIOD_MIN_IND]))))    //检测到进入吸气相
                {//一个手动呼吸过程完成
                    flag_manual_insp = TRUE;
                    flag_no_manual_br = FALSE;    //有一次手工通气
                    Manual_Success_Sound();        //发手动通气成功的声音
                    flag_apnea_30s = 0;
                    count_apnea_30s = 0;
                    flag_apnea_120s = 0;                
                    Cal_Pressure_Value();    
                    Cal_Br_Meas_Val();                //计算一下全部结果                        
                    Init_Paw_Para();                //初始化压力相关的参数                                
                   // Init_Fan_Period_Statistic();    //初始化风扇周期的检测值
                    time_rate_count = 0;
                }
                else if(T1TC >= 0xFFFF)//风轮没有转动
                {
                    flag_manual_insp = TRUE; //进入吸气状态
                    //Init_Fan_Period_Statistic();
                }     
            }            
            break;
        case PHASE_START_VENT://开始启动呼吸
            if (flag_in_manual)    //原来处于手动通气状态
            {
                flag_in_manual = FALSE;
                Reset_Br_Para();        //初始化相关呼吸值
            } 
            
            Init_Vent_Para();
            flag_apnea_30s = FALSE;    //有机动呼吸则清除手动呼吸时的窒息标志
            flag_apnea_120s = FALSE;
            br_mode = Get_Br_Mode_Sel(); 
            if (BRM_VCV == br_mode)    //VCV模式
            {
                flag_in_simv_first = TRUE;
                phase_vent_ctrl = PHASE_VCV_INSP;//进入VCV吸气相
            }
            else if (BRM_SIPPV == br_mode)//SIPPV模式
            {    
                flag_in_simv_first = TRUE;
                phase_vent_ctrl = PHASE_SIPPV_INSP;//进入SIPPV吸气相
            }
            else//SIMV模式
            {
                Init_SIMV_Trigger_Para();
                time_manual_detect_dis = TIME_MANUAL_DETECT_DIS;    //禁止检测手动呼吸的时间
                phase_vent_ctrl = PHASE_NO_VENT;//进入无机动呼吸相    
                phase_vent_ctrl = PHASE_SIMV_TRIGGER;//进入SIMV触发相
            }
            break;
//**********************************VCV模式的呼吸控制*********************************************            
        case PHASE_VCV_INSP://吸气
            Valve_Action_in_Insp();        //执行吸气阀门动作
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //检查阀门状态(进气阀:开;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            if (time_no_vent < 30000)    //累加无机动呼吸时间
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }    
            if (flag_exp_val_err||flag_insp_val_err)    //阀门状态错误,强制停止机动呼吸
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }
            else if(flag_force_exp)
            {//强制呼气
                phase_vent_ctrl = PHASE_VCV_HOLD;    //进入VCV屏气相
            }            
            else if (flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {
                phase_vent_ctrl = PHASE_VCV_HOLD;        //强制进入屏气状态
            }
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//吸气时间到        
                phase_vent_ctrl = PHASE_VCV_HOLD;    //进入屏气相
                vent_ppl_timer = 0;        //最小屏气时间计时器清零
            }
            else if(flag_paw_over)
            {//压力超限
                phase_vent_ctrl = PHASE_VCV_HOLD;    //进入屏气相
                vent_ppl_timer = 0;
            }            
            break;
        case PHASE_VCV_HOLD://屏气
            Valve_Action_in_Hold();        //执行屏气阀门动作
            if (time_no_vent < 30000)    //累加无机动呼吸时间
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }        
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //检查阀门状态(进气阀:关;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            vent_ppl_timer += VENT_CTRL_PER;     //累计屏气时间
            if (flag_exp_val_err||flag_insp_val_err)    //阀门状态错误,强制停止机动呼吸
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
                break;
            }
            else if (flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {
                //paw_no_raise_count = 0;                    //管道压力有提升，清除无提升次数
                //time_no_vent = 0;                         //有机动呼吸,清除无机动呼吸时间
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //吸气被中止标记
                phase_vent_ctrl = PHASE_VCV_EXP;        //强制进入呼气状态
                break;
            }
            /*if(vent_ppl_timer < INSP_VAL_CLS_TIME)
            {//未到最小屏气时间则退出
                break;
            }
            if(vent_ppl_timer == INSP_VAL_CLS_TIME)
            {//到达最小屏气时间则判别管道压力有无提升
                if(airway_p - paw_start <= paw_no_raise_threshold)
                {//管压提升未超过阈值
                    if(paw_no_raise_count < PAW_NO_RAISE_COUNT)
                    {
                        paw_no_raise_count++;//管压无提升发生次数加1
                    }
                }
                else
                {//管压提升超过了
                    paw_no_raise_count = 0;    //清管压无提升发生次数
                    time_no_vent = 0;                         //有机动呼吸,清除无机动呼吸时间
                }
            }*/
            
            if(vent_ctrl_timer >= vent_ppl_end_time)
            {//屏气时间到
                phase_vent_ctrl = PHASE_VCV_EXP;    //进入呼气相
                No_Raised_Paw_Detect_Pro();
                #ifdef _DEBUG
                fan_c = Get_Fan_Counter();
                #endif
                flag_br_ceasing = FALSE;                //清除机动呼吸被中止标志
            }
            else if(flag_paw_over)
            {//压力超限
                phase_vent_ctrl = PHASE_VCV_EXP;    //进入呼气相
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //清除机动呼吸被中止标志
            }
            else if(flag_force_exp)
            {//强制呼气
                flag_br_ceasing = FALSE;                //清吸气被中止标志
                No_Raised_Paw_Detect_Pro();            //压力无提升判别处理
                phase_vent_ctrl = PHASE_VCV_EXP;    //进入VCV呼气相
            }                    
            break;
        case PHASE_VCV_EXP://呼气
            Valve_Action_in_Exp();        //呼气阀门动作
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:开)
            if (time_no_vent < 30000)    //累加无机动呼吸时间
            {    
                time_no_vent += VENT_CTRL_PER;            //
            }                
            if (flag_exp_val_err||flag_insp_val_err)    //阀门状态错误,强制停止机动呼吸
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }            
            else if(vent_ctrl_timer >= vent_exp_end_time)
            {//呼气时间到
                Cal_Pressure_Value();
                Cal_Br_Meas_Val();
                Init_Vent_Para();            //初始化机动呼吸参数
                Init_Paw_Para();            //初始化压力相关的参数
                phase_vent_ctrl = PHASE_START_VENT;    //进入启动相
            }            
            break;
//**********************************VCV模式控制结束*************************************************            
            
//**********************************SIPPV模式的呼吸控制*********************************************    
        case PHASE_SIPPV_INSP://正在SIPPV吸气
            Valve_Action_in_Insp();            //保持进气状态
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);//检查阀门状态(进气阀:开;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if (flag_exp_val_err||flag_insp_val_err)    //阀门状态错误,强制停止机动呼吸
            {
                phase_vent_ctrl = PHASE_STOP_VENT;
            }            
            if(flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {//压力安全阀打开
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //进入SIPPV屏气相
            }
            else if(flag_paw_over)
            {//压力超限
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //进入SIPPV屏气相
            }
            else if(flag_force_exp)
            {//强制呼气
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //进入SIPPV屏气相
            }            
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//吸气时间到
                phase_vent_ctrl = PHASE_SIPPV_HOLD;    //进入SIPPV屏气相
            }
            break;
        case PHASE_SIPPV_HOLD://正在SIPPV屏气
            Valve_Action_in_Hold();            //保持屏气状态
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //检查阀门状态(进气阀:关;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {//压力安全阀打开
                //paw_no_raise_count = 0;                    //管道压力有提升，清除无提升次数
                //time_no_vent = 0;                         //有机动呼吸,清除无机动呼吸时间
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //吸气被中止标记
                Goto_SIPPV_Exp_Phase();                //进入SIPPV呼气相
            }
            else if(flag_paw_over)
            {//压力超限
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;            //清吸气被中止标志
                Goto_SIPPV_Exp_Phase();                //进入SIPPV呼气相
            }
            else if(flag_force_exp)
            {//强制呼气
                flag_br_ceasing = FALSE;                //清吸气被中止标志
                No_Raised_Paw_Detect_Pro();            //压力无提升判别处理
                Goto_SIPPV_Exp_Phase();                //进入SIPPV呼气相
            }            
            else if(vent_ctrl_timer >= vent_ppl_end_time)
            {//屏气时间到
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;            //清吸气被中止标志
                Goto_SIPPV_Exp_Phase();                //进入SIPPV呼气相
            }
            break;
        case PHASE_SIPPV_EXP://正在SIPPV呼气相
            Valve_Action_in_Exp();            //保持呼气状态
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:开)
            if(vent_ctrl_timer >= vent_no_trigger_end_time)
            {//呼气不应期时间到
                exp_end_p = airway_p;    //呼气末初始压力
                phase_vent_ctrl = PHASE_SIPPV_TRIGGER;    //进入SIPPV触发相
                trigger_judge_phase = TRIGGER_PHASE_START;    //启动检测相 
                fan_prd_min_for_trig = UINT16_MAX;        //赋给最大值，以便统计 
            }
            break;
        case PHASE_SIPPV_TRIGGER://正在SIPPV触发相
            Valve_Action_in_Exp();            //保持呼气状态
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:开)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
           // trigger_judge_phase = Pressure_Trig_Det(trigger_judge_phase,Get_Fan_Prd_Ptr(),fan_prd_min_for_trig,
           //                                         paw_trend_buf,airway_p*10/PAW_MUL,Get_P_Trig_Usr_Set(),paw_avr_meas);
            if (TRIGGER_PHASE_SUC == trigger_judge_phase)
            {//触发成功                                        
                vent_exp_end_time = vent_ctrl_timer;    //结束呼气    
            }
            if(vent_ctrl_timer >= vent_exp_end_time)
            {//呼气时间到
                Cal_Pressure_Value();
                Cal_Br_Meas_Val();
                Init_Paw_Para();            //初始化压力相关的参数
               // Init_Fan_Period_Statistic();//初始化风扇周期的统计
                phase_vent_ctrl = PHASE_START_VENT;    //进入启动状态
                
                flag_rcd_prd = FALSE;
            }
            break;
//********************************SIPPV控制结束***********************************************

//********************************SIMV模式****************************************************
        case PHASE_SIMV_TRIGGER://正在SIMV触发相
            Valve_Action_in_Exp();            //保持呼气状态
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:开)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
           // trigger_judge_phase = Pressure_Trig_Det(trigger_judge_phase,Get_Fan_Prd_Ptr(),fan_prd_min_for_trig,
           //                                         paw_trend_buf,airway_p*10/PAW_MUL,Get_P_Trig_Usr_Set(),paw_avr_meas);
            if (TRIGGER_PHASE_SUC == trigger_judge_phase)
            {//触发成功                                        
                vent_trigger_end_time = vent_ctrl_timer;    //结束呼气    
            }

            if(vent_ctrl_timer >= vent_trigger_end_time)
            {//触发吸气
                if(flag_in_simv_first)//因为之前的测量参数不是SIMV模式的参数，所以忽略第一次的计算
                {//第一次计算呼吸值
                    flag_in_simv_first = FALSE;
                }
                else
                {//计算呼吸测量值
                    Cal_Pressure_Value();
                    Cal_Br_Meas_Val();
                }
                Init_Paw_Para();            //初始化压力相关的参数
                //Init_Fan_Period_Statistic();//初始化风扇周期的统计
                time_rate_count = 0;        //清除呼吸频率计时器
                if(BRM_SIMV != Get_Br_Mode_Sel())
                {//呼吸模式变了
                    phase_vent_ctrl = PHASE_START_VENT;    //进入启动状态
                }
                else{
                    Init_SIMV_Control_Para();            //初始化SIMV控制参数
                    phase_vent_ctrl = PHASE_SIMV_INSP;    //进入SIMV吸气相
                }
            }
            break;
        case PHASE_SIMV_INSP://正在SIMV吸气
            Valve_Action_in_Insp();            //保持进气状态
            Check_Val_Status(VAL_OPEN,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //检查阀门状态(进气阀:开;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {//压力安全阀打开
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //进入SIMV屏气相
            }
            else if(flag_paw_over)
            {//压力超限
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //进入SIMV屏气相
            }
            else if(flag_force_exp)
            {//强制呼气
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //进入SIMV屏气相
            }            
            else if(vent_ctrl_timer >= vent_insp_end_time)
            {//吸气时间到
                phase_vent_ctrl = PHASE_SIMV_HOLD;    //进入SIMV屏气相
            }
            break;
        case PHASE_SIMV_HOLD://正在SIMV屏气
            Valve_Action_in_Hold();            //保持屏气状态
            Check_Val_Status(VAL_CLOSE,VAL_CLOSE,VAL_OPEN,VAL_CLOSE);    //检查阀门状态(进气阀:关;呼气阀:关;吸气peep阀:开;呼气PEEP阀:关)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)    //压力安全阀打开
            {//压力安全阀打开
                //paw_no_raise_count = 0;                    //管道压力有提升，清除无提升次数
                //time_no_vent = 0;                         //有机动呼吸,清除无机动呼吸时间
                Init_No_Raised_Paw_Para();
                flag_br_ceasing = TRUE;                    //吸气被中止标记
                phase_vent_ctrl = PHASE_SIMV_EXP;    //进入SIMV呼气相
            }
            else if(flag_paw_over)
            {//压力超限
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //清吸气被中止标志
                phase_vent_ctrl = PHASE_SIMV_EXP;    //进入SIMV呼气相
            }
            else if(flag_force_exp)
            {//强制呼气
                flag_br_ceasing = FALSE;                //清吸气被中止标志
                No_Raised_Paw_Detect_Pro();            //压力无提升判别处理
                phase_vent_ctrl = PHASE_SIMV_EXP;    //进入SIMV呼气相
            }                
            else if(vent_ctrl_timer >= vent_ppl_end_time)
            {//屏气时间到
                No_Raised_Paw_Detect_Pro();
                flag_br_ceasing = FALSE;                //清吸气被中止标志
                phase_vent_ctrl = PHASE_SIMV_EXP;    //进入SIMV呼气相
            }
            fan_prd_min_for_trig = UINT16_MAX;        //赋给最大值，统计呼气周期内的风轮最小周期
            break;
        case PHASE_SIMV_EXP://正在SIMV呼气
            Valve_Action_in_Exp();            //保持呼气状态
            Check_Val_Status(VAL_CLOSE,VAL_OPEN,VAL_CLOSE,VAL_OPEN);    //检查阀门状态(进气阀:关;呼气阀:开;吸气peep阀:关;呼气PEEP阀:开)
            if(time_no_vent < 30000) time_no_vent += VENT_CTRL_PER;
            if(vent_ctrl_timer >= vent_exp_end_time)
            {//呼气时间到
                Init_SIMV_Trigger_Para();    //初始化SIMV触发参数  
                phase_vent_ctrl = PHASE_SIMV_TRIGGER;    //进入SIMV触发相
               // fan_per_ptr = Get_Fan_Prd_Ptr();    //获取最近3次的结果
                if ((fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD0_IND])&&
                    (fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD1_IND])&&
                    (fan_prd_min_for_trig < fan_per_ptr[FAN_PERIOD2_IND]))
                {//风轮已经达到最大速度,则直接进入减速相
                    trigger_judge_phase = TRIGGER_PHASE_FAN_DEC;    
                }
                else
                {    
                    trigger_judge_phase = TRIGGER_PHASE_START;    //启动检测相  
                }
            }
            break;
//*******************************SIMV模式控制结束**********************************************                                
        case PHASE_STOP_VENT://停止机动呼吸
            paw_no_raise_count = 0; //清除管道压力无提升次数
            flag_br_ceasing = FALSE;    //清除机动呼吸期间,机动呼吸被终止标志
            time_no_vent = 0;         //机动呼吸停止,清除无机动呼吸的时间
            time_manual_detect_dis = TIME_MANUAL_DETECT_DIS;    //禁止检测手动呼吸的时间
            phase_vent_ctrl = PHASE_NO_VENT;//进入无机动呼吸相            
            break;
        default :break;                         
    }
}

//**********************************************************************
//函数  名:Get_Br_Status
//函数功能:查询呼吸状态
//入口参数:无
//出口参数:某一位为1表示有对应的异常事件发生
//**********************************************************************
sint32 Get_Br_Status(void)
{
    sint32 br_status = 0;
    if (TRUE == flag_insp_val_err) br_status |= (1<<FLAG_INLET_VAL_ERR_BITS);    //进气阀状态出错
    if (TRUE == flag_exp_val_err)    br_status |= (1<<FLAG_OUTLET_VAL_ERR_BITS);    //出气阀状态出错
    if (VAL_OPEN == flag_mpl501_status) br_status |= (1<<FLAG_MPL501_OPEN_BITS);//压力安全阀MPL501打开
    if (time_no_vent >= 30000) br_status |= (1<<FLAG_NO_VENT_30S_BITS);            //在机动呼吸期间,连续30s没有检测到有机动呼吸
    if (TRUE == flag_paw_over) br_status |= (1<<FLAG_P_OVER_BITS);                //机动呼吸压力超限
    if (TRUE == flag_paw_negative) br_status |= (1<<FLAG_P_NEG_BITS);            //负压标志
    if (paw_sus_higher_time >= TIME_PAW_SUS_HIGHER)    br_status |= (1<<FLAG_P_SUS_HIGHER_BITS);    //管道压力持续过高
    if (paw_sus_high_time >= TIME_PAW_SUS_HIGH)    br_status |= (1<<FLAG_P_SUS_HIGH_BITS);            //管道压力持续较高
    if (paw_no_raise_count >= 2)    br_status |= (1<<FLAG_P_NO_RAISE_BITS);        //管道压力无提升
    if (TRUE == flag_br_ceasing)    br_status |= (1<<FLAG_BR_CEASING_BITS);        //吸气被中止(机动呼吸期间压力安全阀打开)
    if (TRUE == flag_apnea_30s)        br_status |= (1<<FLAG_APNEA_30S_BITS);        //手动状态下30s没有呼吸
    if (TRUE == flag_apnea_120s)    br_status |= (1<<FLAG_APNEA_120S_BITS);        //手动状态下120s没有呼吸
    if (TRUE == flag_insp_peep_val_err)        br_status |= (1<<FLAG_INSP_PEEP_ERR_BITS);    //吸气PEEP恒压阀状态错误
    if (TRUE == flag_exp_peep_val_err)        br_status |= (1<<FLAG_EXP_PEEP_ERR_BITS);    //吸气PEEP恒压阀状态错误            
    return(br_status);
}

//**********************************************************************
//函数  名:Force_Stop_Vent
//函数功能:强制停止机动呼吸
//入口参数:无
//出口参数:无
//**********************************************************************
void Force_Stop_Vent(void)
{
    if (phase_vent_ctrl != PHASE_NO_VENT)
    {
        phase_vent_ctrl = PHASE_STOP_VENT;
    }
}

/****************************************************************************
* 名称：Demarc_Vent_Control_Pro()
* 功能：在标定模式下机动呼吸的控制(用于标定潮气量,放在时间中断中,2ms一次)
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Demarc_Vent_Control_Pro(void)
{
    vent_ctrl_timer += VENT_CTRL_PER;        //每2ms进入一次
    flag_mpl501_status = Get_MPL501_Status(); //检查压力安全阀
    switch(phase_vent_ctrl)
    {
        case PHASE_NO_VENT://当前无机动呼吸
            Valve_Action_in_Manual();        //保持自由状态状态
            break;
        case PHASE_START_VENT://启动机动呼吸
            //Init_Vt_Ad();
            flag_mpl501_status = VAL_CLOSE;//清压力安全阀打开标志
            vent_ctrl_timer = 0;            //时间清0
            phase_vent_ctrl = PHASE_VCV_INSP;    //进入吸气相
            break;
        case PHASE_VCV_INSP://正在吸气
            Valve_Action_in_Insp();        //保持进气状态
            if(vent_ctrl_timer >= DEMA_INSP_END_TIME)
            {//吸气时间到
                phase_vent_ctrl = PHASE_VCV_HOLD;    //进入屏气相
                vent_ppl_timer = 0;                //最小屏气时间清0(屏气100ms)
            }
            else if(flag_mpl501_status == VAL_OPEN)
            {//压力安全阀打开
                phase_vent_ctrl = PHASE_VCV_HOLD;    //进入屏气相
            }
            break;
        case PHASE_VCV_HOLD://正在屏气
            Valve_Action_in_Hold();        //保持屏气状态
            vent_ppl_timer += VENT_CTRL_PER;
            if(flag_mpl501_status == VAL_OPEN)
            {//压力安全阀打开或最小屏气时间到
                phase_vent_ctrl = PHASE_VCV_EXP;    //进入呼气相
            }
            else if(vent_ppl_timer >= INSP_VAL_CLS_TIME)
            {//压力安全阀打开或最小屏气时间到
                phase_vent_ctrl = PHASE_VCV_EXP;    //进入呼气相
            }
            break;
        case PHASE_VCV_EXP://正在呼气
            Valve_Action_in_Exp();        //保持呼气状态
            if(vent_ctrl_timer >= DEMA_VENT_CYCLE)
            {//呼吸周期到
                //demar_laps = Calc_Filt_Fan_Lap();        //计算滤波的风扇圈数
                demarc_vte = Cal_Vt(demar_laps,&demarc_vte_uinits);
                flag_mpl501_status = VAL_CLOSE;//清压力安全阀打开标志
                vent_ctrl_timer = 0;            //时间清0
                phase_vent_ctrl = PHASE_VCV_INSP;    //进入吸气相
            }
            break;
        case PHASE_PAW_DEM: //正在标定压力传感器
            Valve_Action_in_Exp(); //关闭进气阀，打开呼气PEEP调节阀，关闭吸气PEEP恒压阀
            break;    
        case PHASE_STOP_VENT://停止
            //Init_Vt_Ad();        //相关数值清0
            demarc_vte = 0;
            phase_vent_ctrl = PHASE_NO_VENT;    //进入无机动状态
            break;
    }
}

/****************************************************************************
* 名称：Calc_Plimit_Para()
* 功能：计算管道压力限制报警判断的数值
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Calc_Plimit_Para(void)
{
    //压力限制判断值(plimit_comp为提前补偿值)
    plimit_pressure = paw_limit_set * 10 - plimit_comp;
    
    //机动呼吸时候的持续管道压力限制(0.1cmH2O)
    //Plimit <=30cmH2O, Sustained Pressure = 6+PEEP cmH2O
    //Plimit <=60cmH2O, Sustained Pressure = 20%Plimit+PEEP cmH2O
    //Plimit >60cmH2O, Sustained Pressure = 12+PEEP cmH2O
    if(paw_limit_set <= 30)
    {
        paw_sub_high_thr_vent = 60 + 30;    //设定自然PEEP为3cmH2O
    }
    else if(paw_limit_set <= 60)
    {
        paw_sub_high_thr_vent = paw_limit_set * 2 + 30;    //设定自然PEEP为3cmH2O
    }
    else{
        paw_sub_high_thr_vent = 120 + 30;    //设定自然PEEP为3cmH2O
    }
    
    //手动呼吸时候的持续管道压力限制(0.1cmH2O)
    //Plimit <=60cmH2O, Sustained Pressure = 50%Plimit
    //Plimit >60cmH2O, Sustained Pressure = 30cmH2O
    if(paw_limit_set <= 60)
    {
        paw_sub_high_thr_manual = paw_limit_set * 5;    //50%Plimit
    }
    else{
        paw_sub_high_thr_manual = 300;                    //30cmH2O
    }
    paw_sub_high_thr_vent = paw_sub_high_thr_manual;    //因为PEEP可调，故取手动值
}

//**********************************************************************
//函数  名;Get_P_Max_Statistic
//函数功能:获取最大压力的统计值
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_P_Max_Statistic(void)
{
    return(paw_max_statistic);
}

//**********************************************************************
//函数  名;Get_P_Min_Statistic
//函数功能:获取最小压力的统计值
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_P_Min_Statistic(void)
{
    return(paw_min_statistic);
}

//**********************************************************************
//函数  名;Get_Pl_Use
//函数功能:获取压力限制值
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_Pl_Use(void)
{
    return(plimit_pressure);
}

//**********************************************************************
//函数  名;Get_P_Raise
//函数功能:获取压力提升值
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_P_Raise(void)
{
    return(airway_p-paw_start);
}

//**********************************************************************
//函数  名;Get_Br_Timer
//函数功能:获取呼吸计时时间
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_Br_Timer(void)
{
    return(vent_ctrl_timer);
}

//**********************************************************************
//函数  名;Get_Exp_End_Time
//函数功能:获取机动呼气结束时间
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_Exp_End_Time(void)
{
    return(vent_exp_end_time);
}

//**********************************************************************
//函数  名;Get_Ppl_End_Time
//函数功能:获取机动屏气结束时间
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_Ppl_End_Time(void)
{
    return(vent_ppl_end_time);
}

//**********************************************************************
//函数  名;Get_Insp_End_Time
//函数功能:获取机动吸气结束时间
//入口参数:无
//出口参数:限制值
//**********************************************************************
sint32 Get_Insp_End_Time(void)
{
    return(vent_insp_end_time);
}

//**********************************************************************
//函数  名:Get_Manual_Vent_Phase
//函数功能:获取当前手工通气的状态
//入口参数:无
//出口参数:通气状态(TRUE:吸气状态;FALSE:呼气状态)
//**********************************************************************
uint8 Get_Manual_Vent_Phase(void)
{
    return(flag_manual_insp);
}

//**********************************************************************
//函数  名:Set_Force_Exp_Flag
//函数功能:置位强制呼气标志
//入口参数:无
//出口参数:无
//**********************************************************************
void Set_Force_Exp_Flag(void)
{
    flag_force_exp = TRUE;
}

//**********************************************************************
//函数  名:Clr_Force_Exp_Flag
//函数功能:清零强制呼气标志
//入口参数:无
//出口参数:无
//**********************************************************************
void Clr_Force_Exp_Flag(void)
{
    flag_force_exp = FALSE;
}

//**********************************************************************
//函数  名:Check_Force_Exp_Flag
//函数功能:检查强制呼气标志
//入口参数:无
//出口参数:无
//**********************************************************************
uint8 Check_Force_Exp_Flag(void)
{
    return (flag_force_exp);
}

