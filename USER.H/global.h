//**********************************************************************
//文件名:global.h
//**********************************************************************
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "type_def.h"
#include "lcd2478.h"


#define SOFT_VERSION             1.0

#define TFTLCD     1
#define LCDTYPE    TFTLCD   

////////////////////////////ADD BY WJG 2012.06.01
#define MONOLCD    0
#define TFTLCD     1
#define LCDTYPE    TFTLCD   

////////////////////////////////////////////
#define  USB_ENABLE                 0

////////////////////////////////////////////
#define  DEFAULT_USER_ID            1

#define  DEFAULT_PH_DISPLAY_DOT    0x08
#define  DEFAULT_MV_DISPLAY_DOT    0x02

//默认温度补偿数值
#define  DEFAULT_TEMP_COMP          25
////////////////////////////END

///////////////////////////////////////////////////////
//机器的几种工作模式
///////////////////////////////////////////////////////

#define MACHINE_NORMAL_MODE        0    //正常工作模式
#define MACHINE_SERVICE_MODE       1    //维修模式
#define MACHINE_DEMARC_MODE        2    //标定模式
#define MACHINE_SCREENCAL_MODE     3    //屏幕校准状态

///////////////////////////////////////////////////////
//机器正常工作的几种工作状态
//////////////////////////////////////////////////////
#define STAT_NULL               0x00
#define STAT_NORMAL             0x01
#define STAT_SYS_CAL            0x02
#define STAT_USER_CAL           0x03



///////////////////////////////////////////////////////
//三种溶液标准
//////////////////////////////////////////////////////
#define CHINESE_S               0x00
#define EUROPE_S                0x01
#define AMERICAN_S              0x02

///////////////////////////////////////////////////////
//电导率 状态下 的 测量参数
//////////////////////////////////////////////////////
#define PARAM_DDLV              0x00
#define PARAM_TDS               0x01
#define PARAM_SALTY             0x02
#define PARAM_NULL              0x0A

///////////////////////////////////////////////////////
//电导率 状态下 的 三种测量模式
//////////////////////////////////////////////////////
#define MODE_CONTINUE           0x00
#define MODE_TIMER              0x01
#define MODE_BALANCE            0x02
#define MODE_NULL               0x0A

///////////////////////////////////////////////////////
//电导率 状态下 的 两种测量单位
//////////////////////////////////////////////////////

#define MS_CM                   0x01
#define US_CM                   0x02


///////////////////////////////////////////////////////
//按键的几种状态
//////////////////////////////////////////////////////

#define KEY_PRESS               0x01  //按下
#define KEY_RELEASE             0x02  //松开
#define KEY_DO_START            0x04  //开始处理此按键
#define KEY_DO_OVER             0x08  //处理结束


////////////开机预热时间 秒
#define     PREHOT_TIME           1800
//进入待机时间 s
#define    ENTER_SLEEP_TIME       300

//酸度计 sys校准三个步骤
#define   SYS_CAL_ZER0A           1  //read ad7190ch3     0mv
#define   SYS_CAL_ZER0B           2  //read ad7190ch4     0mv
#define   SYS_CAL_LOW1A           3  //read ad7190ch4   100mv
#define   SYS_CAL_LOW1B           4  //read ad7190ch4   100mv
#define   SYS_CAL_LOW2A           5  //read ad7190ch3  -100mv
#define   SYS_CAL_LOW2B           6  //read ad7190ch4   100mv
#define   SYS_CAL_FULL1           7  //read ad7190ch1  1400mv
#define   SYS_CAL_FULL2           8  //read ad7190ch2 -1400mv
#define   SYS_CAL_FULL3           9  //read ad7190ch2 -2000mv
#define   SYS_CAL_OVER           10
//
//电导率仪 sys校准步骤
#define   SYS_CAL_RANG1_1          1  //依次校准的各个电阻点
#define   SYS_CAL_RANG1_2          2
#define   SYS_CAL_RANG1_3          3
#define   SYS_CAL_RANG1_4          4
#define   SYS_CAL_RANG1_5          5
#define   SYS_CAL_RANG1_6          6
#define   SYS_CAL_RANG1_7          7
#define   SYS_CAL_RANG2_1          8
#define   SYS_CAL_RANG2_2          9
#define   SYS_CAL_RANG2_3         10
#define   SYS_CAL_RANG2_4         11
#define   SYS_CAL_RANG2_5         12
#define   SYS_CAL_RANG2_6         13
#define   SYS_CAL_RANG2_7         14
#define   SYS_CAL_RANG3_1         15  //依次校准的各个电阻点
#define   SYS_CAL_RANG3_2         16
#define   SYS_CAL_RANG3_3         17
#define   SYS_CAL_RANG3_4         18
#define   SYS_CAL_RANG3_5         19
#define   SYS_CAL_RANG3_6         20
#define   SYS_CAL_RANG3_7         21
#define   SYS_CAL_RANG4_1         22
#define   SYS_CAL_RANG4_2         23
#define   SYS_CAL_RANG4_3         24
#define   SYS_CAL_RANG4_4         25
#define   SYS_CAL_RANG4_5         26
#define   SYS_CAL_RANG4_6         27
#define   SYS_CAL_RANG4_7         28
#define   SYS_CAL_RANG5_1         29  //依次校准的各个电阻点
#define   SYS_CAL_RANG5_2         30
#define   SYS_CAL_RANG5_3         31
#define   SYS_CAL_RANG5_4         32
#define   SYS_CAL_RANG5_5         33
#define   SYS_CAL_RANG5_6         34
#define   SYS_CAL_RANG5_7         35
#define   SYS_CAL_RANG6_1         36
#define   SYS_CAL_RANG6_2         37
#define   SYS_CAL_RANG6_3         38
#define   SYS_CAL_RANG6_4         39
#define   SYS_CAL_RANG6_5         40
#define   SYS_CAL_RANG6_6         41
#define   SYS_CAL_RANG6_7         42

