/****************************************************************************
* 文件名：int.c
* 功能：有关中断服务的子程序库
****************************************************************************/


#include "config.h"
#include "i2c_com.h"
#include "target.h"
#include "timer.h"
#include "port.h"
#include "key.h"
#include "fan_cal.h"

//#define _DEBUG
#define 	REJECT_COUNT_NUM		10	//工作状态稳定前丢弃的圈数

int16 time_led_flash;
uint16 fan1_phase_sign,fan2_phase_sign,fan3_phase_sign,fan4_phase_sign;			//风扇相位的判别
int16 fan1_counter,fan2_counter,fan3_counter,fan4_counter;						//风扇圈数计数
int16 fan1_counter_old,fan2_counter_old,fan3_counter_old,fan4_counter_old; //在int.c中定义
uint8 frash_100ms_ok,frash_200ms_ok,frash_500ms_ok;
uint8 time_500us,time_2ms,time_10ms,time_100ms,time_500ms,time_1s;

uint8 flag_start_work;											//开始工作标志
uint8 flag_start_count;											//开始计数标志
uint32 count_insp_exp_cycle,count_insp_exp_cycle_read,count_insp_exp_cycle_valid;			//吸呼循环计数器

void Valve_Active_Insp1_Exp2(void);
uint8 Judge_Leap_Year(uint16 yy);

uint8 month,day,hour,minute,second,weekday;
uint16 year;
void Init_RTC(void);
void Read_Data_Time(void);
extern uint8 flag_adjust_date_time;

void Read_Data_Time(void);
//******************************************************
//
//
//
//******************************************************
void Read_Data_Time(void)
{
	uint32 times;
	
//	if(flag_adjust_date_time == TRUE)
//		return;
		
	times = CTIME0;
	weekday = (times >>24) & 0x07;
	hour = (times>>16) & 0x1F;
	minute = (times>>8) & 0x3F;
	second = times & 0x3F;
	
	times = CTIME1;
	year = (times>>16) & 0xFFF;
	month = (times>>8) & 0xF;
	day = times & 0x1F;
}
/****************************************************************************
* 名称：Judge_Leap_Year()
* 功能：判断闰年
* 入口参数：yy 年值
* 出口参数：1 = 闰年；0 = 非闰年
****************************************************************************/
uint8 Judge_Leap_Year(uint16 yy)
{
	if(yy%4 != 0)				//不能被4整除
	{
		return (0);	
	}
	else						//能被4整除
	{
		if(yy%100 != 0)			//不能被100整除
			return (1);
		else if(yy%400 == 0)	//能被100整除,能被400整除
			return (1);
		else					//能被100整除,不能被400整除
			return (0);
	}
}
/****************************************************************************
* 名称：Valve_Action_in_Insp()
* 功能：吸气时的阀门动作
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Valve_Action_in_Insp(void)
{
	IO0CLR = EXP_VALVE;		//关呼出阀
	IO0CLR = INSP_VALVE;	//开进气阀
}

/****************************************************************************
* 名称：Valve_Action_in_Exp()
* 功能：呼气时的阀门动作
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Valve_Action_in_Exp(void)
{
	IO0SET = INSP_VALVE;	//关进气阀
	IO0SET = EXP_VALVE;		//开呼出阀
}
/****************************************************************************
* 名称：Valve_Action_in_Hold()
* 功能：屏气时的阀门动作
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Valve_Action_in_Hold(void)
{
	IO0SET = INSP_VALVE;	//关进气阀
	IO0CLR = EXP_VALVE;		//关呼出阀
}
/****************************************************************************
* 名称：Valve_Active_Insp1_Exp2()
* 功能：吸呼比1:6电磁阀动作
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Valve_Active_Insp1_Exp2(void)
{
	if(flag_start_work == TRUE)
	{
		if(time_500ms < 2)
		{
			Valve_Action_in_Insp();			//1S
		}
		else if(time_500ms < 4)
		{
			Valve_Action_in_Hold();			//1S
			fan1_counter = 0;
			fan2_counter = 0;
			fan3_counter = 0;
			fan4_counter = 0;
		}
		
		else if(time_500ms < 12)
		{
			Valve_Action_in_Exp();			//4S
		}
		else
		{
		//去掉每次开始工作后前5个吸呼周期
	
			time_500ms = 0;
			Valve_Action_in_Insp();			//1S
			count_insp_exp_cycle++;
			if((count_insp_exp_cycle >= REJECT_COUNT_NUM) && flag_start_count == FALSE)
			{
				flag_start_count = TRUE;
				count_insp_exp_cycle = count_insp_exp_cycle_valid;
			}
	
			if((count_insp_exp_cycle != 0) && (flag_start_count == TRUE))
			{	
				Stat_Fan_Circles();
				count_insp_exp_cycle_valid = count_insp_exp_cycle;
			}
			else
			{
			//清呼吸计数标志	
				fan1_counter_old = fan1_counter;
				fan2_counter_old = fan2_counter;
				fan3_counter_old = fan3_counter;
				fan4_counter_old = fan4_counter;
				
				fan1_counter = 0;
				fan2_counter = 0;
				fan3_counter = 0;
				fan4_counter = 0;
			}
		}
	}
	else
	{
		time_500ms = 0;
		flag_start_count = FALSE;
		Valve_Action_in_Hold();
	}
}
/*************************************************************************
* 函数名: IRQ_UART0
* 功能  : 串口0中断处理程序
* 入口  : 无
* 出口  : 无
*************************************************************************/
void IRQ_UART0(void)
{
}
/****************************************************************************
* 名称：IRQ_Timer0()
* 功能：定时器0中断服务程序(500us)
* 入口参数：无
* 出口参数：无
****************************************************************************/
void IRQ_Timer0(void)
{
	T0IR = 0x01;	 	           	//清除T0中断标志
	
	//CPU 工作灯调制
	time_led_flash++;
	if(time_led_flash==350)
	{
		IO0CLR = LED_CPU_S;
	}
	else if(time_led_flash==400)
	{
		time_led_flash = 0;
		IO0SET = LED_CPU_S;
	}
	
	if(++time_500us < 4)
		return;
	time_500us = 0;
	Scan_Key();			//2ms扫描一次按键

	if(++time_2ms < 5)
		return;
	time_2ms = 0;
	time_10ms++;
//	if(time_10ms == 5)
//		frash_100ms_ok = 1;			//50ms
	
	if(time_10ms < 10)
		return;
	time_10ms = 0;
	frash_100ms_ok = 1;				//100ms

	if(++time_100ms < 5)
		return;
	time_100ms = 0;
	time_500ms++;
	frash_500ms_ok = 1;
	
	Read_Data_Time();
}
/****************************************************************************
* IRQ_EINT0()
* 功能：外部中断0服务程序
* 入口参数：无
* 出口参数：无
****************************************************************************/
void IRQ_EINT0(void)
{
	uint16 di;
	
	di = fan1_phase_sign << 8;
	if(IO0PIN & FLUX1_PHASE_B)
	{//B信号高电平
		di |= 0x01;
	}
	if(EXTPOLAR & 0x01)
	{//上升沿触发
		EXTPOLAR &= 0xFE;		//设置下降沿触发
		di |= 0x10;
	}
	else{//下降沿触发
		EXTPOLAR |= 0x01;		//设置上升沿触发
	}
	EXTINT = 0x01;				//清除EINT0中断标志

	fan1_phase_sign = di;
	if(fan1_phase_sign == 0x0110)
	{//AB:下高->上低
		fan1_counter++;			//圈数增1		
	}
	else if(fan1_phase_sign == 0x0011)
	{//AB:下低->上高
		fan1_counter--;			//圈数减1
	}
}
/****************************************************************************
* IRQ_EINT1()
* 功能：外部中断1服务程序
* 入口参数：无
* 出口参数：无
****************************************************************************/
void IRQ_EINT1(void)
{
	uint16 di;
	
	di = fan3_phase_sign << 8;
	if(IO0PIN & FLUX3_PHASE_A)
	{//B信号高电平
		di |= 0x01;
	}
	if(EXTPOLAR & 0x02)
	{//上升沿触发
		EXTPOLAR &= 0xFD;		//设置下降沿触发
		di |= 0x10;
	}
	else{//下降沿触发
		EXTPOLAR |= 0x02;		//设置上升沿触发
	}
	EXTINT = 0x02;				//清除EINT1中断标志

	fan3_phase_sign = di;
	if(fan3_phase_sign == 0x0110)
	{//AB:下高->上低
		fan3_counter--;			//圈数减1		
	}
	else if(fan3_phase_sign == 0x0011)
	{//AB:下低->上高
		fan3_counter++;			//圈数增1
	}
}

