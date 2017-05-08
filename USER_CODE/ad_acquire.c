//**********************************************************************
//文件名:ad_acquire.c
//函数列表:
//1.初始化定时器1:         Init_Timer1  本程序没有使用
//2.初次化外部中断2:       Init_EINT2
//3.CPLD 原始数据累加函数  epm7064s_filter_raw
//4.快速中断处理:          FIQ_Exception(读取原始数据)
//5.返回累加后的原始数据:  Get_Raw
//**********************************************************************
#include "ad_acquire.h"
#include "config.h"
#include "port.h"
#include "global.h"
#include "type_def.h"
#include "stdlib.h"
#include "filter.h"

#include "ds18b20.h"
#include "AD7190_IO.h"

#define   SAMPLE_CH_SIZE      4

static  uint32 raw[SAMPLE_CH_SIZE];
static  uint32 ddlv_index=0;

static  uint32 raw_ddlv[64];

//**********************************************************************
//函数名称:Init_EINT2()
//函数功能:初始化外部中断2,用于检测涡轮风扇
//入口参数:无
//出口参数:无
//**********************************************************************
void Init_EINT2(void)
{
  
    EXTMODE = 0x04;                  //设置EINT2中断为边沿触发
    EXTPOLAR = 0x00;                 //低电平或者下升沿触发
   
    EXTINT = 0x04;                   //清除EINT2中断标志
    VICIntSelect |= 0x00010000;      //EINT2中断设为FIQ
    VICVectAddr16 = (uint32)FIQ_Exception;
    VICVectPri16 = 0x0;
    VICIntEnable |= 0x00010000;     //使能EINT2中断*/

}

////////////////////////////////////////////////////////////
void  AD7190_Channel_Switch(void)
   {
     
     if((TRUE ==point2_cal_start)&&(STAT_SYS_CAL == current_mode))
     {//校准状态 按需切换
      if(1 == switch_flag)
       {
        switch_flag = 0;
        switch(point2_cal_step)
        {
         case SYS_CAL_ZER0A: select_ch=3;AD7190Configuration(4);break;
         case SYS_CAL_ZER0B: select_ch=4;AD7190Configuration(8);break;
         case SYS_CAL_LOW1A: select_ch=1;AD7190Configuration(1);break;
         case SYS_CAL_LOW1B: select_ch=4;AD7190Configuration(8);break;
         case SYS_CAL_LOW2A: select_ch=2;AD7190Configuration(2);break;
         case SYS_CAL_LOW2B: select_ch=3;AD7190Configuration(4);break;
         case SYS_CAL_FULL1: select_ch=1;AD7190Configuration(1);break;
         case SYS_CAL_FULL2: select_ch=2;AD7190Configuration(2);break;
         //case SYS_CAL_FULL3: select_ch=2;AD7190Configuration(2);break;
         default:break;
        }
       } 
     }
    else if(0 == sys_cal_ok)
      {//没有校准的情况下 默认使用CH1
       if(1!=select_ch) 
        {
         select_ch = 1;
         AD7190Configuration(1);
        }
      }
    else 
      { //正常切换
      
        if(cpu_ch_adc4 >= low1_data[3]) //与校准300mv时候的数据比较                                   //use +high
          {
            if(1!=select_ch)     //>300mv
              {
               select_ch = 1;
               AD7190Configuration(1);
              } 
          } 
        else if(cpu_ch_adc4 >= (zero_data[3]+5))
          {   
           if(4!=select_ch)      //0-300mv
              {
               select_ch = 4;
               AD7190Configuration(8);
              } 
          }
        else if(cpu_ch_adc3 >= low2_data[2]) //与校准-300mv时候的数据比较                                   //use +high
          {
            if(2!=select_ch)      // < -300mv
              {
               select_ch = 2;
               AD7190Configuration(2);
              } 
          }    
           
        else if((cpu_ch_adc3 >= zero_data[2]+20))
         {                        //-300mv-0v
           if(3!=select_ch)
             {
              select_ch = 3;
              AD7190Configuration(4);
             } 
         
         }
        else
        { //在0MV 附近 默认使用 
          if(4!=select_ch)      //0 附近
              {
               select_ch = 4;
               AD7190Configuration(8);
              } 
        }      
      
    }  
 }


//**********************************************************************
//函数名称:FIQ_Exception()
//函数功能:快速中断处理程序,读取AD7190的数据
//**********************************************************************
////////////////////////////////       
void FIQ_Exception(void)
{ 
    uint32 i;
    uint8  buf[4];
    static uint32 Cycle_times=0;
    
    AD7190_DOUT_DAT_MODE;
    ///////////////////////////////////////////
    AD7190_DIN_L;  //when reading DIN must be 0
    ReadFromAD7190(4,buf);		//read data register

    i = (buf[3]<<16)+(buf[2]<<8)+buf[1];
    i>>=4;
    #if SDJ_SOFT
    switch(buf[0]&0x07)
     {
      case 4: raw[0] = i; flag_new_data = 1;break;
      case 5: raw[1] = i; flag_new_data = 1;break;
      case 6: raw[2] = i; flag_new_data = 1;break;
      case 7: raw[3] = i; flag_new_data = 1;break;
      default:            flag_new_data = 0;break;
     }
   
    #endif 
   
    //根据LPC2478AD CH3 CH4 的数值切换不同的通道 此操作放在主循环中进行
    if(MACHINE_NORMAL_MODE == machine_mode)
    AD7190_Channel_Switch();
    ///////////////////////////////////////////////////  
    AD7190_DOUT_INT_MODE;
    EXTINT = 0x04;                //清除EINT0中断标志
}
/////////////////////////////////////////////////////
///////////////////////////////////////////////////// 
 uint32 Get_Raw_From_Int2(uint8 ch)
 {
  //return((raw[SAMPLE_BUF_SIZE-1]+raw[SAMPLE_BUF_SIZE-2])/2);
  return(raw[ch]);
 }