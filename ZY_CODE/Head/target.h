/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           target.h
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
** Descriptions:        Renewed the template, added codes to surport more compilers
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __TARGET_H
#define __TARGET_H


#define	__PRJ_TEMPLET_VER	0x2008070914								/*  工程模板版本为 V1.4			*/

#define  MAIN_OSC_FRQ              12000000L
#define  IRC_OSC_FRQ               12000000L
#define  RTC_OSC_FRQ                  32768L

/*
*********************************************************************************************************
*                                             #define CONSTANTS
*********************************************************************************************************
*/
#define  PCLK_WDT           ( 0)
#define  PCLK_TIMER0        ( 1)
#define  PCLK_TIMER1        ( 2)
#define  PCLK_UART0         ( 3)
#define  PCLK_UART1         ( 4)
#define  PCLK_PWM0          ( 5)
#define  PCLK_PWM1          ( 6)
#define  PCLK_I2C0          ( 7)
#define  PCLK_SPI           ( 8)
#define  PCLK_RTC           ( 9)
#define  PCLK_SSP1          (10)
#define  PCLK_DAC           (11)
#define  PCLK_ADC           (12)
#define  PCLK_CAN1          (13)
#define  PCLK_CAN2          (14)
#define  PCLK_ACF           (15)
#define  PCLK_BAT_RAM       (16)
#define  PCLK_GPIO          (17)
#define  PCLK_PCB           (18)
#define  PCLK_I2C1          (19)
#define  PCLK_SSP0          (21)
#define  PCLK_TIMER2        (22)
#define  PCLK_TIMER3        (23)
#define  PCLK_UART2         (24)
#define  PCLK_UART3         (25)
#define  PCLK_I2C2          (26)
#define  PCLK_MCI           (27)
#define  PCLK_SYSCON        (29)


#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

#ifndef IN_TARGET

// void  sysDelayNS(uint32  uiDly);
/*********************************************************************************************************
* Function name:       sysDelayNS
* Descriptions:        长软件延时
* input parameters:    idly	延时参数，值越大，延时越久
* output parameters:   none
* Returned value:      none
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

extern void Reset (void);
/*********************************************************************************************************
** Function name:           Reset
** Descriptions:            resets the target board.
** input parameters:        None
** output parameters:       None
** Returned value:          None
*********************************************************************************************************/

extern void TargetInit (void);
/*********************************************************************************************************
** Function name:           TargetInit
**
** Descriptions:            Initialize the target board; it is called in a necessary place, change it as
**                          needed
**
** input parameters:        None
** output parameters:       None
** Returned value:          None
*********************************************************************************************************/
#endif                                                                  /*  IN_TARGET                   */

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  _TARGET_H                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
