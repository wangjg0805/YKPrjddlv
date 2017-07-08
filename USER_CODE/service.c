//**********************************************************************
//文件说明              
//功能
//（1）维修调试模式下的功能实现
//函数列表
//1   主界面显示 初始化			Init_Main_Sel
//2   称重传感器显示 初始化		Init_Service_Sensor
//3   温度传感器显示 初始化		Init_Service_Temperature
//4   EEPROM 显示 初始化		Init_Service_EEprom
//5   触摸屏显示 初始化			Init_Service_AD7843
//6   马达显示 初始化 			Init_Service_Motor
//7   接口显示 初始化			Init_Service_Interface
//8   声音亮度显示 初始化		Init_Service_BriANDSpeaker
//9   屏幕保护显示 初始化		Init_Service_Screen
/************************************************************************/
/*10  Start_Service_process  主程序及其调度                             */
/************************************************************************/
//11  主界面 按键判断			Action_MainMenuCfg
//12  子界面 按键判断			Action_SubMainCfg
//13  EEPROM界面 按键判断 		Action_EEPromMainCfg
//14  接口界面 按键判断			Action_InterfaceMainCfg
//15  声音亮度界面 按键判断     Action_BrightSpeakerMainCfg
//16  屏幕测试按键判断          Action_ScreenTestMainCfg
//17  主界面按键处理            Menu_Serv_Main_Key(uint32 key_c,sint32 op);
//18  称重传感器按键处理        Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
//19  温度传感器按键处理        Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
//20  EEPROM 按键处理           Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
//21  触摸屏按键处理            Menu_Serv_AD7843_Key(uint32 key_c,sint32 op);
//22  马达按键处理              Menu_Serv_Motor_Key(uint32 key_c,sint32 op);
//23  接口按键处理              Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
//24  声音亮度按键处理          Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
//25  屏幕测试界面按键处理      Menu_Serv_Screen_Key
//26  主界面显示                Serv_Main_Menu_Disp
//27  称重传感器界面显示        Serv_Sensor_Menu_Disp
//28  温度传感器界面显示        Serv_Temperature_Menu_Disp
//29  EEPROM  传感器界面显示    Serv_EEprom_Menu_Disp
//30  触摸屏界面显示            Serv_AD7843_Menu_Disp
//31  马达界面显示              Serv_Motor_Menu_Disp
//32  接口界面显示              Serv_Interface_Menu_Disp
//33  声音亮度界面显示          Serv_BriANDSpk_Menu_Disp
//34  屏幕测试界面显示          Serv_Screen_Menu_Disp

//**********************************************************************/
#include "service.h"
#include "screencal.h"
#include "menu.h"
#include "Font_Lib.H"
#include "Font_Lib2.H"
#include "disp.h"
#include "lcd2478.h"
#include "global.h"
#include "eeprom.h"
#include "i2c_com.h"
#include "type_def.h"
#include  "timer.h"
#include  "ad7843.h"

#include  "ADS1100.h"


////////////////////////////////////////////////////////////////////////
//
//本文件使用的静态变量
////////////////////////////////////////////////////////////////////////
static  uint32 eeprom_data,eeprom_zero_flash_flag,eeprom_full_flash_flag,eeprom_test_flash_flag;
static  uint32 eeprom_linear_data[10],eeprom_linear_flash_flag;

static  uint32 power_ch,res_ch;   //选择使用哪个电源 ，使用哪个反馈电阻

static  uint32 flash_scr_flag;
static  uint32 interface_data;

//added by wjg 20151118
static  uint32 uart_tmp,logo_tmp,model_tmp,model_display_tmp;

///////////////////////////////////////////////////////////////////////
//函数声明
//////////////////////////////////////////////////////////////////////
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_AD7843_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_DDLV_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op);
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op);

void Serv_Main_Menu_Disp(void);
void Serv_Sensor_Menu_Disp(void);
void Serv_Temperature_Menu_Disp(void);
void Serv_EEprom_Menu_Disp(void);
void Serv_AD7843_Menu_Disp(void);
void Serv_DDLV_Menu_Disp(void);
void Serv_Interface_Menu_Disp(void);
void Serv_BriANDSpk_Menu_Disp(void);
void Serv_Screen_Menu_Disp(void);
/////////////////////////////////////////////////////////////////////////

