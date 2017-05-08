/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:           LCD2478.h
** Last modified Date:  2008-04-07
** Last Version:        1.0
** Descriptions:        the lcd controller driver's head
**
**------------------------------------------------------------------------------------------------------
** Created by:          Houxiaolong
** Created date:        2008-04-07
** Version:             1.0
** Descriptions:        the lcd controller driver's head
**
**------------------------------------------------------------------------------------------------------
** Modified by:         Houxiaolong
** Modified date:       2008-07-10
** Version:             1.1
** Descriptions:        add the mirror operation macro
**                      
********************************************************************************************************/

#ifndef _LCD2478_H
#define _LCD2478_H

#include "type_def.h"
#include "config.h"

///////////////////////////////////////////////////////////////
#define COLOR_NUMBER	16			//Define color number

#define AT070TN83       1
#define AT056TN53       0

#if     AT070TN83 == 1
#define SCR_XSIZE		800			//LCD x size
#define SCR_YSIZE		480			//LCD y size

#define LCD_CLKDIV		0			//LCDCLK divided=1

#define LCD_POWER		1
#define LCD_TFT			1			//TFT mode
#define LCD_WATERMARK	0			//water mark = 4
#if COLOR_NUMBER == 16
//16-color mode
#define LINE_SIZE		(SCR_XSIZE / 8)		//memory line size
#define LCD_BPP			2			//4bpp
#define LCD_BEPO		1			//pixel big-endian
#define LCD_BEBO		1			//data big-endian
#else
//65536-color mode
#define LINE_SIZE		SCR_XSIZE	//memory line size
#define LCD_BPP			6			//16bpp
#define LCD_BEPO		0			//pixel little-endian
#define LCD_BEBO		0			//data little-endian
#endif
#define LCD_EN			1
#define LCD_BGR			1			//red and blue swapped

#define LCD_BCD			0			//PCD div clk valid
#define LCD_PCD			0			//PCD clk divided=2
#define LCD_CPL			(SCR_XSIZE - 1)		//clock per line
#define LCD_IOE			0			//LCDENAB active high
#define LCD_IPC			1			//LCDDCLK active falling edge
#define LCD_IHS			1			//LCDLP active low
#define LCD_IVS			1			//LCDFP active low
#define LCD_CLKSEL		0			//LCDCLK from HCLK

#define LCD_HBP			13			//Horizontal back porch
#define LCD_HFP			13			//Horizontal front porch
#define LCD_HSW			27			//Horizontal pulse width
#define LCD_PPL			(SCR_XSIZE / 16 - 1)//Pixels-per-line

#define LCD_VBP			23			//Vertical back porch
#define LCD_VFP			5			//Vertical front porch
#define LCD_VSW			5			//Vertical pulse width
#define LCD_LPP			(SCR_YSIZE - 1)		//Lines-per-panel
/////////////////////////////////////////////////////////////////
#else
#define SCR_XSIZE		640			//LCD x size
#define SCR_YSIZE		480			//LCD y size

#define LCD_CLKDIV		0			//LCDCLK divided=1

#define LCD_POWER		1
#define LCD_TFT			1			//TFT mode
#define LCD_WATERMARK	0			//water mark = 4
#if COLOR_NUMBER == 16
//16-color mode
#define LINE_SIZE		(SCR_XSIZE / 8)		//memory line size
#define LCD_BPP			2			//4bpp
#define LCD_BEPO		1			//pixel big-endian
#define LCD_BEBO		1			//data big-endian
#else
//65536-color mode
#define LINE_SIZE		SCR_XSIZE	//memory line size
#define LCD_BPP			6			//16bpp
#define LCD_BEPO		0			//pixel little-endian
#define LCD_BEBO		0			//data little-endian
#endif
#define LCD_EN			1
#define LCD_BGR			1			//red and blue swapped

#define LCD_BCD			0			//PCD div clk valid
#define LCD_PCD			0			//PCD clk divided=2
#define LCD_CPL			(SCR_XSIZE - 1)		//clock per line
#define LCD_IOE			0			//LCDENAB active high
#define LCD_IPC			1			//LCDDCLK active falling edge
#define LCD_IHS			1			//LCDLP active low
#define LCD_IVS			1			//LCDFP active low
#define LCD_CLKSEL		0			//LCDCLK from HCLK

#define LCD_HBP			110			//Horizontal back porch
#define LCD_HFP			16			//Horizontal front porch
#define LCD_HSW			27			//Horizontal pulse width
#define LCD_PPL			(SCR_XSIZE / 16 - 1)//Pixels-per-line

#define LCD_VBP			13			//Vertical back porch
#define LCD_VFP			32			//Vertical front porch
#define LCD_VSW			5			//Vertical pulse width
#define LCD_LPP			(SCR_YSIZE - 1)		//Lines-per-panel

#endif

#define LCD_POWER		1
#define LCD_TFT			1			//TFT mode
#define LCD_WATERMARK	0			//water mark = 4

#if COLOR_NUMBER == 16
#define BITS_PER_PIXEL     4 
//16-color mode
#define  COL_BLACK         0   //黑
#define  COL_DEEPBLUE      1   //深蓝
#define  COL_DEEPGREEN     2   //深绿
#define  COL_DEEPCYAN      3   //深青
#define  COL_DEEPRED       4   //深红
#define  COL_DEEPPUPLE     5   //深紫
#define  COL_DEEPYELLOW    6   //深黄
#define  COL_THINGRAY      7   //浅灰

#define  COL_DEEPGRAY      8   //深灰
#define  COL_THINBLUE      9   //浅蓝
#define  COL_THINGREEN    10   //浅绿
#define  COL_THINCYAN     11   //浅青
#define  COL_THINRED      12   //浅红
#define  COL_THINPUPLE    13   //浅紫
#define  COL_THINYELLOW   14   //浅黄
#define  COL_WHITE        15   //白

#define LINE_SIZE		(SCR_XSIZE / 8)		//memory line size
#define LCD_BPP			2			//4bpp
#define LCD_PCD			0			//PCD clk divided=2
#endif

/*********************************************************************************************************
* 名称: Init_LCD2478
* 功能: 初始化LPC2478的LCD显示系统
* 入口参数: 无
* 出口参数: 无
*********************************************************************************************************/
 void Init_LCD2478(void);

#endif 
