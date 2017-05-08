/****************************************Copyright (c)****************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           config.h
** Last modified Date:  2004-09-17
** Last Version:        1.0
** Descriptions:        User Configurable File
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Chenmingji
** Created date:        2004-09-17
** Version: 1.0
** Descriptions:        First version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:  		LinEnqiang
** Modified date: 		2007-05-15
** Version: 1.0
** Descriptions: 		For LPC2300
**
*********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif
 
typedef unsigned char  INT8U;                                           /*  无符号8位整型变量           */
typedef signed   char  INT8S;                                           /*  有符号8位整型变量           */
typedef unsigned short INT16U;                                          /*  无符号16位整型变量          */
typedef signed   short INT16S;                                          /*  有符号16位整型变量          */
typedef unsigned int   INT32U;                                          /*  无符号32位整型变量          */
typedef signed   int   INT32S;                                          /*  有符号32位整型变量          */
typedef float          FP32;                                            /*  单精度浮点数（32位长度）    */
typedef double         FP64;                                            /*  双精度浮点数（64位长度）    */
                              /*  8-bit boolean or logical 
/*********************************************************************************************************
  ARM的特殊代码
  ARM specital code
*********************************************************************************************************/
#include    "LPC2400.h"
#include    "target.h"
#include    "vicControl.h"
#include    "type_def.h"
/*********************************************************************************************************
  应用程序配置
  Application Program Configurations
*********************************************************************************************************/
#include    <stdio.h>
#include    <rt_sys.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>

/*********************************************************************************************************
  本例子的配置
  Configuration of the example
*********************************************************************************************************/
#define Fosc                12000000								    /*	OSC = 12MHz                 */
#define Fcclk               (Fosc*6)								    /*	OSC = 12MHz                 */
#define Fcco                (Fcclk*6)					         	    /*	OSC = 12MHz                 */
#define Fpclk               (Fcclk/4)								    /*	OSC = 12MHz                 */

#endif                                                                  /*  __CONFIG_H                  */ 
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
