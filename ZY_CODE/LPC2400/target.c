/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           target.c
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        header file of the specific codes for LPC2200 target boards
**                      Every project should include a copy of this file, user may modify it as needed
**--------------------------------------------------------------------------------------------------------
** Created by:          Chenmingji
** Created date:        2004-02-02
** Version:             1.0
** Descriptions:        The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Chenmingji
** Modified date:       2004-09-17
** Version:             1.01
** Descriptions:        Renewed the template, added more compiler supports
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
* Function name:       DelayNS
* Descriptions:        长软件延时
* input parameters:    idly	延时参数，值越大，延时越久
* output parameters:   none
* Returned value:      none
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  sysDelayNS(INT32U  uiDly)
{  
    INT32U  i;
    for(; uiDly > 0; uiDly--){
        for(i = 0; i < 5000; i++);
    }
}

/*********************************************************************************************************
** Function name:           TargetInit
**
** Descriptions:            Initialize the target board; it is called in a necessary place, change it as
**                          needed
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
**
** Created by:              Chenmingji
** Created Date:            2004/02/02
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void TargetInit (void)
{ 
    
}
/*********************************************************************************************************
** Function name:     SDRAMInit
** Descriptions:      初始化SDRAM
** input parameters:  none
** output parameters: none
**
** Created by:		  zhangzheng
** Created Date:	  2007.12.10
**-------------------------------------------------------------------------------------------------------
** Modified by:		  zhanghuping		
** Modified date:	  2008.04.25		
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  SDRAMInit( void )
{  
    uint32 i, dummy ;

    LCD_CTRL = 0x00;
    #ifdef __OUT_CHIP                                 
        sysDelayNS(2);
    #else   
    	sysDelayNS(10);
    #endif
    
    PCONP   |= 0x00000800;
    /* 
    * 使能SDRAM总线连接 
    */
    //PINSEL4    = 0x50100000;
    PINSEL4    = 0x10100000;
    PINSEL5    = 0x05010115;
    PINSEL6   |= 0x55555555;
    PINSEL8   |= 0x55555555;
    PINSEL9   |= 0x50550555;
    //PINSEL9   |= 0x50450115;
    EMCControl = 0x00000001;		                                    /* Disable Address mirror       */
 
    /* 
     * 设置SDRAM访问时序. 
     */
    EMCDynamictRP = 1;
	EMCDynamictRAS = 2;
	EMCDynamictSREX = 1;
	EMCDynamictAPR = 0;
	EMCDynamictDAL = 3;
	EMCDynamictWR = 2;
	EMCDynamictRC = 3;
	EMCDynamictRFC = 3;
	EMCDynamictXSR = 1;
	EMCDynamictRRD = 0;
	EMCDynamictMRD = 2;

    EMCDynamicReadConfig = 1;       
   //Default setting, RAS latency 2 CCLKs, CAS latenty 2 CCLKs
	EMCDynamicRASCAS0 = 0x00000202;

//256Mb, 4Mx16, 4 banks, row=13, column=9
	EMCDynamicConfig0 = 0x00000680;
	for(i=0; i<0x80; i++);

//Mem clock enable, CLKOUT runs, send command: NOP
	EMCDynamicControl = 0x00000183;
	for(i=0; i<0x80; i++);

//Send command: PRECHARGE-ALL, shortest possible refresh period
	EMCDynamicControl = 0x00000103;
	for(i=0; i<0x80; i++);

//set 32 CCLKs, between SDRAM refresh, cycles
	EMCDynamicRefresh = 0x00000004;
	for (i=0; i<0x80; i++);					//wait 128 AHB clock cycles

//set 45x16CCLKs=720CCLK=15us, between SDRAM refresh, cycles
	EMCDynamicRefresh = 45;

//To set mode register in SDRAM
//enter mode by issue  MODE command, after finishing
//bailout and back to NORMAL mode
//Mem clock enable, CLKOUT runs, send command: MODE
	EMCDynamicControl = 0x00000083;
	dummy = *((volatile int *)(0xA0023000));
	EMCDynamicControl = 0x00000000;			//Send command: NORMAL
	EMCDynamicConfig0 = 0x00080680;			//Enable buffer
	for (i=0; i<0x80; i++);
}

