//**********************************************************************
//文件名:port.h
//**********************************************************************

#ifndef _PORT_H_
#define _PORT_H_
#include "type_def.h"


#define P0_TXD0            0x00000002        //P0.2,串口TXD脚
#define P0_RXD0            0x00000008        //P0.3,串口RXD脚
#define P0_I2C_SCL0        0x10000000        //P0.28,I2C的SCL脚
#define P0_I2C_SDA0        0x08000000        //P0.27,I2C的SDA脚

#define SPEAK_PWM_SIG      0x00000004        //p1.2 pwm0[1]
#define DRI_PWM_OUT        0x00000008        //p1.3 pwm0[2] 
#define L_DRV_OUT          0x00040000        //P3.18线圈驱动信号
#define P3_WP              0x00020000        //P3.17,EEPROM芯片写保护
#define EO2_LED_CPU_S      0x00000001        //P2.0 CPU工作状态指示灯
#define SPEAKER_PIN        0x02000000        //P0.25 蜂鸣器

#define WIRELESS_RCV_PIN1  0x00000100
#define WIRELESS_RCV_PIN2  0x00000200

void Init_Port(void);

void Speaker_OFF(void);
void Speaker_ON(void);
#endif