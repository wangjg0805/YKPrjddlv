//**********************************************************************
//文件名:self_detect.h
//**********************************************************************

#ifndef _SELF_DETECT_H
#define _SELF_DETECT_H

#include "type_def.h"
#define MCU_PORT_ERR_FLAG_BIT        0x0001    //CPU端口错误
#define MCU_RAM_ERR_FLAG_BIT        0x0002    //RAM错误
#define MCU_ROM_ERR_FLAG_BIT        0x0004    //ROM错误
#define LCD_ERR_FLAG_BIT            0x0008    //LCD自检错误

void Put_SelfDetect_Err(uint32 err);
sint16 Check_CPU_Part(void);
sint16 LCD_Selfdetect(void);

#endif 