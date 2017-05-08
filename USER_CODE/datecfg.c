//**********************************************************************
//文件名:datecfg.c
//文件功能 对日期时间进行配置
//
//函数列表
//1   日期配置下的按键 判断   Action_DateCfg
//2   日期的显示函数          Date_Disp_Pro
//3   时间的显示函数          Time_Disp_Pro
//4   日期配置下的按键 处理   Date_Key_Pro
//5   时间配置下的按键 处理   Time_Key_Pro
//6   日期配置界面的初始化    Date_Oper_Interface
//7 

//**********************************************************************
#include "Font_Lib.h"
#include "font_lib2.h"

#include "rtc.h"
#include "graph.h" 
#include "global.h"
#include "Calendar.h"
#include "normal_operation.h"
#include "lcd2478.h"

static uint8    time_index;
static RTCTime  local_time;
/*///////////////////////////////////////////////////
/*1
/*日期回调函数
//////////////////////////////////////////////////*/
uint32  Action_TimeCfg(uint16 x,uint16 y)
	{
        uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     //if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
	     if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)< 25))
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

//**********************************************************************
//2函数名:Date_Disp_Pro               
//函数功能:日期配置下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void TimeCfg_Disp_Pro(void)
{    
      Set_ASC_Lib(Arial22_Font); //16*29          
      Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
      switch(time_index)
       {
        case 1:
                Set_Chr_Color(COL_WHITE,COL_BLACK);
                Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
                Put_Char(270+4*16,  135, '-');
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	  			Put_Char(270+7*16,  135, '-');
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
  
      			Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
	  			Put_Char(270+4*16,  175, '-');
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Put_Char(270+7*16,  175, '-');
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec); 
	  			break;
        case 2:
                Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Put_Char(270+4*16,  135, '-');
	 		    Set_Chr_Color(COL_WHITE,COL_BLACK);
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	 		    Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
	  			Put_Char(270+7*16,  135, '-');
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
  
      			Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
	  			Put_Char(270+4*16,  175, '-');
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Put_Char(270+7*16,  175, '-');
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec);
	  			break; 
        case 3:
        	    Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Put_Char(270+4*16,  135, '-');
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	  			Put_Char(270+7*16,  135, '-');
	  			Set_Chr_Color(COL_WHITE,COL_BLACK);
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
                Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
      			Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
	  			Put_Char(270+4*16,  175, '-');
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Put_Char(270+7*16,  175, '-');
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec); 
	  			break;
        case 4:
                Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Put_Char(270+4*16,  135, '-');
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	  			Put_Char(270+7*16,  135, '-');
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
                Set_Chr_Color(COL_WHITE,COL_BLACK);
      			Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
      			Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
	  			Put_Char(270+4*16,  175, '-');
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Put_Char(270+7*16,  175, '-');
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec); 
	  			break;
        case 5:
         		Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Put_Char(270+4*16,  135, '-');
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	  			Put_Char(270+7*16,  135, '-');
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
       			Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
	  			Put_Char(270+4*16,  175, '-');
	  			Set_Chr_Color(COL_WHITE,COL_BLACK);
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Set_Chr_Color(COL_WHITE,COL_DEEPGRAY);
	  			Put_Char(270+7*16,  175, '-');
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec); 
	  			break;
        case 6:
        	    Put_Data(270,       135, "%04d", local_time.RTC_Year);
                Put_Char(270+4*16,  135, '-');
	 		    Put_Data(270+5*16,  135, "%02d",local_time.RTC_Mon);
	  			Put_Char(270+7*16,  135, '-');
	  			Put_Data(270+8*16,  135, "%02d",local_time.RTC_Day); 
            	Put_Data(270+2*16,  175, "%02d", local_time.RTC_Hour);
	  			Put_Char(270+4*16,  175, '-');
	  			Put_Data(270+5*16,  175, "%02d",local_time.RTC_Min);
	  			Put_Char(270+7*16,  175, '-');
	  			Set_Chr_Color(COL_WHITE,COL_BLACK);
	  			Put_Data(270+8*16,  175, "%02d",local_time.RTC_Sec); 
	  			break;
        default:break;
       }    
 }

