//**********************************************************************
//文件名:normal_oprarion.c
//说  明:1.工作模式下的菜单管理
//       2.工作模式下的功能调度
//函数说明
//1   
//2  
//3   
//4   
//5   
//6  
//7   
//8  
//9   
//10 
//**********************************************************************
#include "stdio.h"
#include "stdlib.h"

#include "eeprom.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "graph.h"
#include "global.h"
#include "i2c_com.h"
#include "normal_operation.h"
#include "timer.h"
#include "type_def.h"
#include "menu.h"
#include "screencal.H"
#include "filter.h"
#include "mainmenu2.h"
#include "ddlvmenu.h"

#include "datecfg.h"
#include "port.h"
#include "disp.h"
#include "lcd2478.h"
#include "rtc.h"
#include "Calendar.h"
#include "lpc2478_uart.h"
#include "alarm_pro.h"

#include "ads1100.h"
//function1 
static void   Stable_Status_Display(uint32 x,uint32 y);
static void   Negative_Status_Display(void);

static void   New_Data_Display(void);
static void   New_Data_Display2(void);

static void   display_sys_caling_data(void);
static void   display_sys_caling_data2(void);

static void   display_user_caling_data(void);
static void   display_user_caling_data2(void);

static void   display_measure_data(void);
static void   display_measure2_data(void);

static sint32   menu_op;
//variable
//
static  uint32  match_flag,match_point;

//////////////////////////////////////////////
//11 系统校准 过程的 默认AD 参数
//    
//////////////////////////////////////////////////////////////
const uint32 DEFAULT_CAL_DATA[6][2] = 
  {  
     {6950,   736},  //     5            50
     {8032,   780},  //    50           500
     {7360,   730},  //   500          5000
     
     {7140,   650},  //  5000         50000
     {6750,   600},  // 50000        500000
     {7290,   650}   //500000       5000000  
  };


//////////////////////////////////////////////////////////
//01 进入用户校准状态
//////////////////////////////////////////////////////////
void Enter_UserPoint_Cal(void)
   {
    if(FALSE == point2_cal_start)
     {
      current_mode = STAT_USER_CAL;
      point2_cal_start = TRUE;
      point2_cal_step = USER_CAL_STEP1;
      match_flag = FALSE;
      switch_flag=1;
     }
   }
   
//////////////////////////////////////////////////////////
//01 显示当前用户使用校准溶液的状态
//
//////////////////////////////////////////////////////////
void   Display_cal_liquid_area(void)
    {
     Clr_Bar(620, 140, 800-620-10,160,COL_DEEPGRAY);
     
     Set_ASC_Lib(Arial12B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     switch(user_cal_num)
       {
        case 1:
                Put_Bmp(630+50,  150,  liquidNull5566,0);
                Put_Data_Float(690, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                break;
        case 2:
                Put_Bmp(630+15,  150,  liquidNull5566,0); 
                Put_Bmp(630+70,  150,  liquidNull5566,0);    
                Put_Data_Float(655,    190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                Put_Data_Float(630+80, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]); 
                break;
        case 3:
                Put_Bmp(620,      150,  liquidNull5566,0); 
                Put_Bmp(630+45,   150,  liquidNull5566,0); 
                Put_Bmp(630+100,  150,  liquidNull5566,0);  
                Put_Data_Float(630,     190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]);
                Put_Data_Float(640+45,  190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]);
                Put_Data_Float(640+100, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]);
                break;
    
        case 4:
                Put_Bmp(630+15,  150,  liquidNull5566,0); 
                Put_Bmp(630+70,  150,  liquidNull5566,0);    
                Put_Bmp(630+15,  220,  liquidNull5566,0); 
                Put_Bmp(630+70,  220,  liquidNull5566,0);    
               
                Put_Data_Float(655,    190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]); 
                Put_Data_Float(630+80, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]); 
                Put_Data_Float(655,    260,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]); 
                Put_Data_Float(630+80, 260,"%.2f", standard_data_p811[which_standard][user_cal_point[3]]); 
                break;
    
        case 5:
                Put_Bmp(620,      150,  liquidNull5566,0); 
                Put_Bmp(630+45,   150,  liquidNull5566,0); 
                Put_Bmp(630+100,  150,  liquidNull5566,0);  
                Put_Data_Float(630,     190,"%.2f", standard_data_p811[which_standard][user_cal_point[0]]);
                Put_Data_Float(640+45,  190,"%.2f", standard_data_p811[which_standard][user_cal_point[1]]);
                Put_Data_Float(640+100, 190,"%.2f", standard_data_p811[which_standard][user_cal_point[2]]);
                
                Put_Bmp(620,      220,  liquidNull5566,0); 
                Put_Bmp(630+45,   220,  liquidNull5566,0); 
                Put_Data_Float(630,     260,"%.2f", standard_data_p811[which_standard][user_cal_point[3]]);
                Put_Data_Float(640+40,  260,"%.2f", standard_data_p811[which_standard][user_cal_point[4]]);
                break;
                
        default:break;
       }
    }
    
//////////////////////////////////////////////////////////
//03 显示当前用户电极的状态
//
//////////////////////////////////////////////////////////
void   Display_elec_class_area(void)
    {
     uint32 i;
     //水平分割线
     Clr_Bar(600,   300,  SCR_XSIZE-600,   3, COL_THINCYAN);  //分割线
     
     Clr_Bar(610,   345,  SCR_XSIZE-610-10, 50, COL_DEEPGRAY);  //分割线
     
     for(i=0;i<electrode_class;i++)
     Put_Bmp(620+33*i, 355,  fivestar3030,0); 
    
    }   
////////////////////////////////////////////////////////
//04 在指定位置用指定颜色显示 各种工作状态
//   同时刷新小的显示区域
///////////////////////////////////////////////////////
static  void  Mode_Status_Display(void)
 {
 
  switch(current_mode)
  {
   case STAT_USER_CAL:
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  
                  Clr_Bar(65,100, 150,  40,COL_THINGRAY);
                  Clr_Bar(290,100, 210, 40,COL_DEEPGRAY);
                  if(Get_Language_Type() == CHINESE)
                  Set_ASC_Lib(GulimChe48A_Font);
                  else
                  Set_ASC_Lib(Arial16B_Font);
                  
                  Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY); 
                  Put_Line_Str(300, 110,Language_Str("仪器匹配的溶液值","THE STANDARD PH"));
                  
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
                  switch(which_standard)
                  {
                   case CHINESE_S:  Put_Line_Str(90, 110,Language_Str("国标校准","CH-S CAL"));break;
                   case EUROPE_S:   Put_Line_Str(90, 110,Language_Str("欧标校准","EU-S CAL"));break;
                   case AMERICAN_S: Put_Line_Str(90, 110,Language_Str("美标校准","AM-S CAL"));break;
                   default:break;
                  }
                  //显示PH值
                  Clr_Bar( 15,160, 250, 70,COL_THINGRAY);
                  Clr_Bar(265,160, 250, 70,COL_DEEPGRAY);
                  //
                  //Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
                  
                  Put_Line_Str(Language_Pos(40,60),  Language_Pos(230,255),Language_Str("第1种","1st"));
                  Put_Line_Str(Language_Pos(150,150),Language_Pos(230,255),Language_Str("PH","PH")); 
                 
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   				  Put_Line_Str(320, 330,Language_Str("mV","mV")); 
    			  Put_Char(560, 345,'c');
    			  
    			  ///////////////////////增加两个按键
    			  Put_Button_Bar(300,240,300+110, 240+50,3,COL_THINGREEN);//继续
                  Put_Button_Bar(430,240,430+110, 240+50,3,COL_THINRED);  //结束
    			  Set_Chr_Color(COL_BLACK,COL_THINGREEN);
    			  Set_ASC_Lib(Arial16B_Font);
    			  Put_Line_Str(Language_Pos(330, 330), 255,Language_Str("保存","SAVE"));
   				  Set_Chr_Color(COL_BLACK,COL_THINRED);
   				  Put_Line_Str(Language_Pos(460, 460), 255,Language_Str("结束","ABORT"));      
                  break;                      
  
   case STAT_SYS_CAL:
   case STAT_NORMAL: 
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
    			  Put_Line_Str(525, 200,Language_Str("PH","PH"));  
    		      Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   				  Put_Line_Str(320, 330,Language_Str("mV","mV"));
   				  if(TEMP_UNIT_OC == temp_unit) 
    			  Put_Char(560, 345,'c');    
                  else
                  Put_Char(560, 345,'f');
                  break;   
  
  default:
        break;
  }
  ////////////////////////////////////
  Stable_Status_Display(550,150);
 } 