#define   SYS_CAL_END             43
//用户校准

#define   USER_CAL_STEP1          1
#define   USER_CAL_STEP2          2
#define   USER_CAL_STEP3          3
#define   USER_CAL_STEP4          4
#define   USER_CAL_STEP5          5
#define   USER_CAL_END            6


//////////////////////////////////////
//环境温度超出 5-50 度后 报警提示用户

#define   TEMP_SENSOR_LOW           5
#define   TEMP_SENSOR_HIGH         50
#define   TEMP_SENSOR_NC_VALUE    999

#define   TEMP_UNIT_OC           0x01
#define   TEMP_UNIT_OF           0x80


//校验字节
#define   REC_CHECK_DATA1       0x55
#define   REC_CHECK_DATA2       0xaa

//亮度调整等级
#define   BRIGHT_CLASS_TOTAL      20
#define   DEFAULT_BRIGHTNESS       8
#define   SLEEP_BRIGHTNESS         1


//密码采集阶段，数字输入窗口的ID
#define   PASSWORD_INPUT_ID  0x12345678
#define   PASSWORD_DATA1         8866
#define   PASSWORD_DATA2         9999

//控件结构体
struct KeyStruct{
    uint32 X_Position;     /* X */
    uint32 Y_Position;     /* Y */
    uint32 Value;          /* value */
};

//存储记录结构体
typedef union    
{
   float  rec_float;    //数据可以是浮点数，也可以是整数
   uint32 rec_uint32;
}un_rec_data;

//
enum {
	RANGE_200MS = 0,
	RANGE_20MS,
	RANGE_2MS,
	RANGE_200US,
	RANGE_20US,
	RANGE_2US,
};


extern uint32   user_id,phmv_division,which_standard,lcd_brightness;      //显示屏对比度的值
extern uint32   select_ch,switch_flag,match_item,power_fre;
extern uint32   sound_switch,sound_flag;

extern uint32   key_value,Key_Status,electrode_class;
extern struct   KeyStruct Key[15];

extern uint32   machine_mode,current_mode,mode_run_flag;
extern uint32   power_on_flag,power_on_clr_cnt,elapsed_time;
extern uint32   Error_Info_Flag,Warning_Info_Flag;

extern uint32   ph_negative_flag,mv_negative_flag,full_flag;

extern uint32   ad_raw_data,flag_new_data,stable_flag,break_stable_flag,sample_flag;

extern float    mv_data,ph_data,get_coef,ddlv_data;

extern float    elec_cons,temp_cons,TDS_cons;           //电极参数 温补系数 TDS系数
extern uint32   DDLV_Param,DDLV_Mode,DDLV_unit;         //电导率的测量模式
extern uint32   measure_timer_sec,measure_timer_sec_real;
extern uint32   balance_sec,balance_sec_real;
extern  float   balance_data;
//
//extern uint32   filter_phase,switch_action_step;
extern uint32   ddlv_syscal_buf[6][7]; //保存系统校准的数据
//extern uint32   cal_ddlv_CONS[6];

//extern uint32   cal_ddlv_b_flag[6];
//extern float    cal_ddlv_b[6],cal_ddlv_k[6];

extern uint32   cpu_ch_adc3,cpu_ch_adc4;
extern uint32   zero_data[4],low1_data[4],low2_data[4],full1_data[4],full2_data[4],full3_data[4];
extern uint32   user_cal_num,user_cal_data[5];    //存放用户校准的数据 及个数

extern float    sys_coef1, sys_coef2, sys_coef3, sys_coef4, sys_coef5, sys_coef6;
extern float   user_coef1,user_coef2,user_coef3,user_coef4,user_coef5,user_coef_left;
//定义电导率的用户校准系数
extern float    ddlv_R,ddlv_coef;

extern un_rec_data  user_cal_phdata[5];
extern un_rec_data  user_cal_phdata_dis[5];
extern uint32       user_cal_point[5];

extern uint32   point2_cal_start,sys_cal_ok,user_cal_ok,point2_cal_step;

//////////////////////////////////
extern uint32      temp_comp_manual,manual_temp_comp_flag;
extern float       temp_float_ptc;
////////////////////////////增加自动进入待机功能
extern uint32   no_key_time;
//////////////////////////////////
extern uint32   rec_total_num,rec_real_num,rec_total_pages,rec_curre_index;

extern uint32  need_recover,need_recover2,from_menu;
extern uint32  save_buf[LINE_SIZE*SCR_YSIZE/1];//被临时保存的缓冲区用于从数字键盘返回恢复
extern uint32  save_buf2[LINE_SIZE*SCR_YSIZE/1];

extern const   float standard_data_p811[][5],standard_data_p801[][5];
extern const   float DDLV_standard_data[][4]; //电导率用户校准标准溶液

extern const   float temp_comp_coef[2];
///
extern uint32  temp_unit,print_busy,Uart_Printer_Flag;
//
extern uint32  version_811,model_display,LOGO_flag;
//


extern uint8 automode;
extern int   range_man;
extern int   range;
extern float ddlv_raw_data1,ddlv_raw_data2;



extern void    do_factory_call(void);

#endif