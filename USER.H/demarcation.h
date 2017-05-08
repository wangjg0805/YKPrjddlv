//**********************************************************************
//文件名:demarcation.h
//**********************************************************************
#ifndef _DEMARCATION_H
#define _DEMARCATION_H
#include "type_def.h"
//定义菜单及项目号(高8位为菜单号,低8位为选项号)
#define DEMAR_MENU_NUM            5            //标定模式下的菜单个数

#define MENU_DEMAR_LG_SEL            0x00    //语言选择对话框
#define MENU_DEMAR_LG_OP_NUM        2        //选项个数
#define MENU_DEMAR_LG_SEL_CH_OP        0x0000    //中文
#define MENU_DEMAR_LG_SEL_EN_OP        0x0001    //英文

#define DEMAR_MAIN_MENU            0x01        //标定模式主菜单
#define DEMAR_MAIN_MENU_OPS        3            //选项个数
#define DEMAR_MAIN_MENU_VT        0x0100        //标定模式主菜单下的"潮气量标定"选项
#define DEMAR_MAIN_MENU_O2        0x0101        //标定模式主菜单下的"氧气传感器标定"选项
#define DEMAR_MAIN_MENU_PAW        0x0102        //标定模式主菜单下的"压力传感器标定"选项

#define DEMAR_NEW_PAW_CUR        0x02        //标定PAW菜单
#define DEMAR_NEW_PAW_OPS        6            //选项个数
#define DEMAR_NEW_PAW_EXIT        0x0200        //标定PAW时的"退出"按钮
#define DEMAR_NEW_PAW_SAVE        0x0201        //标定PAW时的"保存"按钮
#define DEMAR_NEW_PAW_CHECK        0x0202        //"检查"
#define DEMAR_NEW_PAW_NEXT        0x0203        //标定PAW时的"下一点"按钮
#define DEMAR_NEW_PAW_PRE        0x0204        //标定PAW时的"前一点"按钮
#define DEMAR_NEW_PAW_AD_IN        0x0205        //标定PAW时的"手动输入AD"选项

#define DEMAR_O2_SEN            0x03        //标定氧气传感器
#define DEMAR_O2_SEN_EXIT        0x0300        //标定氧气传感器时"退出"
#define DEMAR_O2_SEN_SAVE        0x0301        //标定氧气传感器时"保存"
#define DEMAR_O2_SEN_CHCEK        0x0302        //检查
#define DEMAR_O2_SEN_NEXT        0x0303        //下一点
#define DEMAR_O2_SEN_PRE        0x0304        //前一点
#define DEMAR_O2_SEN_AD_IN        0x0305        //标定氧气传感器时"输入AD"

#define DEMAR_VT                0x04        //标定潮气量
#define DEMAR_VT_OPS            6            //选项个数
#define DEMAR_VT_EXIT            0X0400        //标定潮气量时"退出"
#define DEMAR_VT_SAVE            0x0401        //标定潮气量时"保存"
#define DEMAR_VT_CHECK            0x0402        //"检查"
#define DEMAR_VT_NEXT            0x0403        //标定潮气量时"下一点"
#define DEMAR_VT_PRE            0x0404        //标定潮气量时"前一点"
#define DEMAR_VT_LPS_IN            0x0405        //标定潮气量时"输入圈数"

#define DEMAR_AD_REQ            0x04        //AD查询界面
#define MENU_AD_REQ_PAW_BAND    0x0400        //"PAW滤波带宽"
#define MENU_AD_REQ_VT_LEN        0x0401        //"潮气量滤波深度"

void Demar_Manage(void);
#endif