///////////////////////////////////////////////////////////////
//电导率的界面
////////////////////////////////////////////////////////
//04A 在指定位置用指定颜色显示 各种工作状态
//    同时刷新小的显示区域
///////////////////////////////////////////////////////
static  void  Mode_Status_Display2(void)
 {
 
  switch(current_mode)
  {
   case STAT_USER_CAL: //电导率用户校准 界面显示
                  Clr_Bar(5, 90, 800-200-10,200,COL_BLACK);
                  
                  Clr_Bar(310,100, 210, 40,COL_DEEPGRAY);
                  
                  if(Get_Language_Type() == CHINESE)
                  Set_ASC_Lib(GulimChe48A_Font);
                  else
                  Set_ASC_Lib(Arial16B_Font);
                  
                  Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY); 
                  Put_Line_Str(320, 110,Language_Str("仪器匹配的数值","THE MATCH DATA"));
                  
                  /*  不显示使用的是哪种标准
                  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
                  switch(which_standard)
                  {
                   case CHINESE_S:  Put_Line_Str(90, 110,Language_Str("国标校准","CH-S CAL"));break;
                   case EUROPE_S:   Put_Line_Str(90, 110,Language_Str("欧标校准","EU-S CAL"));break;
                   case AMERICAN_S: Put_Line_Str(90, 110,Language_Str("美标校准","AM-S CAL"));break;
                   default:break;
                  }
                  //显示PH值
                  Clr_Bar( 15,160, 250, 70,COL_THINGRAY);
                  Clr_Bar(265,160, 250, 70,COL_DEEPGRAY);
                  //
                  Set_ASC_Lib(GulimChe48_Font);
                  Set_Chr_Color(COL_WHITE,COL_BLACK);
                  
                  Put_Line_Str(40, 240,Language_Str("第1种","第1种"));
                  Put_Line_Str(150, 240,Language_Str("PH:","PH:")); 
                  */
                  
                  ///////////////////////增加两个按键
    			  Put_Button_Bar(300,240,300+110, 240+50,3,COL_THINGREEN);//继续
                  Put_Button_Bar(430,240,430+110, 240+50,3,COL_THINRED);  //结束
    			  Set_Chr_Color(COL_BLACK,COL_THINGREEN);
    			  Put_Line_Str(Language_Pos(330, 330), 255,Language_Str("保存","SAVE"));
   				  Set_Chr_Color(COL_BLACK,COL_THINRED);
   				  Put_Line_Str(Language_Pos(460, 460), 255,Language_Str("结束","ABORT"));      
                  break;                      
  
   case STAT_SYS_CAL:
   case STAT_NORMAL: 
                  Clr_Bar(5, 90, 800-200-10+5,200,COL_BLACK);
                  //显示测量模式 
                  Clr_Bar(470, 90, 115,30,COL_WHITE);
                  Set_Chr_Color(COL_DEEPBLUE,COL_WHITE);
                  Set_ASC_Lib(Arial16B_Font);
   			      switch(DDLV_Mode)
                    {
                     case MODE_TIMER:
                     	  Put_Line_Str(Language_Pos(480, 500), 95,Language_Str("定时测量","TIMER"));  
                          
                          break;
                     case MODE_BALANCE:
                          Put_Line_Str(Language_Pos(480, 490), 95,Language_Str("平衡测量","BALANCE"));  
                          Set_ASC_Lib(Arial16B_Font);
                          Clr_Bar(505, 160, 80,30,COL_WHITE);
                          Put_Data_Float(510,165,"%06.2f",balance_data); 
                          break;
                     case MODE_CONTINUE:
                     default:
                      	  Put_Line_Str(Language_Pos(480, 480), 95,Language_Str("连续测量","CONTINUE")); 
                      	  break;              
                    }
                  break;
   default:       break;               
  } 
  
         /////////////////////////display oc
    	  Set_ASC_Lib(GulimChe48_Font);
    	  //Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    	  Set_Chr_Color(COL_WHITE,COL_BLACK);
    	  
    	  if(TEMP_UNIT_OC == temp_unit)
    	  Put_Char(560-350, 345,'c');
    	  else
    	  Put_Char(560-350, 345,'f');    
  
          ////////////////////////////////////
          Stable_Status_Display(50,130);
 }  
 
////////////////////////////////////////////////////////////////
//**********************************************************************
//05A 函数 名:Display_Time_Pro()
//函数功能:   时间显示函数
//入口参数:无
//出口参数:无
//**********************************************************************
void  Display_Time_Pro(RTCTime local_time_buf)
    {
     /////////////////////////////////////////////////////////////*/ 
     Set_ASC_Lib(Arial20B_Font);
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
    
     Put_Data(620,           15, "%04d", local_time_buf.RTC_Year);
	 Put_Char(690,           15, 0x2d);
	 Put_Data(705,           15, "%02d", local_time_buf.RTC_Mon);
	 Put_Char(740,           15, 0x2d);
	 Put_Data(750,           15, "%02d", local_time_buf.RTC_Day);
     //display time
	 Put_Data(653,           45,"%02d", local_time_buf.RTC_Hour);
	 Put_Char(690,           45,0x3a);
     Put_Data(705,           45,"%02d", local_time_buf.RTC_Min);
	 Put_Char(740,           45,0x3a);
	 Put_Data(750,           45,"%02d", local_time_buf.RTC_Sec);
    }

