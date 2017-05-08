//**********************************************************************
//文件名:disp.h
//**********************************************************************
#ifndef _DISP_H_
#define _DISP_H_
#include "type_def.h"
#include "lcd2478.h"

#define CHINESE        0x11    //中文
#define ENGLISH        0x22    //英文
#define LG_TYPE_DEF    ENGLISH

#define WRITE_OVERLAY    0    //覆盖显示
#define WRITE_AND        1    //"与"显示
#define WRITE_OR         2    //"或"显示
#define WRITE_EXOR       3    //"异或"显示
#define WRITE_NOT        4     //反显覆盖(图形数据和字模数据反显)

#define TITLE_HEIGTH     30   

extern uint32  disp_buffer[LINE_SIZE*SCR_YSIZE];    //显示缓冲区

void Init_Cilib(void);             //1
void Set_ASC_Lib(const  uint8 *asc_font_ptr );
void Set_HZ_Lib(const struct font_strc *hz_font_ptr);
void Set_Chr_Color(uint16 chr_color,uint16 back_color);
void Set_Line_Space(sint16 space);
void Set_Char_Space(sint16 space);
void Set_Language_Type(uint16 language);
uint8 Get_Language_Type(void);
uint8 Is_HZstr(char *str); //9
uint16 Get_Char_Height(uint16 chr);
uint16 Get_Char_Width(uint16 chr);
uint8 Save_Lg_Type(void);
uint8 Rd_Lg_Type_EEP(void);
uint8 * Language_Str(uint8 *ch_ptr,uint8 *en_ptr);
uint16 Language_Pos(uint16 ch_pos,uint16 en_pos);
uint16 Put_Char(uint16 x, uint16 y,uint16 chr);  //16
uint16 Get_Line_Height(uint8 *str);
uint16 Get_Line_Width(uint8 *str);
uint16 Put_Line_Str(uint16 x, uint16 y, uint8 *str);
uint16 Get_Str_Height(uint8 *str);
uint16 Get_Str_Width(uint8 *str);
void Put_Str(uint16 cx, uint16 cy, uint8 *str);
void   Put_Data(uint16 cx, uint16 cy,char *msg,uint32 val);
void   Put_Data_Float(uint16 cx, uint16 cy,char *msg,float val); //24
                                                                                                      
#endif