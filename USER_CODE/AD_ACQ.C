//**********************************************************************
//函数  名:Demar_Put_Ad_Req_Frame
//函数功能:在标定模式下显示AD查询界面的框架
//入口参数:无
//出口参数:无
//**********************************************************************
static void Demar_Put_Ad_Req_Frame(void)
{
	short di;
	
	Set_Graphic_Color(COL_BLACK,COL_WHITE);
	Set_Chr_Color(COL_BLACK,COL_WHITE);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式
	Clr_Screen(DISP_NOT_RT);	//用背景色清屏
	Rectangle(5,5,314,234);	//主外框
								//查询表绘制
	Rectangle(8,40,312,201);
	for (di=0; di<6; di++)
	{
		Hor_Line(9,311,63+di*23);
	}
	Ver_Line(41,200,83);
	Ver_Line(41,200,122);		
	Ver_Line(41,200,202);	
	
	Hor_Line(6,313,31);		//标题
	Pattern_Bar(6,6,313,30,0xaa);
	Bar(37,9,254,27);
	Set_Write_Mode(WRITE_EXOR);//异或模式
	Set_HZ_Lib(&ST_11);		//设置字库
	Set_ASC_Lib(ArialN10B_Font);
	Put_Line_Str(126,11,"AD Require");
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Line_Str(15,46,"Channel");				
	Put_Line_Str(94,46,"AD");	
	Put_Line_Str(134,46,"Filt Len");
	Put_Line_Str(220,46,"Filt Band");			
	Put_Line_Str(22,68,"Paw");		
	Put_Line_Str(22,92,"O2");		
	Put_Line_Str(22,114,"Batt_V");	
	Put_Line_Str(22,138,"Batt_I");	
	Put_Line_Str(22,159,"Vref");
	Put_Line_Str(22,184,"Vt");
	
	Put_Paw_AD(DISP_REAL);		
	Put_O2_AD(DISP_REAL);
	Put_Batt_V_AD(DISP_REAL);		
	Put_Batt_I_AD(DISP_REAL);
	Put_Vref_AD(DISP_REAL);
	Put_Vt_Loops(DISP_REAL);
	
	Put_Paw_Filt_Len(DISP_REAL);		
	Put_O2_Filt_Len(DISP_REAL);
	Put_Batt_V_Filt_Len(DISP_REAL);		
	Put_Batt_I_Filt_Len(DISP_REAL);
	Put_Vref_Filt_Len(DISP_REAL);
	Put_Vt_Filt_Len(DISP_REAL);		
	
	Put_Paw_Filt_Band(DISP_REAL|DISP_INVERSE);		
	Put_O2_Filt_Band(DISP_REAL);
	Put_Batt_V_Filt_Band(DISP_REAL);		
	Put_Batt_I_Filt_Band(DISP_REAL);
	Put_Vref_Filt_Band(DISP_REAL);
	Put_Vt_Filt_Band(DISP_REAL);
								
	LCD_Refresh_Area(0,0,319,239);	//全屏刷新	
}

//**********************************************************************
//函数名:Put_Paw_AD
//函数功能:显示PAW的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Paw_AD(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,64,121,85);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,68,paw_ad,1);
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(84,64,121,85);
	}	
}

//**********************************************************************
//函数名:Put_Paw_Filt_Len
//函数功能:显示PAW的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Paw_Filt_Len(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,64,201,85);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(135,68,PAW_AD_FILT_LEN,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,64,201,85);
	}
}

//**********************************************************************
//函数名:Put_Paw_Filt_Band
//函数功能:显示PAW的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Paw_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,64,311,85);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,65,310,84);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (ddisp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,68,paw_ad_filt_band,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,64,311,85);
	}
}

//**********************************************************************
//函数名:Put_O2_AD
//函数功能:显示O2的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_O2_AD(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,64,121,85);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,92,o2_ad,1);	
	if (disp_mode&DISP_REAL)
	{		
		LCD_Refresh_Area(84,87,121,108);
	}
}

//**********************************************************************
//函数名:Put_O2_Filt_Len
//函数功能:显示O2的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_O2_Filt_Len(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,87,201,108);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(135,92,O2_AD_FILT_LEN,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,87,201,108);
	}
}

//**********************************************************************
//函数名:Put_O2_Filt_Band
//函数功能:显示O2的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_O2_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,87,311,108);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,88,310,107);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (ddisp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,92,VALID_O2_AD_SCALE,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,87,311,108);
	}
}

//**********************************************************************
//函数名:Put_Batt_V_AD
//函数功能:显示电池电压的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_V_AD(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,110,121,131);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,114,batt_v_ad,1);	
	if (disp_mode&DISP_REAL)
	{		
		LCD_Refresh_Area(84,110,121,131);
	}
}