///////////////////////////////////////////////////////////////////////
//**********************************************************************
//05 函数 名:Oper_Main_Assi_Pro
//函数功能:主工作界面的辅助处理函数  每300ms执行一次
//入口参数:无
//出口参数:无
//**********************************************************************
static void Oper_Main_Assi_Pro(void)
{
    RTCTime local_time_buf;
    uint16   i;
    ///////////////////
    if(Sec_Change_Flag) {
        Sec_Change_Flag = FALSE;
        Display_Time_Pro(RTCGetTime());
        /////////////////////////////////////每秒定时参数减1
        if(MODE_TIMER == DDLV_Mode) {
            if(measure_timer_sec_real==0) {
                measure_timer_sec_real = measure_timer_sec;
                //save
                Save_RecDDLV();
                //自动返回正常模式
                DDLV_Mode = MODE_CONTINUE; 
            } else
                measure_timer_sec_real--;
         //////////////////////////////////
        Set_ASC_Lib(Arial16B_Font);
        Clr_Bar(535, 125, 50,30,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Put_Data(545,130,"%03d",measure_timer_sec_real); 
       }
    else if (MODE_BALANCE == DDLV_Mode) { //当前测量数据在设置数据范围内，时间才可以减1
        if(ddlv_data == balance_data) {
             if(0 == balance_sec_real) {
               balance_sec_real = balance_sec;
               //save
               Save_RecDDLV();
               //自动返回正常模式
               DDLV_Mode = MODE_CONTINUE;
              } else
                  balance_sec_real--;
        } else
            balance_sec_real = balance_sec;
            
        Set_ASC_Lib(Arial16B_Font);
        Clr_Bar(535, 125, 50,30,COL_WHITE);
        Set_Chr_Color(COL_BLACK,COL_WHITE);
        Put_Data(545,130,"%03d",balance_sec_real);     
    }
        
    //以下信息 显示温度   
    Set_Graphic_Color(COL_WHITE,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Fill_Circle(570-350,330,15);
	
	
	Set_ASC_Lib(Arial16B_Font);	
	if(temp_float_ptc==TEMP_SENSOR_NC_VALUE) {
	    manual_temp_comp_flag = TRUE; 
	    Put_Char(565-350,320,'M');
	    Set_ASC_Lib(GulimChe48A_Font);
	    Set_Chr_Color(COL_WHITE,COL_BLACK);
	    if(TEMP_UNIT_OC == temp_unit)
	        Put_Data_Float(420-365,320,"%4.1f",(float)temp_comp_manual); //此处是手动输入的补偿温度
	    else
	        Put_Data(430-365,320,"%3d",(uint32)(temp_comp_manual*9/5+32.5));
	  ////////////////////////////////////////////////////
	  /*
	  if((temp_comp_manual<TEMP_SENSOR_LOW)||(temp_comp_manual>TEMP_SENSOR_HIGH))
	     Log_Alm(&warn_temp_out,ALM_LEV_WARN);
	  else
	     Del_Alm(&warn_temp_out,ALM_LEV_WARN);
	  */
    } else {
	    manual_temp_comp_flag = FALSE;
	    Put_Char(565-350,320,'A');
	    Set_ASC_Lib(GulimChe48A_Font);
	    Set_Chr_Color(COL_WHITE,COL_BLACK);
	    //Put_Data_Float(420,320,"%4.1f",23.4);
	    if(TEMP_UNIT_OC == temp_unit)
	        Put_Data_Float(420-365,320,"%4.1f",temp_float_ptc);
	    else
	        Put_Data(430-365,320,"%3d",(uint32)(temp_float_ptc*9/5+32.5));
	  ////////////////////////////////////////////////////
	  /*
	  if((temp_float_ptc<TEMP_SENSOR_LOW)||(temp_float_ptc>TEMP_SENSOR_HIGH))
	     Log_Alm(&warn_temp_out,ALM_LEV_WARN);
	  else
	     Del_Alm(&warn_temp_out,ALM_LEV_WARN);
      */
   }
   //add uart
   //if(0==Uart_Printer_Flag)
       UART_SendData();
   }
   Put_Alm_Mess();
}

///////////////////////////////////////////////////////////////////////
//**********************************************************************
//06  函数 名:Oper_Assi_Pro 只显示时间
//函数功能:   其它工作界面的辅助处理函数  每300ms执行一次
//入口参数:无
//出口参数:无
//**********************************************************************
static void Oper_Assi_Pro(void)
{
  ///////////////////
  if(Sec_Change_Flag)
   {
     Sec_Change_Flag = FALSE;
     Display_Time_Pro(RTCGetTime());
   }  
}
///////////////////////////////////////////////////////////
//06A   空函数
//
///////////////////////////////////////////////////////////
static void Oper_Assi_Null(void)
{
  ///////////////////
 
}

////////////////////////////////////////////////////
//07  待机状态下的按键判断
//
///////////////////////////////////////////////////
static uint32  Action_SleepCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<1;i++)
	    {
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	     //////////////////////////////////////////////////////////有按键按下
	  if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	     
	  if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER; 
	      Key_Valid_Sound();
	     }
	      
	  return(Key_Code);   
	 }
/*///////////////////////////////////////////////////
/*08  主界面下的键盘判断 共有10+2+4个按键
//
//////////////////////////////////////////////////*/
static uint32  Action_MainCfg(uint16 x,uint16 y)
	{
	   uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	 
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   
	  //////////////////////////////////////////////////////////有按键按下
	  if(Key_Status == KEY_PRESS) {
	      if(Get_Language_Type() == CHINESE) {
	          switch(Key_Code)
	          {
	           case 1: Put_Bmp(133*0+15,  405,  sysconfig8055,1); break;
               case 2: Put_Bmp(133*1+15,  405,     recall8055,1); break;
               case 3: Put_Bmp(133*2+15,  405,        cal8055,1); break;
               case 4: Put_Bmp(133*3+15,  405,       save8055,1); break;
               case 5: Put_Bmp(133*4+15,  405,    printer8055,1); break;
               case 6: Put_Bmp(133*5+15,  405,      sleep8055,1); break; 
               default:break;
              } 
          } else {
              switch(Key_Code)
	          {
	           case 1: Put_Bmp(133*0+15,  405,  sysconfigen8055,1); break;
               case 2: Put_Bmp(133*1+15,  405,     recallen8055,1); break;
               case 3: Put_Bmp(133*2+15,  405,        calen8055,1); break;
               case 4: Put_Bmp(133*3+15,  405,       saveen8055,1); break;
               case 5: Put_Bmp(133*4+15,  405,    printeren8055,1); break;
               case 6: Put_Bmp(133*5+15,  405,        sleep8055,1); break; 
               default:break;
              }  
          }
          Key_Code = 0xAA55;
          
      } //增加反白显示 直到按键松开 
	   
	   if(Key_Status == KEY_DO_START) {
	    
	       Key_Status = KEY_DO_OVER; 
	       Key_Valid_Sound();
	       if(Get_Language_Type() == CHINESE) {
	           switch(Key_Code)
	           {
	            case 1: Put_Bmp(133*0+15,  405,  sysconfig8055,0); break;
                case 2: Put_Bmp(133*1+15,  405,     recall8055,0); break;
                case 3: Put_Bmp(133*2+15,  405,        cal8055,0); break;
                case 4: Put_Bmp(133*3+15,  405,       save8055,0); break;
                case 5: Put_Bmp(133*4+15,  405,    printer8055,0); break;
                case 6: Put_Bmp(133*5+15,  405,      sleep8055,0); break; 
                default:break;
               }
          } else {
              switch(Key_Code)
	          {
	           case 1: Put_Bmp(133*0+15,  405,  sysconfigen8055,0); break;
               case 2: Put_Bmp(133*1+15,  405,     recallen8055,0); break;
               case 3: Put_Bmp(133*2+15,  405,        calen8055,0); break;
               case 4: Put_Bmp(133*3+15,  405,       saveen8055,0); break;
               case 5: Put_Bmp(133*4+15,  405,    printeren8055,0); break;
               case 6: Put_Bmp(133*5+15,  405,      sleep8055,0); break; 
               default:break;
              }  
         }
         return(Key_Code);    
     }
     
     return(Key_Code);   
}

//////////////////////////////////////////////////////
//09  强制结束校准状态
//    并返回到正常模式
//////////////////////////////////////////////////////
void  Return_Normal_Status(void)
    {
     point2_cal_start = FALSE;
     current_mode     = STAT_NORMAL;
     // update display
     
    Set_Chr_Color(COL_THINYELLOW,COL_BLACK);
    Set_ASC_Lib(ARIALN26B);
    Clr_Bar(640,155,120,50,COL_BLACK);
    Clr_Bar(640,300,120,50,COL_BLACK);
    
    Put_Data_Float(660,170,"%6.3f",elec_cons);
    Put_Data_Float(660,315,"%6.3f",TDS_cons);
    
    }
    
    
void  Clear_User_Cal(void)
  {
   uint8 eeprom_tx_buf[16];
         
         eeprom_tx_buf[0] = 0x0;  //擦除用户校准数据
         eeprom_tx_buf[1] = 0x0;
  	     eeprom_tx_buf[2] = 0x0;
  	     eeprom_tx_buf[3] = 0x0; 
  	     #if SDJ_SOFT
  	     I2C_Send_Str(SYS24C08, EEP_USER_CAL_FLAG,     eeprom_tx_buf, 4);
  	     #else
  	     I2C_Send_Str(SYS24C08, EEP_DDLV_USER_CAL1+12, eeprom_tx_buf, 4);  	     
  	     #endif
  	     
  	     user_cal_ok = FALSE;
  }  	            
