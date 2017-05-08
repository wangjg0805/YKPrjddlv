/*****************************************************************************
 *   rtc.c:  Realtime clock C file for NXP LPC23xx/34xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.12  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include  "iolpc2468.h"

#include  "target.h"
#include "type_def.h"
#include "rtc.h"
#include "Calendar.h"

extern void IRQ_RTC_Handler(void);    //中断函数

extern uint32  elapsed_time;
static RTCTime  Global_Time;
       uint32   Sec_Change_Flag;
/*********************************************************************************
 *   Function name:    RTCHandler
 *   Describtion: the Tick initializtion of Rtc Clock for LPC2378
 *   Author:      Zhangwei
 *   Data:        2007.11.26
 *   Parameters:  NULL
 *   Return :     NULL
 *   History:
 *           2007.11.26 Prelimnary version,first Release
**********************************************************************************/
void IRQ_RTC(void)
{
  
	Sec_Change_Flag = TRUE;
	elapsed_time++;
	RTC_ILR |= ILR_RTCCIF;		/* clear interrupt flag */
    
}
/*****************************************************************************
** Function name:		RTCInit
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCInit( void )
{
  static RTCTime local_time;
  Sec_Change_Flag = FALSE;
  PCONP              |=  (1 << 9); 
  /*--- Initialize registers ---*/    
  RTC_AMR = 0xff;                 //ALARM MASK REGISTER
  RTC_CIIR = 0x1;                 //counter inc int register
  RTC_CISS &= ~(0x01 << 7);	      //disable Subsecond int
  RTC_CCR = 0x10;                 //使用外部 32.768K
  //RTC_PREINT  = BSP_CPU_PclkFreq(PCLK_RTC)/32768 - 1;
  //RTC_PREFRAC = BSP_CPU_PclkFreq(PCLK_RTC)%32768;
                      
  local_time = RTCGetTime();
  if((local_time.RTC_Year < CALENDAR_START_YEAR) ||(local_time.RTC_Year >= CALENDAR_END_YEAR))
	{
        local_time.RTC_Sec = 0;
        local_time.RTC_Min = 0;
        local_time.RTC_Hour = 0;
        local_time.RTC_Day = 1;
        local_time.RTC_Mon = 1;
        local_time.RTC_Year = 2009;
        local_time.RTC_Wday = 4;
        local_time.RTC_Yday = 1;		/* current date 07/12/2006 */
       
        RTCSetTime( local_time );		/* Set local time */
	  }
    VICIntSelect       &= ~(1 << VIC_RTC);                   /* Configure the timer interrupt as an IRQ source           */
    VICVectCntl13        =  13;
    VICVectAddr13       =  (uint32)IRQ_RTC_Handler;              /* Set the vector address                                   */
    VICIntEnable       |=  (1 << VIC_RTC);                   /* Enable the timer interrupt source    */
  //RTCSetAlarmMask(AMRSEC|AMRMIN|AMRHOUR|AMRDOM|AMRDOW|AMRDOY|AMRMON|AMRYEAR);
    RTCStart();
  //RTC_CCR = RTC_CCR & ~(0x1f) | (1 << 4) | (0 << 1) | 0;//32K oscillator,CTC No Reset,Clock Disable.
  //return;
}

/*****************************************************************************
** Function name:		RTCStart
**
** Descriptions:		Start RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStart( void ) 
{
  /*--- Start RTC counters ---*/
  RTC_CCR = CCR_CLKSRC|CCR_CLKEN;
  //RTC_ILR = ILR_RTCCIF;
  //return;
}

/*****************************************************************************
** Function name:		RTCStop
**
** Descriptions:		Stop RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStop( void )
{   
  /*--- Stop RTC counters ---*/
  RTC_CCR &= ~CCR_CLKEN;
  return;
} 

/*****************************************************************************
** Function name:		RTC_CTCReset
**
** Descriptions:		Reset RTC clock tick counter
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_CTCReset( void )
{   
  /*--- Reset CTC ---*/
  RTC_CCR |= CCR_CTCRST;
  return;
}

/*****************************************************************************
** Function name:		RTCSetTime
**
** Descriptions:		Setup RTC timer value
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetTime( RTCTime Time ) 
{

  RTC_SEC   = Time.RTC_Sec;
  RTC_MIN   = Time.RTC_Min;
  RTC_HOUR  = Time.RTC_Hour;
  RTC_DOM   = Time.RTC_Day;
  RTC_MONTH = Time.RTC_Mon;
  RTC_YEAR  = Time.RTC_Year; 
  RTC_DOW   = Time.RTC_Wday;
  RTC_DOY   = Time.RTC_Yday;
}

/*****************************************************************************
** Function name:		RTCSetAlarm
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarm( RTCTime Alarm ) 
{   

    RTC_ALSEC   = Alarm.RTC_Sec;
    RTC_ALMIN   = Alarm.RTC_Min;
    RTC_ALHOUR  = Alarm.RTC_Hour;
    RTC_ALDOM   = Alarm.RTC_Day;
    RTC_ALDOW   = Alarm.RTC_Wday;
    RTC_ALDOY   = Alarm.RTC_Yday;
    RTC_ALMON   = Alarm.RTC_Mon;
    RTC_ALYEAR  = Alarm.RTC_Year;  
   
    return;
}

/*****************************************************************************
** Function name:		RTCGetTime
**
** Descriptions:		Get RTC timer value
**
** parameters:			None
** Returned value:		The data structure of the RTC time table
** 
*****************************************************************************/
RTCTime RTCGetTime( void ) 
{
    RTCTime LocalTime;   
    LocalTime.RTC_Sec   = RTC_SEC;
    LocalTime.RTC_Min   = RTC_MIN;
    LocalTime.RTC_Hour  = RTC_HOUR;
    LocalTime.RTC_Day   = RTC_DOM;
    LocalTime.RTC_Wday  = RTC_DOW;
    LocalTime.RTC_Yday  = RTC_DOY;
    LocalTime.RTC_Mon   = RTC_MONTH;
    LocalTime.RTC_Year  = RTC_YEAR;
    return ( LocalTime );    
}

/*****************************************************************************
** Function name:		RTCSetAlarmMask
**
** Descriptions:		Set RTC timer alarm mask
**
** parameters:			Alarm mask setting
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarmMask( unsigned long AlarmMask ) 
{
  /*--- Set alarm mask ---*/    
  RTC_AMR = AlarmMask;
  return;
}

/*****************************************************************************
**                            End Of File
******************************************************************************/