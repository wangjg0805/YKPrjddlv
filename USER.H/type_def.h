//**********************************************************************
//文件名:type_def.h
//说  明:定义数据类型
//**********************************************************************
#ifndef _TYPE_DEF_H
#define _TYPE_DEF_H

//定义布尔类型
#define OK       0x01
#define ERROR    0x00

#define TRUE     0x01
#define FALSE    0x00

#define ON       0x01
#define OFF      0x00

#define OPEN     0x01
#define CLOSE    0x00

//数据类型的最大最小值
#define UINT8_MAX             255
#define UINT8_MIN               0
#define SINT8_MAX             127
#define SINT8_MIN            -127
#define UINT16_MAX          65535
#define UINT16_MIN              0
#define SINT16_MAX          32767
#define SINT16_MIN         -32767
//#define UINT32_MAX     4294967295
#define UINT32_MIN              0
#define SINT32_MAX     2147483647
#define SINT32_MIN    -2147483647

typedef char                uint8;  //8位无符号数
typedef signed char         sint8;  //8位有符号数
typedef unsigned short      uint16; //16位无符号数
typedef signed short        sint16; //16位有符号数
typedef unsigned int        uint32; //32位无符号数
typedef signed int          sint32; //32位有符号数

#endif