//////////////////////////////////////////////////////
//10  按 结束 按钮后 程序的 处理
//    USER_CAL_STEP1   删除以前的 校准成果
//    USER_CAL_STEP234 保存已经校准后的数据
//
//////////////////////////////////////////////////////
void  End_UserCal_Pro(void)
    {
     //记录保存了几个校准点
     uint8 eeprom_tx_buf[16];
     uint32 i;
     
     switch(point2_cal_step)
       {
        case USER_CAL_STEP1: break;
        
        case USER_CAL_STEP2:
        case USER_CAL_STEP3:
        case USER_CAL_STEP4:
        case USER_CAL_STEP5:
        case USER_CAL_END:
               user_cal_num = point2_cal_step-1;
               sort_cal_buf(user_cal_num);
               save_cal_data(user_cal_num);
               //Get_User_Coef123();
               break;
        default:
               break;   
       }
       
       
       /////////////如果不是在 STEP1阶段 就做 校准OK 标志
       if(USER_CAL_STEP1 != point2_cal_step)          
             {
               eeprom_tx_buf[0] = user_cal_num;
  	           eeprom_tx_buf[1] = 0x55;
  	           eeprom_tx_buf[2] = 0xaa;
  	           eeprom_tx_buf[3] = 0x55; 
  	           //////////////////////默认是按照从低到高排序的 
  	           eeprom_tx_buf[8]  = user_cal_point[0];
  	           eeprom_tx_buf[9]  = user_cal_point[1];
  	           eeprom_tx_buf[10] = user_cal_point[2];
  	           eeprom_tx_buf[11] = user_cal_point[3]; 
  	           eeprom_tx_buf[12] = user_cal_point[4];
  	           
  	           I2C_Send_Str(SYS24C08, EEP_USER_CAL_FLAG, eeprom_tx_buf, 16);	
  	           ////////////////////////use new coef
  	           for(i=0;i<10000;i++)
  	            {;}
  	           
  	           user_cal_ok = 1;
  	           Get_User_Coef123(); 
  	           ///////////////////////////更新显示区域
  	           Display_cal_liquid_area();
  	           Display_elec_class_area();   
               
             }
      ////////////////////////////////////////     
      Return_Normal_Status();
    }
    
//////////////////////////////////////////////
//11 系统校准 过程的 自动处理
//    
//////////////////////////////////////////////////////////////    

//////////////////////////////////////////////
//11 系统校准 过程的 自动处理
//    
////////////// ////////////////////////////////////////////////      
const uint32 DDLV_SYSCAL_STEP[7] = {300,3000,2000,1000,500,300,200};

void  Sys_Cal_Pro2(void)
  {      
    uint8 i,j; 
    uint8 eeprom_tx_buf[16];
    static uint32 tmp = 0xffffffff;
    
    //uint32   tmp = Get_Filter_ADS1100Data(); //获取100欧时候的数据
    uint32 raw_tmp = 1234567;
   
    i = point2_cal_step%7;
    switch(i)
       {
        case  SYS_CAL_RANG1_1:   
              if(raw_tmp > 10000) {
                  eeprom_tx_buf[0] =  tmp & 0xff;
                  eeprom_tx_buf[1] = (tmp>>8) & 0xff;
                  point2_cal_step++;
                  tmp = raw_tmp;
              }    
              break;
         
        case  SYS_CAL_RANG1_2:
        case  SYS_CAL_RANG1_3:
        case  SYS_CAL_RANG1_4:
        case  SYS_CAL_RANG1_5:
        case  SYS_CAL_RANG1_6:
              if(raw_tmp < tmp - DDLV_SYSCAL_STEP[i-1]) {
                  tmp = raw_tmp;
                  eeprom_tx_buf[(i-1)*2]   =  tmp & 0xff;
                  eeprom_tx_buf[(i-1)*2+1] = (tmp>>8) & 0xff;
                  point2_cal_step++;
              }    
              break;
        case  0:
              if(raw_tmp < tmp - DDLV_SYSCAL_STEP[i]) {
                  tmp = raw_tmp;
                  eeprom_tx_buf[12]   =  tmp & 0xff;
                  eeprom_tx_buf[13]   = (tmp>>8) & 0xff;
                  eeprom_tx_buf[14]   =  0x55;
                  eeprom_tx_buf[15]   =  0xaa;
                  Write_EEPROM(EEP_DDLV_SYSCAL_RANGE1+0x10*(point2_cal_step/7),eeprom_tx_buf, 16); 
  	              for(i=0;i<20000;i++){;}
  	              point2_cal_step++;
  	              range_man++;
  	              if(range_man > RANGE_2US) {
  	              //////////////5M校准完毕后手动返回测量模式  
                      eeprom_tx_buf[0] = 0x88;
  	                  eeprom_tx_buf[1] = 0x99;
  	                  eeprom_tx_buf[2] = 0xaa;
  	                  eeprom_tx_buf[3] = 0xbb;  
  	                  Write_EEPROM(EEP_DDLV_SYSCAL_CHECK,eeprom_tx_buf, 4); 
  	                  for(i=0;i<20000;i++){;}	  
  	                 
                      Init_Sys_Cal2_Varible();
                      Return_Normal_Status();
                      automode = 1;
                      range = RANGE_200MS;
                  }
              }    
              break;
        default:
              break;
       }      
   }   

   
//////////////////////////////////////////////////////////////////////
//12  按 save 按钮后 程序的 处理 只有在稳定后按此按钮才起作用
//
//只使用1种 溶液校准
///////////////////////////////////////////////////////////////////////   
  void  Save_UserCal_Pro(void)
  {  ////////////////校准时候的动作确认
     uint8 eeprom_tx_buf[4];
     un_rec_data tmp;
   
     tmp.rec_float = DDLV_standard_data[which_standard][match_point]/ddlv_data; //计算斜率 此数据就是电极常数
     elec_cons = tmp.rec_float;
     
     eeprom_tx_buf[0]  = (uint8)((tmp.rec_uint32>>24)&0xff);
  	 eeprom_tx_buf[1]  = (uint8)((tmp.rec_uint32>>16)&0xff);
  	 eeprom_tx_buf[2]  = (uint8)((tmp.rec_uint32>>8)&0xff);
  	 eeprom_tx_buf[3]  = (uint8)(tmp.rec_uint32&0xff); 
  	 //替换掉 此参数  	 	           
  	 I2C_Send_Str(SYS24C08, EEP_DDLV_CONS_ADDR, eeprom_tx_buf,4);
  }
  
//**********************************************************************
//13 函数  名:Sleep_Key_Pro
//函数功能:待机界面下的按键处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Sleep_Key_Pro(uint32 key_c,sint32 op)
   {
    sint32 op_id;
    uint32 i;
    op_id = op;  
    no_key_time = 0;      //休眠模式计时清零  
    //if(Key_Status != KEY_DO_START)
    // {
    // return(op_id);
    //}
    i = Action_SleepCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(1 == i)
        {
         Set_LCD_Brightness(lcd_brightness); //返回到正常亮度
         op_id = MENU_OP_OPER;           //返回
        }
    return(op_id);  
   }
