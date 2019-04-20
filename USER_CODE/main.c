//**********************************************************************
//�ļ�   ��:
//˵       ��:CPU�ͺ�--LPC2478
//�汾   ��:
//�޸���ʷ:
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

//////////////////////////////////////��ʱ������������ʱ
////////////////////////////////////////////////
//��ʱ����
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
   // ����ADCģ������
   AD0CR = (0xC1 << 0)					 |	                            /* SEL=0x01,ѡ��ͨ��0            */
			(3   << 8) |	                                                /* 18M/4 = 4.5M       */
			(1   << 16)					 |	                            /* BURST=1,ʹ��Burstģʽ��       */
			                                                            /* ��Ӳ�����ϵؽ���ת��          */
			(0   << 17)					 |	                            /* CLKS=0, ʹ��11clockת��       */
			(1   << 21)					 |                              /* PDN=1,��������ģʽ            */
			(0   << 24);				                                    /* START=0,��Burstģʽ�£�       */
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
     
    if (WDMOD&0x04) WDMOD &= ~0x04;//���Ź���λ
    
    Init_Cilib();
   	Init_Graph();
    Init_Port();          //��ʼ��IO��
    
    #if USB_ENABLE
    USB_Init();
    USB_Connect(1);         // USB Connect
    #endif
   
    //////////////////////////���������ڲ˵��н���
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
    
    //Init_Timer0(125);     //����100us��ʱ
    Init_Timer0_New();
    RTCInit();
    Init_I2C(250000);     //��ʼ��I2C����
    /////////////////////////////////////
    Init_Lpc2478ADC();
    ///////////////////////////
    
    ADS1100Pin_Init();
  
    //ʹ��Ĭ������ �����ڸ�������
    match_item = MATCH_ITEM_6;
    MATCH_ITEM_F;    //�Զ��л�����������
    
    measure_init();
    /////////////////////////////////����������ʾ
    //Sleep_process();  
    Key_Status = KEY_RELEASE+KEY_DO_OVER ;   //key release
    ////////////////////////////////////////////////
    machine_mode =  SystemInit();    //�������洦��
    if(MACHINE_SERVICE_MODE == machine_mode)
    Start_Service_process();  
    else
    Normal_Operation();
   
    while (1)
    {;}
}
