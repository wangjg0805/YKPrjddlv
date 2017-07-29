//**********************************************************************
//文件名:global.c
//函数列表
//1  初始化全局变量  		    InitGlobalVarible 
//2  初始化语言变量   		    InitLanguageVarible  
//3  初始化亮度变量  		    InitBrightnessVarible
//4  初始化校准屏幕变量   		InitScreenVarible
//5  初始化满量程变量   		Init_Cal_Varible
//6  初始化 0点变量			    Init_Zer0_Weigh_Varible
//7  线性校准数据计算     		Get_Linecal_Var
//8   初始化线性校准变量		Init_LineCal_Varible
//8a  读取当前记录的存放位置    Search_Rec_Pos
//9   显示已消耗时间   		    Display_elapsed_time
//10  待机画面显示  		    Display_Sleep_Area
//11  待机时间显示   		    Display_Sleep_Time
//12  待机状态     			    Sleep_process
//13  开机系统初始化  			SystemInit

//14  根据XY,构建按键数据结构   Create_ButtonList

//15 放置置零标志    		    Set_Zer0_Flag
//16 放置去皮标志   		    Set_Tare_Flag
//17 放置采集百分比基准标志		Set_Sam100_Flag
//18 放置累加标志        		Set_Totaladd_Flag
//19 放置采集目标值数据标志   	Set_SamTarget_Flag

//**********************************************************************
#include "global.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "graph.h"
#include "ad7843.h"
#include "disp.h"
#include "Font_Lib.h"
#include "Font_Lib2.h"
#include "type_def.h"
#include "alarm_pro.h"

#include "ddlvmenu.h"

#include "rtc.h"
#include "ADS1100.h"


uint32      user_id,phmv_division,which_standard,lcd_brightness;        //显示屏对比度的值
uint32      select_ch,switch_flag,match_item,power_fre;
uint32      sound_switch,sound_flag;

uint32      Key_Status,key_value,electrode_class;  //按键数据以及状态
struct      KeyStruct  Key[15];

uint32      machine_mode,current_mode,mode_run_flag;      //机器的数据状态
uint32      power_on_flag,power_on_clr_cnt,elapsed_time;  //开机耗时记录
uint32      Error_Info_Flag,Warning_Info_Flag;

uint32      ph_negative_flag,mv_negative_flag,full_flag;

uint32      ad_raw_data,flag_new_data,stable_flag,break_stable_flag,sample_flag;

float       mv_data,ph_data,get_coef,ddlv_data;

float       elec_cons,temp_cons,TDS_cons;           //电极常数 温补系数 TDS 系数
uint32      DDLV_Param,DDLV_Mode,DDLV_unit;         //电导率的测量模式
uint32      measure_timer_sec,measure_timer_sec_real;
uint32      balance_sec,balance_sec_real;
float       balance_data;
   
//////////////////////////////电导率校准相关变量
//uint32      filter_phase,switch_action_step;
  uint32      ddlv_syscal_buf[6][7];   //6档 每档位7个点
//uint32      cal_ddlv_CONS[6];

//uint32      cal_ddlv_b_flag[6];
//float       cal_ddlv_b[6],cal_ddlv_k[6]; //根据上述数据得到系数


uint32      cpu_ch_adc3,cpu_ch_adc4;
uint32      zero_data[4],low1_data[4],low2_data[4],full1_data[4],full2_data[4],full3_data[4]; //保存系统校准的数据
uint32      user_cal_num,user_cal_data[5];    //存放用户校准的数据

float        sys_coef1, sys_coef2, sys_coef3, sys_coef4, sys_coef5, sys_coef6;
float       user_coef1,user_coef2,user_coef3,user_coef4,user_coef5,user_coef_left;
//定义电导率的用户校准系数
float       ddlv_R,ddlv_coef;

//最多允许5个用户校准点 
un_rec_data user_cal_phdata[5];
un_rec_data user_cal_phdata_dis[5];

uint32      user_cal_point[5];

uint32      point2_cal_start,sys_cal_ok,user_cal_ok,point2_cal_step;

uint32      temp_comp_manual,manual_temp_comp_flag;
float       temp_float_ptc;

////////////////////////////增加自动进入待机功能
uint32      no_key_time;

////////////////////////////增加记录功能
uint32      rec_total_num,rec_real_num;  //记录当前存储的记录位置，初始化的时候设置
uint32      rec_total_pages,rec_curre_index;
//////////////////////
uint32 need_recover,need_recover2,from_menu;
uint32 save_buf[LINE_SIZE*SCR_YSIZE/1]; //被临时保存的缓冲区用于从数字键盘返回恢复
uint32 save_buf2[LINE_SIZE*SCR_YSIZE/1];//被临时保存的缓冲区用于返回到设置界面
//增加温度单位转换功能 打印功能
uint32 temp_unit,print_busy,Uart_Printer_Flag;

//增加LOGO开关功能 版本选择功能
uint32 version_811,model_display,LOGO_flag;


uint8 automode;
int   range_man;
int   range;
float ddlv_raw_data1,ddlv_raw_data2;


//定义用户校准使用的溶液 
//定义用户校准使用的溶液 
const   float standard_data_p811[][5] = 
{
 {1.680,4.003,6.864, 9.182,12.460},
 {1.679,4.005,7.000,10.012,12.454},
 {1.679,4.006,6.865, 9.180,12.454}
};

const   float standard_data_p801[][5] = 
{
 {1.68, 4.00, 6.86,  9.18, 12.46},
 {1.68, 4.01, 7.00, 10.01, 12.45},
 {1.68, 4.01, 6.87,  9.18, 12.45}
};
 
 /////////////////////////////////////具体的温补数据
const   float standard1_data[][17] = 
{
 {
  1.668,1.669,1.671,1.673,1.676,1.680,1.684,1.688,
  1.694,1.700,1.706,1.713,1.721,1.739,1.759,1.782,1.795
 },
 {
  4.006,3.999,3.996,3.996,3.998,4.003,4.010,4.019,
  4.029,4.042,4.055,4.070,4.087,4.122,4.161,4.203,4.224
 },
 {
  6.981,6.949,6.921,6.898,6.879,6.864,6.852,6.844,
  6.838,6.834,6.833,6.834,6.837,6.847,6.862,6.881,6.891
 },
 {
  9.458,9.391,9.330,9.276,9.226,9.182,9.142,9.105,
  9.072,9.042,9.015,8.990,8.968,8.926,8.890,8.856,8.839
 },
 {
  13.416,13.210,13.011,12.820,12.637,12.460,12.292,12.130,
  11.975,11.828,11.697,11.553,11.426,11.426,11.426,11.426,11.426
 }
};

const  uint32 standard1_data_temp[] = 
{
 0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,95,100
};

//定义温度补偿相关系数
const   float temp_coef[] = 
{
 54.197,55.189,56.181,57.173,58.165,59.157,60.149,61.141,62.133,63.126,64.118,
 65.110,66.102,67.094,68.086,69.078,70.070,71.062,72.054,73.046,74.038,74.038
};
const   float temp_data[] = 
{ //22个点
  0, 5,10,15,20,25,30,35,40, 45, 50,
 55,60,65,70,75,80,85,90,95,100,200
};

