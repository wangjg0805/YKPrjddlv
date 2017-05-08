//**********************************************************************
//文件名:disp_menu.c
//**********************************************************************

#include "global.h"
#include "lcd2478_graph.h"
//#include "disp.h"
#include "FontLib.h"
#include "graph.h"
#include "timer.h"

//**********************************************************************
//函数  名:Disp_VT_Meas_Main
//函数功能:在主界面中显示VT测量值的菜单
//入口参数:vt_m--潮气量测量值
//			vt_units--潮气量测量值的单位(10--0.1ml;100--0.01ml)
//			vt_alarm_h--潮气量报警上限(ml)
//			vt_alarm_l--潮气量报警下限(ml)
//出口参数:无
//**********************************************************************
void Disp_VT_Meas_Main(int vt_m,int vt_units,int vt_alarm_h,int vt_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (0,117,63,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(0,62,125,0xaa);
	Hor_Pattern_Line(0,62,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	ch_x = 31-Get_Line_Width("潮气量")/2;
	en_x = 31-Get_Line_Width("Vte")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("潮气量","Vte"));
	
	Set_ASC_Lib(Arial8_Font);
	en_x = 31-Get_Line_Width("mL")/2;
	Put_Line_Str(en_x,177,"mL");
	Put_Numeral(36,135,vt_alarm_h,1);//报警上限
	Put_Numeral(36,169,vt_alarm_l,1);//报警下限
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//潮气量测量值
	if ( (vt_m>vt_alarm_h+10)||(vt_m+10<vt_alarm_l) )
	{
		en_x = 31-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 31-Get_Num_Width(vt_m,vt_units)/2;
		Put_Numeral(en_x,145,vt_m,vt_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值			
}
//**********************************************************************
//函数  名:Disp_VT_Meas_Main_Titl
//函数功能:在主界面中显示VT测量值菜单的标题和单位
//入口参数:disp_mode(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_VT_Meas_Main_Titl(char disp_mode)
{
	unsigned short en_x,ch_x;
	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (0,117,62,189);
	Hor_Pattern_Line(0,62,125,0xaa);
	Hor_Pattern_Line(0,62,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Set_ASC_Lib(Arial12B_Font);	//设置字库
	Set_HZ_Lib(&ST_11);
	ch_x = 31-Get_Line_Width("潮气量")/2;
	en_x = 31-Get_Line_Width("Vte")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("潮气量","Vte"));
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(0,117,62,189);
	}	
}

//**********************************************************************
//函数  名:Disp_Vt_AlmH_Main
//函数功能:在主菜单中显示潮气量报警上下限
//入口参数: vt_alarm_h--潮气量报警上限(ml)
//			vt_alarm_l--潮气量报警下限(ml)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Vt_AlmH_Main(int vt_alarm_h,int vt_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar(36,135,59,145);				//清除上限区
	Bar(36,169,59,178);				//清除下限区
	Set_ASC_Lib(Arial8_Font);
	Set_Write_Mode(WRITE_OR);
	Put_Numeral(36,135,vt_alarm_h,1);//报警上限
	Put_Numeral(36,169,vt_alarm_l,1);//报警下限	
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(36,135,59,145);
		LCD_Refresh_Area(36,169,59,178);
	}
}

//**********************************************************************
//函数  名:Disp_Vt_MeasVl_Main
//函数功能:在主菜单中显示潮气量测量值
//入口参数:vt_m--潮气量测量值
//			vt_units--潮气量测量值的单位(10--0.1ml;100--0.01ml)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Vt_MeasVl_Main(int vt_m,int vt_units,int disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar(0,146,61,168);			//数值区
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//潮气量测量值
	if ( (vt_m>2000)||(vt_m<0) )
	{
		en_x = 31-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 31-Get_Num_Width(vt_m,vt_units)/2;
		Put_Numeral(en_x,145,vt_m,vt_units);
	}
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(0,146,61,168);
	}		

}

//**********************************************************************
//函数  名:Disp_Fr_Meas_Main
//函数功能:在主界面中显示呼吸频率测量值的菜单
//入口参数:fr--呼吸频率(单位:次/min)
//出口参数:无
//**********************************************************************
void Disp_Fr_Meas_Main(int fr)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (62,117,65,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(62,126,125,0xaa);	//虚线边框
	Hor_Pattern_Line(62,126,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	ch_x = 94-Get_Line_Width("呼吸频率")/2;
	en_x = 94-Get_Line_Width("Fr")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("呼吸频率","Fr"));
	
	Set_ASC_Lib(Arial8_Font);
	en_x = 94-Get_Line_Width("/min")/2;
	Put_Line_Str(en_x,177,"/min");

	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//呼吸频率测量值
	if (fr>200)
	{
		en_x = 94-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 94-Get_Num_Width(fr,1)/2;
		Put_Numeral(en_x,145,fr,1);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值			
}

//**********************************************************************
//函数  名:Disp_Fr_Meas_Main_Title
//函数功能:在主界面中显示呼吸频率测量值菜单的标题和框架
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Fr_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (62,117,126,189);
	Hor_Pattern_Line(62,126,125,0xaa);	//虚线边框
	Hor_Pattern_Line(62,126,189,0xaa);
	Ver_Pattern_Line(125,189,62,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);	//设置字库
	Set_HZ_Lib(&ST_11);	
	ch_x = 94-Get_Line_Width("呼吸频率")/2;
	en_x = 94-Get_Line_Width("Fr")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("呼吸频率","Fr"));	
	Set_ASC_Lib(Arial8_Font);
	en_x = 94-Get_Line_Width("/min")/2;
	Put_Line_Str(en_x,177,"/min");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(62,117,126,189);
	}
}

//**********************************************************************
//函数  名:Disp_Fr_MeasVl_Main
//函数功能:在主菜单中显示呼吸频率的测量值
//入口参数:fr--呼吸频率(单位:次/min)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Fr_MeasVl_Main(int fr,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (63,146,125,168);	
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//呼吸频率测量值
	if (fr>200)
	{
		en_x = 94-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 94-Get_Num_Width(fr,1)/2;
		Put_Numeral(en_x,145,fr,1);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(63,146,125,168);
	}	
}