//**********************************************************************
//1函数  名:Init_Main_Sel
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void Init_Main_Sel(sint32 op)
{
    uint32 xy[8][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
	Ver_Line(80,SCR_YSIZE-5,640/2);	
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 210,40,Language_Str("请选择检测内容","Please choose one item")); 
    
    for(i=0;i<4;i++)
     Clr_Bar( 30,90+90*i,250,80,COL_DEEPGRAY);
    for(i=0;i<4;i++)
     Clr_Bar(360,90+90*i,250,80,COL_DEEPGRAY);
   	
   	Set_Chr_Color(COL_DEEPGREEN,COL_DEEPGRAY);
    Put_Line_Str( 50+60,120,Language_Str("SENSOR",          "SENSOR"));
	Put_Line_Str( 50+30,210,Language_Str("TEMPERATURE",     "TEMPERATURE"));
	Put_Line_Str( 50+60,300,Language_Str("EEPROM",          "EEPROM"));
	Put_Line_Str( 50+80,390,Language_Str("AD7843",          "AD7843"));
	
	Put_Line_Str( 390+60,120,Language_Str("DDLV",           "DDLV"));
	Put_Line_Str( 390+30,210,Language_Str("INTERFACE",      "INTERFACE"));
	Put_Line_Str( 390+20,300,Language_Str("BRIGHT&SPEAKER", "BRIGHT&SPEAKER"));
	Put_Line_Str( 390+60,390,Language_Str("SCREEN",         "SCREEN"));

    ////////////////////////////////////////create keylist 
    for(i=0;i<4;i++)
    {
     xy[i][0] = 20+250/2;
     xy[i][1] = 120+90*i;
    }
     for(;i<8;i++)
     {
      xy[i][0] = 360+250/2;
      xy[i][1] = 120+90*(i-4);
     }
    
    Create_ButtonList(xy,8);            
 }
