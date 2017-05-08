//**********************************************************************
//文件  名:FontLib.H
//**********************************************************************
#ifndef _FONT_LIB_H
#define _FONT_LIB_H
#include "type_def.h"

struct font_strc
{
    const uint16 *chr_code;         //字符编码，ASCII字符采用ASCII编码，汉字采用区位码
          uint32 chr_sum;           //字符个数总和
          uint8  width;             //字符宽度，单位为像素
          uint8  height;            //字符高度，单位为像素
    const uint8  *chr_lattice_ptr;  //字符点阵数据指针
};

extern const  uint8 Arial12B_Font[];  //小字体
extern const  uint8 Arial16B_Font[];  //正常使用字体
extern const  uint8 Arial18B_Font[];  //正常使用字体
extern const  uint8 Arial20B_Font[];  //正常使用字体
extern const  uint8 ARIALN26B[];
extern const struct font_strc ST_11;

#endif
