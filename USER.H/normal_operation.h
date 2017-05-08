//**********************************************************************
//文件名:normal_opration.h
//**********************************************************************
#ifndef    _NORMAL_OPERATION_H
#define    _NORMAL_OPERATION_H

#include "type_def.h"

#define MENU_NUM                              18    //菜单个数

//MAIN OPER
#define MENU_OP_OPER                  0x00000000    //主工作菜单
#define MENU_SLEEP_OP_OPER            0x00000100    //sleepmode
#define MENU_FLOAT_INPUT_OPER         0x00000200    //小数点数据输入
#define MENU_UINT_INPUT_OPER          0x00000300    //整数数据输入
#define MENU_DATA_CALL_OPER           0x00000400    //

#define MENU_SYSTEM_CONF_OPER         0x00000500
//以下是系统设置子菜单项目
#define MENU_TIME_CFG_OPER            0x00000600    //
#define MENU_LANG_SEL_OPER            0x00000700    //
#define MENU_BRIG_CFG_OPER            0x00000800

#define MENU_STAND_SEL_OPER           0x00000900    //
#define MENU_DIVISION_SEL_OPER        0x00000A00    //
#define MENU_TEMP_COMP_OPER           0x00000B00

#define MENU_USER_ID_OPER             0x00000C00
#define MENU_SCR_CAL_OPER             0x00000D00    //

//////////////电导率增加 菜单
#define MENU_DDLV_CONS_OPER           0x00000E00    //
#define MENU_MODE_SEL_OPER            0x00000F00    //
#define MENU_BALANCE_SEL_OPER         0x00001000
#define MENU_OPER_CONFIRM_OPER        0x00001100

#define MENU_OPER_ADD_OPER            0x00001200


/////////////////////////////////////////////////

#define   MAIN_BUT_WIDTH        100
#define   MAIN_BUT_HEIGTH       100
#define   DATE_TIME_WIDTH       160
#define   DATE_TIME_HEIGTH       30

#define   DATE_CFG_WIDTH1        80
#define   DATE_CFG_HEIGTH1       40
#define   DATE_CFG_WIDTH2        80
#define   DATE_CFG_HEIGTH2       60

extern   void      Normal_Operation(void);    
extern   void      Set_Menu_Op(sint32 cur_menu_op);    
extern   uint32    cali_off_newdata(uint32 data_no_line); 
                                                                                 
#endif                                                                                                                                                            