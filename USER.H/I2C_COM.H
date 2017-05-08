/****************************************************************************
* 文件名：i2c_com.h
* 功能：硬件I2C软件包。(头文件)
* 说明：主程序要配置好I2C总线接口(I2C引脚功能，并已使能I2C主模式)
****************************************************************************/

#ifndef  I2C_COM_H
#define  I2C_COM_H

#include "type_def.h"

//SYS24C08     DEBUG USE
//USER24C08   ALWAYE USE							                                             
#define   SYS24C08   0xA0     //1010XP1P0X                        
#define   USER24C08  0xA8     //64PAGE *16BYTE = 1kb  

#define	ONE_BYTE_SUBA	1
#define TWO_BYTE_SUBA	2
#define X_ADD_8_SUBA	3

extern void Init_I2C(uint32 fi2c);
extern  uint8 I2C_Send_Byte( uint8 sla,  uint8 dat);
extern  uint8 I2C_Send_Str( uint8 sla,  uint8 suba,  uint8 *s,  uint8 no);
extern  uint8 I2C_Rcv_Byte( uint8 sla,  uint8 *dat);
extern  uint8 I2C_Rcv_Str( uint8 sla,  uint8 suba,  uint8 *s,  uint8 no);

#endif