//**********************************************************************
//14 函数 名:Main_Key_Pro
//函数功能:工作界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Main_Key_Pro(uint32 key_c,sint32 op)
{
    
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //{
    // return(op_id);
    //}
    i = Action_MainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xAA55)
    {
      switch(i)
       {
  	    case 1://
  	           op_id = MENU_SYSTEM_CONF_OPER;
  	           break;
    	case 2://
    	       op_id = MENU_DATA_CALL_OPER;
    	       break;
    	case 3://user cal
    	       
    	       Enter_UserPoint_Cal();
    	       break;
    	case 4://save
    	       if(current_mode == STAT_NORMAL)  //存储
    	          {
    	         
    	             Save_RecDDLV();
    	         
    	          } 
    	       break;
    	case 5://print
               //检查打印条件是否满足
               if((0 == print_busy) &&(1==Uart_Printer_Flag))
               {
                 print_busy = 1;
                 UART_PrintData(); 
               }
    	       break;
    	case 6:
    	       if(current_mode==STAT_SYS_CAL)
    	          Return_Normal_Status();
    	       else if(current_mode==STAT_USER_CAL)
    	          {
    	           Clear_User_Cal();
    	           Return_Normal_Status();
    	          } 
    	       else
    	       op_id = MENU_SLEEP_OP_OPER;
    	       break;
    	case 7://save in cal mode
    	       if((TRUE==match_flag)&&(current_mode == STAT_USER_CAL)&&((TRUE == stable_flag)))  //匹配到溶液并且稳定后 save才起作用
    	    
    	       Save_UserCal_Pro();
    	       
    	       Return_Normal_Status();
    	    
    	       break;
    	case 8://exit in cal mode
    	      if(current_mode == STAT_USER_CAL)
                  Return_Normal_Status();
          
    	       break;     
    	default:break;
   	   }
   	  //Key_Status = KEY_DO_OVER;
    }
    return(op_id);    
}

//**********************************************************************
//15 函数名:Oper_Sleep_Pro               
//函数功能:待机模式下界面显示
//入口参数:无
//出口参数:无
//**********************************************************************
void  Oper_Sleep_Pro(void)
     {
      Display_Sleep_Time();   
     }
     
//**********************************************************************
//16函数 名:Oper_Disp_Pro               
//函数功能:工作模式下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Oper_Disp_Pro(void)
{
   static uint32 stable_change_tmp,mode_change_tmp,negative_change_tmp;
  
   //////////////////////////////////////////工作状态显示
     if(mode_change_tmp != current_mode)
       {
        mode_change_tmp = current_mode;
   
        Mode_Status_Display2();
       
        }
   /////////////////////////////////////////稳定状态显示       
 	 if(stable_change_tmp != stable_flag)
  	   {  
   	    stable_change_tmp  = stable_flag;
   	
   	    Stable_Status_Display(50,130);
   	
  		} 
  		   
   /////////////////////////

   New_Data_Display2();
}

//**********************************************************************
//17函数名:Set_Menu_Op
//函数功能:设置函数当前所处的界面
//入口参数:cur_menu_op
//出口参数:无
//**********************************************************************
void Set_Menu_Op(sint32 cur_menu_op)
{
    menu_op = cur_menu_op;
}
/**********************************************************************
//18 待机界面的初始化
//
//////////////////////////////////////////////////////////////////////*/
void   Put_Sleep_Interface(sint32 op)
    {
     uint32 xy[1][2];
     
     Set_LCD_Brightness(SLEEP_BRIGHTNESS);
     ////////////////////////////////////待机状态下把背光设置到较暗的亮度
     Display_Sleep_Area();
     
     xy[0][0] =  SCR_XSIZE-120+110/2;
     xy[0][1] =  410+ 60/2;
     Create_ButtonList(xy,1);    
    }
//**********************************************************************
//19 函数  名:Put_Oper_Interface
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void Put_Oper_Interface(sint32 op)
{
    uint32 xy[8][2];
    uint32 i,j,k;  

    Clr_Bar(0,0,SCR_XSIZE,  SCR_YSIZE,   COL_THINCYAN);      //ALL black
    Clr_Bar(3,3,SCR_XSIZE-6,SCR_YSIZE-6, COL_THINBLUE);      //ALL black
    ////水平
    Clr_Bar(0,        80,      SCR_XSIZE,   3, COL_THINCYAN);  //分割线
    //Clr_Bar(0,       400,      SCR_XSIZE,   3, COL_THINCYAN);  //分割线
    Clr_Bar(0,   400-100,  SCR_XSIZE-200,   3, COL_THINCYAN);  //分割线
   
    
    ////垂直
    //Clr_Bar(400,     300,             3,100, COL_THINCYAN);  //分割线
    Clr_Bar(600,      80,             3,320, COL_THINCYAN);  //分割线
 
    //显示LOGO
    if(TRUE == LOGO_flag)
    Put_Bmp(5,6,logo21070,0);
    //显示操作者名称：
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_THINBLUE);
       
     if(TRUE == LOGO_flag)
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(90,90),    15,Language_Str("P812","P812")); //DISPLAY model
         else
         Put_Str(Language_Pos(90,90),    15,Language_Str("P802","P802"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(90,90),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(90,90),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(135,135), 30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    }
    else
    {
      if(1 == model_display)
       { 
        if(TRUE == version_811)
         Put_Str(Language_Pos(10,10),    15,Language_Str("P812","P812")); //DISPLAY model
         else
         Put_Str(Language_Pos(10,10),    15,Language_Str("P802","P802"));
        //////////////////////////////////////////////////////////////////
         Put_Str(Language_Pos(10,10),    45,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   45,"%03d", (uint16)user_id);            //DISPLAY oper
       }
      else
       {
         Put_Str(Language_Pos(10,10),    30,Language_Str("UID:","UID:")); //DISPLAY model
         Put_Data(Language_Pos(55,55),   30,"%03d", (uint16)user_id);            //DISPLAY oper
       } 
    
    }
    
    //主显示区域黑底白字
    Clr_Bar(3,80+3,    SCR_XSIZE-200-3,SCR_YSIZE-160-3,     COL_BLACK);        //ALL black
    //mV 显示区 温度显示区
    //Clr_Bar(  3,80+220+3,SCR_XSIZE-400-3,SCR_YSIZE-160-220-3, COL_THINGRAY);    
    //Clr_Bar(403,80+220+3,SCR_XSIZE-600-3,SCR_YSIZE-160-220-3, COL_THINGRAY);   
    //系统信息区
    Clr_Bar(603,    80+3,          200-6,    SCR_YSIZE-160-3, COL_DEEPGRAY);   
        

    Set_ASC_Lib(Arial16B_Font);
    Put_Str(650,125,Language_Str("电极常数",    "E-NODE CONS"));
    Put_Str(650,270,Language_Str("TDS 系数",    "TDS-COEF"));
    Set_Chr_Color(COL_THINYELLOW,COL_BLACK);
    Set_ASC_Lib(ARIALN26B);
    Clr_Bar(640,155,120,50,COL_BLACK);
    Clr_Bar(640,300,120,50,COL_BLACK);
    
    Put_Data_Float(660,170,"%6.3f",elec_cons);
    Put_Data_Float(660,315,"%6.3f",TDS_cons);
  
    
    //按键区
    Clr_Bar(3,400+3, SCR_XSIZE-6,80-6,COL_BLACK); 
    
    if (Get_Language_Type() == CHINESE)
    {
     Put_Bmp(133*0+15,  405,  sysconfig8055,0); 
     Put_Bmp(133*1+15,  405,     recall8055,0); 
     Put_Bmp(133*2+15,  405,        cal8055,0); 
     Put_Bmp(133*3+15,  405,       save8055,0); 
     Put_Bmp(133*4+15,  405,    printer8055,0);
    }
    else
    {
     Put_Bmp(133*0+15,  405,  sysconfigen8055,0); 
     Put_Bmp(133*1+15,  405,     recallen8055,0); 
     Put_Bmp(133*2+15,  405,        calen8055,0); 
     Put_Bmp(133*3+15,  405,       saveen8055,0); 
     Put_Bmp(133*4+15,  405,    printeren8055,0);
    }
    Put_Bmp(133*5+15,  405,      sleep8055,0);
    
      ////////////////////////////////////////create keylist 
    for(i=0;i<6;i++)
    {
    xy[i][0]    = 65+133*i;
    xy[i][1]    = 400+80/2;
    }
   //////////////////////////////////add key2
    xy[i][0]      = 300+110/2;
    xy[i++][1]    = 240+40/2;
    xy[i][0]      = 430+110/2;
    xy[i++][1]    = 240+40/2;
  
    Create_ButtonList(xy,8);            
    /////////////////////////////////////REFLASH
   Mode_Status_Display2();
   
}
//******************************************************************************************************
//20 函数  名:Normal_Operation
//函数功能:管理机器在正常工作模式下的各种操作
//入口参数:无
//出口参数:无
//******************************************************************************************************