//**********************************************************************
//5函数名:Time_Key_Pro
//函数功能:时间配置界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
 sint32 TimeCfg_Key_Pro(uint32 key_c,sint32 op)
{
 sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_TimeCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //+
  	           switch(time_index)
  	           {
  	            case 1:
  	                    local_time.RTC_Year++;
  	                    if(local_time.RTC_Year>CALENDAR_END_YEAR)
  	                    local_time.RTC_Year = CALENDAR_START_YEAR;
  	                    break;
  	            case 2:
  	                    local_time.RTC_Mon++;
  	                    if(local_time.RTC_Mon>12)
  	                    local_time.RTC_Mon = 1;
  	                    break;
  	            case 3:
  	                    local_time.RTC_Day++;
  	                    if(local_time.RTC_Day> GetCalendarDay(local_time.RTC_Year,local_time.RTC_Mon))
  	                    local_time.RTC_Day = 1;
       	                break;
  	            case 4:
  	                    local_time.RTC_Hour++;
  	                    if(local_time.RTC_Hour==24)
  	                    local_time.RTC_Hour = 0;
  	                    break;
  	            case 5:
  	                    local_time.RTC_Min++;
  	                    if(local_time.RTC_Min==60)
  	                    local_time.RTC_Min = 0;
  	                    break;
  	            case 6:
  	                    local_time.RTC_Sec++;
  	                    if(local_time.RTC_Sec==60)
  	                    local_time.RTC_Sec = 0;
       	                break;
  	            default:break;
  	           }
  	           break;
    	case 2://-
    	       switch(time_index)
  	           {
  	            case 1:
  	                    local_time.RTC_Year--;
  	                    if(local_time.RTC_Year < CALENDAR_START_YEAR)
  	                    local_time.RTC_Year    = CALENDAR_START_YEAR;
   		                break;
  	            case 2:
  	                    local_time.RTC_Mon--;
  	                    if(local_time.RTC_Mon == 0)
  	                    local_time.RTC_Mon = 12;
   		                break;
  	            case 3:
  	                    local_time.RTC_Day--;
   		                if(local_time.RTC_Day == 0)
				        local_time.RTC_Day = GetCalendarDay(local_time.RTC_Year,local_time.RTC_Mon); //得到当月的最多天数
   		                break;
  	            case 4:
  	                    if(local_time.RTC_Hour == 0)
  	                    local_time.RTC_Hour = 23;
  	                    else
  	                    local_time.RTC_Hour--;
   		                break;
  	            case 5:
  	                    if(local_time.RTC_Min == 0)
  	                    local_time.RTC_Min = 59;
  	                    else
  	                    local_time.RTC_Min--;
   		                break;
  	            case 6:
  	                    if(local_time.RTC_Sec == 0)
  	                    local_time.RTC_Sec = 59;
  	                    else
  	                    local_time.RTC_Sec--;
   		                break;
  	            default:break;
  	           }
  	           break; 
        case 3://SWITCH
       	       time_index++;
       	       if(7==time_index)
       	       time_index = 1;
       	       break;
   		case 5://ok
   		       RTCSetTime(local_time);
   		case 4://cancel
   		       need_recover2 = 1;
   		       op_id =  MENU_SYSTEM_CONF_OPER; //回到配置界面
   		       break;
   	  	default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//6函数名:Date_Oper_Interface
//函数功能:日期配置 界面的初始化
//入口参数:cur_menu_op
//出口参数:无
//**********************************************************************
void  TimeCfg_Oper_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
    //Create window
    Create_Window_Frame(200,80,400,190,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(210,85,Language_Str("时间设置","DateTime Configuration")); 
  
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    for(i=0;i<3;i++)
    Bar(220+i*100,220,300+100*i,260);
    
    Set_Chr_Color(COL_WHITE,COL_DEEPBLUE);
    Set_ASC_Lib(Arial22_Font);   
    Put_Char(250+0*100,225,'+');
    Put_Char(250+1*100,225,'-');
    Set_ASC_Lib(Arial16B_Font);   
    Put_Line_Str(Language_Pos(435, 428),230,Language_Str("切换","SWITCH"));
    
    Put_Button_Bar(510,130,510+90, 130+50,3,COL_BLACK);//OK
    Put_Button_Bar(510,205,510+90, 205+50,3,COL_BLACK);//SWITCH
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(535, 540),  145,Language_Str("取消","Esc"));
    Put_Line_Str(Language_Pos(535, 545),  220,Language_Str("确定","Ok"));
    
    //fill text
    //create button list
    for(i=0;i<3;i++)
    {
    xy[i][0] = 260 + i*100;
    xy[i][1] = 240;
    }
    //////////////////cancel ok
    xy[i][0]    = 510+90/2;
    xy[i++][1]  = 130+50/2;
    xy[i][0]    = 510+90/2;
    xy[i][1]    = 205+50/2;
    Create_ButtonList(xy,5);  
    time_index = 5;           //默认是调整分钟
    local_time  = RTCGetTime();           
}