//**********************************************************************
//2   称重传感器界面初始化
//**********************************************************************
void Init_Service_Sensor(sint32 op)
{
    uint32 xy[5][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(  5,SCR_XSIZE-5,100);
	Hor_Line(  5,SCR_XSIZE-5,150);
    Hor_Line(  5,SCR_XSIZE-5,200);
    Hor_Line(  5,SCR_XSIZE-5,250);
    Hor_Line(  5,SCR_XSIZE-5,300);
    Hor_Line(  5,SCR_XSIZE-5,350);
    
    Ver_Line(100,350,200);
    Ver_Line(100,350,200+150*1);
    Ver_Line(100,350,200+150*2);
    Ver_Line(100,350,200+150*3);
	
	////////////////////////////
	for(i=0;i<4;i++)
	{
	 Clr_Bar(100+100*i,420,80,50,COL_THINBLUE);
     Set_Chr_Color(COL_WHITE,COL_THINBLUE);
	 Put_Line_Str(120+100*i,435,Language_Str("CH","CH"));
	 Put_Char(150+100*i,435,0x30+i);
	}
	
	Clr_Bar(680,420,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 710,435,Language_Str("退出","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 330,35,Language_Str("传感器数据测试","SENSOR DATA ANALYZE")); 

    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str( 250,      120,Language_Str("CH1",    "CH1"));
    Put_Line_Str( 250+150*1,120,Language_Str("CH2",    "CH2"));
    Put_Line_Str( 250+150*2,120,Language_Str("CH3",    "CH3"));
    Put_Line_Str( 250+150*3,120,Language_Str("CH4",    "CH4"));         
    Set_Chr_Color(COL_THINRED,COL_BLACK);
    Put_Line_Str( 50,170,Language_Str("原始数据",    "RAW DATA"));
    //Put_Line_Str( 50,220,Language_Str("第一级加工",  "PROC1"));
	//Put_Line_Str( 50,270,Language_Str("第二级加工",  "PROC2"));
	Put_Line_Str( 50,320,Language_Str("第三级加工",  "PROC3"));
	
	for(i=0;i<4;i++)
    {
     xy[i][0] = 150+100*i;
     xy[i][1] = 420+50/2;
    }
	
     xy[i][0] = 680+100/2;
     xy[i][1] = 420+50/2;
     Create_ButtonList(xy,5);
 }
//**********************************************************************
//3   温度传感器界面初始化
//**********************************************************************
void Init_Service_Temperature(sint32 op)
{
    uint32 xy[1][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Hor_Line(5,SCR_XSIZE-5,200);
	Hor_Line(5,SCR_XSIZE-5,250);
	
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("温度传感器数据测试","TEMP SENSOR ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("温度传感器原始数据","DATA1"));
	Put_Line_Str( 50,170,Language_Str("温度值",            "DATA2"));
  
   
    for(i=0;i<1;i++)
    {
     xy[i][0] = 500+100/2;;
     xy[i][1] = 425;
    }
     Create_ButtonList(xy,1);          
 }
//**********************************************************************
//4   EEPROM 界面初始化
//**********************************************************************
void Init_Service_EEprom(sint32 op)
{
    uint32 xy[6][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);  //
    Hor_Line(5,SCR_XSIZE-5,200);  //
    Hor_Line(5,SCR_XSIZE-5,250);  //
    Hor_Line(5,SCR_XSIZE-5,300);  //
    Hor_Line(5,SCR_XSIZE-5,350);  //
    
	Ver_Line(100,350,SCR_YSIZE/2);
		
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("EEPROM  数据测试","EEPROM DATA ANALYZE")); 
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,115,Language_Str("满量程数据",     "FULL RANGE DATA")); 
	Put_Line_Str(  50,165,Language_Str("UART TYPE",      "UART TYPE")); 
	Put_Line_Str(  50,215,Language_Str("型号选择",       "MODEL SEL"));
	Put_Line_Str(  50,265,Language_Str("型号开关",       "MODEL ON/OFF"));
	Put_Line_Str(  50,315,Language_Str("LOGO开关",       "LOGO ON/OFF"));
	
	Clr_Bar( 250, 100,150,45,COL_THINBLUE);
	Clr_Bar( 250, 150,150,45,COL_THINBLUE);
	Clr_Bar( 250, 200,150,45,COL_THINBLUE);
	Clr_Bar( 250, 250,150,45,COL_THINBLUE);
	Clr_Bar( 250, 300,150,45,COL_THINBLUE);
	
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 280, 110,Language_Str("读",       "READ")); 
	Put_Line_Str( 280, 160,Language_Str("选择",     "SEL"));
	Put_Line_Str( 280, 210,Language_Str("选择",     "SEL"));
	Put_Line_Str( 280, 260,Language_Str("选择",     "SEL"));
	Put_Line_Str( 280, 310,Language_Str("选择",     "SEL"));
	
	Clr_Bar(520,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 550,415,Language_Str("退出","EXIT"));
		  	
	
    //5个测试按钮
    for(i=0;i<5;i++)
    {
     xy[i][0] = 250 + 150/2;
     xy[i][1] = 50/2 + 100 + 50*i ;
    }
    //1个退出按钮
   
     xy[i][0] = 570;
     xy[i][1] = 430;
    
    Create_ButtonList(xy,6);
    
    uart_tmp  = 0x55;
    logo_tmp  = 0x55;
    model_tmp = 0x55;  
    model_display_tmp = 0x55;                
 }
//**********************************************************************
//5  触摸芯片 界面初始化
//**********************************************************************
void Init_Service_AD7843(sint32 op)
{
    uint32 xy[1][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,SCR_YSIZE-5);
    Hor_Line(5,SCR_XSIZE-5,80);		
    
	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Hor_Line(5,SCR_XSIZE-5,200);
    Ver_Line(100,200,SCR_XSIZE/2);
	
	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
		  	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("触摸屏数据测试","TOUCH SCREEN DATA ANALYZE")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_BLACK);
    Put_Line_Str( 50,120,Language_Str("触摸屏原始数据","DATA1"));
	Put_Line_Str( 50,170,Language_Str("屏幕坐标值",    "DATA2"));
   
    for(i=0;i<1;i++)
    {
     xy[i][0] = 550;
     xy[i][1] = 425;
    }
     Create_ButtonList(xy,1);                     
 }
 
//**********************************************************************
//6    马达 界面初始化
//**********************************************************************
void Init_Service_DDLV(sint32 op)
{
    uint32 xy[4][2];
    uint32 i,j;
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150); //title
   	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 300,40,Language_Str("电导率工厂校准","Factory CAL Process")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	

	Clr_Bar(50, 105,200,40,COL_THINBLUE);	
    Clr_Bar(50, 400,120,40,COL_THINBLUE);		
	Clr_Bar(250,400,120,40,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(80,80),  115,Language_Str("切换通道",  "SWITCH RANGE"));	
	Put_Line_Str( Language_Pos(80,80),  410,Language_Str("保存",    "SAVE"));
	Put_Line_Str( Language_Pos(280,280),410,Language_Str("SKIP",    "SKIP"));
	
	for(i=0;i<6;i++) {
	    for(j=0;j<7;j++)
	     Put_Data(50+100*j,180+30*i,"%d",ddlv_syscal_buf[i][j]);
	}
			
     xy[0][0] = 150;
     xy[0][1] = 125;
     
     xy[1][0] = 110;
     xy[1][1] = 420;
     xy[2][0] = 310;
     xy[2][1] = 420;
   
     xy[3][0] = 550;
     xy[3][1] = 425;
   
     Create_ButtonList(xy,4);   
     
     automode = 0;
     range_man  = RANGE_200MS;
     point2_cal_step = SYS_CAL_RANG1_1; //手动校准
       
 }