void Normal_Operation(void)
{  
    struct menu_str menu_op[MENU_NUM]=
     //菜单ID                菜单界面初始化         菜单选项处理              显示处理         辅助处理
    {
     {MENU_OP_OPER,           Put_Oper_Interface,               Main_Key_Pro,          Oper_Disp_Pro,  Oper_Main_Assi_Pro},
     {MENU_SLEEP_OP_OPER,     Put_Sleep_Interface,             Sleep_Key_Pro,         Oper_Sleep_Pro,      Oper_Assi_Null},    //待机画面
     {MENU_FLOAT_INPUT_OPER,  FloatInput_Interface,  	  FloatInput_Key_Pro,    FloatInput_Disp_Pro,  	    Oper_Assi_Pro},    // INPUT num  
     {MENU_UINT_INPUT_OPER,   UINTInput_Interface,         UINTInput_Key_Pro,     UINTInput_Disp_Pro,  		Oper_Assi_Pro},    // INPUT pcs
     {MENU_DATA_CALL_OPER,    Data_Call_Interface,         Data_Call_Key_Pro,     Data_Call_Disp_Pro,  		Oper_Assi_Pro},    //数据回显示
     //////系统配置菜单
     {MENU_SYSTEM_CONF_OPER,  SystemConf_Interface,       SystemConf_Key_Pro,    SystemConf_Disp_Pro,  		Oper_Assi_Pro},    //SYSTEM CFG 
   
     {MENU_TIME_CFG_OPER,     TimeCfg_Oper_Interface,        TimeCfg_Key_Pro,       TimeCfg_Disp_Pro,       Oper_Assi_Pro},    //TIME cfg
     {MENU_LANG_SEL_OPER,     LangSel_Interface,             LangSel_Key_Pro,       LangSel_Disp_Pro,  		Oper_Assi_Pro},    //lange CFG 
     {MENU_BRIG_CFG_OPER,     Brig_Adjust_Interface,     Brig_Adjust_Key_Pro,   Brig_Adjust_Disp_Pro,  		Oper_Assi_Pro},    //数据回显示
       
     {MENU_STAND_SEL_OPER,    StandardSelInput_Interface,   Standard_Key_Pro,  Standard_Sel_Disp_Pro,  		Oper_Assi_Pro},    //标准选择
     {MENU_DIVISION_SEL_OPER, DivisionSel_Interface,     DivisionSel_Key_Pro,   DivisionSel_Disp_Pro,  		Oper_Assi_Pro},    //分辨率选择
     {MENU_TEMP_COMP_OPER,    TempcompCfg_Interface,     TempcompCfg_Key_Pro,   TempcompCfg_Disp_Pro,  		Oper_Assi_Pro},    //温度补偿选择
   
     {MENU_USER_ID_OPER,      UserIDInput_Interface,          UserID_Key_Pro,        UserID_Disp_Pro,  		Oper_Assi_Pro},    //用户ID输入
     {MENU_SCR_CAL_OPER,      ScrCalInput_Interface,          ScrCal_Key_Pro,        ScrCal_Disp_Pro,      Oper_Assi_Null},    //屏幕校准
     //电导率增加 菜单
     {MENU_DDLV_CONS_OPER,    DDLV_CONS_Input_Interface,   DDLV_CONS_Key_Pro,     DDLV_CONS_Disp_Pro,  	   Oper_Assi_Null},     //电导常数选择
     {MENU_MODE_SEL_OPER,     ModeInput_Interface,              Mode_Key_Pro,          Mode_Disp_Pro,      Oper_Assi_Null},     //参数2
     {MENU_BALANCE_SEL_OPER,  BalanceInput_Interface,   BalanceInput_Key_Pro,       Balance_Disp_Pro,  	   Oper_Assi_Null},     //参数3
   
     {MENU_OPER_CONFIRM_OPER, OperConfirmInput_Interface,    Confirm_Key_Pro,       Confirm_Disp_Pro,  	   Oper_Assi_Null}      //操作确认菜单 
     
     };
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
    uint32 display_cnt,speak_cnt;
    uint16 temp_ad;
    
    cur_op  = 0xffffffff;  
    next_op = MENU_OP_OPER;
    automode = 1;
    range = RANGE_200MS;
     
    while (1)
     {
        display_cnt++;
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            (*(menu_op[(next_op>>8)&0xff].menu_init))(next_op);
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        Set_Menu_Op(cur_op);
        
        //////////////////////////////// 
        measure_proc();
         
        //read ptc temp
        while(!(ADDR0&0x80000000));
        temp_ad = (ADDR0>>6)&0x3ff;
        filter_ad_NTC(temp_ad); ///1024.0001 *3300;
           
        ////////////////////////////////////////////////////////////////           
        key_value = Get_PositionXY();        //键盘扫描
        if(key_value)    
            next_op = (*(menu_op[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);     
        
        if(display_cnt%2 == 0)     
            (*(menu_op[(cur_op>>8)&0xff].disp_pro))();    //显示处理函数
            
        if(1 == sound_flag) {
            sound_flag = 0;
            Speaker_ON();
            for(speak_cnt=0;speak_cnt<150000;speak_cnt++) {;}
            Speaker_OFF();
        }   
     
        (*(menu_op[(cur_op>>8)&0xff].assi_pro))();    //辅助处理函数
    }
 }

  
////////////////////////////////////////////////////////
//21    在指定位置用指定颜色显示稳定符号
//
///////////////////////////////////////////////////////
static void Stable_Status_Display(uint32 x,uint32 y)
 {
  if(TRUE == stable_flag)
   {
    Set_Color(COL_THINGREEN);
    Fill_Circle(x,y,20);
   }
   else
   {
    Set_Color(COL_THINRED);
    Fill_Circle(x,y,20);
   } 
    
 }
 
////////////////////////////////////////////////////////
//22    在指定位置用指定颜色显示正负符号
//      PH 符号
///////////////////////////////////////////////////////
static void Negative_Status_Display(void)
 {
    Clr_Bar(15,125,64,130,COL_BLACK); //擦除
    Set_ASC_Lib(GulimChe150_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(TRUE == ph_negative_flag)
      Put_Char(15,125,'-');
 }  
////////////////////////////////////////////////////////
//23   根据已经校准溶液 的系数 计算 
//
///////////////////////////////////////////////////////
 static float   User_Cal_Done(float ph_tmp)
   {
    //根据当前ph_tmp 的数值 取对应的系数进行 线性校准，从高端开始
    float tmp;
    //首先判断用户校准了几个点
    switch(user_cal_num)
      {
       case 1:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else 
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef_left;
                }
              break;  
       case 2:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef_left;  
                } 
              break;  
       case 3:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef_left;  
                }  
              break; 
        case 4:
              if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else if(ph_tmp < user_cal_phdata[3].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef4;  
                }  
              else
                {
                 tmp = ph_tmp-user_cal_phdata[3].rec_float;
                 tmp = user_cal_phdata_dis[3].rec_float + tmp*user_coef_left;  
                }  
              break; 
        case 5:
                if(ph_tmp < user_cal_phdata[0].rec_float)
                tmp = ph_tmp * user_coef1;
              else if(ph_tmp < user_cal_phdata[1].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[0].rec_float;
                 tmp = user_cal_phdata_dis[0].rec_float + tmp*user_coef2;
                }
              else if(ph_tmp < user_cal_phdata[2].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[1].rec_float;
                 tmp = user_cal_phdata_dis[1].rec_float + tmp*user_coef3;  
                } 
              else if(ph_tmp < user_cal_phdata[3].rec_float)
                {
                 tmp = ph_tmp-user_cal_phdata[2].rec_float;
                 tmp = user_cal_phdata_dis[2].rec_float + tmp*user_coef4;  
                } 
              else if(ph_tmp < user_cal_phdata[4].rec_float) 
                {
                 tmp = ph_tmp-user_cal_phdata[3].rec_float;
                 tmp = user_cal_phdata_dis[3].rec_float + tmp*user_coef5;  
                }  
              else
                {
                 tmp = ph_tmp-user_cal_phdata[4].rec_float;
                 tmp = user_cal_phdata_dis[4].rec_float + tmp*user_coef_left;  
                }  
              break;               
       default:
                tmp = ph_tmp;
               break;
      }
    //////////////////////////////////////////////
    return tmp;
   }
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//24A  电导率仪的测量数据 显示
//
//////////////////////////////////////////////////////////