/****************************************************************************
* IRQ_EINT2()
* 功能：外部中断2服务程序
* 入口参数：无
* 出口参数：无
****************************************************************************/
void IRQ_EINT2(void)
{
	uint16 di;
	
	di = fan2_phase_sign << 8;
	if(IO0PIN & FLUX2_PHASE_B)
	{//B信号高电平
		di |= 0x01;
	}
	if(EXTPOLAR & 0x04)
	{//上升沿触发
		EXTPOLAR &= 0xFB;		//设置下降沿触发
		di |= 0x10;
	}
	else{//下降沿触发
		EXTPOLAR |= 0x04;		//设置上升沿触发
	}
	EXTINT = 0x04;				//清除EINT2中断标志

	fan2_phase_sign = di;
	if(fan2_phase_sign == 0x0110)
	{//AB:下高->上低
		fan2_counter++;			//圈数增1		
	}
	else if(fan2_phase_sign == 0x0011)
	{//AB:下低->上高
		fan2_counter--;			//圈数减1
	}
}

/****************************************************************************
* IRQ_EINT3()
* 功能：外部中断3服务程序
* 入口参数：无
* 出口参数：无
****************************************************************************/
void IRQ_EINT3(void)
{
	uint16 di;
	
	di = fan4_phase_sign << 8;
	if(IO0PIN & FLUX4_PHASE_B)
	{//B信号高电平
		di |= 0x01;
	}
	if(EXTPOLAR & 0x08)
	{//上升沿触发
		EXTPOLAR &= 0xF7;		//设置下降沿触发
		di |= 0x10;
	}
	else{//下降沿触发
		EXTPOLAR |= 0x08;		//设置上升沿触发
	}
	EXTINT = 0x08;				//清除EINT3中断标志

	fan4_phase_sign = di;
	if(fan4_phase_sign == 0x0110)
	{//AB:下高->上低
		fan4_counter++;			//圈数增1		
	}
	else if(fan4_phase_sign == 0x0011)
	{//AB:下低->上高
		fan4_counter--;			//圈数减1
	}
}

/****************************************************************************
* 名称：FIQ_Exception()
* 功能：快速中断处理程序,用于检测涡轮风扇
* 入口参数：无
* 出口参数：无
****************************************************************************/
void FIQ_Exception(void)
{
}