//**********************************************************************
//函数  名:Disp_Insp_Flx_Meas_Main
//函数功能:在主界面中显示分钟通气测量值的菜单
//入口参数: fl_m--分钟通气测量值
//			fl_units--分钟通气测量值的单位(10--0.1L/min;100--0.01L/min)
//			fl_alarm_h--分钟通气报警上限(0.1L/min)
//			fl_alarm_l--分钟通气报警下限(0.1L/min)
//出口参数:无
//**********************************************************************
void Disp_Insp_Flx_Meas_Main(int fl_m,int fl_units,int fl_alarm_h,int fl_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (126,117,66,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(126,191,125,0xaa);
	Hor_Pattern_Line(126,191,189,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	ch_x = 159-Get_Line_Width("分钟通气")/2;
	en_x = 159-Get_Line_Width("Ve")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("分钟通气","Ve"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 159-Get_Line_Width("L/min")/2;
	Put_Line_Str(en_x,177,"L/min");
	Put_Numeral(165,135,fl_alarm_h,10);//报警上限
	Put_Numeral(165,169,fl_alarm_l,10);//报警下限

	Set_ASC_Lib(Arial20B_Font);			//分钟通气测量值
	if ( (fl_m>fl_alarm_h+10)||(fl_m+10<fl_alarm_l) )
	{
		en_x = 159-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 159-Get_Num_Width(fl_m,fl_units)/2;
		Put_Numeral(en_x,145,fl_m,fl_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值			
}

//**********************************************************************
//函数  名:Disp_Insp_Flx_Meas_Main_Title
//函数功能:在主界面中显示分钟通气的标题
//入口参数:disp_mode--刷新方式
//出口参数:无
//**********************************************************************
void Disp_Insp_Flx_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (126,117,191,189);
	Hor_Pattern_Line(126,191,125,0xaa);
	Hor_Pattern_Line(126,191,189,0xaa);
	Ver_Pattern_Line(125,189,126,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);
	Set_HZ_Lib(&ST_11);	
	ch_x = 159-Get_Line_Width("分钟通气")/2;
	en_x = 159-Get_Line_Width("Ve")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("分钟通气","Ve"));	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 159-Get_Line_Width("L/min")/2;
	Put_Line_Str(en_x,177,"L/min");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(126,117,191,189);
	}
}

//**********************************************************************
//函数  名:Disp_Insp_Flx_Alm_Main
//函数功能:在主界面中显示分钟通气的报警上下限
//入口参数:fl_alarm_h--分钟通气报警上限(0.1L/min)
//			fl_alarm_l--分钟通气报警下限(0.1L/min)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Insp_Flx_Alm_Main(int fl_alarm_h,int fl_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (163,135,190,145);			//清除上限区域
	Bar (163,169,190,178);			//清除下限区
	Set_ASC_Lib(Arial8_Font);
	Set_Write_Mode(WRITE_OR);
	Put_Numeral(165,135,fl_alarm_h,10);//报警上限
	Put_Numeral(165,169,fl_alarm_l,10);//报警下限	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(163,135,190,145);
		LCD_Refresh_Area(163,169,190,178);
	}	
}

//**********************************************************************
//函数  名:Disp_Insp_Flx_Vl_Main
//函数功能:在主菜单中显示分钟通气的测量值
//入口参数:fl_m--分钟通气测量值
//			fl_units--分钟通气测量值的单位(10--0.1L/min;100--0.01L/min)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Insp_Flx_Vl_Main(int fl_m,int fl_units,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (127,146,190,168);
	
	Set_ASC_Lib(Arial20B_Font);			//分钟通气测量值
	Set_Write_Mode(WRITE_OR);
	if ( (fl_m>1000)||(fl_m<0) )
	{
		en_x = 159-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 159-Get_Num_Width(fl_m,fl_units)/2;
		Put_Numeral(en_x,145,fl_m,fl_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(127,146,190,168);	
	}			
}

//**********************************************************************
//函数  名:Disp_Paw_Max_Meas_Main
//函数功能:在主界面中显示最大压力测量值的菜单
//入口参数: paw_max--最大压力测量值
//			paw_units--最大压力测量值的单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//出口参数:无
//**********************************************************************
void Disp_Paw_Max_Meas_Main(int paw_max,int paw_units)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (191,117,66,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,256,125,0xaa);
	Hor_Pattern_Line(191,256,189,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
	ch_x = 224-Get_Line_Width("最大压力")/2;	//显示标题
	en_x = 224-Get_Line_Width("Pmax")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("最大压力","Pmax"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 224-Get_Line_Width("cmH2O")/2;
	Put_Line_Str(en_x,177,"cmH2O");

	Set_ASC_Lib(Arial20B_Font);			//最大压力测量值
	if (paw_max >= 200)
	{
		en_x = 224-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 224-Get_Num_Width(paw_max,paw_units)/2;
		Put_Numeral(en_x,145,paw_max,paw_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值			
}

//**********************************************************************
//函数  名:Disp_Paw_Max_Meas_Main_Title
//函数功能:在主菜单中显示最大压力测量值菜单的标题
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Paw_Max_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar (191,117,256,189);
	Hor_Pattern_Line(191,256,125,0xaa);
	Hor_Pattern_Line(191,256,189,0xaa);
	Ver_Pattern_Line(125,189,191,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);	
	Set_ASC_Lib(Arial12B_Font);//设置字库
	Set_HZ_Lib(&ST_11);	
	ch_x = 224-Get_Line_Width("最大压力")/2;	//显示标题
	en_x = 224-Get_Line_Width("Pmax")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("最大压力","Pmax"));
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 224-Get_Line_Width("cmH2O")/2;
	Put_Line_Str(en_x,177,"cmH2O");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,117,256,189);
	}
}

//**********************************************************************
//函数  名:Disp_Paw_Max_Meas_Vl_Main
//函数功能:在主菜单中显示最大压力的测量值
//入口参数:paw_max--最大压力测量值
//			paw_units--最大压力测量值的单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Paw_Max_Meas_Vl_Main(int paw_max,int paw_units,char disp_mode)
{
	unsigned short en_x;
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar (192,146,255,168);
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//最大压力测量值
	if (paw_max >= 200)
	{
		en_x = 224-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 224-Get_Num_Width(paw_max,paw_units)/2;
		Put_Numeral(en_x,145,paw_max,paw_units);
	}
	
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(192,146,255,168);
	}		
}

//**********************************************************************
//函数  名:Disp_O2_Meas_Main
//函数功能:在主界面中显示氧气浓度测量值的菜单
//入口参数: o2_m--氧气浓度测量值
//			o2_units--氧气浓度测量值的单位(10--0.1%;100--0.01%)
//			o2_alarm_h--氧气浓度报警上限(1%)
//			o2_alarm_l--氧气浓度报警下限(1%)
//出口参数:无
//**********************************************************************
void Disp_O2_Meas_Main(int o2_m,int o2_units,int o2_alarm_h,int o2_alarm_l)
{
	unsigned short ch_x,en_x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (256,117,64,73,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(256,319,125,0xaa);
	Hor_Pattern_Line(256,319,189,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
	ch_x = 288-Get_Line_Width("氧气浓度")/2;
	en_x = 288-Get_Line_Width("O2")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("氧气浓度","O2"));
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 288-Get_Line_Width("%")/2;
	Put_Line_Str(en_x,177,"%");
	Put_Numeral(297,135,o2_alarm_h,1);//报警上限
	Put_Numeral(297,169,o2_alarm_l,1);//报警下限

	Set_ASC_Lib(Arial20B_Font);			//氧气浓度测量值
	if ( (o2_m>o2_alarm_h+10)||(o2_m+10<o2_alarm_l) )
	{
		en_x = 288-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 288-Get_Num_Width(o2_m,o2_units)/2;
		Put_Numeral(en_x,145,o2_m,o2_units);
	}
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值			
}

//**********************************************************************
//函数  名:Disp_O2_Meas_Main_Title
//函数功能:在主界面中显示氧气浓度测量值菜单的标题
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Disp_O2_Meas_Main_Title(char disp_mode)
{
	unsigned short ch_x,en_x;

	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (256,117,319,189);
	Hor_Pattern_Line(256,319,125,0xaa);
	Hor_Pattern_Line(256,319,189,0xaa);
	Ver_Pattern_Line(125,189,256,0xaa);
		
	Set_ASC_Lib(Arial12B_Font);//设置字库
	Set_HZ_Lib(&ST_11);
	ch_x = 288-Get_Line_Width("氧气浓度")/2;
	en_x = 288-Get_Line_Width("O2")/2;
	Put_Line_Str(Language_Pos(ch_x,en_x),117,Language_Str("氧气浓度","O2"));	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);
	en_x = 288-Get_Line_Width("%")/2;
	Put_Line_Str(en_x,177,"%");	
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(256,117,319,189);
	}
}

