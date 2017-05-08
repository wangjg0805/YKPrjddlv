//**********************************************************************
//文件名:vent_ctrl.h
//**********************************************************************
extern uint8 trigger_judge_phase;

#ifndef _VENT_CTRL_H
#define _VENT_CTRL_H
#include "type_def.h"
#define FLAG_INLET_VAL_ERR_BITS        0        //进气阀状态出错标志位
#define FLAG_OUTLET_VAL_ERR_BITS    1        //出气阀状态出错标志位
#define FLAG_MPL501_OPEN_BITS        2        //压力安全阀打开状态标志位
#define FLAG_NO_VENT_30S_BITS        3        //机动呼吸期间,连续30s没有检测到有呼吸
#define FLAG_P_OVER_BITS            4        //压力超限标志位
#define FLAG_P_NEG_BITS                5        //负压标志
#define FLAG_P_SUS_HIGHER_BITS        6        //管道压力持续过高标志位
#define FLAG_P_SUS_HIGH_BITS        7        //管道压力持续较高标志位
#define FLAG_P_NO_RAISE_BITS        8        //管道压力无提升
#define FLAG_BR_CEASING_BITS        9        //吸气被中止(机动呼吸期间,压力安全阀被打开)
#define FLAG_APNEA_30S_BITS            10        //手动状态下30s没有呼吸的标志位
#define FLAG_APNEA_120S_BITS        11        //手动状态下120s没有呼吸的标志位
#define FLAG_INSP_PEEP_ERR_BITS        12        //吸气PEEP恒压阀状态错误标志位
#define FLAG_EXP_PEEP_ERR_BITS        13        //呼气PEEP调节阀状态错误标志位

void Vent_Control_Detct(void);
void Demarc_Vent_Control_Pro(void);
sint32 Get_Br_Status(void);
void Force_Stop_Vent(void);
void Calc_Plimit_Para(void);
sint32 Get_P_Max_Statistic(void);
sint32 Get_P_Min_Statistic(void);
sint32 Get_Pl_Use(void);
sint32 Get_P_Raise(void);
sint32 Get_Br_Timer(void);
sint32 Get_Exp_End_Time(void);
sint32 Get_Ppl_End_Time(void);
sint32 Get_Insp_End_Time(void);
uint8 Get_Manual_Vent_Phase(void);
void Set_Force_Exp_Flag(void);
void Clr_Force_Exp_Flag(void);
uint8 Check_Force_Exp_Flag(void);
#endif