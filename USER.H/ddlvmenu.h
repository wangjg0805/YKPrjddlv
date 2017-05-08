#ifndef _DDLVMENU_H
#define _DDLVMENU_H

#include  "type_def.h"

//Floatinput
extern void    FloatInput_Interface(sint32 cur_menu_op);
extern sint32  FloatInput_Key_Pro(uint32 key_c,sint32 op);
extern void    FloatInput_Disp_Pro(void);

//电导常数输入
extern void    DDLV_CONS_Input_Interface(sint32 cur_menu_op);
extern sint32  DDLV_CONS_Key_Pro(uint32 key_c,sint32 op);
extern void    DDLV_CONS_Disp_Pro(void);

//测试模式选择 
extern void    ModeInput_Interface(sint32 cur_menu_op);
extern sint32  Mode_Key_Pro(uint32 key_c,sint32 op);
extern void    Mode_Disp_Pro(void);

//balance configuration 
extern void    BalanceInput_Interface(sint32 cur_menu_op);
extern sint32  BalanceInput_Key_Pro(uint32 key_c,sint32 op);
extern void    Balance_Disp_Pro(void);

//para3
extern void    Para3Input_Interface(sint32 cur_menu_op);
extern sint32  Para3_Key_Pro(uint32 key_c,sint32 op);
extern void    Para3_Disp_Pro(void);

#endif