//**********************************************************************
//函数  名:Disp_O2_Meas_Alm_Main
//函数功能:在主菜单中显示氧气报警上下限
//入口参数:o2_alarm_h--氧气浓度报警上限(1%)
//			o2_alarm_l--氧气浓度报警下限(1%)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_O2_Meas_Alm_Main(int o2_alarm_h,int o2_alarm_l,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (297,135,319,145);	
	Bar (297,169,319,178);

	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial8_Font);	
	Put_Numeral(297,135,o2_alarm_h,1);//报警上限
	Put_Numeral(297,169,o2_alarm_l,1);//报警下限	
	
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(297,135,319,145);
		LCD_Refresh_Area(297,169,319,178);
	}	
}

//**********************************************************************
//函数  名:Disp_O2_Meas_Vl_Main
//函数功能:在主界面中显示氧气浓度的测量值
//入口参数:o2_m--氧气浓度测量值
//			o2_units--氧气浓度测量值的单位(10--0.1%;100--0.01%)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_O2_Meas_Vl_Main(int o2_m,int o2_units,char disp_mode)
{
	unsigned short en_x;
	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar (257,146,319,168);
	
	Set_Write_Mode(WRITE_OR);
	Set_ASC_Lib(Arial20B_Font);			//氧气浓度测量值
	if ( (o2_m>100*o2_units)||(o2_m<0) )
	{
		en_x = 288-Get_Line_Width("----")/2;
		Put_Line_Str(en_x,145,"----");
	}
	else
	{
		en_x = 288-Get_Num_Width(o2_m,o2_units)/2;
		Put_Numeral(en_x,145,o2_m,o2_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(257,146,319,168);
	}			
}

//**********************************************************************
//函数  名:Disp_Pavr_Meas_Main
//函数功能:主界面中显示平均压力
//入口参数:pavr--压力平均值
//		   pavr_units--压力单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//出口参数:无
//**********************************************************************
void Disp_Pavr_Meas_Main(int pavr,int pavr_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (191,96,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,96,0xaa);
	Hor_Pattern_Line(191,319,114,0xaa);
	Ver_Pattern_Line(96,114,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,98,Language_Str("平均压力","Pavr"));
	
	Set_ASC_Lib(Arial12B_Font);
	if (pavr >= 100)
	{
		x = Put_Line_Str(x+3,98,"--");
	}
	else
	{
		x = Put_Numeral(x+3,98,pavr,pavr_units);
	}
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,100,"cmH2O");	
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值		
}

//**********************************************************************
//函数  名:Disp_Pavr_Meas_Main_Title
//函数功能:在主界面中显示平均压力测量值的菜单标题
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Pavr_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Hor_Pattern_Line(191,319,96,0xaa);
	Hor_Pattern_Line(191,319,114,0xaa);
	Ver_Pattern_Line(96,114,191,0xaa);	
	Bar(192,97,253,113);
	Bar(283,97,319,113);
		
	Set_HZ_Lib(&ST_11);
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);	
	Put_Line_Str(193,98,Language_Str("平均压力","Pavr"));
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(283,100,"cmH2O");
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,96,253,114);
		LCD_Refresh_Area(283,96,319,114);
	}		
}

//**********************************************************************
//函数  名:Disp_Pavr_vl_Meas_Main
//函数功能:在主界面中显示平均压力测量值
//入口参数:pavr--压力平均值
//		   pavr_units--压力单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无	
//**********************************************************************
void Disp_Pavr_vl_Meas_Main(int pavr,int pavr_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar(254,97,282,113);
	
	Set_Write_Mode(WRITE_OR);	
	Set_ASC_Lib(Arial12B_Font);
	if (pavr >= 1000)
	{
		Put_Line_Str(254,98,"---");
	}
	else
	{
		Put_Numeral(254,98,pavr,pavr_units);
	}
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(254,96,282,114);
	}		
}

//**********************************************************************
//函数  名:Disp_Peep_Meas_Main
//函数功能:主界面中显示呼末正压
//入口参数:peep--呼末正压
//		   peep_units--压力单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//出口参数:无
//**********************************************************************
void Disp_Peep_Meas_Main(int peep,int peep_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (191,78,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,78,0xaa);
	Hor_Pattern_Line(191,319,96,0xaa);
	Ver_Pattern_Line(78,96,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,80,Language_Str("呼末正压","Peep"));
	
	Set_ASC_Lib(Arial12B_Font);
	if (peep>=100)
	{
		x = Put_Line_Str(x+3,80,"--");
	}
	else
	{
		x = Put_Numeral(x+3,80,peep,peep_units);
	}
	
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,82,"cmH2O");	
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值		
}

//**********************************************************************
//函数  名:Disp_Peep_Meas_Main_Title
//函数功能:在主界面中显示呼末正压测量值菜单的标题
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Peep_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Hor_Pattern_Line(191,319,78,0xaa);
	Hor_Pattern_Line(191,319,96,0xaa);
	Ver_Pattern_Line(78,96,191,0xaa);	
	Bar(192,79,253,95);
	Bar(283,79,319,95);
		
	Set_ASC_Lib(Arial12B_Font);//设置字库
	Set_HZ_Lib(&ST_11);	
	Set_Write_Mode(WRITE_OR);	
	Put_Line_Str(193,80,Language_Str("呼末正压","Peep"));
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(283,82,"cmH2O");
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,78,253,96);
		LCD_Refresh_Area(283,78,319,96);
	}	
}

//**********************************************************************
//函数  名:Disp_Peep_Meas_Vl_Main
//函数功能:在主界面中显示呼末正压测量值
//入口参数:peep--呼末正压
//		   peep_units--压力单位(1--1cmH2O;10--0.1cmH2O;100--0.01cmH2O)	
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Peep_Meas_Vl_Main(int peep,int peep_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Bar(254,79,282,95);	
	
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);	
	if (peep>=1000)
	{
		Put_Line_Str(254,80,"---");
	}
	else
	{
		Put_Numeral(254,80,peep,peep_units);
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(254,78,282,96);
	}	
}

