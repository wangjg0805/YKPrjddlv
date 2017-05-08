//**********************************************************************
//文件名:eeprom.h
//**********************************************************************
#ifndef _EEPROM_H
#define _EEPROM_H
#include "type_def.h"

#define Macro_En_EEPROM()             (FIO3CLR = P3_WP)
#define Macro_Dis_EEPROM()            (FIO3SET = P3_WP)

//保存零点 和满量程数值
#define    EEP_ZERO_ADDR1              0x00    //零点
#define    EEP_ZERO_ADDR2              0x04    //零点
#define    EEP_ZERO_ADDR3              0x08    //零点
#define    EEP_ZERO_ADDR4              0x0C    //零点

#define    EEP_LOW1_ADDR1              0x10    //LOW1
#define    EEP_LOW1_ADDR2              0x14    
#define    EEP_LOW1_ADDR3              0x18    
#define    EEP_LOW1_ADDR4              0x1C    

#define    EEP_LOW2_ADDR1              0x20    //负向满量程点
#define    EEP_LOW2_ADDR2              0x24    
#define    EEP_LOW2_ADDR3              0x28    
#define    EEP_LOW2_ADDR4              0x2C  

#define    EEP_FULL1_ADDR1             0x30    //正向满量程点
#define    EEP_FULL1_ADDR2             0x34    
#define    EEP_FULL1_ADDR3             0x38    
#define    EEP_FULL1_ADDR4             0x3C    

#define    EEP_FULL2_ADDR1             0x40    //负向1400量程点
#define    EEP_FULL2_ADDR2             0x44    
#define    EEP_FULL2_ADDR3             0x48    
#define    EEP_FULL2_ADDR4             0x4C  

#define    EEP_FULL3_ADDR1             0x50   //负向2000量程点
#define    EEP_FULL3_ADDR2             0x51
#define    EEP_FULL3_ADDR3             0x52
#define    EEP_FULL3_ADDR4             0x53 
  
#define    EEP_FULL4_ADDR1             0x5C   //存放标志与校验码
#define    EEP_FULL4_ADDR2             0x5D
#define    EEP_FULL4_ADDR3             0x5E
#define    EEP_FULL4_ADDR4             0x5F 
 

//保存 屏幕校准数值
#define   EEP_SCREEN_ADDR              0x60 //三个坐标点数据各占4个字节+校验     = 13个字节
//保存 系统参数
//1:语言设置 2亮度 3无线配置
#define   EEP_LANGUAGE_ADDR            0x70
#define   EEP_BRIGHT_ADDR              0x74
#define   EEP_STANDARD_ADDR            0x78
#define   EEP_TEMP_COMP_ADDR           0x7c   //保存数据可能为摄氏度 也可能是华氏度
#define   EEP_DIV_ADDR                 0x80
#define   EEP_USER_ID_ADDR             0x84

#define   EEP_DDLV_MODE_ADDR           0x90
//从90地址开始依次存放了以下参数 
//90:       存放当前的测量参数
//91:       定时当前的测量模式
//9293:     定时模式下的定时时间 最大为 999S
//9495:     平衡模式下的平衡时间 最大为  60S
//96979899: 平衡模式下的平衡数据 浮点数
//9E9F:     有效数据标志 0X55AA

////////////////////DDLV CONS SAVE
#define   EEP_DDLV_CONS_ADDR           0xA0    //占据16个字节 
//从A0地址开始依次存放了以下参数 
//A0A1A2A3:   存放  电极常数
//A4A5A6A7    存放  温补系数
//A8A9A10A11: 存放  TDS 转换系数
//AEAF:     有效数据标志 0X55AA


//增加用户校准数据存储 最多5个数据
#define    EEP_USER_CAL_ADDR1          0xB0   
#define    EEP_USER_CAL_ADDR2          0xB8   
#define    EEP_USER_CAL_ADDR3          0xC0
#define    EEP_USER_CAL_ADDR4          0xC8
#define    EEP_USER_CAL_ADDR5          0xD0
//存放的是校准的第几个点
#define    EEP_USER_CAL_FLAG           0xE0

#define    EEP_USER_CAL_POINT1         0xEB
#define    EEP_USER_CAL_POINT2         0xEC
#define    EEP_USER_CAL_POINT3         0xED
#define    EEP_USER_CAL_POINT4         0xEE
#define    EEP_USER_CAL_POINT5         0xEF
  
  
//定义测试地址 更改为参数设置
//0xf0-f3 :用于保存UART 连接类型
//0xf4-f7 :用于设置LOGO 是否显示
//0xf8-fb :用于选择机器型号

#define   EEP_UART_ADDR                0xF0       //EEPROM读写测试字节
#define   EEP_MODEL_ADDR               0xF4       //
#define   EEP_MODEL_DIS_ADDR           0xF8       //
#define   EEP_LOGO_DIS_ADDR            0xFC       //


//开辟出额外字节用于存放 电导率系统校准数据
#define    EEP_DDLV_SYSCAL_RANGE1       0x100      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_RANGE2       0x110      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_RANGE3       0x120      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_RANGE4       0x130      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_RANGE5       0x140      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_RANGE6       0x150      //使用校准电阻的数据
#define    EEP_DDLV_SYSCAL_CHECK        0x160      //使用校准电阻的数据
 
  
//用户校准数据存储 最多5个数据
#define    EEP_DDLV_USER_CAL1          0x170   
#define    EEP_DDLV_USER_CAL2          0x174   
#define    EEP_DDLV_USER_CAL3          0x178
#define    EEP_DDLV_USER_CHECK         0x17e

///////////////////////////////////////////////////////////////////////////////////
//以下信息用于存储用户测量数据
//AT24C01 128Bytes   AT24C08 1024Bytes   AT24C64   8192Bytes   
//AT24C02 256Bytes   AT24C16 2048Bytes   AT24C128 16384Bytes
//AT24C04 512Bytes   AT24C32 4096Bytes   AT24C256 32768Bytes
//计算公式 100*16=1600BYTES 至少使用24C16 页写缓冲最大为16BYTE
//如果使用24C32或以上 页写缓存变大，可以考虑修改程序

#define    EEP_BYTE_PER_REC           0x10     //用户每条记录占用字节数
#define    REC_MAX_NUM                 100     //最大存储记录条数
#define    RECNUM_PER_PAGE              10     //每页有10条记录

#define    EEP_REC_TOTAL_ADDR        0x190     //存放记录总数
#define    EEP_REC_TOTAL_ADDR_BAK    0x194     //记录总数备份   
#define    EEP_REC_START_ADDR        0x1A0     //存放用户记录开始地址

///////////////////////////////////////////////////////////////////
 uint8 Read_EEPROM(uint16 addr,  uint8 *rptr, uint16 rlen);
 uint8 Read_EEPROM_1B(uint16 addr,  uint8 *r1ptr);
 uint8 Read_EEPROM_2B(uint16 addr, uint16 *r2ptr);
 uint8 Read_EEPROM_4B(uint16 addr, uint32 *r4ptr);
 uint8 Write_EEPROM(uint16 addr,  uint8 *wptr, uint16 wlen);
 uint8 Write_EEPROM_1B(uint16 addr,  uint8 w1b);
 uint8 Write_EEPROM_2B(uint16 addr, uint16 w2b);
 uint8 Write_EEPROM_4B(uint16 addr, uint32 w4b);
 uint8 Check_EEP_Wr_ERR(void);
 uint8 EEPROM_RW_Test(void);
#endif