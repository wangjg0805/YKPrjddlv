
//**********************************************************************
//文件说明              
//功能
//（1）更多模式 菜单处理
//（2）亮度 菜单处理
//**********************************************************************
#include "FontLib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "type_def.h"
#include "disp.h"
#include "i2c_com.h"
#include "eeprom.h"

static uint32 language_var_tmp;
////////////////////////////////////////////////////////
static uint32 num_buf[6],num_buf_full;
static float  Num_Input_tmp;
static uint32 Num_Input_COUNT;//输入的第几位
////////////////////////////////////////////////////////
static uint32 PCSInput_tmp;
////////////////////////////////////////////////////////
void   Get_NumInput_Tmp(uint32 data);
void   Get_PCSInput_Tmp(uint32 data);

void ModeSel_Disp_Pro(void)
{
      
 }
/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
uint32  Action_ModeSelMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 80/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   return(Key_Code);
	}
//**********************************************************************
//函数  名:Put_Oper_Interface
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void ModeSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    static uint32 xy[6][2];
    
    //Create window
    Create_Window_Frame(0,75,400,480-180,3,COL_DEEPGREEN);
   
    Set_Graphic_Color(COL_THINGREEN,COL_DEEPGREEN);
    Bar( 30,130,  30+100,130+70);
    Bar(150,130, 150+100,130+70);
    Bar(270,130, 270+100,130+70);
    
    Bar( 30,260,  30+100,260+70);
    Bar(150,260, 150+100,260+70);
    Bar(270,260, 270+100,260+70);
    
    Set_Chr_Color(COL_WHITE,COL_DEEPGREEN);
    Set_ASC_Lib(Arial14B_Font);
    Put_Line_Str( 50, 210,Language_Str("称重",  "WEIGH"));
    Put_Line_Str(170, 210,Language_Str("计数",  " PCS"));
    Put_Line_Str(280, 210,Language_Str("百分比"," 100%"));
    Put_Line_Str( 45, 340,Language_Str("密度",  "DENSITY"));
    Put_Line_Str(160, 340,Language_Str(" ",     " NULL"));
    Put_Line_Str(290, 340,Language_Str("退出",  "EXIT"));
    
    //create button list
    xy[0][0] =   30+50;    xy[0][1] = 130+70/2;
    xy[1][0] =  150+50;    xy[1][1] = 130+70/2;
    xy[2][0] =  270+50;    xy[2][1] = 130+70/2;
    xy[3][0] =   30+50;    xy[3][1] = 260+70/2;
    xy[4][0] =  150+50;    xy[4][1] = 260+70/2;
    xy[5][0] =  270+50;    xy[5][1] = 260+70/2;
                  
    Create_ButtonList(xy,6);               
}
//**********************************************************************
//函数  名:Menu_Oper_Op_Pro
//函数功能:日期配置界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
sint32 ModeSel_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
     if(Key_Status != KEY_DO_START)
    return(op_id);
    i = Action_ModeSelMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //选择称重模式
  	           current_mode = STAT_WEIGH;
  	           break;
    	case 2://选择计数模式
    	       current_mode = STAT_PCS;
    	       break;
       	case 3://
       	       current_mode = STAT_100;
       	       break;
   		case 4://选择密度模式
   		       current_mode = STAT_DENSITY;
   		       break;
   		case 5://NULL
   		       break;
   		//case 6://EXIT
   		//      op_id =  MENU_FUN_SEL_OPER; //回到主菜单界面
   		//      break;
   	  	default:break;
   	   }
   	    Key_Status = KEY_RELEASE;
    }
return(op_id);
}
////////22222
/*///////////////////////////////////////////////////
/*显示选中符号
//////////////////////////////////////////////////*/
void Display_SelLang(void)
{
   Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   switch(language_var_tmp)
   {case CHINESE:
                 Circle(180,150,9);
                 Circle(180,150,8);
                 Circle(180,150,7);
                 Fill_Circle(180,150,4);
                 Clr_Bar(180-10,230-10,20,20,COL_DEEPGRAY);
                 break;
    case ENGLISH:
                 Circle(180,230,9);
                 Circle(180,230,8);
                 Circle(180,230,7);
                 Fill_Circle(180,230,4);
                 Clr_Bar(180-10,150-10,20,20,COL_DEEPGRAY);
                 break;
    default:break;
    }
 }  
/*///////////////////////////////////////////////////
/*键盘响应函数
//////////////////////////////////////////////////*/
uint32  Action_LangMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   return(Key_Code);
	}
