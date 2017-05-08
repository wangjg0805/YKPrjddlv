/*********************************************************************************************************
** 文件名:  LCD2478_graph.h
** 说明  :	LCD2478_graph.c的头文件
** 版本  :	V1.2
*********************************************************************************************************/
#ifndef _LCD2478_GRAPH_H_
#define _LCD2478_GRAPH_H_

#include "type_def.h"
#include "lcd2478.h"

////////////////addby wjg
#define  TITLE_HEIGTH       30

//声明函数的调用规范
void Set_Graphic_Color(uint16 fore_color,uint16 back_col);
void Get_Graphic_Color(uint16 *fc,uint16 *bc);
void Set_Write_Mode(uint16 mode);
uint16 Get_Write_Mode(void);
void Cover_Mask_Dispbuf(uint32 *dispbuf, uint32 dispdata, uint32 mask);

void Put_Pixel(uint16 px, uint16 py, uint16 pc);
uint16 Get_Pixel(uint16 px, uint16 py);
void Hor_Line(uint16 x1, uint16 x2, uint16 y);
void Hor_Pattern_Line(uint16 x1, uint16 x2, uint16 y, uint32 pattern);
void Ver_Line(uint16 y1, uint16 y2, uint16 x);
void Ver_Pattern_Line(uint16 y1, uint16 y2, uint16 x, uint32 pattern);
void Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
void Rectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
void Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
void Pattern_Bar(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 patt);
void Clr_Bar(uint16 x1, uint16 y1, uint16 width, uint16 height, uint16 bc);
void Circle(uint16 x, uint16 y, uint16 r);
void Fill_Circle(uint16 x, uint16 y, uint16 r);
uint16 Put_Numeral(uint16 x, uint16 y,sint32 num,sint32 format);
uint32 Image_Size(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
void Get_Image(uint16 x1, uint16 y1, uint16 x2, uint16 y2, void *img);
void Put_Image(uint16 x1, uint16 y1, void *img);
void Put_Bmp(uint16 x1, uint16 y1, void *bmp, uint32 reverse_flag);
void Put_Button_Frame(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth);
void Put_Button_Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth, uint16 bc);
void Create_Window_Frame(uint16 x1, uint16 y1, uint16 x2, uint16 y2, sint16 depth, uint16 bc);
void Clr_Screen(void);


#endif	
/*_LCD2478_GRAPH_H_*/