//**********************************************************************
//7   接口电路 界面初始化
//**********************************************************************
void Init_Service_Interface(sint32 op)
 {
    uint32 xy[4][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    Ver_Line(100,SCR_XSIZE-5,SCR_XSIZE/2);

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150); //title
    Hor_Line(5,SCR_XSIZE-5,200); //led1
	Hor_Line(5,SCR_XSIZE-5,250); //led2
	Hor_Line(5,SCR_XSIZE-5,300); //led3
	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 200,40,Language_Str("外围接口电路测试","CPU IO TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(  50,120,Language_Str("输出接口","OUTPUT SIGNAL"));
	Put_Line_Str( 370,120,Language_Str("输入接口","INPUT  SIGNAL"));
	Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
	Put_Line_Str(  50,170,Language_Str("运行指示灯",  "SYSTEM RUN"));
	Put_Line_Str(  50,220,Language_Str("无线指示灯1", "WIRELESS LED1"));
	Put_Line_Str(  50,270,Language_Str("无线指示灯2", "WIRELESS LED2"));
	
	Clr_Bar(200,155,80,40,COL_THINBLUE);
	Clr_Bar(200,205,80,40,COL_THINBLUE);
	Clr_Bar(200,255,80,40,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( Language_Pos(220,210),165,Language_Str("开关",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),215,Language_Str("开关",  "ON-OFF"));
	Put_Line_Str( Language_Pos(220,210),265,Language_Str("开关",  "ON-OFF"));
	
    for(i=0;i<3;i++)
    {
     xy[i][0] = 240;
     xy[i][1] = 175+50*i;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,4);                     
 }

//**********************************************************************
//8   声音亮度 界面初始化
//**********************************************************************
void Init_Service_BriANDSpeaker(sint32 op)
 {
   uint32 xy[5][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
    

	Set_Color(COL_DEEPGREEN);
	Hor_Line(5,SCR_XSIZE-5,100);
	Hor_Line(5,SCR_XSIZE-5,150);
    Ver_Line(100,475,SCR_XSIZE/2);
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( Language_Pos(250,230),40,Language_Str("亮度声音测试","BRIGHT SPEAKER TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str(  Language_Pos(120,100),120,Language_Str("亮度调整","BRIGHTNESS ADJUST"));
	Put_Line_Str(  Language_Pos(440,420),120,Language_Str("音量调整","SPEAKER ADJUST"));

	Clr_Bar(50,170,200,80,COL_THINBLUE);  //亮度增加
	Clr_Bar(50,270,200,80,COL_THINBLUE);  //亮度减小
	Clr_Bar(370,170,200,80,COL_THINBLUE);  //声音增加
	Clr_Bar(370,270,200,80,COL_THINBLUE);  //声音减小
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 100,200,Language_Str("亮度增加",  "BRIGHT ADD"));
	Put_Line_Str( 100,300,Language_Str("亮度减小",  "BRIGHT SUB"));
	Put_Line_Str( 420,200,Language_Str("声音增加",  "VOLUME ADD"));
	Put_Line_Str( 420,300,Language_Str("声音减小",  "VOLUME SUB"));
	
    for(i=0;i<2;i++)
    {
     xy[i][0] = 150;
     xy[i][1] = 210+100*i;
    }
   for(;i<4;i++)
    {
     xy[i][0] = 470;
     xy[i][1] = 210+100*(i-2);
    }
   
     xy[i][0] = 500;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,5);                      
 }
//**********************************************************************
//9  屏幕测试 界面初始化
//**********************************************************************
void Init_Service_Screen(sint32 op)
{
   uint32 xy[3][2];
    uint32 i;  
    Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);      //ALL
    Set_Color(COL_WHITE);
    Rectangle(5,5,SCR_XSIZE-5,475);
    Hor_Line(5,SCR_XSIZE-5,80);		
   	 	
	Set_Chr_Color(COL_WHITE,COL_BLACK);
	Put_Line_Str( 250,40,Language_Str("屏幕颜色测试","SCREEN TEST")); 

	Clr_Bar(500,400,100,50,COL_THINRED);
    Set_Chr_Color(COL_BLACK,COL_THINRED);
	Put_Line_Str( 530,415,Language_Str("退出","EXIT"));
	
	
	Clr_Bar(100,400,150,50,COL_THINBLUE);
	Clr_Bar(300,400,150,50,COL_THINBLUE);
	Set_Chr_Color(COL_BLACK,COL_THINBLUE);
	Put_Line_Str( 150,415,Language_Str("开始",  "START"));
	Put_Line_Str( 350,415,Language_Str("停止",  "STOP"));
	
    for(i=0;i<2;i++)
    {
     xy[i][0] = 175+200*i;
     xy[i][1] = 425;
    }
   
     xy[i][0] = 550;
     xy[i][1] = 425;
   
     Create_ButtonList(xy,3);                
  
           
 }
 
//******************************************************************************************************
//10函数 名 Start_Service_process
//函数功能:调试模式主菜单项目 及其调度
//入口参数:无
//出口参数:无 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//******************************************************************************************************
void  Start_Service_process(void)
{
 struct menu_str menu[9] = {
    {MENU_SERV_MAIN_SEL,        Init_Main_Sel,              Menu_Serv_Main_Key,           Serv_Main_Menu_Disp,        NULL},//主界面
    
    {MENU_SERV_SEL_SENSOR_OP,   Init_Service_Sensor,        Menu_Serv_Sensor_Key,       Serv_Sensor_Menu_Disp,      NULL},//电磁传感器
    {MENU_SERV_SEL_TEMP_OP,     Init_Service_Temperature,   Menu_Serv_Sensor_Key,  Serv_Temperature_Menu_Disp,      NULL},//温度传感器
    {MENU_SERV_SEL_EEPROM_OP,   Init_Service_EEprom,        Menu_Serv_EEprom_Key,       Serv_EEprom_Menu_Disp,      NULL},//EEPROM
    {MENU_SERV_SEL_AD7843_OP,   Init_Service_AD7843,        Menu_Serv_AD7843_Key,       Serv_AD7843_Menu_Disp,      NULL},//AD7843
    {MENU_SERV_SEL_DDLV_OP,     Init_Service_DDLV,          Menu_Serv_DDLV_Key,           Serv_DDLV_Menu_Disp,      NULL},//DDLV
    {MENU_SERV_SEL_INTERFACE_OP,Init_Service_Interface,     Menu_Serv_Interface_Key, Serv_Interface_Menu_Disp,      NULL},//INTERFACE
    {MENU_SERV_SEL_BRISPK_OP,   Init_Service_BriANDSpeaker, Menu_Serv_BriANDSpk_Key, Serv_BriANDSpk_Menu_Disp,      NULL},//亮度声音
    {MENU_SERV_SEL_SCREEN_OP,   Init_Service_Screen,        Menu_Serv_Screen_Key,       Serv_Screen_Menu_Disp,      NULL} //屏幕
  
    };//扩展口
    sint32 cur_op,next_op;    //当前选项和所切换到的选项(bits[7:0]:选项号;bits[15:8]:界面号;bits[23:16]:选项的显示状态)
    
    cur_op =0xffffffff ;  
    next_op = MENU_SERV_MAIN_SEL;
    
    while (1)
    {
        if ((cur_op^next_op)&0x0000ff00)//当前所处的界面和准备切换到的界面不一样
        {//初始化界面
            (*(menu[(next_op>>8)&0xff].menu_init))(next_op);
          
        }
        cur_op = next_op;        //"界面/菜单/选项"之间的切换
        //////////////////////////////////////////////////////////////
       
        measure_proc();
           
        ////////////////////////////////////////////////////////////////           
        key_value = Get_PositionXY();        //键盘扫描
        if(key_value)        //键盘处理流程：根据不同键值，返回不同的窗口界面 
        {                   //在该窗口按键流程中处理不同的功能，处理完毕再返回到主界面
         next_op = (*(menu[(cur_op>>8)&0xff].op_pro))(key_value,cur_op);     
        }
             
        (*(menu[(cur_op>>8)&0xff].disp_pro))();    //显示处理函数
       
             
        LED_CPU_RUN_Cpl(); //CPU工作指示灯闪烁频率1Hz
                      
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*11  主界面 键盘响应函数MAIN
//////////////////////////////////////////////////*/
uint32  Action_MainMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<8;i++)
	   {
	     if((abs((*p).X_Position-x)< 300/2) && (abs((*p).Y_Position-y)< 80/2))
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
/*12  子项目键盘响应函数 
//////////////////////////////////////////////////*/
uint32  Action_SensorMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*13  EEPROM键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_EEPromMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*13  EEPROM键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_AD7843MainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<1;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*14  interface 键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_InterfaceMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 50/2))
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
/*15  brightness and speaker  键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_BrightSpeakerMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 80/2))
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
/*16  SCREEN TEST  键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_ScreenTestMainCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 50/2))
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
//17函数名:Menu_Serv_Main_Key
//函数功能:主界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
static  sint32 Menu_Serv_Main_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://sensor
  	           #if SDJ_SOFT
  	           op_id = MENU_SERV_SEL_SENSOR_OP;
  	           #endif
  	           break;
    	case 2://temperature
    	       //op_id = MENU_SERV_SEL_TEMP_OP;
    	       break;
       	case 3://eeprom
       	       op_id = MENU_SERV_SEL_EEPROM_OP;
       	       break;
       	case 4://ad7843
       	       op_id = MENU_SERV_SEL_AD7843_OP;
               break;
        case 5://
              
  	           op_id = MENU_SERV_SEL_DDLV_OP;
  	         
  	           break;
    	case 6://interface
    	       //op_id = MENU_SERV_SEL_INTERFACE_OP;
    	       break;
       	case 7://brightness and speaker
       	       //op_id = MENU_SERV_SEL_BRISPK_OP;
       	       break;
       	case 8://screen
       	       op_id = MENU_SERV_SEL_SCREEN_OP;
       	        break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//18   称重传感器界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Sensor_Key(uint32 key_c,sint32 op)
{
     sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_SensorMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    
    #if SDJ_SOFT
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1:select_ch = 1;AD7190Configuration(1);break;
  	    case 2:select_ch = 2;AD7190Configuration(2);break;
  	    case 3:select_ch = 3;AD7190Configuration(4);break;
  	    case 4:select_ch = 4;AD7190Configuration(8);break;
  	   	case 5://return
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    #endif
    
    return(op_id);
  }
//**********************************************************************
//19   温度传感器界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Temperature_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_MainMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://CHINESE
  	           //language_var_tmp = CHINESE ; //Set_Language_Type(CHINESE);
  	           break;
    	case 2://ENGLISH
    	       //language_var_tmp = ENGLISH;  //Set_Language_Type(ENGLISH);
    	       break;
       	case 3://Esc
       	       //op_id =  MENU_OP_OPER;  // return
       	       break;
       	case 4://OK
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//20   EEPROM 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_EEprom_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_EEPromMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	   	case 1://read full range data
    	       if(OK == Init_Sys_Cal_Varible())
  	           eeprom_full_flash_flag = TRUE;
  	           else
  	           eeprom_full_flash_flag = FALSE;
    	       break;
       	case 2://uart_type
       	       Uart_Printer_Flag++;
       	       EEPROM_para_config(EEP_UART_ADDR,Uart_Printer_Flag&0x01);
       	       break;
    	case 3://model select
  	           version_811++;
    	       EEPROM_para_config(EEP_MODEL_ADDR,version_811&0x01);
    	       break;
      	
    	case 4://model display sel
  	           model_display++;
    	       EEPROM_para_config(EEP_MODEL_DIS_ADDR,model_display&0x01);
    	       break;
      	
    	case 5://LOGO flag
    	       LOGO_flag++;
    	       EEPROM_para_config(EEP_LOGO_DIS_ADDR,LOGO_flag&0x01);
    	       break;
        
      	case 6://exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//21   触摸屏 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_AD7843_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_AD7843MainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1:
  	            op_id =  MENU_SERV_MAIN_SEL;
  	           break;
    	
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  
  }
//**********************************************************************
//22  马达界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_DDLV_Key(uint32 key_c,sint32 op)
{
    static uint8 buf[16];
    sint32 op_id;
    uint32 i,j,k;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
   // return(op_id);
    i = Action_InterfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://切换 电源
  	           range_man++;
  	           
  	           if(range_man > RANGE_2US) {
  	               range_man = RANGE_200MS;
  	           
  	           }
  	           break;
    	case 2:
    	       if(stable_flag == 1) {  //only in stable status
    	           k = ddlv_raw_data1;
    	           break_stable_flag = 1;
    	           j = point2_cal_step%7;
    	           switch(j) {
    	           case  SYS_CAL_RANG1_1:   
                   case  SYS_CAL_RANG1_2:
                   case  SYS_CAL_RANG1_3:
                   case  SYS_CAL_RANG1_4:
                   case  SYS_CAL_RANG1_5:
                   case  SYS_CAL_RANG1_6:
                       buf[(j-1)*2]   =  k & 0xff;
                       buf[(j-1)*2+1] = (k>>8) & 0xff;   
                       break;
               	   case  0:
                       buf[12]   =  k & 0xff;
                       buf[13]   = (k>>8) & 0xff;
                       buf[14]   =  0x55;
                       buf[15]   =  0xaa;
                       Write_EEPROM(EEP_DDLV_SYSCAL_RANGE1+0x10*(point2_cal_step/7-1),buf, 16); 
  	                   for(i=0;i<20000;i++){;}
  	                   if(range_man == RANGE_2US) {
  	                   //////////////5M校准完毕后手动返回测量模式  
                          buf[0] = 0x88;
  	                      buf[1] = 0x99;
  	                      buf[2] = 0xaa;
  	                      buf[3] = 0xbb;  
  	                      Write_EEPROM(EEP_DDLV_SYSCAL_CHECK,buf, 4); 
  	                      for(i=0;i<20000;i++){;}	  
                          Init_Sys_Cal2_Varible();
                          i = 123456789;
                       }
    	               break;
    	           default:
    	               break;
    	           }
    	       
    	           if(123456789 == i) {
    	                op_id =  MENU_SERV_MAIN_SEL; 
    	       
    	            } else {
    	                point2_cal_step++;
    	                range_man = (point2_cal_step-1) / 7; 
    	           }
    	       }
    	       break;  
       	
       	case 3:  //skip
       	       if(1 == point2_cal_step%7) {
       	           point2_cal_step += 7;
       	           range_man = (point2_cal_step-1) / 7;
       	       }    
       	       break;
       	
       	 
       	case 4://
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    
    
    return(op_id);
  }  
//**********************************************************************
//23  接口 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Interface_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
   // return(op_id);
    i = Action_InterfaceMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://系统运行灯取反
  	           LED_CPU_RUN_Cpl();
  	           break;
    	case 2://WIRELESS LED1
    	       //Wireless_Run_Led1_Cpl();
    	       break;
       	case 3://WIRELESS LED1
       	       //Wireless_Run_Led2_Cpl();
       	       break;
       	case 4://EXIT
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//24  声音亮度界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_BriANDSpk_Key(uint32 key_c,sint32 op)
{
    uint8  brightness_tx_buf[4];
    uint32 i,j;
    sint32 op_id;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightSpeakerMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://亮度增加
  	           lcd_brightness++;
  	           if(lcd_brightness > BRIGHT_CLASS_TOTAL)
  	              lcd_brightness = BRIGHT_CLASS_TOTAL;
  	           Set_LCD_Brightness(lcd_brightness);
  	           
  	           brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
  	           break;
    	case 2://亮度减小
    	       if(lcd_brightness>1)
    	       lcd_brightness--;
    	       Set_LCD_Brightness(lcd_brightness);
    	       
    	       brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
    	       break;
       	case 3://音量增加
       	       
       	       break;
       	case 4://音量减小
       	             	       
       	       break;       
   	    case 5://exit
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }
//**********************************************************************
//25   屏幕测试 界面的键盘处理
//**********************************************************************
static  sint32 Menu_Serv_Screen_Key(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    //if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ScreenTestMainCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://start
  	           flash_scr_flag = TRUE;
  	           break;
    	case 2://stop
    	       flash_scr_flag = FALSE;
    	       break;
        case 3://exit
       	       ////////////////////////////////
       	       op_id =  MENU_SERV_MAIN_SEL;
       	       break;       
   		default:break;
   	   }
   	  //  Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }

//**********************************************************************
//26函数名:Serv_Main_Menu_Disp               
//函数功能:主界面下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void Serv_Main_Menu_Disp(void)
{

}

//**********************************************************************
//27 称重传感器界面显示
//**********************************************************************
void Serv_Sensor_Menu_Disp(void)
 {
  uint32 i;
 
   
      Set_Chr_Color(COL_WHITE,COL_BLACK);
    
 }    
//**********************************************************************
//28  温度传感器界面显示
//**********************************************************************
void Serv_Temperature_Menu_Disp(void)
{
}
//**********************************************************************
//29  EEPROM  传感器界面显示
//**********************************************************************
void Serv_EEprom_Menu_Disp(void)
 {
   //此处显示当前的配置内容
   if(Uart_Printer_Flag != uart_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      uart_tmp = Uart_Printer_Flag;
      if(uart_tmp&0x01)
	  Put_Line_Str(500, 160,Language_Str("TO PRINTER",  "TO PRINTER")); 
      else
      Put_Line_Str(500, 160,Language_Str("TO PC     ",  "TO PC     "));  
     }
  
   if(version_811 != model_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      model_tmp = version_811;
      #if SDJ_SOFT
      if(model_tmp&0x01)
	  Put_Line_Str(500, 210,Language_Str("P811",  "P811")); 
      else
      Put_Line_Str(500, 210,Language_Str("P801",  "P801"));  
      #else
      if(model_tmp&0x01)
      Put_Line_Str(500, 210,Language_Str("P812",  "P812")); 
      else
      Put_Line_Str(500, 210,Language_Str("P802",  "P802"));  
      
      #endif
     }
     
   if(model_display != model_display_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      model_display_tmp = model_display;
      if(model_display&0x01)
	  Put_Line_Str(500, 260,Language_Str("YES",  "YES")); 
      else
      Put_Line_Str(500, 260,Language_Str("NO ",  "NO "));  
     }
     
   if(LOGO_flag != logo_tmp)
     {
      Set_Chr_Color(COL_WHITE,COL_BLACK);
      logo_tmp = LOGO_flag;
      if(logo_tmp&0x01)
	  Put_Line_Str(500, 310,Language_Str("YES",   "YES")); 
      else
      Put_Line_Str(500, 310,Language_Str("NO ",   " NO"));  
     }
  
  
  }
//**********************************************************************
//30  触摸屏界面显示
//**********************************************************************
void Serv_AD7843_Menu_Disp(void)
 {
  uint32 i,j;
  machine_mode =  MACHINE_SCREENCAL_MODE;
  i = Get_PositionXY(); //read ad data
  
  machine_mode =  MACHINE_NORMAL_MODE;
  j = Get_PositionXY(); //read XY data
  
  if((i!=0)&&(j!=0))
   {
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(370+200,110,"%05d",i>>16);
    Put_Data(470+200,110,"%05d",i&0xffff);
    Put_Data(370+200,160,"%05d",j>>16);
    Put_Data(470+200,160,"%05d",j&0xffff);
   }
   else
   {
   Clr_Bar(370+200,110,200,40,COL_BLACK);
   Clr_Bar(370+200,160,200,40,COL_BLACK);
   }
 }
//**********************************************************************
//31  马达界面显示
//**********************************************************************
const uint32 res_cal_data[] = {
0,
10,          20,        30,         40,        50,         60,        70,
50,          60,        80,        100,       250,        500,       600,
500,        600,       800,       1000,      2000,       5000,      6000,
5000,      6000,      8000,      10000,     20000,      50000,     60000,      //k
50000,    60000,     80000,     100000,    200000,     500000,    600000,      //k
500000, 1000000,   5000000,   10000000,  20000000,   50000000, 100000000       //M
};


void Serv_DDLV_Menu_Disp(void)
{
  uint32 i,j;

  Set_Chr_Color(COL_WHITE,COL_BLACK);
  Put_Data(300,115,"%d",range_man);  //power_ch
  Put_Data_Float(300,115,"%7.1f",ddlv_raw_data1);    //res_ch
  Put_Data_Float(400,115,"%7.1f",ddlv_raw_data2);    //res_ch
  if( sample_flag == 1)
       Put_Line_Str(Language_Pos(600, 550),  115,Language_Str("OK","OK"));
  else
       Put_Line_Str(Language_Pos(600, 550),  115,Language_Str("NO","NO"));      
  
  
  Put_Data(700,115,"%d",point2_cal_step);                //steps
  
  if(point2_cal_step > 36) {
      Put_Data_Float(600,50,"%8.1f",res_cal_data[point2_cal_step] / 1000000.0);
      Put_Char(700,50,'M');  
  } else if(point2_cal_step > 17) {
      Put_Data_Float(600,50,"%8.1f",res_cal_data[point2_cal_step] / 1000.0);
      Put_Char(700,50,'K');  
  } else
      Put_Data(600, 50,"%8d",res_cal_data[point2_cal_step]);  //cal_res
      
  
}
//**********************************************************************
//32  接口界面显示
//**********************************************************************
void Serv_Interface_Menu_Disp(void)
{
}
//**********************************************************************
//33  声音亮度界面显示
//**********************************************************************
void Serv_BriANDSpk_Menu_Disp(void)
{
}
//**********************************************************************
//34  屏幕测试界面显示
//**********************************************************************
void Serv_Screen_Menu_Disp(void)
{
 uint32 i;
 //800/16 = 50
 static  uint32 j;
 static  uint32 flash_var = 0;
 static  uint32 flash_col_var = 0;
 
 flash_var++;
 if(flash_var > 16)
  {
   flash_var = 0;
   j++;
   if(16 == j)
   j=0;
  }
  
 if(TRUE == flash_scr_flag)
 {
  Clr_Bar(40*flash_var,100,40,260,(flash_col_var+j)%16); //实现偏移显示
  flash_col_var++;
  if(16 == flash_col_var)
   flash_col_var = 0;
  
 }
 
}