//**********************************************************************
//函数名:Put_Batt_V_Filt_Len
//函数功能:显示电池电压的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_V_Filt_Len(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,100,201,131);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(135,114,BATT_V_AD_FILT_LEN,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,100,201,131);
	}
}

//**********************************************************************
//函数名:Put_Batt_V_Filt_Band
//函数功能:显示电池电压的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_V_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,110,311,131);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,111,310,130);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (ddisp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,92,BATT_V_AD_FILT_BAND,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,110,311,131);
	}
}

//**********************************************************************
//函数名:Put_Batt_I_AD
//函数功能:显示电池电流的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_I_AD(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,133,121,154);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,138,batt_i_ad,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(84,133,121,154);
	}
}

//**********************************************************************
//函数名:Put_Batt_I_Filt_Len
//函数功能:显示电池电流的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_I_Filt_Len(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,133,201,154);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(135,138,BATT_I_AD_FILT_LEN,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,133,201,154);
	}
}

//**********************************************************************
//函数名:Put_Batt_I_Filt_Band
//函数功能:显示电池电流的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Batt_I_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,133,311,154);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,134,310,153);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (disp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,138,BATT_I_AD_FILT_BAND,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,133,311,154);
	}
}

//**********************************************************************
//函数名:Put_Vref_AD
//函数功能:显示参考电压的AD值
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vref_AD(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,156,121,177);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,159,vref_ad,1);	
	if (disp_mode&DISP_REAL)
	{		
		LCD_Refresh_Area(84,156,121,177);
	}
}

//**********************************************************************
//函数名:Put_Vref_Filt_Len
//函数功能:显示参考电压的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vref_Filt_Len(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,156,201,177);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(135,159,VREF_AD_FILT_LEN,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,156,201,177);
	}
}

//**********************************************************************
//函数名:Put_Vref_Filt_Band
//函数功能:显示参考电压的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vref_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,156,311,177);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,157,310,176);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (disp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,159,VREF_AD_FILT_BAND,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,156,311,177);
	}
}

//**********************************************************************
//函数名:Put_Vt_Loops
//函数功能:显示潮气量的圈数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vt_Loops(char disp_mode)
{
	Set_Graphic_Color(COL_WHITE,COL_BLACK);
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(84,179,121,200);
	Set_ASC_Lib(ArialN10B_Font);	
	Set_Write_Mode(WRITE_OR);//或模式
	Put_Numeral(88,184,Calc_Filt_Fan_Lap(),1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(84,179,121,200);
	}
}

//**********************************************************************
//函数名:Put_Vt_Filt_Len
//函数功能:显示潮气量的滤波次数
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vt_Filt_Len(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(123,179,201,200);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(124,180,200,199);
	}	
	Set_ASC_Lib(ArialN10B_Font);	
	if (disp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(135,184,vte_filter_const,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(123,179,201,200);
	}
}

//**********************************************************************
//函数名:Put_Vt_Filt_Band
//函数功能:显示参潮气量的滤波带宽
//入口参数:disp_mode--显示方式
//出口参数:无
//**********************************************************************
void Put_Vt_Filt_Band(char disp_mode)
{
	if (disp_mode&DISP_INVERSE)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
	}else
	{
		Set_Graphic_Color(COL_WHITE,COL_BLACK);
	}
	Set_Write_Mode(WRITE_OVERLAY);//覆盖模式	
	Bar(203,179,311,200);
	if (disp_mode&DISP_INPUT)
	{
		Set_Graphic_Color(COL_BLACK,COL_WHITE);
		Rectangle(204,180,310,199);
	}
	Set_ASC_Lib(ArialN10B_Font);
	if (disp_mode&DISP_INVERSE)
	{
		Set_Write_Mode(WRITE_EXOR);//异或模式
	}else
	{	
		Set_Write_Mode(WRITE_OR);//或模式
	}
	Put_Numeral(220,184,5,1);	
	if (disp_mode&DISP_REAL)
	{	
		LCD_Refresh_Area(203,179,311,200);
	}
}