//**********************************************************************
//函数  名:Disp_Humor_Meas_Main
//函数功能:主界面中显示顺应性
//入口参数:humor--顺应性
//		   humor_units--顺应性单位(1--1ml/cmH2O;10--0.1ml/cmH2O;100--0.01ml/cmH2O)	
//出口参数:无
//**********************************************************************
void Disp_Humor_Meas_Main(int humor,int humor_units)
{
	unsigned short x;

	//Set_Refresh_Mode(0);	//默认为没有事实刷新
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	//Set_Write_Mode(WRITE_OVERLAY);//默认为覆盖模式
	
	Clr_Bar (191,59,129,19,COL_WHITE);
	Set_ASC_Lib(Arial12B_Font);
	Hor_Pattern_Line(191,319,59,0xaa);	//边框
	Hor_Pattern_Line(191,319,78,0xaa);
	Ver_Pattern_Line(59,78,191,0xaa);
	
	Set_Write_Mode(WRITE_OR);	
	x = Put_Line_Str(193,61,Language_Str("顺应性","Humor"));//标题
	
	Set_ASC_Lib(Arial12B_Font);
	if (humor >= 100)
	{
		x = Put_Line_Str(x+3,61,"--");
	}
	else
	{	
		x = Put_Numeral(x+3,61,humor,humor_units);	//顺应性的值
	}
	
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(x+3,63,"ml/cmH2O");	//单位
	Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值		
}

//**********************************************************************
//函数  名:Disp_Humor_Meas_Main_Title
//函数功能:在主界面中显示顺应性测量值的菜单标题
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Humor_Meas_Main_Title(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Hor_Pattern_Line(191,319,59,0xaa);	//边框
	Hor_Pattern_Line(191,319,78,0xaa);
	Ver_Pattern_Line(59,78,191,0xaa);			
	Bar(192,60,239,77);
	Bar(269,60,319,77);
			
	Set_ASC_Lib(Arial12B_Font);//设置字库
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OR);
	Put_Line_Str(193,61,Language_Str("顺应性","Humor"));//标题
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(269,63,"ml/cmH2O");	//单位
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(191,59,239,77);
		LCD_Refresh_Area(269,59,319,77);
	}
}

//**********************************************************************
//函数  名:Disp_Humor_Meas_Vl_Main
//函数功能:在主界面中显示顺应性的测量值
//入口参数:humor--顺应性
//		   humor_units--顺应性单位(1--1ml/cmH2O;10--0.1ml/cmH2O;100--0.01ml/cmH2O)	
//			disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Disp_Humor_Meas_Vl_Main(int humor,int humor_units,char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(240,60,268,77);	
	
	Set_ASC_Lib(Arial12B_Font);
	Set_Write_Mode(WRITE_OR);
	if (humor >= 1000)
	{
		Put_Line_Str(240,61,"--");
	}
	else
	{	
		Put_Numeral(240,61,humor,humor_units);	//顺应性的值
	}
	
	if (disp_mode&0x80)
	{
		LCD_Refresh_Area(240,59,268,77);
	}	
}

//**********************************************************************
//函数  名:Disp_Asph_Alm_Set_Main
//函数功能:在主界面中显示窒息报警设置栏
//入口参数:asph_alm--(ON:开;OFF:关)
//		   disp_mode--显示方式(最高位为1表示即时刷新,次高位为1表示初始化控制变量)
//出口参数:无
//**********************************************************************
void Disp_Asph_Alm_Set_Main(char asph_alm,char disp_mode)
{
	static char asph_alm_disp;	//保存窒息报警显示值,如果asph_alm与asph_alm_disp不一样则闪烁2次
	static char flash_count;
	static char disp_title_count;	//标题显示记录,不为1表示标题已经显示过,无需再显示
	static int flash_time;
	unsigned short di;
	
	if (disp_mode&0x40)	//仅仅初始化变量
	{
		asph_alm_disp = 0;
		flash_count = 0;
		disp_title_count = 0;
		flash_time = 0;
		return;
	}
	
	if (!disp_title_count)
	{//未显示标题则先显示标题
		disp_title_count = 1;
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);	//覆盖显示
		Bar(191,40,280,57);
		Bar(281,40,319,57);
		Set_Write_Mode(WRITE_EXOR);	//异或显示
		Set_ASC_Lib(Arial12B_Font);
		di = Put_Line_Str(194,42,Language_Str("窒息报警","HumAlarm"));
		Put_Line_Str(di,40,Language_Str("----","----"));
		if (asph_alm == OFF)
		{
			Put_Line_Str(282,42,Language_Str("关闭","OFF"));
		}
		else
		{
			Put_Line_Str(282,42,Language_Str("开","ON"));
		}		
		if (disp_mode&0x80) //即时刷新
		{
			LCD_Refresh_Area(191,40,280,57);
			LCD_Refresh_Area(281,40,319,57);
		}
		asph_alm_disp = asph_alm;		
	}

	if (asph_alm_disp != asph_alm)	//设置闪烁次数
	{
		asph_alm_disp = asph_alm;	//保存设置显示
		flash_count = 6;
		flash_time = Get_Cur_Time();	//记录起始闪烁时间
	}
	
	if (flash_count)	//比较闪烁时间
	{
		if (Cal_Time_Dif(flash_time)>300)
		{//时间到
			flash_count--;
			flash_time = Get_Cur_Time();//重新记录闪烁时间
			switch (flash_count)
			{
				case 8:
				case 6:
				case 4:
				case 2:
				case 0:			
					Set_Graphic_Color(COL_BLACK,COL_WHITE);
					Set_Chr_Color(COL_BLACK,COL_WHITE);
					Set_Write_Mode(WRITE_OVERLAY);	//覆盖显示
					Bar(281,40,319,57);
					Set_Write_Mode(WRITE_EXOR);	//异或显示
					Set_ASC_Lib(Arial12B_Font);
					if (asph_alm == OFF)
					{
						Put_Line_Str(282,42,Language_Str("关闭","OFF"));
					}
					else
					{
						Put_Line_Str(282,42,Language_Str("开","ON"));
					}	
					Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值	
					break;
				case 1:
				case 3:
				case 5:
				case 7:
					Set_Graphic_Color(COL_BLACK,COL_WHITE);
					Set_Chr_Color(COL_BLACK,COL_WHITE);
					Set_Write_Mode(WRITE_OVERLAY);	//覆盖显示
					Bar(281,40,319,57);
					break;	
				default:break;
			}
			if (disp_mode&0x80)//即时刷新
			{
				LCD_Refresh_Area(281,40,319,57);
			}			
		}	
	}
}

//**********************************************************************
//函数  名:Disp_Alm_Area_Main
//函数功能:显示顶部报警区域
//入口参数:alarm--报警监测量
//			disp_mode--显示方式(最高位为1表示即时刷新,次高位为1表示清除控制量)
//出口参数:无 
//**********************************************************************
void Disp_Alm_Area_Main(unsigned int alarm,char disp_mode)
{
	static unsigned int area1_item,area2_item,erea3_item,area4_item;	//记录报警项目
	static char disp_title_flag;
	
	//判别要显示的报警项目
	

	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Pattern_Bar(23,0,319,38,0xaa);
	
	//根据报警项目显示报警信息
	
	if (disp_mode&0x80)
	{
		if (!disp_title_flag)
		{
			LCD_Refresh_Area(23,0,319,38);
			disp_title_flag = 1;
		}
	}
}