/*********************************************************************************************************
** Function name:           TargetResetInit
** Descriptions:            Initialize the target
** input parameters:        None
** output parameters:       None
** Returned value:          None
** Created by:              Chenmingji
** Created Date:            2004/02/02
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern void Reset(void);
void TargetResetInit (void)
{
	uint32  m;
    uint32  n;
    uint32  clk_div;
    uint32  clk_div_usb;


    m           =       11;                                     /* PLL Multiplier = 20, MSEL bits = 12 - 1 = 11             */
    n           =        0;                                     /* PLL Divider    =  1, NSEL bits =  1 - 1 =  0             */
    //clk_div     =        5;                                     /* Configure the  ARM Core clock div to 6. CCLKSEL =  6 - 1 */
    clk_div     =        3;
    clk_div_usb =        5;                                     /* Configure the USB clock divider to 6, USBSEL  = 6 - 1    */

#ifdef __IN_RAM 
    SDRAMInit();                                                        /* 配置外部SDRAM                */  
    MEMMAP = 0x02;                   									/* remap 从片内Flash			*/                                                                               
#endif 
#ifdef __IN_CHIP 
    SDRAMInit();                                                        /* 配置外部SDRAM                */  
    MEMMAP = 0x01;                   									/* remap 从片内Flash			*/                                                                               
#endif                                                                  /* __IN_CHIP                    */

#ifdef __OUT_CHIP    
   
    /*  
     * Edit for LPC2460/70 
     * 将中断向量表复制到片内
     * 的SRAM的前64个字节
     */
    uint8 i;
    uint32 *cp1,*cp2;
    cp1=(uint32*)Reset;
    cp2=(uint32*)0x40000000; 
    for(i=0;i<16;i++)
    {  
       *cp2++ = *cp1++;
    }   
    MEMMAP           = 0x02;                   							/* remap 到内部RAM	  	        */

	EMCControl       = 0x01;											/* 使能外部EMC，禁止镜像        */
	SDRAMInit();                                                        /* 配置外部SDRAM                */
#endif                                                                  /* __OUT_CHIP                   */ 		

#ifdef __DEBUG_SDRAM 
    /*  
     * Edit for LPC2460/70 
     * 将中断向量表复制到片内
     * 的SRAM的前64个字节
     */   
    uint8 i;
    uint32 *cp1,*cp2;
    cp1=(uint32*)Reset;
    cp2=(uint32*)0x40000000; 
    for(i=0;i<16;i++)
    {  
       *cp2++ = *cp1++;
    }  
    MEMMAP = 0x2;                  	 	                                /* remap 从内部SRAM映射         */
#endif                                                                  /* __DEBUG_SDRAM                */

	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;														/* Enable PLL, disconnected     */
		PLLFEED = 0xAA;
		PLLFEED = 0x55;
    }

    PLLCON = 0;															/* Disable PLL, disconnected    */
    PLLFEED = 0xAA;
    PLLFEED = 0x55;
    while(PLLSTAT & (3 << 24));	
	
    SCS = (SCS & 0x04)|0x20;             								/* Enable the mainOSC,1MHz~20MHz*/
	while((SCS & 0x40) == 0 );			 								/* Wait until main OSC is usable*/
	
	CLKSRCSEL = 0x01;					 								/* select main OSC as the PLL 	*/
																		/* clock source 				*/
										 
    //PLLCFG  = (((PLL_NValue) << 16)|(PLL_MValue));    
    PLLCFG      =  (m << 0) | (n << 16);   
	PLLCON = 1;
    PLLFEED = 0xAA;														/* Enable but disconnect the PLL*/
    PLLFEED = 0x55;	
    while (((PLLSTAT & (1 << 24)) == 0));								/* Wait until the PLL is usable */
	
	CCLKCFG = clk_div;	
	   	
	//#if USE_USB 			
 	USBCLKCFG = clk_div_usb;										/* usbclk = 480 MHz/10 = 48 MHz */
 	//PCONP |= 0x80000000;											/* Turn On USB PCLK 			*/
	//#else 	  		
    //		USBCLKCFG = USBCLKDivValue;	
	//	PCONP &= 0x7FFFFFFF;    	
	//#endif                                                              /* USE_USB                      */ 		
	while ( ((PLLSTAT & (1 << 26)) == 0) );								/* Check lock bit status 		*/
	//while (((PLLSTAT & 0x00007FFF) != PLL_MValue) && (((PLLSTAT & 0x00FF0000) >> 16) != PLL_NValue));
	
 	PLLCON  = 3;														/* connect the PLL 				*/
    PLLFEED = 0xAA;
    PLLFEED = 0x55;	    						
	while (((PLLSTAT & (1 << 25))!= (1 << 25)));						/* Wait until the PLL is 		*/ 
																		/* connected and locked 		*/
	
	/*  
	 *  Set system timers for each component  
	 */
    PCLKSEL0 = 0x00000000;												/* PCLK is 1/4 CCLK 	        */
    PCLKSEL1 = 0x00000000;

    PCLKSEL0 |= (1<<4);
   	/* 
	 *  Set memory accelerater module
	 */     
   MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
    #if Fcclk < 40000000
    	MAMTIM = 2;
    #else
   		MAMTIM = 3;
    #endif