//**********************************************************************
//函数  名:Demar_Ad_Req_Pro
//函数功能:查询所以通道AD值
//入口参数:menu_option--菜单号(高8位)及默认选项(低8位)
//出口参数:所切换到的界面号(高8位)及进入后的焦点选项号(低八位)
//**********************************************************************
static unsigned short Demar_Ad_Req_Pro(unsigned short menu_option)
{
	unsigned char key_code;
	unsigned short key_pressed;
	unsigned int di,dj;
	char op_st = 0;
	
	Demar_Put_Ad_Req_Frame();
	Get_Key_Code();	//清除原来的键码
	phase_vent_ctrl = PHASE_START_VENT;//启动机动呼吸
	while (1)
	{
		if (flag_10ms_ok)	//10ms
		{
			flag_10ms_ok =  FALSE;
			key_code = Get_Key_Code();
			key_pressed = Get_Key_Status();
			switch (key_code)
			{
				case KEY_INC|KEY_PRESSED://"增长"键
					if (op_st == 1)	//正在输入参数
					{
						if (menu_option==MENU_AD_REQ_PAW_BAND)	//修改PAW滤波带宽
						{
							paw_ad_filt_band++;
							Put_Paw_Filt_Band(DISP_REAL|DISP_INPUT);
						}
						else if (menu_option==MENU_AD_REQ_VT_LEN)
						{
							vte_filter_const++;
							Put_Vt_Filt_Len(DISP_REAL|DISP_INPUT);
						}
					}
					else
					{
						if (menu_option==MENU_AD_REQ_PAW_BAND)	//修改PAW滤波带宽
						{
							menu_option = MENU_AD_REQ_VT_LEN;
							Put_Paw_Filt_Band(DISP_REAL);
							Put_Vt_Filt_Len(DISP_REAL|DISP_INVERSE);
						}
						else if (menu_option==MENU_AD_REQ_VT_LEN)
						{
							menu_option = MENU_AD_REQ_PAW_BAND;
							Put_Paw_Filt_Band(DISP_REAL|DISP_INVERSE);
							Put_Vt_Filt_Len(DISP_REAL);							
						}						
					}
					Key_Valid_Sound();	//发按键有效音
					break;
				case KEY_DEC|KEY_PRESSED://"减小"键
					if (op_st == 1)	//正在输入参数
					{
						if (menu_option==MENU_AD_REQ_PAW_BAND)	//修改PAW滤波带宽
						{
							paw_ad_filt_band--;
							Put_Paw_Filt_Band(DISP_REAL|DISP_INPUT);
						}
						else if (menu_option==MENU_AD_REQ_VT_LEN)
						{
							vte_filter_const--;
							Put_Vt_Filt_Len(DISP_REAL|DISP_INPUT);
						}
					}
					else
					{
						if (menu_option==MENU_AD_REQ_PAW_BAND)	//修改PAW滤波带宽
						{
							menu_option = MENU_AD_REQ_VT_LEN;
							Put_Paw_Filt_Band(DISP_REAL);
							Put_Vt_Filt_Len(DISP_REAL|DISP_INVERSE);							
						}
						else if (menu_option==MENU_AD_REQ_VT_LEN)
						{
							menu_option = MENU_AD_REQ_PAW_BAND;
							Put_Paw_Filt_Band(DISP_REAL|DISP_INVERSE);
							Put_Vt_Filt_Len(DISP_REAL);								
						}						
					}
					Key_Valid_Sound();	//发按键有效音
					break;					
				case KEY_OK|KEY_PRESSED:	//"确认"键
					Key_Valid_Sound();	//发按键有效音				
					op_st ^= 0x01;		//修改选项状态
					if (menu_option==MENU_AD_REQ_PAW_BAND)	//修改PAW滤波带宽
					{
						if (op_st)
						{
							Put_Paw_Filt_Band(DISP_REAL|DISP_INPUT);	
						}else
						{
							Put_Paw_Filt_Band(DISP_REAL|DISP_INVERSE);				
						}
					}
					else if (menu_option==MENU_AD_REQ_VT_LEN)
					{
						if (op_st)
						{
							Put_Vt_Filt_Len(DISP_REAL|DISP_INPUT);	
						}else
						{
							Put_Vt_Filt_Len(DISP_REAL|DISP_INVERSE);				
						}						
					}						
					break;	
				case KEY_MENU|KEY_PRESSED_2S://"菜单键"键
					Key_Valid_Sound();	//发按键有效音
					return (DEMAR_MAIN_MENU_VT);
					break;				
				default:
					if (key_code&KEY_PRESSED_5S)//有键按下,但是无效
					{
						Key_No_Valid_Sound();	//发按键无效音
					}
					break;
			}
		}
			
		if (flag_100ms_ok)
		{
			flag_100ms_ok = FALSE;	
			Put_Paw_AD(DISP_REAL);		
			Put_O2_AD(DISP_REAL);
			Put_Batt_V_AD(DISP_REAL);		
			Put_Batt_I_AD(DISP_REAL);
			Put_Vref_AD(DISP_REAL);
			Put_Vt_Loops(DISP_REAL);						
		}
}