//**********************************************************************
//函数  名:Disp_Alm_Silence_Main
//函数功能:在主菜单中显示报警静音
//入口参数:alm_sl_time--报警静音时间(单位:s)
//			disp_mode--(最高位为1表示即时刷新;次高位为1表示初始化控制变量)
//出口参数:无
//**********************************************************************
void Disp_Alm_Silence_Main(int alm_sl_time,char disp_mode)
{
	unsigned short x;
	static char disp_title;
	static char alm_silence_flag;	//开报警音标志
	static int alm_sl_time_disp;
	
	if (disp_mode&0x40)
	{
		disp_title = 0;
		alm_silence_flag = 0;
		alm_sl_time_disp = 0xffffffff;
		return;
	}
	
	if (alm_sl_time)	//报警静音时间不为0则显示报警静音图标
	{
		if (!alm_silence_flag)
		{
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
			if (disp_mode & 0x80)
			{
				Set_Refresh_Mode(1);
			}
			Put_Image(0,0,Alarm_Sil_Icon,WRITE_OVERLAY);
			alm_silence_flag = 1;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Rectangle(0,18,21,38);
			Set_Refresh_Mode(0);
		}	
	}
	else	//报警静音时间为0,显示报警扬声器图标
	{
		if (alm_silence_flag)
		{
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
			if (disp_mode & 0x80)
			{
				Set_Refresh_Mode(1);
			}		
			Put_Image(0,0,Alarm_Loud_Icon,WRITE_OVERLAY);
			alm_silence_flag = 0;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Rectangle(0,18,21,38);		
			Set_Refresh_Mode(0);
		}	
	}

	if (alm_sl_time_disp != alm_sl_time)
	{	
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
		Bar(1,19,20,37);
						
		Set_ASC_Lib(Arial12B_Font);
		Set_Write_Mode(WRITE_OR);		
		if (alm_sl_time>99 || alm_sl_time==0)
		{
			x = 11-Get_Line_Width("--")/2;
			Put_Line_Str(x,21,"--");
		}
		else
		{
			x = 11-Get_Num_Width(alm_sl_time,1)/2;
			Put_Numeral(x,21,alm_sl_time,1);
		}
		if (disp_mode&0x80)
		{
			LCD_Refresh_Area(1,19,20,37);
		}	
		alm_sl_time_disp = alm_sl_time;//保存显示值
	}		
}

//**********************************************************************
//函数  名:Disp_Brth_Mode_Main
//函数功能:在主界面下显示呼吸模式
//入口参数:br_mode--呼吸模式
//			br_meth--呼吸方式(手动/自动)
//			disp_mode--最高位为1表示实时刷新,次高位为1表示初始化控制变量
//出口参数:无
//**********************************************************************
void Disp_Brth_Mode_Main(char br_mode,char br_meth,char disp_mode)
{
	unsigned short x;
	static char flash_count=0;	//=0表示呼气图标;=1表示吸气图标(最高位为1表示显示)
	static char flash_flag=0;		//=1已经闪烁
	static int flash_time;		//记录闪烁时间
	static char disp_title_count;//标题显示
	
	
	if (disp_mode &0x40)	//初始化显示控制变量
	{
		flash_count = 0;
		flash_flag = 0;
		disp_title_count = 0;
		flash_time = 0;
		return;
	}
	
	if (!flash_flag)
	{
		flash_flag = 1;
		flash_time = Get_Cur_Time();	//记录起始时间
	}
	
	if (Cal_Time_Dif(flash_time) > 500)	//改变现实效果
	{
		flash_time = Get_Cur_Time();	//重新记录起始时间
		if (flash_count)
		{
			flash_count = 0|0x80;
		}
		else
		{
			flash_count = 1|0x80;
		}		
	}

	if (!disp_title_count)//显示标题
	{	
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
		Bar(0,190,61,207);
		Bar(0,208,14,239);
		Bar(47,208,61,239);
	
		Set_Write_Mode(WRITE_OR);	//或模式
		Set_ASC_Lib(Arial12B_Font);	//设置字库
		Set_HZ_Lib(&ST_11);
		if (br_mode == BRM_VCV)	//显示呼吸模式
		{
			x = 31-Get_Line_Width("VCV")/2;
			Put_Line_Str(x,191,"VCV");
		}
		if (disp_mode&0x80)//实时刷新
		{
			LCD_Refresh_Area(0,190,61,239);
			LCD_Refresh_Area(0,208,14,239);
			LCD_Refresh_Area(47,208,61,239);
		}
	}	
	
	if (br_meth == BR_AUTO)	//显示机动呼吸图标
	{
		if (flash_count&0x80)
		{
			flash_count &= ~0x80;
			
			if (flash_count)//动画显示
			{
				Put_Image(15,208,Vent_Icon_Press,WRITE_OVERLAY);
			}		
			else
			{
				Put_Image(15,208,Vent_Icon,WRITE_OVERLAY);
			}
			
			if (disp_mode&0x80)//即时刷新
			{
				LCD_Refresh_Area(15,208,46,239);
			}
		}		
	}
	
	disp_title_count = 1;		
}

//**********************************************************************
//函数  名:Disp_Fr_Set_Main
//函数功能:在主界面下显示呼吸频率设置
//入口参数:fr_set--呼吸频率
//			disp_mode(最高位为1表示即时刷新;次高位为1表示只初始化控制变量)
//出口参数:无
//**********************************************************************
void Disp_Fr_Set_Main(int fr_set,char disp_mode)
{
	static char flash_count;	//=0表示显示;=1表示擦除(最高位为1表示刷新显示)
	static char flashing_flag;	//=1表示闪烁显示;=0表示正常显示
	static int flash_time;
	static char title_disp_count;
	static int fr_set_disp;	//记录上次显示的数值
	unsigned short en_x,ch_x;
	
	if (disp_mode & 0x40)//初始化控制变量为下次显示作准备
	{
		flash_count = 0;
		flashing_flag = 0;
		title_disp_count = 0;
		fr_set_disp = 0;
		flash_time = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//需要进入闪烁模式
	{
		flash_time = Get_Cur_Time();		//记录时间
		flashing_flag = 1;
		flash_count = 0|0x80;				//刷新显示
	}
	
	if (fr_set_disp != fr_set)	//数值被修改了,则必须先显示
	{
		if (flashing_flag)	//正在闪烁则必须先显示
		{
			flash_time = Get_Cur_Time();	//记录时间
			flash_count = 0|0x80;	//显示
		}
	}
	
	
	if (!title_disp_count)
	{//显示标题及单位
		//Set_Refresh_Mode(0);	//默认为没有事实刷新
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
		Bar(62,190,125,207);	
		Bar(62,229,125,239);
		
		Set_Write_Mode(WRITE_EXOR);	//异或模式
		Set_ASC_Lib(Arial12B_Font);	//设置字库
		Set_HZ_Lib(&ST_11);
		ch_x = 94-Get_Line_Width("呼吸频率")/2;
		en_x = 94-Get_Line_Width("Fr")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("呼吸频率","Fr"));
			
		Set_ASC_Lib(Arial8_Font);
		en_x = 94-Get_Line_Width("/min")/2;
		Put_Line_Str(en_x,228,"/min");
		if (disp_mode&0x80)	//即时刷新
		{
			LCD_Refresh_Area(62,190,125,207);
			LCD_Refresh_Area(62,229,125,239);
		}
	}
	
	if (!flashing_flag)	//不闪烁
	{
		if ((!title_disp_count)||(fr_set_disp != fr_set))	//第一次显示或者数值被修改了
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Bar(62,208,125,228);
			
			Set_Write_Mode(WRITE_EXOR);	//异或模式		
			Set_ASC_Lib(Arial18B_Font);
			en_x = 94-Get_Num_Width(fr_set,1)/2;
			Put_Numeral(en_x,207,fr_set,1);
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(62,208,125,228);
			}
		}
	}
	else	//闪烁显示
	{
	
		if (Cal_Time_Dif(flash_time)>500)		//比较时间,时间到则改变显示方式
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();//重新记录时间
		}
			
		if (flash_count&0x80)	//刷新时间到
		{
			flash_count &= ~0x80;
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Bar(62,208,125,228);			
			if (!flash_count)	//显示数值
			{
				Set_Write_Mode(WRITE_EXOR);	//异或模式
				Set_ASC_Lib(Arial18B_Font);
				en_x = 94-Get_Num_Width(fr_set,1)/2;
				Put_Numeral(en_x,207,fr_set,1);
				//Rectangle(62,190,125,239);			
			}
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(62,208,125,228);
			}			
		}	
	}

	fr_set_disp = fr_set;	//记录显示的值	
	title_disp_count = 1;	 
}