//电导率仪 定义用户校准使用的溶液 
const   float DDLV_standard_data[][4] = 
{
 {146.6, 1408, 12850, 111300},  //中国标准
 {146.6, 1408, 12850, 111300},  //外国标准1 
 {146.6, 1408, 12850, 111300}   //外国标准2
 
};



////////////////////////////////////////////////////////////
///////////////////////////////////////////////////
//functions 
//////////////////////////////////////////////////	
//////////////////////////////////////////////////
//1
//开机初始化 部分全局变量
///////////////////////////////////////////////////
void InitGlobalVarible(void)
	{
	 //Key_Status = KEY_RELEASE ;   //移到待机中初始化
	 ////////////////////////////added after
	 stable_flag       = FALSE;
	 power_on_flag     = TRUE;
	 point2_cal_start  = FALSE;
	 point2_cal_step   = 0;
	 current_mode      = STAT_NORMAL;
     //////////////////////////////////
     electrode_class   = 4;
     ////////////////////////////////
     //elec_cons = 1;
     no_key_time = 0;
     sound_switch = ON;
     
     temp_float_ptc = 0;
	}
 //////////////////////////////////////////////////
 //2
 //开机调取语言设置
 ///////////////////////////////////////////////////
uint8  InitLanguageVarible(void)
	{
	 uint32 i;
	 uint8  LangCalData[4];
	 
	 Read_EEPROM(EEP_LANGUAGE_ADDR, LangCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 if(LangCalData[3] != LangCalData[0] + REC_CHECK_DATA1)
	   {
	    Set_Language_Type(CHINESE);
	    return(ERROR);
	   } 
	  else
	   {
	    Set_Language_Type(LangCalData[0]);
	    return(OK);
	   }
	 } 	
 //////////////////////////////////////////////////
 //3a
 //开机调取声音设置
 ///////////////////////////////////////////////////
uint8  InitParamVariable(void)
	{
	 uint32 i;
	 uint8  tmp[16];
	
	 Read_EEPROM(EEP_UART_ADDR, tmp, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 //////////////////////////////////
	 if(tmp[3]==(tmp[0]+tmp[1]+tmp[2]))
	    Uart_Printer_Flag = tmp[2]&0x01;
	 else
	    Uart_Printer_Flag = 0x01;
	 /////////////////////////////////////
	 if(tmp[7]==(tmp[4]+tmp[5]+tmp[6]))
	    version_811 = tmp[6]&0x01;
	 else
	    version_811 = 0x01;

	 ////////////////////////////////////
	 if(tmp[11]==(tmp[8]+tmp[9]+tmp[10]))
	    model_display = tmp[10]&0x01;
	 else
	    model_display = 0x01;


	 if(tmp[15]==(tmp[12]+tmp[13]+tmp[14]))
	    LOGO_flag = tmp[14]&0x01;
	 else
	    LOGO_flag = 0x01;
	 } 		 
 //////////////////////////////////////////////////
 //3
 //开机调取屏幕亮度设置
 ///////////////////////////////////////////////////
uint8  InitBrightnessVarible(void)
	{
	 uint32 i;
	 uint8  BriCalData[4];
	 
	 Read_EEPROM(EEP_BRIGHT_ADDR, BriCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 if((BriCalData[3] == BriCalData[0] + REC_CHECK_DATA1)&&(BriCalData[0]<=BRIGHT_CLASS_TOTAL))
	   {
	    lcd_brightness = BriCalData[0];  
	   
	    Set_LCD_Brightness(lcd_brightness);
	    return(OK);
	   } 
	  else
	   {
	    lcd_brightness = DEFAULT_BRIGHTNESS;          //default 
	    Set_LCD_Brightness(lcd_brightness);
	    return(ERROR);
	   }
	 } 		
 //////////////////////////////////////////////////
 //3a
 //开机调取手动温度补偿的设置
 ///////////////////////////////////////////////////
uint8  InitTempCompVarible(void)
	{
	 uint32 i;
	 uint8  Temp[4];
	 
	 Read_EEPROM(EEP_TEMP_COMP_ADDR, Temp, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 //标志0x55aa is OK, 则最后两个字节内容可以判定为OK
	 if(Temp[3] == Temp[0] + Temp[1]+ REC_CHECK_DATA1)
	   {
	    temp_comp_manual = Temp[0];
	    temp_unit =  Temp[1];
	    return(OK);
	   } 
	  else
	   {
	    temp_comp_manual = DEFAULT_TEMP_COMP;          //default 
	    temp_unit =  TEMP_UNIT_OC;
	    return(ERROR);
	   }
	  
	 } 		  
 //////////////////////////////////////////////////
 //4
 //开机调取屏幕校准数据，如果误差太大，则使用默认数据
 ///////////////////////////////////////////////////
uint8  InitScreenVarible(void)
	{
	 uint32 i;
	 uint16 x00,y00,xmid0,ymid0,xmax0,ymax0;
	 uint8  ScreenCalData[13];
	 uint8  CheckSum;
	  
	 Read_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);
	 for(i=0;i<1000;i++){;}   //delay
	 CheckSum = 0;
	 for(i=0;i<12;i++)
	 CheckSum += ScreenCalData[i];
	 x00   = (ScreenCalData[0] << 8)  + ScreenCalData[1];
     y00   = (ScreenCalData[2] << 8)  + ScreenCalData[3];
	 xmid0 = (ScreenCalData[4] << 8)  + ScreenCalData[5];
	 ymid0 = (ScreenCalData[6] << 8)  + ScreenCalData[7];
	 xmax0 = (ScreenCalData[8] << 8)  + ScreenCalData[9];
	 ymax0 = (ScreenCalData[10] << 8) + ScreenCalData[11];
	 if((CheckSum != ScreenCalData[12])||Set_Originalx0y0(x00,y00)||Set_XmidYmid(xmid0,ymid0)||Set_XmaxYmax(xmax0,ymax0)) 
      {
	  x0 =   X0_DEFAULT;
	  y0 =   Y0_DEFAULT;
	  xmid = XMID_DEFAULT;
	  ymid = YMID_DEFAULT;
	  xmax = XMAX_DEFAULT;
	  ymax = YMAX_DEFAULT;
	  return(ERROR);                 //error
   	  }
	  else
	  return(OK);
	}
 //////////////////////////////////////////////////
 //5
 //开机调取校准数据
  ///////////////////////////////////////////////////
uint8  Init_Sys_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  SysCalData[16];
	 
	 Read_EEPROM(EEP_FULL4_ADDR1, SysCalData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 i = (SysCalData[0]<<24) + (SysCalData[1]<<16) + (SysCalData[2]<<8) + SysCalData[3];  
	 
	 if(0x55aaaa55 == i)
	 {
	  Read_EEPROM(EEP_ZERO_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  zero_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3]; 
	 
	  Read_EEPROM(EEP_LOW1_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  low1_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  Read_EEPROM(EEP_LOW2_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  low2_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  Read_EEPROM(EEP_FULL1_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full1_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
      //////////////////////////
	  Read_EEPROM(EEP_FULL2_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full2_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	   //////////////////////////
	  Read_EEPROM(EEP_FULL3_ADDR1, SysCalData, 16);
	  for(i=0;i<1000;i++){;}   //delay
	  for(i=0;i<4;i++)
	  full3_data[i] = (SysCalData[4*i+0]<<24) + (SysCalData[4*i+1]<<16) + (SysCalData[4*i+2]<<8) + SysCalData[4*i+3];
	  
	  sys_coef1 = 1300/(full1_data[0]-low1_data[0]+0.0001);
	  sys_coef2 = 1300/(full2_data[0]-low2_data[0]+0.0001);
	  sys_coef3 =  100/( low2_data[1]-zero_data[0]+0.0001);
	  sys_coef4 =  100/( low1_data[1]-zero_data[1]+0.0001);
	  sys_coef5 =  600/(full3_data[0]-full2_data[0]+0.0001);
	  
	  sys_cal_ok = TRUE; 
	  return(OK);
	 }
	 else
	  {
	   sys_cal_ok = FALSE;
	   return(ERROR);
	  }
    }	
 /////////////////////////////////////////////////////
 
 /////////////////////////////////////////////////////
 /////////////////////////////////////////////////////
 uint8  Init_Sys_Cal2_Varible(void)
	{
	 uint32 i,j;
	 uint8  rx_buf[16];
	 
	 Read_EEPROM(EEP_DDLV_SYSCAL_CHECK, rx_buf, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 
	 i = (rx_buf[3]<<24) + (rx_buf[2]<<16) + (rx_buf[1]<<8) + rx_buf[0];  
	 
	 if(0xbbaa9988 == i)
	 {
	   for(j=0;j<6;j++) {
	       Read_EEPROM(EEP_DDLV_SYSCAL_RANGE1 + 0x10*j, rx_buf, 16);
	       for(i=0;i<1000;i++){;}   //delay
	       for(i=0;i<7;i++) {   
	           ddlv_syscal_buf[j][i] = (rx_buf[2*i+1]<<8) + rx_buf[2*i];
	        
	       } 
	   }
	   sys_cal_ok = TRUE; 
	   rmap_var_init();
	   return(OK);
	 }
	 else
	 {
	    for(j=0;j<6;j++) {
	       for(i=0;i<7;i++) {   
	           ddlv_syscal_buf[j][i] = 0;
	       }
	    }     
	  sys_cal_ok = FALSE;
	  rmap_var_init();
	  return(ERROR);
	 }	
    
  }
 /////////////////////////////////////////////////////
 //////////
uint32  Get_electrode_class(void)
     {
       uint32 i,class_tmp;
       float  tmp;
       tmp = 0;
       for(i=0;i<user_cal_num;i++)
         tmp = tmp + fabs(user_cal_phdata[i].rec_float - user_cal_phdata_dis[i].rec_float);
       
       tmp = tmp/user_cal_num;
       
       if(tmp < 0.1)
          class_tmp = 5;
       else if(tmp < 0.2)
          class_tmp = 4;
       else if(tmp < 0.3)
          class_tmp = 3;
       else if(tmp < 0.4)
          class_tmp = 2;
       else 
          class_tmp = 1;
       //////////////////
	   return(class_tmp);
	 }           
 //////////////////////////////////////////////////////////////////////////////
 //
 //
 ///////////////////////////////////////////////////////////////////////////////
 float Get_One_Coef(float display_value,float real_value,uint32 special_value)
    {
     float tmp;
     
     if(1 == special_value)
       {
        tmp = (18 - display_value)/(18 - real_value);    
       }
     else
       {
        tmp =  display_value /  real_value; 
       }    
     return(tmp);  
    }
 
    
 void  Get_User_Coef123(void)
	  {
	   uint32 i;
	   uint8  UserCalData[16];
	   float  tmp1,tmp2;
	   
	    
	   Read_EEPROM(EEP_USER_CAL_ADDR1, UserCalData, 16); 
	   /////////////////////////////////////////////
	   user_cal_phdata_dis[0].rec_uint32 = (UserCalData[0]<<24) + (UserCalData[1]<<16)  + (UserCalData[2]<<8) + UserCalData[3];
	   user_cal_phdata[0].rec_uint32 = (UserCalData[4]<<24) + (UserCalData[5]<<16)  + (UserCalData[6]<<8) + UserCalData[7];
	   user_coef1      =  Get_One_Coef(user_cal_phdata_dis[0].rec_float,user_cal_phdata[0].rec_float,0);//得到第一个系数
	   if(1 == user_cal_num)
	    { //得到最后的系数
	     user_coef_left = Get_One_Coef(user_cal_phdata_dis[0].rec_float,user_cal_phdata[0].rec_float,1);
	     //
	    }
	   else
	    {
	      user_cal_phdata_dis[1].rec_uint32 = (UserCalData[8]<<24) + (UserCalData[9]<<16)  + (UserCalData[10]<<8)  + UserCalData[11];
	      user_cal_phdata[1].rec_uint32 = (UserCalData[12]<<24) + (UserCalData[13]<<16)  + (UserCalData[14]<<8) + UserCalData[15];
	      tmp1 = user_cal_phdata_dis[1].rec_float - user_cal_phdata_dis[0].rec_float;
	      tmp2 = user_cal_phdata[1].rec_float     - user_cal_phdata[0].rec_float;
	      user_coef2      =  Get_One_Coef(tmp1,tmp2,0);//得到第二个系数     
	      if(2 == user_cal_num)
	       {
	        user_coef_left = Get_One_Coef(user_cal_phdata_dis[1].rec_float,user_cal_phdata[1].rec_float,1);
	        //electrode_class = Get_electrode_class();
	       }
	      else
	       {
	         for(i=0;i<1000;i++){;}   //delay
	         Read_EEPROM(EEP_USER_CAL_ADDR3, UserCalData, 16);  //读取第三四个校准溶液
	         user_cal_phdata_dis[2].rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];
	         user_cal_phdata[2].rec_uint32 = (UserCalData[4]<<24)  + (UserCalData[5]<<16)   + (UserCalData[6]<<8)  + UserCalData[7];
	         tmp1 = user_cal_phdata_dis[2].rec_float - user_cal_phdata_dis[1].rec_float;
	         tmp2 = user_cal_phdata[2].rec_float     - user_cal_phdata[1].rec_float;
	         user_coef3      =  Get_One_Coef(tmp1,tmp2,0);//得到第三个系数
	         if(3 == user_cal_num)
	          {
	           user_coef_left = Get_One_Coef(user_cal_phdata_dis[2].rec_float,user_cal_phdata[2].rec_float,1);
	           //electrode_class = Get_electrode_class();
	          }
	         else 
	          {
	           user_cal_phdata_dis[3].rec_uint32 = (UserCalData[8]<<24) + (UserCalData[9]<<16)  + (UserCalData[10]<<8)  + UserCalData[11];
	           user_cal_phdata[3].rec_uint32 = (UserCalData[12]<<24) + (UserCalData[13]<<16)  + (UserCalData[14]<<8) + UserCalData[15];
	           tmp1 = user_cal_phdata_dis[3].rec_float - user_cal_phdata_dis[2].rec_float;
	           tmp2 = user_cal_phdata[3].rec_float     - user_cal_phdata[2].rec_float;
	           user_coef4      =  Get_One_Coef(tmp1,tmp2,0);//得到第四个系数     
	           if(4 == user_cal_num)
	            {
	             user_coef_left = Get_One_Coef(user_cal_phdata_dis[3].rec_float,user_cal_phdata[3].rec_float,1);
	             //electrode_class = Get_electrode_class();
	            }
	           else
	            {
	              for(i=0;i<1000;i++){;}   //delay
	              Read_EEPROM(EEP_USER_CAL_ADDR5, UserCalData, 8);  //读取第五个校准溶液
	              user_cal_phdata_dis[4].rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];
	              user_cal_phdata[4].rec_uint32 = (UserCalData[4]<<24)  + (UserCalData[5]<<16)   + (UserCalData[6]<<8)  + UserCalData[7];
	              tmp1 = user_cal_phdata_dis[4].rec_float - user_cal_phdata_dis[3].rec_float;
	              tmp2 = user_cal_phdata[4].rec_float     - user_cal_phdata[3].rec_float;
	              user_coef5      =  Get_One_Coef(tmp1,tmp2,0);//得到第五个系数
	              user_coef_left = Get_One_Coef(user_cal_phdata_dis[4].rec_float,user_cal_phdata[4].rec_float,1);
	              //electrode_class = Get_electrode_class();
	            
	            }   
	         }    
	      }   
	   }
	  //得到电极状态 
	  electrode_class = Get_electrode_class();  	
    }
 //////////////////////////////////////////////////
 //5
 //开机调取校准数据
  ///////////////////////////////////////////////////
uint8  Init_User_Cal_Varible(void)
	{
	 float  tmp;
	 uint32 i;
	 uint8  UserCalData[16];
	 
	 Read_EEPROM(EEP_USER_CAL_FLAG, UserCalData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 i = (UserCalData[3]<<16) + (UserCalData[2]<<8) + UserCalData[1];  
	 
	 if(0x55aa55 == i)
	  {
	   user_cal_num = UserCalData[0];
	   user_cal_point[0] = UserCalData[8];
	   user_cal_point[1] = UserCalData[9];
	   user_cal_point[2] = UserCalData[10];
	   user_cal_point[3] = UserCalData[11];
	   user_cal_point[4] = UserCalData[12];
	  
	   user_cal_ok = TRUE;
	   Get_User_Coef123();
	   return(OK);    
	  }
	 else
	  {
	   user_cal_ok = FALSE;
	   return(ERROR);
	  }
    }    
    
//////////////////////////////////////////////////
 //5
 //开机调取校准数据
  ///////////////////////////////////////////////////
uint8  Init_User_Cal_Varible2(void)
	{
	 un_rec_data  tmp;
	 uint32 i;
	 uint8  UserCalData[16];
	 
	 Read_EEPROM(EEP_DDLV_USER_CAL1, UserCalData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((REC_CHECK_DATA1==UserCalData[14])&&(REC_CHECK_DATA2==UserCalData[15]))
	  {
	   tmp.rec_uint32 = (UserCalData[0]<<24)  + (UserCalData[1]<<16)   + (UserCalData[2]<<8)  + UserCalData[3];  
	   ddlv_coef = tmp.rec_float;
	   user_cal_ok = TRUE;
	   return(OK);    
	  }
	 else
	  {
	   ddlv_coef   = 1;
	   user_cal_ok = FALSE;
	   return(ERROR);
	  }
    }        
  //////////////////////////////////////////////////
 //5
 //开机调取 选择 的哪种国家标准
  ///////////////////////////////////////////////////
uint8  Init_StandVar_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  StandData[4];
	 
	 Read_EEPROM(EEP_STANDARD_ADDR, StandData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(StandData[3] == StandData[0] + REC_CHECK_DATA1)
	  {
	   which_standard =  StandData[0]; 
	   return(OK);
	  }
	 else
	  {
	   which_standard =  CHINESE_S;
	   return(ERROR);
	  }
    }    
  //////////////////////////////////////////////////
 //5
 //开机调取用户选择的模式相关的数据
  ///////////////////////////////////////////////////   
   uint8  Init_Mode_Config_Varible(void)
	{
	 uint32 i;
	 un_rec_data tmp;
	 uint8  ModeData[16];
	 
	 Read_EEPROM(EEP_DDLV_MODE_ADDR, ModeData, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((ModeData[14]==REC_CHECK_DATA1)&&(ModeData[15]==REC_CHECK_DATA2))
	  {
	   DDLV_Param = ModeData[0];
       DDLV_Mode  = ModeData[1];
       measure_timer_sec = (ModeData[2]<<8) + ModeData[3];
       balance_sec       = ModeData[5];
       tmp.rec_uint32 = (ModeData[6]<<24)  + (ModeData[7]<<16)   + (ModeData[8]<<8)  + ModeData[9];
       balance_data = tmp.rec_float;
       //
       measure_timer_sec_real = measure_timer_sec;
  	   balance_sec_real = balance_sec;
  	           
	   return(OK);
	  }
	 else
	  {
	   DDLV_Param = PARAM_DDLV; 
	   DDLV_Mode  = MODE_CONTINUE;
	   measure_timer_sec = 999;
	   balance_data = 999.99;
       balance_sec  = 59;
	   //
       measure_timer_sec_real = measure_timer_sec;
  	   balance_sec_real = balance_sec;
	   return(ERROR);
	  }
    }    
   //////////////////////////////////////////////////
 //5a
 //开机调取DDLV 常数设置
  ///////////////////////////////////////////////////   
   uint8  Init_DDLV_CONS_Varible(void)
	{
	 uint32 i;
	 uint8  CONS_Data[16];
	 un_rec_data tmp;
	 
	 Read_EEPROM(EEP_DDLV_CONS_ADDR, CONS_Data, 16);
	 for(i=0;i<1000;i++){;}   //delay
	 if((CONS_Data[14]==REC_CHECK_DATA1)&&(CONS_Data[15]==REC_CHECK_DATA2))
	  {
	   tmp.rec_uint32 = (CONS_Data[0]<<24) + (CONS_Data[1]<<16)  + (CONS_Data[2]<<8)  + CONS_Data[3];
       elec_cons = tmp.rec_float;
       tmp.rec_uint32 = (CONS_Data[4]<<24) + (CONS_Data[5]<<16)  + (CONS_Data[6]<<8)  + CONS_Data[7];
       temp_cons  = tmp.rec_float;   
       tmp.rec_uint32 = (CONS_Data[8]<<24) + (CONS_Data[9]<<16)  + (CONS_Data[10]<<8) + CONS_Data[11];
       TDS_cons  = tmp.rec_float;    
             
	   return(OK);
	  }
	 else
	  {
	   elec_cons = 1.0;
	   temp_cons = 2.0;
	   TDS_cons  = 0.5;
	 
	   return(ERROR);
	  }
    }    
       
  //////////////////////////////////////////////////
 //5
 //开机调取校准数据
  ///////////////////////////////////////////////////
uint8  Init_DivisionVar_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  DIVData[4];
	 
	 Read_EEPROM(EEP_DIV_ADDR, DIVData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(DIVData[3] == DIVData[0] + REC_CHECK_DATA1)
	  {
	   phmv_division =  DIVData[0]; 
	   return(OK);
	  }
	 else
	  {//8421
	   phmv_division =  DEFAULT_PH_DISPLAY_DOT + DEFAULT_MV_DISPLAY_DOT;
	   return(ERROR);
	  }
    }
    
   //5
 //开机调取校准数据
  ///////////////////////////////////////////////////
uint8  Init_UserID_Cal_Varible(void)
	{
	 uint32 i;
	 uint8  UserIData[4];
	 
	 Read_EEPROM(EEP_USER_ID_ADDR, UserIData, 4);
	 for(i=0;i<1000;i++){;}   //delay
	 if(UserIData[3] == UserIData[0] + UserIData[1] + UserIData[2])
	  {
	   user_id =  (UserIData[1]<<8) + UserIData[0]; 
	   return(OK);
	  }
	 else
	  {
	   user_id =  DEFAULT_USER_ID;
	   return(ERROR);
	  }
    }
    
    
//////////////////////////////////////////////////
//8a
//开机查找当前记录的存放位置
//每条记录占用16个字节

///////////////////////////////////////////////////	     
void  Search_Rec_Pos(void)
     {
       uint32 i,j;
 	   uint8  BufferData[8];
	 
       Read_EEPROM(EEP_REC_TOTAL_ADDR,BufferData,8);
	   for(j=0;j<1000;j++){;}   //delay
	   
	   i = (BufferData[0]<<24)+(BufferData[1]<<16)+(BufferData[2]<<8)+BufferData[3];
	   j = (BufferData[4]<<24)+(BufferData[5]<<16)+(BufferData[6]<<8)+BufferData[7];
	   
	   if((0xffffffff==i)&&(0xffffffff==j)||(i!=j))
	   {//空芯片
	    for(i=0;i<8;i++)
	       BufferData[i] = 0;
	    Write_EEPROM(EEP_REC_TOTAL_ADDR,BufferData, 8); //初始化为0
		for(j=0;j<40000;j++)	{;}	                 //delay
		//默认状态
		rec_total_num = 0;  //累加数据多少，因此提供了寻址
		rec_real_num  = 0;  //有效数据有多少，最多只能是系统可存储数目
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }
	   ////////////////////////////////////////////检查
	   else if(i==j)
	   {                        //正确,返回当前可以添加的
	    rec_total_num = i;      //当前总记录数
	   
	    if(i < RECNUM_PER_PAGE) //当前记录数小于一页
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
		  rec_total_pages = 1;
	     }
	    else if(i < REC_MAX_NUM) //当前记录数还未处于覆盖状态
	     {
	      rec_real_num    = i;
	      rec_curre_index = i;
	      rec_total_pages = i/RECNUM_PER_PAGE + 1; //有多页
	     } 
	    else
	     { //当前记录数已经处于覆盖模式
	      rec_real_num    = REC_MAX_NUM;  //有效数固定为最大数
	      rec_curre_index = i%REC_MAX_NUM;  
	      rec_total_pages = REC_MAX_NUM/RECNUM_PER_PAGE; //页数固定
	     }
	   }
	  /* else
	   {//异常处理认为无数据
	    rec_total_num = 0;
		rec_curre_index = 0;
		rec_total_pages = 1;
	   }*/
    }   
///////////////////////////////////////////////////////////
//9      显示已耗费时间
///////////////////////////////////////////////////////////
void  Display_elapsed_time(uint32 time_sec)
   {  
       uint32 i,hour,mint,sec;
       hour = 0;
       mint = 0;
       sec  = 0;        
       if(time_sec>3599)
         {
          hour = time_sec/3600;
          time_sec = time_sec%3600;
          }
       if(time_sec>59)
         {
          mint = time_sec/60;
          time_sec = time_sec%60;
          }        
          sec = time_sec;
                  
         Put_Data(150+515,        30,"%02d", hour);
	     Put_Char(150+485+4*15+3, 30,0x3a);
         Put_Data(150+560,        30,"%02d", mint);
	     Put_Char(150+485+7*15+3, 30,0x3a);
	     Put_Data(150+605,        30,"%02d", sec); 
   
     }	     
	    
//////////////////////////////////////////////////
//10  待机画面显示
///////////////////////////////////////////////////
void  Display_Sleep_Area(void)
	{
      Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);
     
	  if(ENGLISH == Get_Language_Type())
	     Put_Bmp(SCR_XSIZE-120,410,start11060e,0);    //display START
      else
         Put_Bmp(SCR_XSIZE-120,410,start11060c,0);    //display START
    
      Set_ASC_Lib(Arial16B_Font);
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      Put_Str(Language_Pos(150+400,150+370), 5,Language_Str("系统时间"," System Time"));
      Put_Str(Language_Pos(150+400,150+370),30,Language_Str("运行时间","Elapsed Time"));
     }

///////////////////////////////////////////////////////////
//11
//    待机时间显示
///////////////////////////////////////////////////////////
void Display_Sleep_Time(void)
  { 
   RTCTime local_time_buf;
   if(Sec_Change_Flag)
        {
         Sec_Change_Flag = FALSE;
      
         local_time_buf = RTCGetTime();
         Set_ASC_Lib(Arial20B_Font);
         Set_Chr_Color(COL_WHITE,COL_BLACK);
         //display time
	     Put_Data(150+515,        5,"%02d", local_time_buf.RTC_Hour);
	     Put_Char(150+485+4*15+3, 5,0x3a);
         Put_Data(150+560,        5,"%02d", local_time_buf.RTC_Min);
	     Put_Char(150+485+7*15+3, 5,0x3a);
	     Put_Data(150+605,        5,"%02d", local_time_buf.RTC_Sec);
	     Display_elapsed_time(elapsed_time);
        } 
   }  
///////////////////////////////////////////////////////////
//12
//    开机预热时间不够，进入待机模式
///////////////////////////////////////////////////////////      
void  Sleep_process(void)
    {
      
      uint32 i,X_position,Y_position,Quit_flag;
      
      Quit_flag = FALSE;
         
      InitScreenVarible();         //把屏幕校准数据提前调出来
      InitLanguageVarible();       //把语言参数提前调出来 
      InitBrightnessVarible();     //屏幕亮度
      
      Key_Status = KEY_RELEASE+KEY_DO_OVER ;   //key release
      elapsed_time = 0;            //
         
      Display_Sleep_Area();  
        
      do
      {
       ///////////////////////////////keyscan
       for(i=0;i<1000;i++){;}   //delay
       key_value = Get_PositionXY();   //键盘扫描
       if(Key_Status == KEY_DO_START)
       {
	     X_position = (key_value>>16)&0x0000ffff;
	     Y_position =  key_value & 0x0000ffff;
	     if((X_position>670)&&(X_position<780)&&(Y_position>410)&&(Y_position<470))
	         Quit_flag = 0x55aa;
	      else
	         Quit_flag = FALSE;
	     Key_Status = KEY_DO_OVER;  
        }
       /////////////////////////////display time
       Display_Sleep_Time();
      
       ////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time > PREHOT_TIME)) //预热时间大于30分钟
           Quit_flag = TRUE;
       /////////////////////////////////////////////////////////////////////
       if((0x55aa == Quit_flag)&&(elapsed_time < PREHOT_TIME)) //预热时间小于30分钟
        {
         ////////////////////////借用PCS输入状态采集开机密码
         UINTInput_Interface();
         Set_ASC_Lib(Arial16B_Font);
         Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
         Put_Str(Language_Pos(230-100,200-100),85,Language_Str("系统预热时间不足 请输入开机密码","Not Enough Prehot,Please input Password")); 
       
         do
         {
           i = UINTInput_Key_Pro(Get_PositionXY(),PASSWORD_INPUT_ID); //
               UINTInput_Disp_Pro();
           if((i==PASSWORD_DATA1)||(i==PASSWORD_DATA2))
             {
              Quit_flag = TRUE;  
             }
             else if(i != PASSWORD_INPUT_ID) //密码无效，重新刷新输入
              UINTInput_Interface(); 
          } 
          while(TRUE !=Quit_flag);
         }
      }   
      while(FALSE == Quit_flag);
    }
 
//////////////////////////////////////////////////
//13 开机初始化所有全局变量
//return 
///////////////////////////////////////////////////
uint8  SystemInit(void)
{   
    uint32 key_times=0;
    uint32 key_buf[4]={0,0,0,0};
    uint32 i,j,key_posx,key_posy;
     
    InitGlobalVarible();
    Search_Rec_Pos();        //查找当前存放的记录位置
     
    InitScreenVarible();         //把屏幕校准数据提前调出来
    InitLanguageVarible();       //把语言参数提前调出来 
    InitBrightnessVarible();     //屏幕亮度
      
    ////////added by wjg 20151118
    InitParamVariable(); 
  
    //DRAW TITLE
    Clr_Bar(0,0,800,100,COL_BLACK);								//TITLE
	
	//Put_Bmp(0,0,logo22560,0);
   
	Clr_Bar(0,100,800,380,COL_BLACK);
	
		
	//test end
	
	Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);  
  
  	if(1==version_811)  
     Put_Str(280,160,Language_Str("P812型 电导率计",  "P812  Electric Conductometer"));
    else
     Put_Str(280,160,Language_Str("P802型 电导率仪",  "P802  Electric Conductometer"));
  
  	 Put_Str(280,210,Language_Str("系统初始化 ......",  "System initializing......"));
	
	Clr_Bar(0,300,800,35,COL_DEEPRED);
	
	for(i=0;i<SCR_XSIZE/2;i++)
	{
	Clr_Bar(0,300,i*2,35,COL_DEEPGREEN);
    if(i < SCR_XSIZE/4)
	Set_Chr_Color(COL_BLACK,COL_DEEPRED);
	else
	Set_Chr_Color(COL_BLACK,COL_DEEPGREEN);
	Put_Numeral(310, 310,i/4,1);
	Put_Char(310+30,310,'%');     //display 88%
	for(j=0;j<300;j++){;}    //调整进度条快慢
//////////////////////////////////////////////////////STEP1:SCREENCAL_data
	if(i == 50)      //
	 {
	 if(ERROR == InitScreenVarible())
     {
      Set_Chr_Color(COL_DEEPRED,COL_BLACK);
	  Put_Str(200,380,"ScreenCAL ERROR!,Please Recal Screen");
	 }
	else
     {
      Set_Chr_Color(COL_THINGREEN,COL_BLACK);
      Put_Str(200,380,"Screen CAL Data OK!");
	 }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP2:CAL_DATA
	if(i == 70)
	{
	 
	
	  if(ERROR == Init_Sys_Cal2_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"SystemData ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"System Data OK!");
	    }
    
      for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}	
			
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 90)
	{
	   j = Init_User_Cal_Varible2();
	
	  if(ERROR == j)
	    {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"UserData ERROR!,Use Default Data");
	   	}
	  else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"UserData OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 110)
	{
	 if(ERROR == Init_StandVar_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"StandardVar ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"StandardVar Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 130)
	{
	 if(ERROR == Init_DivisionVar_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"DivisionVar ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"DivisionVar Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
////////////////////////////////////////////////////STEP3:linecal data
	if(i == 150)
	{
	 if(ERROR == Init_UserID_Cal_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"UserID ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"UserID Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
///////////////////////////////////////////////////////////////////
	if(i == 170)
	{
	 if(ERROR == Init_Mode_Config_Varible())  //读取上次 的测量模式 
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"MODE CONFIG ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"MODE Data OK!");
	    }
	  for(j=0;j<990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}	
	//////////////////////////////////////////读取DDLV 常数
    if(i == 185)
	{
	 if(ERROR == Init_DDLV_CONS_Varible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"DDLV_CONS CONFIG ERROR!,Use Default Data");
	   	 }
	   else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"DDLV_CONS Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	  
	}		
////////////////////////////////////////////////////STEP3:temp data
	if(i == 200)
	{
	   if( ERROR == InitTempCompVarible())
   	     {
         Set_Chr_Color(COL_DEEPRED,COL_BLACK);
  	     Put_Str(200,380,"Temperature Data ERROR!");
	   	 }
	    else
        {
         Set_Chr_Color(COL_THINGREEN,COL_BLACK);
	     Put_Str(200,380,"Temperature Data OK!");
	    }
	  for(j=0;j<1990000;j++){;}    //warning delay
	  Clr_Bar(200,380,500,30,COL_BLACK);
	}
////////////////////////////////////////////////////STEP4:检测开机键盘并进入相应的界面
	if((i>50) && (key_times<4))  //监测4个按键
	{
	 key_value = Get_PositionXY();   //键盘扫描
	 if(Key_Status == KEY_DO_START)
     {
      Key_Status = KEY_DO_OVER;           //开始处理
      key_times++;
                                          //处理完毕
     }	
	}
   }  
/////////////////////////////////////////////////STEP6:GLOBAL VARIABLE
    if(key_times > 1)
     return(MACHINE_SERVICE_MODE);     //维修模式
    else 
     return(MACHINE_NORMAL_MODE);        //正常模式
}
///////////////////////////////////////////////////
//14  根据X Y位置构建按键数据 
//////////////////////////////////////////////////
void  Create_ButtonList(uint32 (*p)[2],uint32 num)
	{
	         uint32 i;
	 struct  KeyStruct *pKey;
	 pKey = Key;
	 for(i=0;i<num;i++)
	 	{
		(*pKey).X_Position = (*p)[0];
		(*pKey).Y_Position = (*p)[1];
		(*pKey).Value      = i+1;
		 pKey++;
		 p++;
		}		
	}
	
///////////////////////////////////////////////////////////
// 根据标准 和 实时温度 寻找要匹配的校准数据
//返回0: 
// 返回 浮点数 有问题????? 所以返回UINT32
///////////////////////////////////////////////////////////
uint32  Search_user_cal(uint32 cal_point)
     {
      uint32  i;
      un_rec_data tmp;
      switch(which_standard)
       {
        case CHINESE_S:
                       //temp_p = standard1_data[user_cal_point[0]][0]; 
                       //num = 17;
                       ////////////////////
                       if(TRUE == manual_temp_comp_flag)
                          tmp.rec_float = (float)temp_comp_manual;
                       else
                          tmp.rec_float = temp_float_ptc;
                       ////////////////////////////////   
                       for(i=0;i<18;i++)
                          {
                           if(tmp.rec_float < (standard1_data_temp[i]+0.00001))
                              break;
                          }
                       //////////////////////i 就是当前比较的上限数组
                       if(i==0)
                          tmp.rec_float = standard1_data[cal_point][0];
                       else if(i==17)
                          tmp.rec_float = standard1_data[cal_point][16];
                       else
                         {
                          tmp.rec_float = tmp.rec_float - standard1_data_temp[i-1];//在两点之间的位置(0-5
                          //计算两点之间的温度 所对应的PH
                          tmp.rec_float = tmp.rec_float * (standard1_data[cal_point][i]-standard1_data[cal_point][i-1])/5;
                          tmp.rec_float += standard1_data[cal_point][i-1];
                         }
                       break;
        case EUROPE_S:
                       tmp.rec_float = standard_data_p811[1][cal_point];
                       break;
        case AMERICAN_S:
                       tmp.rec_float = standard_data_p811[2][cal_point];
                       break;
        default:       break;                
       }
     /////////////////////////////////////
      return(tmp.rec_uint32);  
     } 

///////////////////////////////////////////////////////////
// 温补函数 根据温度寻找到要使用的 转换系数
// 返回 系数
///////////////////////////////////////////////////////////
float   Search_Coef_Temp(void)
       {
        float temp,tmp;
        uint32 i;
        if(TRUE == manual_temp_comp_flag)
           temp = temp_comp_manual;
        else
           temp = temp_float_ptc;
        ////////////////////////////////   
        for(i=0;i<22;i++)
          {
          
          if(temp < (temp_data[i]+0.00001))
             break;
          }
        //////////////////////i 就是当前比较的上限数组
        if(i==0)
          tmp = temp_coef[0];
        else 
          {
           tmp = temp - temp_data[i-1];//在两点之间的位置(0-5
            //计算两点之间的温度 所对应的PH
           tmp = tmp * (temp_coef[i]-temp_coef[i-1])/5;
           tmp += temp_coef[i-1];
          }       
        ///////////////////////////////////////////// 
        get_coef = tmp;
        
       }
       
///////////////////////////////////////////////////////////
//20 保存到指定位置一条记录
//并更新相关变量
//返回0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_Rec(void)
    {
     un_rec_data rec_data;
     uint32 i;
     uint8 check_data;
     uint8 Send_Data[16];
     RTCTime LocalTime;   
     //if(FALSE == stable_flag)
     //{
     // Log_Alm(&tips_save_fault,ALM_LEV_TIPS);  //
     //}
     //else
     //{ 
       LocalTime = RTCGetTime();
       Send_Data[0] = (user_id>>8)&0xff;
       Send_Data[1] =      user_id&0xff;
       if(TRUE == manual_temp_comp_flag)
        {
         Send_Data[2] = (uint8)temp_comp_manual;//
         Send_Data[3] = 0;
        }
       else
        {
         Send_Data[2] = (uint8)temp_float_ptc;//
         Send_Data[3] = (uint8)(((uint32)(temp_float_ptc*10))%10);
        }  
        
       //月日小时分钟  4 5 5 6
       i=(LocalTime.RTC_Mon<<16)+(LocalTime.RTC_Day<<11)+(LocalTime.RTC_Hour<<6)+(LocalTime.RTC_Min&0x3f);
       Send_Data[4] = i&0xff;
       Send_Data[5] = (i>>8) &0xff;
       Send_Data[6] = (i>>16)&0xff;
     
       rec_data.rec_float = ph_data;
       Send_Data[7]  = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[8]  = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[9]  =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[10] =       rec_data.rec_uint32&0xff;
       rec_data.rec_float = mv_data;
       Send_Data[11] = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[12] = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[13] =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[14] =       rec_data.rec_uint32&0xff;
       
       ////////////////////////计算15个字节的 校验和
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	   for(i=0;i<20000;i++)	{;}	         //delay
	  
	   for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	   Read_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	   for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //更新总记录数
        
        if(rec_real_num < REC_MAX_NUM) //更新有效条数
        rec_real_num++;
                                       //更新总页数 
        rec_total_pages =  rec_real_num/RECNUM_PER_PAGE + 1;
                                      
        rec_curre_index++;             //更新索引
        if(REC_MAX_NUM == rec_curre_index)
        rec_curre_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        Log_Alm(&tips_save_ok,ALM_LEV_TIPS);  //正常保存
       }
       else
       {
        Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
    //}
  }  

///////////////////////////////////////////////////////////
//20 保存到指定位置一条记录
//并更新相关变量
//返回0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_RecDDLV(void)
    {
     un_rec_data rec_data;
     uint32 i;
     uint8 check_data;
     uint8 Send_Data[16];
     RTCTime LocalTime;   
    
       LocalTime = RTCGetTime();
       Send_Data[0] = (user_id>>8)&0xff;
       Send_Data[1] =      user_id&0xff;
       if(TRUE == manual_temp_comp_flag)
        {
         Send_Data[2] = (uint8)temp_comp_manual;//
         Send_Data[3] = 0;
        }
       else
        {
         Send_Data[2] = (uint8)temp_float_ptc;//
         Send_Data[3] = (uint8)(((uint32)(temp_float_ptc*10))%10);
        }  
        
       //月日小时分钟  4 5 5 6
       i=(LocalTime.RTC_Mon<<16)+(LocalTime.RTC_Day<<11)+(LocalTime.RTC_Hour<<6)+(LocalTime.RTC_Min&0x3f);
       Send_Data[4] = i&0xff;
       Send_Data[5] = (i>>8) &0xff;
       Send_Data[6] = (i>>16)&0xff;
     
       rec_data.rec_float = ddlv_data;
       Send_Data[7]  = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[8]  = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[9]  =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[10] =       rec_data.rec_uint32&0xff;
       rec_data.rec_float = mv_data;
       Send_Data[11] = (rec_data.rec_uint32>>24)&0xff;
       Send_Data[12] = (rec_data.rec_uint32>>16)&0xff;
       Send_Data[13] =  (rec_data.rec_uint32>>8)&0xff;
       Send_Data[14] =       rec_data.rec_uint32&0xff;
       
       ////////////////////////计算15个字节的 校验和
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	   for(i=0;i<20000;i++)	{;}	         //delay
	  
	   for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	   Read_EEPROM(EEP_REC_START_ADDR+rec_curre_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	   for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //更新总记录数
        
        if(rec_real_num < REC_MAX_NUM) //更新有效条数
        rec_real_num++;
                                       //更新总页数 
        rec_total_pages =  rec_real_num/RECNUM_PER_PAGE + 1;
                                      
        rec_curre_index++;             //更新索引
        if(REC_MAX_NUM == rec_curre_index)
        rec_curre_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        Log_Alm(&tips_save_ok,ALM_LEV_TIPS);  //正常保存
       }
       else
       {
        Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
  }  

//////////////////////////////////////////////////////////////////////////////////
//
//匹配激励电源和反馈电阻，让ADS1100得到合适的数据
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//
// 温补过程
//
//////////////////////////////////////////////////
void  temp_comp_proc(void)
   {
    uint32 i;
    float  tmp;
    ///////////////////////////////////得到需要补偿的温度
    if(TRUE == manual_temp_comp_flag)
     tmp = temp_comp_manual; 
    else
     tmp = temp_float_ptc;
    ///////////////////////////////////根据温度偏离25标准度的差值和 温度系数 调整 ddlv_data
    if(tmp > 25)      
     ddlv_data = ddlv_data*(1 - (tmp-25)*temp_cons/100);
    else
     ddlv_data = ddlv_data*(1 + (25-tmp)*temp_cons/100);
    
   }
   
//////////////////////////////////////////
//恢复出厂设置 以下内容
//用户ID  亮度 
//用户校准数据     
//
//////////////////////////////////////////////////
void  do_factory_call(void)
    {
     uint8 tx_buf[16];
     uint32 i;
     ///////////////UID   
     user_id   = DEFAULT_USER_ID; 
     tx_buf[0] = (uint8)(user_id&0xff);
  	 tx_buf[1] = (uint8)((user_id>>8)&0xff);
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0]+tx_buf[1]+tx_buf[2]; 
  	 Write_EEPROM(EEP_USER_ID_ADDR,tx_buf, 4); 
  	 for(i=0;i<100000;i++);
  	 
     ///////////////BRIGHT   
     lcd_brightness = DEFAULT_BRIGHTNESS;
     Set_LCD_Brightness(lcd_brightness);
     tx_buf[0] = (uint8)(lcd_brightness);
  	 tx_buf[1] = 0;
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	 Write_EEPROM(EEP_BRIGHT_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////PH MV display dot  
     phmv_division = DEFAULT_PH_DISPLAY_DOT + DEFAULT_MV_DISPLAY_DOT; 
     tx_buf[0] = (uint8)(phmv_division);
  	 tx_buf[1] = 0;
  	 tx_buf[2] = 0;
  	 tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	 Write_EEPROM(EEP_DIV_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////温补单位 数值
     temp_comp_manual = DEFAULT_TEMP_COMP;    //25摄氏度
     temp_unit        = TEMP_UNIT_OC;      
     tx_buf[0] = (uint8)temp_comp_manual;
     tx_buf[1] = temp_unit;
     tx_buf[2] = 0;
     tx_buf[3] = tx_buf[0] + tx_buf[1] + REC_CHECK_DATA1;
     Write_EEPROM(EEP_TEMP_COMP_ADDR,tx_buf, 4);
     for(i=0;i<100000;i++); 
     
     ///////////////把用户校准数据删除
     Clear_User_Cal();
	 	  
     //以下为DDLV 参数
    
     DDLV_Param = PARAM_DDLV; 
	 DDLV_Mode  = MODE_CONTINUE;
	 measure_timer_sec = 999;
	 balance_data = 999.99;
     balance_sec  = 59;
	 ////////////////////////////////////////////////////
     tx_buf[0]  = (uint8)(DDLV_Param & 0xff);
     tx_buf[1]  = (uint8)(DDLV_Mode & 0xff);
     tx_buf[2]  = (uint8)((measure_timer_sec>>8)&0xff);
  	 tx_buf[3]  = (uint8)(measure_timer_sec&0xff);
  	 tx_buf[4]  = 0;
  	 tx_buf[5]  = (uint8)(balance_sec&0xff);
  	         
  	 //tx_buf[6]  = (uint8)((balance_data_tmp.rec_uint32>>24)&0xff);
  	 //tx_buf[7]  = (uint8)((balance_data_tmp.rec_uint32>>16)&0xff);
  	 //tx_buf[8]  = (uint8)((balance_data_tmp.rec_uint32>>8)&0xff);
  	 //tx_buf[9]  = (uint8)( balance_data_tmp.rec_uint32&0xff);
  	           
  	 tx_buf[14] = REC_CHECK_DATA1;
  	 tx_buf[15] = REC_CHECK_DATA2;   
  	 Write_EEPROM(EEP_DDLV_MODE_ADDR,tx_buf, 16);
  	          
     measure_timer_sec_real = measure_timer_sec;
  	 balance_sec_real = balance_sec;
    
   
    }
   
//////////////////////////////////////////
//将用户校准数据排序    
//
//////////////////////////////////////////////////   
void  sort_cal_buf(uint32 cal_numer)
      {
        uint32 i,j,point_tmp;
        float  data_tmp;
        
        for(i=0;i<cal_numer-1;i++)
          {
           j = i+1;
           for(;j<cal_numer;j++)
            {
             if(user_cal_point[i] > user_cal_point[j])
               {
                point_tmp = user_cal_point[i];
                user_cal_point[i] = user_cal_point[j];
                user_cal_point[j] = point_tmp;
                ////
                data_tmp = user_cal_phdata[i].rec_float;
                user_cal_phdata[i].rec_float = user_cal_phdata[j].rec_float;
                user_cal_phdata[j].rec_float = data_tmp;
               }
            } //从小到大排序
          }  //共进行cal_numer-1 次循环
       }
//////////////////////////////////////////
//
//保存用户校准数据
//
//////////////////////////////////////////////////   
   
void  save_cal_data(uint32 cal_numer)
     { 
      uint8   eeprom_tx_buf[16];
      uint32  i,j;
     
      for(i=0;i<cal_numer;i++)
         {
               user_cal_phdata_dis[i].rec_uint32 = Search_user_cal(user_cal_point[i]); //需要调教到的数据
  	           eeprom_tx_buf[0] = (uint8)(user_cal_phdata_dis[i].rec_uint32>>24);
  	           eeprom_tx_buf[1] = (uint8)((user_cal_phdata_dis[i].rec_uint32>>16)&0xff);
  	           eeprom_tx_buf[2] = (uint8)((user_cal_phdata_dis[i].rec_uint32>>8)&0xff);
  	           eeprom_tx_buf[3] = (uint8)(user_cal_phdata_dis[i].rec_uint32&0xff); 
  	           ///////////////////////////////实际PH数据
               eeprom_tx_buf[4] = (uint8)(user_cal_phdata[i].rec_uint32>>24);
  	           eeprom_tx_buf[5] = (uint8)((user_cal_phdata[i].rec_uint32>>16)&0xff);
  	           eeprom_tx_buf[6] = (uint8)((user_cal_phdata[i].rec_uint32>>8)&0xff);
  	           eeprom_tx_buf[7] = (uint8)(user_cal_phdata[i].rec_uint32&0xff); 
  	          
  	           I2C_Send_Str(SYS24C08, EEP_USER_CAL_ADDR1+8*i, eeprom_tx_buf, 8);	 
               for(j=0;j<10000;j++)
  	            {;}
  	      }      
  	  }         
  	       