//**********************************************************************
//函数  名:Put_Oper_Interface
//函数功能:语言选择初始化 函数
//入口参数:op--选项
//出口参数:无
//**********************************************************************	
void LangSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    static uint32 xy[4][2];
    
//Create window
    Create_Window_Frame(0,75,200,480-180,3,COL_DEEPGRAY);
   
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
    Bar(10,130,10+150,130+50);    //chinese
    Bar(10,210,10+150,210+50);    //english
    
    Bar(10,310, 10+80,310+50);
    Bar(110,310,110+80,310+50);
    Set_Chr_Color(COL_THINGRAY,COL_DEEPBLUE);
    Set_ASC_Lib(Arial14B_Font);
    Put_Line_Str(10+50,  130+15,Language_Str("中文",   "中文"));
    Put_Line_Str(10+40,  210+15,Language_Str("ENGLISH","ENGLISH"));
    Put_Line_Str(10+10,  310+15,Language_Str("取消",   "CANCEL"));
    Put_Line_Str(110+10, 310+15,Language_Str("确定",   "  OK"));
    //create button list
    xy[0][0] = 10+75;    xy[0][1] = 130+50/2;
    xy[1][0] = 10+75;    xy[1][1] = 210+50/2;
    xy[2][0] = 10+80/2;  xy[2][1] = 310+50/2;
    xy[3][0] = 110+80/2; xy[3][1] = 310+50/2;
                  
    Create_ButtonList(xy,4); 
    ///////////////////////////////
    Display_SelLang();   
}
//**********************************************************************
//函数  名:ASDSel_Key_Pro
//函数功能:ASD 菜单下的按键处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
sint32 LangSel_Key_Pro(uint32 key_c,sint32 op)
{
    uint8  eeprom_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
     if(Key_Status != KEY_DO_START)
    return(op_id);
    i = Action_LangMenuCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
       switch(i)
       {
  	    case 1://CHINESE
  	           language_var_tmp = CHINESE ; //Set_Language_Type(CHINESE);
  	           break;
    	case 2://ENGLISH
    	       language_var_tmp = ENGLISH;  //Set_Language_Type(ENGLISH);
    	       break;
       	case 3://EXIT
       	       op_id =  MENU_OP_OPER_NON;  // return
       	       break;
       	case 4://OK
       	       Set_Language_Type(language_var_tmp);
       	       eeprom_tx_buf[0] = (uint8)(language_var_tmp);
  	           eeprom_tx_buf[1] = 0;
  	           eeprom_tx_buf[2] = 0;
  	           eeprom_tx_buf[3] = eeprom_tx_buf[0] + 0x55; 
  	           I2C_Send_Str(SYS24C08,EEP_LANGUAGE_ADDR,eeprom_tx_buf, 4);
  	           ////////////////////////////////
       	       op_id =  MENU_OP_OPER_NON;
       	       break;       
   		default:break;
   	   }
   	    Key_Status = KEY_RELEASE;
    }
return(op_id);
}
//**********************************************************************
//函数  名:Oper_Disp_Pro               
//函数功能:日期配置下的显示处理
//入口参数:无
//出口参数:无
//**********************************************************************
void LangSel_Disp_Pro(void)
{
   Display_SelLang();
  }

////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/*输入单件重量
//////////////////////////////////////////////////*/
void NumberInput_Disp_Pro(void)
{
   //Clr_Bar(300,120,200,32,COL_THINGRAY);
   Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   Set_ASC_Lib(Arial20B_Font);
   
   Put_Data_Float(360,   125, "%7.4f",Num_Input_tmp);     
 }
/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
#define  NUMBER_WIDTH       80
#define  NUMBER_HEIGTH      60
#define  NUM_WEI_INPUT      01
#define  NUM_PCS_INPUT      02

