#ifndef _MAINMENU2_H
#define _MAINMENU2_H

#include  "type_def.h"

//bright
extern void    Brig_Adjust_Interface(sint32 cur_menu_op);
extern sint32  Brig_Adjust_Key_Pro(uint32 key_c,sint32 op);
extern void    Brig_Adjust_Disp_Pro(void);
//数据回显示
extern void    Data_Call_Interface(sint32 cur_menu_op);
extern sint32  Data_Call_Key_Pro(uint32 key_c,sint32 op);
extern void    Data_Call_Disp_Pro(void);


//
extern void    DivisionSel_Interface(sint32 cur_menu_op);
extern sint32  DivisionSel_Key_Pro(uint32 key_c,sint32 op);
extern void    DivisionSel_Disp_Pro(void);
//wireless config
extern void    TempcompCfg_Interface(sint32 cur_menu_op);
extern sint32  TempcompCfg_Key_Pro(uint32 key_c,sint32 op);
extern void    TempcompCfg_Disp_Pro(void);
//wireless config
extern void    UserIDInput_Interface(sint32 cur_menu_op);
extern sint32  UserID_Key_Pro(uint32 key_c,sint32 op);
extern void    UserID_Disp_Pro(void);

//wireless config
extern void    StandardSelInput_Interface(sint32 cur_menu_op);
extern sint32  Standard_Key_Pro(uint32 key_c,sint32 op);
extern void    Standard_Sel_Disp_Pro(void);
//SCREEN CAL
extern void    ScrCalInput_Interface(sint32 cur_menu_op);
extern sint32  ScrCal_Key_Pro(uint32 key_c,sint32 op);
extern void    ScrCal_Disp_Pro(void);

//language
extern void    LangSel_Interface(sint32 cur_menu_op);
extern sint32  LangSel_Key_Pro(uint32 key_c,sint32 op);
extern void    LangSel_Disp_Pro(void);

//Uint input
extern void    UINTInput_Interface(sint32 cur_menu_op);
extern sint32  UINTInput_Key_Pro(uint32 key_c,sint32 op);
extern void    UINTInput_Disp_Pro(void);

//system config 
extern void    SystemConf_Interface(sint32 cur_menu_op);
extern sint32  SystemConf_Key_Pro(uint32 key_c,sint32 op);
extern void    SystemConf_Disp_Pro(void);

//system config 
extern void    OperConfirmInput_Interface(sint32 cur_menu_op);
extern sint32  Confirm_Key_Pro(uint32 key_c,sint32 op);
extern void    Confirm_Disp_Pro(void);


#endif