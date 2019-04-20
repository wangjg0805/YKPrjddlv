//**********************************************************************
//文件   名:
//说       明:CPU型号--LPC2478
//版本   号:
//修改历史:
//**********************************************************************
#include "port.h"
#include "Font_Lib.h"
#include "disp.h"
#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "timer.h"

#include "service.h"
#include "normal_operation.h"
#include "ad_acquire.h"
#include "i2c_com.h"
#include "filter.h"
#include "speaker.h"

#include "config.h"
#include "stdio.h"
///////////////////////////////////////USB HEADFILE
#include  "usbcfg.h"

#include  "ADS1100.h"
///////////////////////////////////////
uint8 Start_Menu_Pro(void);

#define SDRAM_ADDR	  0xA0000000 
#define SDRAM_SIZE     0x2000000	    //32M bytes=256Mbits

//////////////////////////////////////临时函数，用于延时
////////////////////////////////////////////////
//延时函数
///////////////////////////////////////////////
static void delay(uint32 iMs)
{
    uint8 i,cnt;
     for(cnt=0;cnt<iMs;cnt++)
    {
     for(i=0;i<100;i++)
     ;
     }
}
//
void  Init_Lpc2478ADC(void)
  {
   PCONP  |= 1 << 12;
   // 进行ADC模块设置
   AD0CR = (0xC1 << 0)					 |	                            /* SEL=0x01,选择通道0            */
			(3   << 8) |	                                                /* 18M/4 = 4.5M       */
			(1   << 16)					 |	                            /* BURST=1,使用Burst模式，       */
			                                                            /* 让硬件不断地进行转换          */
			(0   << 17)					 |	                            /* CLKS=0, 使用11clock转换       */
			(1   << 21)					 |                              /* PDN=1,正常工作模式            */
			(0   << 24);				                                    /* START=0,在Burst模式下，       */
	 //delay(2000);
	 
	// AD0CR |= (1<<24);	                                                      
  
  }
////////////////////////////////////////////////////////
sint32 main(void)
{
  /*   
   uint32 i,err;
   volatile uint16 *piAddr1;
   */ 

   
   uint32 i,tmp;
     
    if (WDMOD&0x04) WDMOD &= ~0x04;//看门狗复位
    
    Init_Cilib();
   	Init_Graph();
    Init_Port();          //初始化IO口
    
    #if USB_ENABLE
    USB_Init();
    USB_Connect(1);         // USB Connect
    #endif
   
    //////////////////////////其它调整在菜单中进行
    Init_Brightness_PWM(); //use pwm[0]
    Init_Sound_PWM();
    ////////////////////////////////////// 
  	///////////////////////
    ADS7843PIN_Init();
    ///////////////////////
    ///////////////////////
    UART_Init(9600);
    //////////////////////////////
    Print_Init();
    //UART_PrintData(); 
    /////////////////////////////////////////////over  
    
    //Init_Timer0(125);     //启动100us定时
    Init_Timer0_New();
    RTCInit();
    Init_I2C(250000);     //初始化I2C总线
    /////////////////////////////////////
    Init_Lpc2478ADC();
    ///////////////////////////
    
    ADS1100Pin_Init();
  
    //使用默认配置 适用于高区测量
    match_item = MATCH_ITEM_6;
    MATCH_ITEM_F;    //自动切换到测量高区
    
    measure_init();
    /////////////////////////////////待机画面显示
    //Sleep_process();  
    Key_Status = KEY_RELEASE+KEY_DO_OVER ;   //key release
    ////////////////////////////////////////////////
    machine_mode =  SystemInit();    //开机画面处理
    if(MACHINE_SERVICE_MODE == machine_mode)
    Start_Service_process();  
    else
    Normal_Operation();
   
    while (1)
    {;}
}