//**********************************************************************
//函数  名:Disp_IE_Set_Main
//函数功能:在主界面下显示吸呼比设置
//入口参数:ie_set--呼吸频率
//			disp_mode(最高位为1表示即时刷新,次高位为1表示初始化显示控制参数)
//出口参数:无
//**********************************************************************
void Disp_IE_Set_Main(int ie_set,char disp_mode)
{
	static char flash_count;	//=0表示显示;=1表示擦除(最高位决定在闪烁时是否显示)
	static char flashing_flag;	//=1表示闪烁显示;=0表示正常显示
	static char title_disp_count;	//显示标题计数控制
	static int flash_time;
	static int ie_set_disp;	//记录上次显示的数值
	unsigned short en_x,ch_x;
	char ie1_fm,ie2_fm;
	
	if (disp_mode&0x40)	//初始化控制参数,为下次显示作准备,本次不显示
	{
		flash_count = 0;
		flashing_flag = 0;
		title_disp_count = 0;
		flash_time = 0;
		ie_set_disp = 0;
		return;
	}
	
	if (ie_set > IE_IND_MAX) return;	//设置值错误
	if (ie_set < IE_IND_MIN) return;	//设置值错误
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//需要进入闪烁模式
	{
		flash_time = Get_Cur_Time();		//记录时间
		flashing_flag = 1;
		flash_count = 0|0x80;
	}
	
	if (ie_set_disp != ie_set)	//数值被修改了,则必须先显示
	{
		if (flashing_flag)	//正在闪烁则必须先显示
		{
			flash_time = Get_Cur_Time();	//记录时间
			flash_count = 0|0x80;	//显示设置值
		}
	}

	if (!title_disp_count)	//第一次进入,显示标题
	{
		//Set_Refresh_Mode(0);	//默认为没有事实刷新
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
		Bar(127,190,190,207);	
		Bar(127,229,190,239);	
	
		Set_Write_Mode(WRITE_EXOR);	//异或模式
		Set_HZ_Lib(&ST_11);			//设置字库
		Set_ASC_Lib(Arial12B_Font);
		ch_x = 159-Get_Line_Width("吸呼比")/2;
		en_x = 159-Get_Line_Width("IE")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("吸呼比","IE"));
		if (disp_mode&0x80)	//即时刷新
		{
			LCD_Refresh_Area(127,190,190,207);
			LCD_Refresh_Area(127,229,190,239);
		}
	}	
	
	if (!flashing_flag)	//不闪烁
	{
		if ((!title_disp_count)||(ie_set_disp != ie_set))	//参数被修改或者第一次显示则显示参数
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Bar(127,208,190,228);	
			Set_ASC_Lib(Arial18B_Font);
			Set_Write_Mode(WRITE_EXOR);	//异或模式
			ie1_fm = (IE_tab[ie_set][0]<10)?1:10;
			ie2_fm = (IE_tab[ie_set][1]<10)?1:10;
			en_x = 159-Get_Num_Width(IE_tab[ie_set][0],ie1_fm)/2-
					Get_Num_Width(IE_tab[ie_set][1],ie2_fm)/2-
					Get_Line_Width(":")/2;
			en_x = Put_Numeral(en_x,207,IE_tab[ie_set][0],ie1_fm);
			en_x = Put_Line_Str(en_x,207,":");
			Put_Numeral(en_x,207,IE_tab[ie_set][1],ie2_fm);
			if (disp_mode&0x80)
			{
				LCD_Refresh_Area(127,208,190,228);
			}
		}	
	}
	else	//闪烁显示
	{
		if (Cal_Time_Dif(flash_time)>500)		//比较时间,时间到则改变显示方式
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();	//重新记录时间
		}
		
		if (flash_count&0x80)	//显示时间到
		{	
			flash_count &= ~0x80;	//清除显示时间到标记
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Bar(127,208,190,228);				
			if (!flash_count)	//显示数值
			{
				Set_Write_Mode(WRITE_EXOR);	//异或模式
				Set_ASC_Lib(Arial18B_Font);
				ie1_fm = (IE_tab[ie_set][0]<10)?1:10;
				ie2_fm = (IE_tab[ie_set][1]<10)?1:10;
				en_x = 159-Get_Num_Width(IE_tab[ie_set][0],ie1_fm)/2-
						Get_Num_Width(IE_tab[ie_set][1],ie2_fm)/2-
						Get_Line_Width(":")/2;
				en_x = Put_Numeral(en_x,207,IE_tab[ie_set][0],ie1_fm);
				en_x = Put_Line_Str(en_x,207,":");
				Put_Numeral(en_x,207,IE_tab[ie_set][1],ie2_fm);
				//Rectangle(127,190,190,239);			
			}
			if (disp_mode&0x80)
			{
				LCD_Refresh_Area(127,208,190,228);
			}			
		}
	}
	
	ie_set_disp = ie_set; //保存显示的数值	
	title_disp_count = 1;				 
}

