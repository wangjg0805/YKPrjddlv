//**********************************************************************
//文件名:menu.h
//**********************************************************************
#ifndef _MENU_H
#define _MENU_H
#include "type_def.h"

struct menu_str
{ 
    sint32 menu_id;        //菜单号
    void (*menu_init)(sint32 op);//菜单初始化函数指针
    sint32 (*op_pro)(uint32 key_c,sint32 op);    //选项处理函数指针
    void (*disp_pro)(void);    //显示处理函数指针
    void (*assi_pro)(void);    //辅助处理函数指针
};
struct op_str
{
    sint32 op_id;                       //选项号
    sint32 (*op_pro)(uint32 key_c);    //选项处理函数指针
};
 
#endif