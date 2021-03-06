#ifndef _MAINMENU_H
#define _MAINMENU_H

#include  "type_def.h"

extern void     UnitSel_Interface(sint32 cur_menu_op);
extern sint32   UnitSel_Key_Pro(uint32 key_c,sint32 op);
extern void     UnitSel_Disp_Pro(void);
//ASD
extern void     ASDSel_Interface(sint32 cur_menu_op);
extern sint32   ASDSel_Key_Pro(uint32 key_c,sint32 op);
extern void     ASDSel_Disp_Pro(void);
//PRT
extern void     PRTSel_Interface(sint32 cur_menu_op);
extern sint32   PRTSel_Key_Pro(uint32 key_c,sint32 op);
extern void     PRTSel_Disp_Pro(void);

//LNT
extern void     LNTSel_Interface(sint32 cur_menu_op);
extern sint32   LNTSel_Key_Pro(uint32 key_c,sint32 op);
extern void     LNTSel_Disp_Pro(void);
//CAL-10POINT
extern void     Cal_10point_Interface(sint32 cur_menu_op);
extern sint32   Cal_10point_Key_Pro(uint32 key_c,sint32 op);
extern void     Cal_10point_Disp_Pro(void);

#endif