//**********************************************************************
//函数  名:Disp_Pl_Set_Main
//函数功能:在主界面下显示压力限制设置
//入口参数:pl_set--压力限制
//			pl_set_units--压力限制单位(10--0.1cmH2O)
//			disp_mode--显示模式(最高位为1表示即时刷新,次高位为1表示仅仅初
///						始化显示的控制参数)
//出口参数:无
//**********************************************************************
void Disp_Pl_Set_Main(int pl_set,int pl_set_units,char disp_mode)
{
	static char flash_count;	//=0表示显示;=1表示擦除(最高位表示显示,其他位决定显示方式)
	static char flashing_flag;	//=1表示闪烁显示;=0表示正常显示
	static char disp_title_count;
	static int flash_time;
	static int pl_set_disp;	//记录上次显示的数值
	unsigned short en_x,ch_x;
	
	if (disp_mode&0x40)		//初始化显示控制参数,为下次显示作准备
	{
		flash_count = 0;
		flashing_flag = 0;
		disp_title_count = 0;
		flash_time = 0;
		pl_set_disp = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//需要进入闪烁模式
	{
		flash_time = Get_Cur_Time();		//记录时间
		flashing_flag = 1;
		flash_count = 0|0x80;
	}
	
	if (pl_set_disp != pl_set)	//数值被修改了,则必须先显示
	{		
		if (flashing_flag)	//正在闪烁则必须先显示
		{
			flash_time = Get_Cur_Time();	//记录时间
			flash_count = 0|0x80;	//显示
		}
	}
		
	if (!disp_title_count)	//标题没有被显示则显示标题
	{
		//Set_Refresh_Mode(0);	//默认为没有事实刷新
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
		Bar(192,190,255,207);	
		Bar(192,229,255,239);	
	
		Set_Write_Mode(WRITE_EXOR);	//异或模式
		Set_ASC_Lib(Arial12B_Font);	//设置字库
		Set_HZ_Lib(&ST_11);
		ch_x = 224-Get_Line_Width("压力限制")/2;
		en_x = 224-Get_Line_Width("Plimit")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("压力限制","Plimit"));
	
		Set_ASC_Lib(Arial8_Font);
		en_x = 224-Get_Line_Width("cmH2O")/2;
		Put_Line_Str(en_x,228,"cmH2O");
		if (disp_mode&0x80)	//即时刷新
		{
			LCD_Refresh_Area(192,190,255,207);
			LCD_Refresh_Area(192,229,255,239);
		}
	}	
	
	if (!flashing_flag)	//不闪烁
	{
		if ((pl_set_disp != pl_set)||(!disp_title_count))	//参数被修改过或者是第一次显示
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Set_HZ_Lib(&ST_11);	
			Bar(192,208,255,228);	
			Set_Write_Mode(WRITE_EXOR);	//异或模式
			Set_ASC_Lib(Arial18B_Font);
			en_x = 224-Get_Num_Width(pl_set,pl_set_units)/2;
			Put_Numeral(en_x,207,pl_set,pl_set_units);
			Set_Write_Mode(WRITE_OVERLAY);	//恢复为默认值	
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(192,208,255,228);
			}
		}	
	}
	else	//闪烁显示
	{
		if (Cal_Time_Dif(flash_time)>500)		//比较时间,时间到则改变显示方式
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time = Get_Cur_Time();	//重新记录时间
		}	
		if (flash_count&0x80)	//显示
		{
			flash_count &= ~0x80;	//清除显示标志
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);
			Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
			Bar(192,208,255,228);	
			if (!flash_count)
			{
				Set_Write_Mode(WRITE_EXOR);	//异或模式	
				Set_HZ_Lib(&ST_11);		//设置字库		
				Set_ASC_Lib(Arial18B_Font);
				en_x = 224-Get_Num_Width(pl_set,pl_set_units)/2;
				Put_Numeral(en_x,207,pl_set,pl_set_units);
				//Rectangle(192,190,255,239);
			}				
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(192,208,255,228);
			}		
		}
	}
	
	pl_set_disp = pl_set; //保存显示记录
	disp_title_count = 1; //标记标题显示					 
}

//**********************************************************************
//函数  名:Disp_Hold_Set_Main
//函数功能:在主界面下显示吸气暂停设置
//入口参数:hold_set--吸气暂停
//			hold_set_units--吸气暂停单位(10--0.1%)
//			disp_mode--显示模式(最高位为1表示即时刷新,次高位为1表示仅仅
//						初始化显示的静态控制参数)
//出口参数:无
//**********************************************************************
void Disp_Hold_Set_Main(int hold_set,int hold_set_units,char disp_mode)
{
	static char flash_count;	//=0表示显示;=1表示擦除(最高位为1表示显示)
	static char flashing_flag;	//=1表示闪烁显示;=0表示正常显示
	static int flash_time;		//记录闪烁的起始时间
	static int hold_set_disp;	//记录上次显示的数值
	static char disp_title_count;		//=0时刷新一次全部
	unsigned short en_x,ch_x;
	
	if (disp_mode&0x40)	//仅仅初始化控制参数
	{
		flash_count = 0;
		flashing_flag = 0;
		flash_time = 0;
		hold_set_disp = 0;
		disp_title_count = 0;
		return;
	}
	
	if ( ((disp_mode&0x0f)==DISP_FLASH)&&(!flashing_flag) )	//未进入闪烁模式则需要进入闪烁模式
	{
		flash_time = Get_Cur_Time();		//记录时间
		flashing_flag = 1;
		flash_count = 0|0x80;				//显示设置值区域
	}
	else if ((disp_mode&0x0f)==DISP_NORMAL)
	{
		flashing_flag = 0;
		flash_count = 0;
	}
	
	if (hold_set_disp != hold_set)	//数值被修改了,则必须先显示
	{
		if (flashing_flag)	//正在闪烁则必须先显示
		{
			flash_time =  Get_Cur_Time();	//记录时间
			flash_count = 0|0x80;	//显示
		}
	} 
	
	if (!disp_title_count)
	{//显示标题及单位		
		//Set_Refresh_Mode(0);	//默认为没有实时刷新
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Chr_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
		Bar(257,190,319,207);	
		Bar(257,229,319,239);
		Set_Write_Mode(WRITE_EXOR);	//异或模式
		Set_ASC_Lib(Arial12B_Font);	//设置字库
		Set_HZ_Lib(&ST_11);
		ch_x = 288-Get_Line_Width("吸气暂停")/2;
		en_x = 288-Get_Line_Width("Hold")/2;
		Put_Line_Str(Language_Pos(ch_x,en_x),191,Language_Str("吸气暂停","Hold"));
		Set_ASC_Lib(Arial8_Font);
		en_x = 288-Get_Line_Width("%")/2;
		Put_Line_Str(en_x,228,"%");
		Set_Write_Mode(WRITE_OVERLAY);
		if (disp_mode&0x80)	//即时刷新
		{
			LCD_Refresh_Area(257,190,319,207);
			LCD_Refresh_Area(257,229,319,239);
		}
	}
		
	if (!flashing_flag)	//不闪烁
	{
		if ((hold_set_disp != hold_set)||(!disp_title_count))	//参数被修改了则显示
		{
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);		
			Set_Write_Mode(WRITE_OVERLAY);
			Bar(257,208,319,228);		
			Set_ASC_Lib(Arial18B_Font);
			Set_HZ_Lib(&ST_11);
			Set_Write_Mode(WRITE_EXOR);	//异或模式		
			if (hold_set>100)
			{
				en_x = 288-Get_Line_Width("---")/2;
				Put_Line_Str(en_x,207,"---");
			}
			else if (hold_set == 0)
			{
				en_x = 288-Get_Line_Width("关闭")/2;
				Put_Line_Str(en_x,207,"关闭");				
			}
			else
			{
				en_x = 288-Get_Num_Width(hold_set,hold_set_units)/2;
				Put_Numeral(en_x,207,hold_set,hold_set_units);
			}
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(257,208,319,228);
			}
			Set_Write_Mode(WRITE_OVERLAY);			
		}	
			
	}
	else	//闪烁显示
	{
		if (Cal_Time_Dif(flash_time) > 500)		//比较时间,时间到则改变显示方式
		{
			flash_count++;
			flash_count %= 2;
			flash_count |= 0x80;
			flash_time =  Get_Cur_Time();	//重新记录时间
		}
		
		if (flash_count&0x80)	//显示
		{//显示设置值
			flash_count &= ~0x80;	//清除显示标志
			Set_Graphic_Color(COL_BLACK,COL_WHITE);
			Set_Chr_Color(COL_BLACK,COL_WHITE);		
			Set_Write_Mode(WRITE_OVERLAY);
			Bar(257,208,319,228);
			if (!flash_count)	//显示数值
			{
				Set_Write_Mode(WRITE_EXOR);	//异或模式			
				Set_ASC_Lib(Arial18B_Font);						
				if (hold_set>100)
				{
					en_x = 288-Get_Line_Width("---")/2;
					Put_Line_Str(en_x,207,"---");
				}
				else if (hold_set == 0)
				{
					en_x = 288-Get_Line_Width("关闭")/2;
					Put_Line_Str(en_x,207,"关闭");				
				}				
				else
				{		
					en_x = 288-Get_Num_Width(hold_set,hold_set_units)/2;
					Put_Numeral(en_x,207,hold_set,hold_set_units);
				}
				//Rectangle(257,190,319,239);			
			}
			if (disp_mode&0x80)	//即时刷新
			{
				LCD_Refresh_Area(257,208,319,228);
			}			
		 }
	}
	hold_set_disp = hold_set;	//保存显示值
	disp_title_count = 0x01;	//标记标题被显示过了 		 
}

