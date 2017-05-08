//**********************************************************************
//文件名:timer.c
//说明:
//1.看门狗喂狗时要禁止中断,防止中断打断喂狗操作
//函数列表:
//1.初始定时器0:Init_Timer0
//2.定时器0的中断处理:IRQ_Timer0
//3.读取当前时间(分钟计时器):Get_Cur_Time
//4.计算时间差:Cal_Time_Dif
//5 延时几个ms Delay_Nms
//6.喂狗操作:WDT_Feed
//7.初始化看门狗:Init_WDT
//**********************************************************************
#include "config.h"
#include "global.h"
#include "ad_acquire.h"
#include "math.h"
#include "stdio.h"
#include "math.h"
#include "type_def.h"
#include "port.h" 
#include "stdlib.h"
#include "timer.h"
 
#include "ads1100.h" 

extern void IRQ_Timer0_Handler(void);    //中断函数句柄

volatile uint32 one_minute_timer;    //分钟定时器(分辨率:1ms)
volatile uint32 timer_5ms;
volatile uint8  flag_5ms_ok;
volatile uint32 timer_10ms;
volatile uint8  flag_10ms_ok;
volatile uint32 timer_100ms;
volatile uint8  flag_100ms_ok;
volatile uint32 timer_300ms;
volatile uint8  flag_300ms_ok;
volatile uint8  flag_1ms_ok;
volatile uint8  flag_1ms_ok_for_delay;
volatile uint32 timer_500ms;
volatile uint8  flag_500ms_ok;
volatile uint32 timer_1s;
volatile uint8  flag_1s_ok;
volatile uint32 warning_alm_timer;   //警示类报警显示定时器
volatile uint32 tips_alm_timer;       //提示类报警显示定时器



void timer_init(void);

//#define timer_now()                 (T0TC)

uint32 timer_wait(uint32 h)
{
	while( (timer_now() - h) & 0x80000000 );
	return h;
}

//**********************************************************************
//函数  名:Init_Timer0
//函数功能:初始化定时器0,进行定时
//入口参数:us--定时时间(最大1000000)
//出口参数:无
//**********************************************************************
void Init_Timer0(uint32 us)
{   /* Fcclk = Fosc*4 = 11.0592MHz*4 = 44.2368MHz
       Fpclk = Fcclk/4 = 44.2368MHz/4 = 11.0592MHz
    */
    uint8 i;

    
   /* 设置定时器0中断IRQ */
    #if (LCDTYPE == TFTLCD)
    VICIntSelect &= ~(1 << VIC_TIMER0);               // 定时器0中断设为IRQ
    VICVectPri4  =  0x04;                             //irq priority is 8 (0-15)
    VICVectAddr4 =  (uint32)IRQ_Timer0_Handler;       // 设置中断服务程序地址向量 
    VICIntEnable |= (1 << VIC_TIMER0);;                    // 使能定时器0中断
    #else
    VICIntSelect &= ~0x00000010;                      // 定时器0中断设为IRQ
    VICVectCntl1 = 0x24;                              // 定时器0中断通道分配IRQ slot 1
    VICVectAddr1 = (uint32)IRQ_Timer0_Handler;        // 设置中断服务程序地址向量 
    VICIntEnable = 0x00000010;                        // 使能定时器0中断
    #endif 
    if (us > 1000000) return;
    
    T0TCR = 0x02;                                     // 复位T0TC
    T0TCR = 0;   
    T0PC  = 0;                                        // 设置定时器不预分频
    T0MR0 = ((Fpclk / 6250) * us) / 160;              // 比较值(定时值)
    T0MCR = 0x03;                                     // 匹配通道0匹配中断并复位T0TC
    T0CCR               =   0;                        // Capture is disabled.     
    T0EMR               =   0;                        // No external match output.
    T0TCR               =   1;                        // Enable timer 0 
}

void Init_Timer0_New(void)
{  
    uint8 i;
       
    T0TCR = 0x02;   
    T0PR =  Fpclk / TIMER_TICK_PER_SEC - 1;                               // 复位T0TC
    T0PC  = 0;
    T0TC  = 0;                                        // 设置定时器不预分频
    T0MCR = 0;                                     // 匹配通道0匹配中断并复位T0TC
    T0CCR = 0;                        // Capture is disabled.     
    T0CTCR = 0;                        // No external match output.
    T0TCR  = 1;                        // Enable timer 0 
}


