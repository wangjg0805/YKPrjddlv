//**********************************************************************
//文件名:speaker.h
//**********************************************************************
#ifndef _SPEAKER_H
#define _SPEAKER_H

#include "type_def.h"

//定义声音类型
#define SOUND_TYPE_NONE            0    //无声音
#define SOUND_TYPE_START           1   //启动时发出的声音

//报警有两种类型声音
#define SOUND_TYPE_WARNING         2    //报警类声音
#define SOUND_TYPE_TIPS            3    //提示类声音

//按键有两种声音
#define SOUND_TYPE_KEY_VALID       4    //按键有效音
#define SOUND_TYPE_KEY_NO_VALID    5    //按键无效音


//维修模式下使用
#define SOUND_TYPE_SER_ON         11    //维修模式下打开扬声器声音

//定义X9313的管脚

#define SPK_X9313_CS		(1<<25)
#define SPK_X9313_INC		(1<<23)
#define SPK_X9313_UD		(1<<24)

#define Set_Ext_OutPort1(X)  FIO0SET = X;
#define Clr_Ext_OutPort1(X)  FIO0CLR = X;

void  Set_LCD_Brightness(uint8 brightness);
void  Set_Default_Bri(void);
void  Init_Brightness_PWM(void);
void  Init_PWM0_DRIVER(void);
#endif