#endif
    MAMCR = 2;      	
   
    return;
}

/*
*********************************************************************************************************
*                                      Get the CPU Clock Frequency
*
* Description : This function reads CPU registers to determine the CPU clock frequency
*
* Arguements  : None
*
* Returns     : The CPU Core clock in Hz
*
* Notes       : None
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      Get the CPU Clock Frequency
*
* Description : This function reads CPU registers to determine the CPU clock frequency
*
* Arguements  : None
*
* Returns     : The CPU Core clock in Hz
*
* Notes       : None
*********************************************************************************************************
*/

uint32  BSP_CPU_ClkFreq (void)
{
    uint32  msel;
    uint32  nsel;
    uint32  fin;
    uint32  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco                    */
    uint32  clk_div;
    uint32  clk_freq;


    switch (CLKSRCSEL & 0x03) {                                 /* Determine the current clock source                       */
        case 0:
             fin        =  IRC_OSC_FRQ;
             break;

        case 1:
             fin        =  MAIN_OSC_FRQ;
             break;

        case 2:
             fin        =  RTC_OSC_FRQ;
             break;

        default:
             fin        =  IRC_OSC_FRQ;
             break;
    }

    if ((PLLSTAT & (1 << 25)) > 0) {                                /* If the PLL is currently enabled and connected        */
        msel        = (uint32)(PLLSTAT & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
        nsel        = (uint32)((PLLSTAT >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
        pll_clk_feq = (2 * msel * fin / nsel);                      /* Compute the PLL output frequency                     */
    } else {
        pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
    }

    clk_div         = (uint8)(CCLKCFG & 0x0F) + 1;             /* Obtain the CPU core clock divider                    */
    clk_freq        = (uint32)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

    return (clk_freq);
}
/*
*********************************************************************************************************
*                                      Get a Peripheral Clock Frequency
*
* Description : This function reads CPU registers to determine the the clock frequency for the specified
*               peripheral
*
* Arguements  : An ID, one of PCLK_??? defined in bsp.c
*
* Returns     : The peripheral's clock in Hz
*********************************************************************************************************
*/


uint32  BSP_CPU_PclkFreq (uint8  pclk)
{
    uint32  clk_freq;
    uint32  selection;


    clk_freq    = BSP_CPU_ClkFreq();

    switch (pclk) {
        case PCLK_WDT:
        case PCLK_TIMER0:
        case PCLK_TIMER1:
        case PCLK_UART0:
        case PCLK_UART1:
        case PCLK_PWM0:
        case PCLK_PWM1:
        case PCLK_I2C0:
        case PCLK_SPI:
        case PCLK_RTC:
        case PCLK_SSP1:
        case PCLK_DAC:
        case PCLK_ADC:
        case PCLK_CAN1:
        case PCLK_CAN2:
        case PCLK_ACF:
             selection = ((PCLKSEL0 >> (pclk * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        case PCLK_BAT_RAM:
        case PCLK_GPIO:
        case PCLK_PCB:
        case PCLK_I2C1:
        case PCLK_SSP0:
        case PCLK_TIMER2:
        case PCLK_TIMER3:
        case PCLK_UART2:
        case PCLK_UART3:
        case PCLK_I2C2:
        case PCLK_MCI:
        case PCLK_SYSCON:
             selection = ((PCLKSEL1 >> ((pclk - 16) * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        default:
             return (0);
    }
}


/*********************************************************************************************************
  以下为一些与系统相关的库函数的实现
  具体作用请ads的参考编译器与库函数手册
  用户可以根据自己的要求修改
*********************************************************************************************************/

#pragma import(__use_no_semihosting_swi)
#pragma import(__use_two_region_memory)

void _ttywrch (int ch)
{
    ch = ch;
}

/*********************************************************************************************************
  End File
*********************************************************************************************************/