float GetNewElec_cons(void)
{
	float tmp;
	
	if(STAT_USER_CAL == current_mode) {
		if((elec_cons > 0.007)&&(elec_cons < 0.013))
			tmp = 0.01;
		else if((elec_cons > 0.07)&&(elec_cons < 0.13))
			tmp = 0.1;
     	else if((elec_cons > 0.7)&&(elec_cons < 1.3))
			tmp = 1.0;
		else if((elec_cons > 7)&&(elec_cons < 13))
			tmp = 10;
		else
		    tmp = elec_cons;	
	} else {
		tmp = elec_cons;
	}
		
	return(tmp);
}


static void  New_Data_Display2(void)
 {

       //计算电导率
/*
       if(ddlv_R < 500)                    /////////////////////////////////////////
         {
          ddlv_data = 1000 / ddlv_R * ddlv_coef;  //用户常数起作用
          ddlv_data = ddlv_data * elec_cons;      //电极常数起作用
          DDLV_unit = MS_CM; 
         }
       else
         {
          ddlv_data = 1000 / (ddlv_R/1000) * ddlv_coef;
          ddlv_data = ddlv_data * elec_cons;      //电极常数起作用
          DDLV_unit = US_CM;
         }
  */
   switch(range) {
   case RANGE_200MS:
   case RANGE_20MS: 
       DDLV_unit = MS_CM;
       ddlv_data = ddlv_raw_data2/1000;
       break;
   case RANGE_2MS:
   case RANGE_200US:
   case RANGE_20US:
   case RANGE_2US:
       DDLV_unit = US_CM;
       ddlv_data = ddlv_raw_data2;
       break;
   default:
       break;
   }        
       //elec_cons compensation
       ddlv_data = ddlv_data * GetNewElec_cons();
       
       temp_comp_proc();
       ////////////////////////////温度补偿完后，然后根据设定的参数显示相应的数据
       if(PARAM_TDS == DDLV_Param)
         {
          ddlv_data = ddlv_data * TDS_cons;  //TDS 系数起作用
         }
       else if(PARAM_SALTY == DDLV_Param)
         {
            if(TRUE == manual_temp_comp_flag)
              {
               if((temp_comp_manual>0) && (temp_comp_manual<40))
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * temp_comp_manual - 6171.9;
               else
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * 25 - 6171.9;  //以25度为准
              }
            else
              {
               if((temp_float_ptc>0) && (temp_float_ptc<40))
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * temp_float_ptc - 6171.9;
               else
                ddlv_data = 1.3888*ddlv_data -0.02478*ddlv_data * 25 - 6171.9;  //以25度为准
              }
         } 
      ////////////////////////////////////////////////////////   
  
   
  ///////////////////////////////////// 
  switch(current_mode)
  {
   case STAT_NORMAL:      display_measure2_data();break;  
   case STAT_SYS_CAL:     display_sys_caling_data2();break;
   case STAT_USER_CAL:    display_user_caling_data2();break;

   default:break;
  }
 } 
 
//////////////////////////////////////////////////////
//25   测量数据 在主显示窗口的 显示
//
//////////////////////////////////////////////////////
extern uint32 stable_time_cnt;