uint32  Action_NumInputMenuCfg(uint16 x,uint16 y,uint32 from_window)
	{
       uint32 i,Key_Code,a,b;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	  
	    for(i=0;i<15;i++)
	    {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	    }
	  
	   //////////////////////////////////////////////////////////有按键按下
	   if((i<15)&&(Key_Status == KEY_PRESS))
		{                              //按住反显示
 	      //增加反白显示 直到按键松开
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 Set_Chr_Color(COL_BLACK,COL_THINGRAY);
		 Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_THINGRAY);
	
		 Key_Code = 0xaa55;
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-5,0x30+i);   
		     break;
            case 10:  //退格
			
		     Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("退格","BACK"));   
		     break;
		    case 11:  //.
			
		      Put_Char(290, 330,'.');   
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("退格","BACK"));   
		     break;
		    case 13:  //返回
			 Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("返回","ESC"));   
		     break;
		    case 14:  //确认
			 Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("确认","OK"));   
		     break;
		    default:break;
		   }
		 }
		///////////////////////////////////////////
		 else if((i<15)&&(Key_Status == KEY_DO_START))
		{                             
 	      //增加反白显示 直到按键松开 
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 Set_Chr_Color(COL_WHITE,COL_BLACK);
		 Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,3,COL_BLACK); 
		 
		  switch(i)
		   {
		    case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-5,0x30+i);   
		     break;
            case 10:  //退格
			
		     Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("退格","BACK"));   
		     break;
		    case 11:  //.
			      if(NUM_WEI_INPUT == from_window)
		             Put_Char(290, 330,'.'); 
		               
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("退格","BACK"));   
		     break;
		    case 13:  //返回
			 Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("返回","ESC"));   
		     break;
		    case 14:  //确认
			 Put_Str(Key[i].X_Position-15,Key[i].Y_Position-5,Language_Str("确认","OK"));   
		     break;
		    default:break;
		   }
		} 
	   return(Key_Code);
	}
//**********************************************************************
//函数  名:Put_Oper_Interface
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void NumberInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    static uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150,80,500,300,3,COL_DEEPGRAY);
   
    Clr_Bar(300,120,200,32,COL_THINGRAY);    //窗口显示区
    
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,160,160+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,230,160+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,300,160+100*i+80,300+60,3,COL_BLACK);
    
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Set_ASC_Lib(Arial20B_Font);
    for(i=0;i<5;i++)
    Put_Data( 190+100*i, 175,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 190+100*i, 245,"%d",5+i);
    
    Put_Line_Str(175, 315,Language_Str("退格", "BACK"));
    Put_Char(290, 310,'.');
    //Put_Line_Str(380, 270,Language_Str("空", "NULL"));
    Put_Line_Str(480, 320,Language_Str("返回", "RETURN"));
    Put_Line_Str(580, 320,Language_Str("确认", "OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   160 + 100*i + 80/2;   
         xy[i+5*j][1] =   160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    for(i=0;i<6;i++)
     num_buf[i]  = 0;
     Num_Input_tmp   = 0.0;
     Num_Input_COUNT = 0;  
               
    Create_ButtonList(xy,15);               
}
//**********************************************************************
//函数  名:Menu_Oper_Op_Pro
//函数功能:日期配置界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
sint32 NumInput_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    i = Action_NumInputMenuCfg((key_c>>16)&0xffff,key_c&0xffff,NUM_WEI_INPUT); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
    	case 2:
       	case 3:
   		case 4:
   		case 5:
   	    case 6:
    	case 7:
       	case 8:
   		case 9:
   		case 10:
   		case 11:  //back    
   		case 12:  //.
   		      Get_NumInput_Tmp(i-1);
   		      break;
   		case 13:   //null
   	          break;
   		case 14:   //return
   		      op_id = MENU_OP_OPER;   //返回主菜单
   		      break;
   		case 15:   //ok     
   		       if((Num_Input_tmp<100.0)&&(Num_Input_tmp > 0.0005))  //单件重量的范围
   		         weight_per_pcs = Num_Input_tmp;
   		       op_id =  MENU_OP_OPER; //返回主菜单
   		      break;
   	  	default:break;
   	   }
   	  Key_Status = KEY_RELEASE;
    }
return(op_id);
}
///////////////////////////////////////////////////
//
///////////////////////////////////////////////////
//小数点处理
void   dot_key_process(void)
   {
    uint32 i;
    if(Num_Input_COUNT == 0)
      {
       for(i=0;i<6;i++)
       num_buf[i] = 0;
       Num_Input_COUNT = 2;
      }
    if(Num_Input_COUNT == 1)
       Num_Input_COUNT = 2;
        
   }
///////////////////////////////////////////   
//退格键处理
void   back_key_process(void)
	{
     if(TRUE == num_buf_full) //末尾
       {
        num_buf[5] = 0;
        num_buf_full = FALSE;
       }
     else if(1 == Num_Input_COUNT)
       {
        num_buf[1] = 0; 
        Num_Input_COUNT = 0;
       }
     else if(Num_Input_COUNT > 0)
      {
       num_buf[Num_Input_COUNT-1] = 0;
       Num_Input_COUNT--;
  	  }
  	}	
