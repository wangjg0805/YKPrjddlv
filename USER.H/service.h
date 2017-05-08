//**********************************************************************
//文件名:service.h
//**********************************************************************
#ifndef _SERVICE_H
#define _SERVICE_H

#include "type_def.h"

#define MENU_SERV_MAIN_SEL               0x00       //主菜单
#define MENU_SERV_MAIN_OP_NUM               8       //选项个数

#define MENU_SERV_SEL_SENSOR_OP        0x0100       //AD数据 读取原始数据与滤波后的数值
#define MENU_SERV_SEL_TEMP_OP          0x0200       //温度   读取温度数
#define MENU_SERV_SEL_EEPROM_OP        0x0300       //EEPROM 读写测试
#define MENU_SERV_SEL_AD7843_OP        0x0400       //AD7843 原始信号
#define MENU_SERV_SEL_DDLV_OP          0x0500       //电导率 电路测试
#define MENU_SERV_SEL_INTERFACE_OP     0x0600       //测试   接口信号。如指示灯等
                      //程序运行指示灯
                      //无线信号指示灯
                      //无线输入信号
#define MENU_SERV_SEL_BRISPK_OP        0x0700       //测试   输出连续的亮度变化
#define MENU_SERV_SEL_SCREEN_OP        0x0800       //屏幕   颜色输出

extern void  Start_Service_process(void);
#endif