//**********************************************************************
//函数  名:Set_Paw_Scale
//函数功能:设置压力波形的压力轴
//入口参数:p_limit--压力限制值(单位:1cmH2O)
//出口参数:无
//**********************************************************************
void Set_Paw_Scale(int p_limit)
{
	if (p_limit <= 20) 
	{
		wave_press_scale = 20;
		return;
	}
	if (p_limit <= 40)
	{
		wave_press_scale = 40;
		return;
	}
	if (p_limit <= 60)
	{
		wave_press_scale = 60;
		return;
	}
	
	wave_press_scale = 80;		
}

//**********************************************************************
//函数  名:Set_Time_Scale
//函数功能:设置压力波形的时间轴
//入口参数:rate--呼吸频率
//出口参数:无
//**********************************************************************
void Set_Time_Scale(int rate)
{
	if (rate > 30)
	{
		wave_time_scale = 5;
		return;
	}
	if (rate > 20)
	{
		wave_time_scale = 10;
		return;
	}
	wave_time_scale = 15;
}

//**********************************************************************
//函数  名:Put_Wave_Time_Axis
//函数功能:显示压力波形的时间轴
//入口参数:disp_mode--刷新方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Put_Wave_Time_Axis(char disp_mode)
{
	unsigned short di;

	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	if (disp_mode&0x80)//即时刷新
	{
		Set_Refresh_Mode(1);
	}
	Hor_Line(23,180,106);
	for (di=0; di<6; di++)
	{
		Ver_Line(107,108,25+di*31);
	}
	Set_Refresh_Mode(0);	//关闭即时刷新	
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Bar(162,48,181,57);
	Set_ASC_Lib(Arial8_Font);
	di = Put_Numeral(162,48,wave_time_scale,1);
	Put_Line_Str(di,48,"s");
	if (disp_mode&0x80)//即时刷新
	{
		LCD_Refresh_Area(162,48,181,57);
	}
}

//**********************************************************************
//函数  名:Put_Wave_P_Axis
//函数功能:显示压力波形的压力轴
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Put_Wave_P_Axis(char disp_mode)
{
	unsigned short di;
		
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OR);//或模式
	if (disp_mode&0x80)//实时刷新
	{
		Set_Refresh_Mode(1);
	}
	Ver_Line(63,106,23);
	for (di=0; di<5; di++)
	{
		Hor_Line(21,22,63+di*10);
		Put_Numeral(6,100-di*10,wave_press_scale*di/4,1);
	}
	Set_ASC_Lib(Arial8_Font);
	Put_Line_Str(5,46,"cmH2O");
	Set_Refresh_Mode(0);	
}

//**********************************************************************
//函数  名:Put_Wave_Axis
//函数功能:显示压力波的时间轴和压力轴
//入口参数:disp_mode--显示方式(最高位为1表示即时刷新)
//出口参数:无
//**********************************************************************
void Put_Wave_Axis(char disp_mode)
{
	Put_Wave_Time_Axis(disp_mode);
	Put_Wave_P_Axis(disp_mode);
	Set_HZ_Lib(&ST_11);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	if (disp_mode&0x80)
	{
		Set_Refresh_Mode(1);
	}
	Put_Line_Str(76,46,"压力波");
	Set_Refresh_Mode(0);
}

//**********************************************************************
//函数  名:Ini_Wave
//函数功能:初始化压力波
//入口参数:无
//出口参数:无
//**********************************************************************
void Ini_Wave(void)
{
	unsigned short di;
	
	for (di=0; di<sizeof(disp_press_wave); di++)
	{//清除压力波形数据
		disp_press_wave[di] = 0x00;
	}
	//清除显示的波形
	Set_Refresh_Mode(1);	//实时刷新
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Bar(26,63,180,103);
	Set_Refresh_Mode(0);	//恢复默认刷新模式
	wave_draw_pos = 0;
	wave_brush_pos = 1;
	wave_first_disp_flag = TRUE;
}

//**********************************************************************
//函数  名:Disp_Wave
//函数功能:显示压力波形
//入口参数:pressure--压力值
//			pressure_units--压力单位(10--0.1cmH2O)
//出口参数:无
//**********************************************************************
void Disp_Wave(int pressure,int pressure_units)
{
	static int disp_time;	//记录前次的显示时间
	static int time_dif;	//累计未刷新时间
	int y_press;			

	time_dif += Cal_Time_Dif(disp_time)*WAVE_TIME_C;
	disp_time = Get_Cur_Time();
	if ( (time_dif >= wave_time_scale*1000)||(wave_first_disp_flag) )
	{//显示时间到或者第一次显示
		if (wave_first_disp_flag)
		{
			wave_first_disp_flag = FALSE;
			disp_time = Get_Cur_Time();
			time_dif = 0;
		}
		else
		{
			time_dif -= wave_time_scale*1000;	//扣除累计未刷新的时间	
		}
		//计算压力轴长度
		y_press = (pressure*WAVE_PRESS_C)/(wave_press_scale*pressure_units);
		if (y_press > WAVE_PRESS_C-1)
		{
			y_press = WAVE_PRESS_C-1;
		}
		else if (y_press < 0)
		{
			y_press = 0;
		}
		//记录波形
		disp_press_wave[wave_draw_pos] = y_press;
		disp_press_wave[wave_brush_pos] = 0x00;
		//显示波形
		Set_Refresh_Mode(1);		//实时刷新
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
		Ver_Line(WAVE_PRESS_ORI-y_press,WAVE_PRESS_ORI,WAVE_TIME_ORI+wave_draw_pos);
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
		Ver_Line(WAVE_PRESS_ORI-WAVE_PRESS_C+1,WAVE_PRESS_ORI,WAVE_TIME_ORI+wave_brush_pos);
		Set_Refresh_Mode(0);		//恢复刷新默认值
		
		if (++wave_draw_pos > WAVE_TIME_C-1) wave_draw_pos = 0;
		if (++wave_brush_pos > WAVE_TIME_C-1) wave_brush_pos = 0;
	}
}