////////////////////////////////////////////////
void  Get_NumInput_Tmp(uint32 i)
{
 uint32 tmp_var;
 if(11 == i)
    dot_key_process();
    else if(10 == i)
    back_key_process();
    else
    {   //1-10 数字键
      if(1 == Num_Input_COUNT)
       {
        num_buf[0] = num_buf[1];
        num_buf[1] = i;
       }
       else
        num_buf[Num_Input_COUNT] = i;
       
       Num_Input_COUNT++;
     ///////////////////////////整数修正 
     if(1==Num_Input_COUNT)
      {num_buf[1] = num_buf[0];
       num_buf[0] = 0;
      }
     ////////////////////////////////// 
     if(6 == Num_Input_COUNT)
       {
        num_buf_full = TRUE;
        Num_Input_COUNT = 5;
       }
      else
        num_buf_full = FALSE;
    }
    
  ////////////////////////////////////update tmp
  Num_Input_tmp = num_buf[0]*10+num_buf[1]+num_buf[2]*0.1+num_buf[3]*0.01+num_buf[4]*0.001+num_buf[5]*0.0001;
}
/////////////////////////////////////////4444444444444444444444444444444
////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/*输入样品数
//////////////////////////////////////////////////*/
void PCSInput_Disp_Pro(void)
{
   uint32 i,j,k;
   
   //Clr_Bar(300,120,200,32,COL_THINGRAY);
   Set_Chr_Color(COL_BLACK,COL_THINGRAY);
   Set_ASC_Lib(Arial20B_Font);
      
   Put_Data(350,     125, "%4d",PCSInput_tmp);
 }
/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
uint32  Action_MoreCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<15;i++)
	   {
	     if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   return(Key_Code);
	}
//**********************************************************************
//函数  名:Put_Oper_Interface
//函数功能:显示工作 模式界面
//入口参数:op--选项
//出口参数:无
//**********************************************************************
void PCSInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    static uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150,80,500,300,3,COL_DEEPGRAY);
   
    Clr_Bar(300,120,200,32,COL_THINGRAY);    //窗口显示区
    
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,160,160+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,230,160+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160+100*i,300,160+100*i+80,300+60,3,COL_BLACK);
    
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Set_ASC_Lib(Arial20B_Font);
    for(i=0;i<5;i++)
    Put_Data( 190+100*i, 175,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 190+100*i, 245,"%d",5+i);
    
    Put_Line_Str(175, 315,Language_Str("退格", "BACK"));
    //Put_Char(280, 250,'.');
    //Put_Line_Str(280, 270,Language_Str("空", "NULL"));
    //Put_Line_Str(380, 270,Language_Str("空", "NULL"));
    Put_Line_Str(480, 320,Language_Str("返回", "RETURN"));
    Put_Line_Str(580, 320,Language_Str("确认", "OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   160 + 100*i + 80/2;   
         xy[i+5*j][1] =   160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    PCSInput_tmp = 0;
               
    Create_ButtonList(xy,15);               
}
//**********************************************************************
//函数  名:Menu_Oper_Op_Pro
//函数功能:日期配置界面下的选项处理
//入口参数:key_c--按键
//           op--当前选项
//出口参数:处理后所处选项的ID
//**********************************************************************
sint32 MoreCfg_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i;
    float  x;
    op_id = op;         //first
    i = Action_MoreCfg((key_c>>16)&0xffff,key_c&0xffff); 
    if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1:
    	case 2:
       	case 3:
   		case 4:
   		case 5:
   	    case 6:
    	case 7:
       	case 8:
   		case 9:
   		case 10:
   		case 11:  //back    
   		//case 12:  //.
   		      Get_PCSInput_Tmp(i-1);
   		      break;
   		case 13:   //null
   	          break;
   		case 14:   //return
   		      op_id = MENU_OP_OPER;   //返回主菜单
   		      break;
   		case 15:   //ok     
   		       if((PCSInput_tmp<10000)&&(PCSInput_tmp>10)&&(TRUE==positive_flag))  //PCS的范围
   		          {
   		           x = net_weight/PCSInput_tmp;
   		           if( x > 0.0005)
   		             weight_per_pcs = x;
   		           }   
   		       op_id =  MENU_OP_OPER; //返回主菜单
   		      break;
   	  	default:break;
   	   }
   	    Key_Status = KEY_RELEASE;
    }
return(op_id);
}
///////////////////////////////////////////////////
//
///////////////////////////////////////////////////
void  Get_PCSInput_Tmp(uint32 i)
{
 if((10==i)&&(PCSInput_tmp>0)) //back
    {
     PCSInput_tmp = PCSInput_tmp/10; 
    }
 else if((i<10)&&(PCSInput_tmp<1000)) 
        {
         PCSInput_tmp =PCSInput_tmp*10+i;
        }
  ////////////////////////////////////
}
