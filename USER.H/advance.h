#ifndef _ADVANCE_H
#define _ADVANCE_H

#define MENU_AD_LG_SEL                0x00    //语言选择对话框
#define MENU_AD_LG_OP_NUM            2        //选项个数
#define MENU_AD_LG_SEL_CH_OP        0x0000    //中文
#define MENU_AD_LG_SEL_EN_OP        0x0001    //英文

#define MENU_AD_PARA                0x01    //高级参数菜单
#define MENU_AD_PARA_OP_NUM            4        //选项个数
#define MENU_AD_VT_FILT_CONST_OP    0x0100//潮气量滤波常数
#define MENU_AD_O2_FLASH_CYC_OP        0x0101//氧浓度刷新周期
#define MENU_AD_PL_COMP_OP            0x0102//压力限制补偿
#define MENU_AD_PAW_NO_R_OP            0x0103//管压无提升
void Advance_Manage(void);

#endif