static void  display_main_area(void)
  {
    float x;
    //display ph_data 
    //////////////////////////////////////////////////////////\
    //ph_data = 7.0
    Set_ASC_Lib(GulimChe150_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(FALSE == full_flag)
      {
       /////////////////////////////////////P811
       if(TRUE == version_811)
       {
        if(phmv_division&0x01)
           Put_Data_Float(100,125,"%6.2f",ph_data);
        else
           Put_Data_Float(100,125,"%6.3f",ph_data);
       }
       else
       {
        if(phmv_division&0x01)
           Put_Data_Float(100,125,"%6.1f",ph_data);
        else
           Put_Data_Float(100,125,"%6.2f",ph_data);
       }  
      } 
    else
       Put_Str(100,125,"------");
    //display mv_data   
    //////////////////////////////////////////////////////////
    //mv_data = 0;
    Set_ASC_Lib(GulimChe48A_Font);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    if(TRUE == version_811)
    {
      if(phmv_division&0x08)
         Put_Data_Float(60,320,"%7.2f",mv_data); 
      else
         Put_Data_Float(60,320,"%7.1f",mv_data);
    }
    else
    {
      if(phmv_division&0x08)
         Put_Data_Float(60,320,"%7.1f",mv_data); 
      else
         Put_Data(      60,320,"%5d",  (uint32)mv_data); 
    }
    //display mv flag
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
     Put_Char(20,320,'-');  
     
     Negative_Status_Display();

  }
///////////////////////////////////////////////////////  
//25A  电导率 仪器 测量数据 在主显示窗口的 显示
//
//////////////////////////////////////////////////////

static void  display_main_area3(void)
{
   if(ddlv_data < 0) 
       return;
        
   Set_ASC_Lib(GulimChe150_Font);
   Set_Chr_Color(COL_WHITE,COL_BLACK);
  
   switch(DDLV_Param) {
   case PARAM_DDLV:
       if((ddlv_data >= 200)&&(ddlv_data < 10000))
           Put_Data(110,125+30, "%5d",((uint32)ddlv_data));
       else if (ddlv_data >= 20)
           Put_Data_Float(110,125+30, "%5.1f",ddlv_data);
       else 
           Put_Data_Float(110,125+30, "%5.2f",ddlv_data);     
       break;
   case PARAM_SALTY:
   
       Put_Data_Float(110,125+30, "%5.1f",ddlv_data);
       break;
   case PARAM_TDS:
       Put_Data_Float(110,125+30, "%5.2f",ddlv_data);
       break;
   default:
       break;
   }
    //////////////////////////////////////////////////////////
   /*
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(5,150, "%d",range);
    Put_Data_Float(5,180, "%8.1f",ddlv_raw_data1);
    Put_Data_Float(5,210, "%8.1f",ddlv_raw_data2);
    */
    Set_ASC_Lib(Unit_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    switch(DDLV_Param) {
    case PARAM_DDLV:
        if(US_CM == DDLV_unit)
            Put_Line_Str(465, 245+70,Language_Str("uS/cm","uS/cm")); 
        else
            Put_Line_Str(465, 245+70,Language_Str("mS/cm","mS/cm"));
        break;
    case PARAM_TDS:
        //if(US_CM == DDLV_unit)
        //    Put_Line_Str(465, 245+70,Language_Str("mg/L","mg/L")); 
        //else
            Put_Line_Str(465, 245+70,Language_Str(" ppm"," ppm")); 
        break;
    case PARAM_SALTY:
         //盐度只有一个单位:  %
        Put_Char(500, 245+70,'%');
        break;
    default:
        break;     
    }
}


//////////////////////////////////////////////////////
//26  正常状态下的 显示 
//    前提是先处理 警告错误信息
//
//////////////////////////////////////////////////////
static void  display_measure_data(void)
 {
   //删除其它模式下的报警信息
   Del_Alm(&warn_ph_out,ALM_LEV_WARN);
   Del_Alm(&warn_mv_out,ALM_LEV_WARN);
   Del_Alm(&warn_xx_out,ALM_LEV_WARN);
 
   //////////////////////////////////////// 
   display_main_area(); 
}  

static void  display_measure2_data(void)
 {
   //删除其它模式下的报警信息
   Del_Alm(&warn_ph_out,ALM_LEV_WARN);
   Del_Alm(&warn_mv_out,ALM_LEV_WARN);
   Del_Alm(&warn_xx_out,ALM_LEV_WARN);
 
   //////////////////////////////////////// 
   display_main_area3(); 
}  

//////////////////////////////////////////////////////
//27  酸度计 系统校准状态下的 显示
//
///////////////////////////////////////////////////////
static void  display_sys_caling_data2(void)
 {
   
   static uint32 process_num = 0;
 
   ///////////////////////////////////////////////校准状态显示
     process_num = process_num+10;
      if(100 == process_num)
         process_num = 0;
     ////////////////////////////////////把校准过程移动到显示里面自动进行下一步
     if(TRUE == stable_flag)
        Sys_Cal_Pro2();
     
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     
     Put_Line_Str(180, 230,Language_Str("当前校准步数是 ", "Now cali step"));
     Put_Data(180+200, 230,"%02d", point2_cal_step);
    
     Clr_Bar(120, 150,    30,40,COL_DEEPGREEN);
     Clr_Bar(150, 150,300-30,40,COL_THINBLUE);
     Clr_Bar(120+3*process_num, 152,12,36,COL_DEEPGREEN);
       
    
} 

//////////////////////////////////////////////////////
//28  用户校准状态下的 显示
//
///////////////////////////////////////////////////////
static void  display_user_caling_data(void)
 {
  uint32 i;
  float  tmp;
  static uint32 flash_cnt;
  
  flash_cnt++;
  //把校准过程移动到显示里面自动进行下一步
  //显示系统PH  显示匹配到的PH
  Set_ASC_Lib(GulimChe48A_Font);
  Set_Chr_Color(COL_BLACK,COL_THINGRAY); 
  if(TRUE == version_811)
    {
     if(phmv_division&0x01)
     Put_Data_Float(55,165, "%6.2f",ph_data);
     else
     Put_Data_Float(55,165, "%6.3f",ph_data);
    }
  else
    {
     if(phmv_division&0x01)
     Put_Data_Float(55,165, "%6.1f",ph_data);
     else
     Put_Data_Float(55,165, "%6.2f",ph_data);
    }
  //显示正负 号
  if(TRUE == ph_negative_flag)
    Put_Char(20,160,'-');
  else  
    Put_Char(20,160,'+');
    
  //查找匹配的溶液值
 
  for(i=0;i<5;i++)
   {
    match_point = i;
    if(TRUE == version_811)
      {
       if(ph_data > standard_data_p811[which_standard][i])
         {
          tmp = ph_data - standard_data_p811[which_standard][i];
         }
       else
         tmp = standard_data_p811[which_standard][i] - ph_data;
      }
    else
      {
       if(ph_data > standard_data_p801[which_standard][i])
         {
          tmp = ph_data - standard_data_p801[which_standard][i];
         }
       else
         tmp = standard_data_p801[which_standard][i] - ph_data;
      }  
    //////////////////////////////////////////////////////////////////      
    if(tmp < 0.3)
     {
      match_flag  = TRUE;
      break; 
     }
     else
     {
      match_flag  = FALSE;
     } 
   }
   
   ///////////////////////////////////////////闪烁匹配的溶液值
  if(((flash_cnt%6)<3)&&(TRUE==match_flag))
     {
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
     }
  else
     {
      Set_Chr_Color(COL_DEEPGRAY,COL_DEEPGRAY); 
     }
   
   if(TRUE == version_811)    
     Put_Data_Float(270,165, "%6.3f",standard_data_p811[which_standard][match_point]);
   else
     Put_Data_Float(270,165, "%6.2f",standard_data_p801[which_standard][match_point]);
 
  //display mv 
  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
 
  if(TRUE == version_811)
  {
    if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.2f",mv_data); 
    else
     Put_Data_Float(60,320,"%7.1f",mv_data);
  }
  else
  {
    if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.1f",mv_data); 
    else
     Put_Data(      60,320,"%5d",  (uint32)mv_data);
  }
  //////////////////////////////////////////////
   Set_ASC_Lib(GulimChe48A_Font); 
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
    Put_Char(20,320,'-');     
}  

//////////////////////////////////////////////////////
//28  DDLV 校准状态下的 显示xxxxxxxxxxxxxxxxxxxxxxxxxx
//
///////////////////////////////////////////////////////
static void  display_user_caling_data2(void)
 {
  uint32 i;
  float  tmp;
  static uint32 flash_cnt;
  
  flash_cnt++;
  //把校准过程移动到显示里面自动进行下一步
  //显示系统PH  显示匹配到的PH
  Set_ASC_Lib(GulimChe48A_Font);
  Set_Chr_Color(COL_BLACK,COL_THINGRAY); 
  Put_Data_Float(55,165, "%6.1f",ddlv_data);
  
  /*显示正负 号
  if(TRUE == ph_negative_flag)
    Put_Char(20,160,'-');
  else  
    Put_Char(20,160,'+');
  */
    
  //查找匹配的溶液值 只使用一种标准
  which_standard = 0;
  
  for(i=0;i<4;i++)
   {
    match_point = i;
    
    if(ddlv_data > DDLV_standard_data[which_standard][i])
       {
        tmp = ddlv_data - DDLV_standard_data[which_standard][i];
       }
    else
        tmp = DDLV_standard_data[which_standard][i] - ddlv_data;
        
    if(tmp <  DDLV_standard_data[which_standard][i] / 2)
     {
      match_flag  = TRUE;
      break; 
     }
    else
     {
      match_flag  = FALSE;
     } 
   }
   
   ///////////////////////////////////////////闪烁匹配的溶液值
  if(((flash_cnt%6)<3)&&(TRUE==match_flag))
     {
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
     }
  else
     {
      Set_Chr_Color(COL_DEEPGRAY,COL_DEEPGRAY); 
     }
     
   Put_Data_Float(270,165, "%6.3f",DDLV_standard_data[which_standard][match_point]);
  
  /*/display mv 
  Set_Chr_Color(COL_BLACK,COL_THINGRAY);
  if(phmv_division&0x08)
     Put_Data_Float(60,320,"%7.2f",mv_data); 
  else
     Put_Data_Float(60,320,"%7.1f",mv_data);
  //////////////////////////////////////////////
   Set_ASC_Lib(GulimChe48A_Font); 
    if(TRUE == mv_negative_flag)
       {
        Set_Chr_Color(COL_BLACK,COL_THINGRAY);
       }
    else
       {
        Set_Chr_Color(COL_THINGRAY,COL_THINGRAY);
       }
     
    Put_Char(20,320,'-');*/     
}  

////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////  

