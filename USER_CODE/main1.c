////注意Target.c    while((PLLSTAT & (1 << 10)) == 0);
//**********************************************************************
//文件   名:
//说       明:CPU型号--LPC2138
//版本   号:
//修改历史:
//**********************************************************************
#include "port.h"
#include "FontLib.h"
#include "disp.h"
#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "timer.h"
#include "scr_cal.h"
#include "service.h"
#include "normal_operation.h"
#include "ad_acquire.h"
#include "i2c_com.h"
#include "filter.h"
#include "speaker.h"
#include "type_def.h"

#include "config.h"
#include "stdio.h"
///////////////////////////////////////USB HEADFILE
#include  "usbcfg.h"

///////////////////////////////////////
uint8 Start_Menu_Pro(void);

#define SDRAM_ADDR	  0xA0000000 
#define SDRAM_SIZE     0x2000000	    //32M bytes=256Mbits

sint32 main(void)
{
            uint32  n;    
   volatile uint32 i,err;
   volatile uint16 *piAddr1;
    
    if (WDMOD&0x04) WDMOD &= ~0x04;//看门狗复位
     //////////////////////////////
    ///////////////////////////////test
       
    //SCS = 	SCS | 0x01 ;    //use FAST IO  
    //PINSEL3  = 0x00000000;       		            // 设定P3口的连接方式
    //FIO1DIR   = LED_RD|LED_WR|LED_CFG|LED_SUSP;     //////p1.14 15 16 17
    //FIO1SET   =	LED_RD|LED_WR|LED_CFG|LED_SUSP;
	//FIO3DIR &= ~(KEY0|KEY1|KEY2|KEY3);  ///////////	
	/////////////
    //USB_Init();
    //USB_Connect(1);         // USB Connect
    /*while(1){
     //	 if(0 == (FIO3PIN&KEY0))
         {
     	  IO1CLR = LED0;
     	  IO1SET = LED1|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY1))
         {
     	  IO1CLR = LED1;
     	  IO1SET = LED0|LED2|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY2))
         {
     	  IO1CLR = LED2;
     	  IO1SET = LED0|LED1|LED3;
     	  }
     	  if(0 == (FIO3PIN&KEY3))
         {
     	  IO1CLR = LED3;
     	  IO1SET = LED1|LED2|LED0;
     	  }
     	sysDelayNS(500);
       }*/
   //////////////////////////////////test end  */
    Init_PWM0_DRIVER();    //GP21 use
   /////////////////////////
    Init_Cilib();
   	Init_Graph();
   
    Init_Port();          //初始化IO口
    #if USB_ENABLE
    USB_Init();
    USB_Connect(1);         // USB Connect
    #endif
    //////////////////////////其它调整在菜单中进行
    Init_Brightness_PWM();
    ////////////////////////////////////// 
      
   	///////////////////////
    ADS7843PIN_Init();
    ///////////////////////
    //Set_Language_Type(ENGLISH);
    //Set_Language_Type(CHINESE);
    RTCInit();
    ///////////////////////
    Init_Timer0(200);     //启动200us定时
    Init_I2C(250000);     //初始化I2C总线
    Init_Timer1();        //初始化定时器1测量中断引脚的周期脉冲数
    UART_Init(9600);
    //gp21Configuration();  //
    //GP21PIN_Init();
	//GP21_init();
    //Init_EINT2();
    //epm7064s Configuration();  //
    Epm7064sPIN_Init();
    filter_init();
	Init_EINT2();
    /////////////////////////////////待机画面显示
    Sleep_process();  
    ////////////////////////////////////////////////
    machine_mode =  SystemInit();    //开机画面处理
    if(MACHINE_SERVICE_MODE == machine_mode)
    Start_Service_process();  
    else
    Normal_Operation();
   
    while (1)
    {;}
}