//**********************************************************************
//函数  名:IRQ_Timer0
//函数功能:定时器0的中断处理(500us)
//入口参数:无
//出口参数:无
//**********************************************************************

//**********************************************************************
//函数  名:IRQ_Timer0
//函数功能:定时器0的中断处理(500us)
//入口参数:无
//出口参数:无
//**********************************************************************

////////////////////以 125US 作为时间基准

void IRQ_Timer0(void)
{
    
    T0IR = 0xff;                        //清除T0中断标志    
    
   
    //1ms到
    flag_1ms_ok = TRUE;
    flag_1ms_ok_for_delay = TRUE;
    ///////////////////////////////////
        
    one_minute_timer++;        //定时1分钟
    if (one_minute_timer > 60000)
    {
        one_minute_timer = 0;
    }
    
    if (timer_5ms++ >= 5)//5ms到
    {
        timer_5ms = 0;
        flag_5ms_ok = TRUE;
        
     
    } 
     
    timer_10ms++;
    if (timer_10ms < 10) return;
    timer_10ms = 0;        //10ms到
    flag_10ms_ok = TRUE;
    
    ////////////////////////10ms 处理声音
    Key_Sound_Pro();
    Alarm_Sound_Pro();
      
    timer_100ms++;
    
       
    if (timer_100ms < 10) return;
    timer_100ms = 0;    //100ms到
    flag_100ms_ok = TRUE;
    
   
      
    warning_alm_timer++;
    tips_alm_timer++;
    Set_Clr_Alarm_Sound(); //检查报警信息
    //////////////////////////
   
    timer_300ms++;
    if (timer_300ms == 3)
    {
     timer_300ms = 0;
     flag_300ms_ok = TRUE;  //200ms到
   
    }
    
    timer_500ms++;
    if (timer_500ms<5) return;
    timer_500ms = 0;    //500ms到
    flag_500ms_ok = TRUE;
    
    timer_1s++;
    if (timer_1s<2) return;
    timer_1s = 0;    //1s到
    flag_1s_ok = TRUE; 
          
    no_key_time++;

}

//**********************************************************************
//函数  名:Get_Cur_Time
//函数功能:获取当前时间(分钟定时器的时间)
//入口参数:无
//出口参数:时间(ms)
//**********************************************************************
sint32 Get_Cur_Time(void)
{
    return (one_minute_timer);
}

//**********************************************************************
//函数  名:Cal_Time_Dif
//函数功能:计算时间差(单位:ms),以one_minute_timer为时钟
//入口参数:start_time--计时的起始时间点(ms)
//出口参数:时间差(ms)
//**********************************************************************
sint32 Cal_Time_Dif(sint32 start_time)
{
    if (one_minute_timer >= start_time)
    {
        return (one_minute_timer-start_time);
    }
    else
    {//超过1分钟
        return(60000-start_time+one_minute_timer);
    }
}

//**********************************************************************
//函数  名:Delay_Nms
//函数功能:延时N ms
//入口参数:time--延时时间(单位:ms)
//出口参数:无
//**********************************************************************
void Delay_Nms(uint32 time)
{
    while (time)
    {
        if (flag_1ms_ok_for_delay)
        {
            flag_1ms_ok_for_delay = FALSE;
            time--;
        }
    }
}

/****************************************************************************
* 名称：WDT_Feed()
* 功能：看门狗喂狗操作
* 入口参数：无
* 出口参数：无
****************************************************************************/
void WDT_Feed(void)
{
uint32 int_bak;
    int_bak = VICIntEnable;        //读中断使能状态
    VICIntEnClr = int_bak;        //禁中
    WDFEED = 0xAA;                //喂狗
    WDFEED = 0x55;
    VICIntEnable = int_bak;        //允中
}

/****************************************************************************
* 名称：Init_WDT()
* 功能：看门狗初始化
* 入口参数：最大喂狗间隔(单位:ms, 最大10000)
* 出口参数：无
****************************************************************************/
void Init_WDT(uint16 wdt_time)
{
    WDTC = (Fpclk / 4000) * wdt_time;    // 设置WDT定时器
    WDCLKSEL = 0x01;                     //选择APB CLK
    WDMOD = 0x03;                        // 设置并启动WDT
}
/////////////////////////////////////////////////////
