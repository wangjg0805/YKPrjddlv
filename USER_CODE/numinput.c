#include  "disp.h"
#include  "lcd2478.h"
#include  "global.h"
#include  "FontLib.h"
#include  "FontLib2.h"

#include "graph.h"
#include "type_def.h"
#include "menu.h"
#include "mainmenu.h"
#include "mainmenu2.h"


#define  NUMINPUT_X0       200
#define  NUMINPUT_Y0       150
#define  NUMBER_WIDTH      80
#define  NUMBER_HEIGTH     50
#define  NUMBER_SPACE      10
#define  INDICATION_HEIGTH 60
#define  NUMBERWIN_WIDTH      NUMBER_WIDTH*4+NUMBER_SPACE*8  
#define  NUMBERWIN_HEIGTH     TITLE_HEIGTH+INDICATION_HEIGTH+NUMBER_HEIGTH*3+NUMBER_SPACE*6
#define  NUMBER_DEPTH         -3
//#define  NUMBER_COL        COL_WHITE
#define  BACKG_COL         COL_DEEPGRAY
#define	 BUTTON_COL        COL_DEEPGRAY

#define  KEY_OK         100
#define  KEY_ESC         99

/*///////////////////////////////////////////////////
/*
/*DISPLAY touchPosition
//////////////////////////////////////////////////*/
static uint32    Key_Indicator;     //指示键盘
static uint32    ShiftNumber;
static uint32    WinFromMode;	    //来自哪一个窗口按键
static uint32    Key_IndicatorXY[2];

void  Display_NumInput(void)
      {
	  uint16 x,y,z;
	  //Current_Window = POWER_WIN;
	  Key_Indicator = 0;
	  ShiftNumber = 0;
	  //WinFromMode = KeyArea;
	 Create_Window_Frame(NUMINPUT_X0,NUMINPUT_Y0,NUMBERWIN_WIDTH,NUMBERWIN_HEIGTH,NUMBER_DEPTH,BACKG_COL);
	 Clr_Bar(NUMINPUT_X0+abs(NUMBER_DEPTH), NUMINPUT_Y0+abs(NUMBER_DEPTH)+TITLE_HEIGTH, NUMBERWIN_WIDTH,INDICATION_HEIGTH, COL_DEEPGRAY); //create NULL AREA
	 Key_IndicatorXY[0] = NUMINPUT_X0+abs(NUMBER_DEPTH) + (NUMBERWIN_WIDTH)/2-54; //shift left 1.5
   	 Key_IndicatorXY[1] = NUMINPUT_Y0+abs(NUMBER_DEPTH)+TITLE_HEIGTH +6 ;
	 x = NUMINPUT_X0 +  abs(NUMBER_DEPTH);
	 y = NUMINPUT_Y0 +	abs(NUMBER_DEPTH)+TITLE_HEIGTH+INDICATION_HEIGTH;
	 z = x+NUMBER_SPACE;
	Put_Button_Bar(z,     1*NUMBER_SPACE+y+0*NUMBER_HEIGTH, z+NUMBER_WIDTH,1*NUMBER_SPACE+y+1*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     3*NUMBER_SPACE+y+1*NUMBER_HEIGTH, z+NUMBER_WIDTH,3*NUMBER_SPACE+y+2*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     5*NUMBER_SPACE+y+2*NUMBER_HEIGTH, z+NUMBER_WIDTH,5*NUMBER_SPACE+y+3*NUMBER_HEIGTH,2,COL_WHITE);
	Key[0].X_Position = z+NUMBER_WIDTH/2;
	Key[0].Y_Position = 1*NUMBER_SPACE+y+NUMBER_HEIGTH/2;
	Key[0].Value      = 0;
	Key[4].X_Position = z+NUMBER_WIDTH/2;
	Key[4].Y_Position = (3*NUMBER_SPACE+y+1*NUMBER_HEIGTH)+NUMBER_HEIGTH/2;
	Key[4].Value      = 4;
	Key[10].X_Position = z+NUMBER_WIDTH/2;
	Key[10].Y_Position = 5*NUMBER_SPACE+y+2*NUMBER_HEIGTH+NUMBER_HEIGTH/2;
	Key[10].Value      = KEY_ESC;
	/////////////////////
	z = 3*NUMBER_SPACE+x+1*NUMBER_WIDTH;
	Put_Button_Bar(z,     1*NUMBER_SPACE+y+0*NUMBER_HEIGTH, z+NUMBER_WIDTH,1*NUMBER_SPACE+y+1*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     3*NUMBER_SPACE+y+1*NUMBER_HEIGTH, z+NUMBER_WIDTH,3*NUMBER_SPACE+y+2*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     5*NUMBER_SPACE+y+2*NUMBER_HEIGTH, z+NUMBER_WIDTH,5*NUMBER_SPACE+y+3*NUMBER_HEIGTH,2,COL_WHITE);
	Key[1].X_Position = z+NUMBER_WIDTH/2;
	Key[1].Y_Position = 1*NUMBER_SPACE+y+NUMBER_HEIGTH/2;
	Key[1].Value      = 1;
	Key[5].X_Position = z+NUMBER_WIDTH/2;
	Key[5].Y_Position = (3*NUMBER_SPACE+y+1*NUMBER_HEIGTH)+NUMBER_HEIGTH/2;
	Key[5].Value      = 5;
	Key[8].X_Position = z+NUMBER_WIDTH/2;
	Key[8].Y_Position = 5*NUMBER_SPACE+y+2*NUMBER_HEIGTH+NUMBER_HEIGTH/2;
	Key[8].Value      = 8;
	//////////////////////
	z = 5*NUMBER_SPACE+x+2*NUMBER_WIDTH;
	Put_Button_Bar(z,     1*NUMBER_SPACE+y+0*NUMBER_HEIGTH, z+NUMBER_WIDTH,1*NUMBER_SPACE+y+1*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     3*NUMBER_SPACE+y+1*NUMBER_HEIGTH, z+NUMBER_WIDTH,3*NUMBER_SPACE+y+2*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     5*NUMBER_SPACE+y+2*NUMBER_HEIGTH, z+NUMBER_WIDTH,5*NUMBER_SPACE+y+3*NUMBER_HEIGTH,2,COL_WHITE);
    Key[2].X_Position = z+NUMBER_WIDTH/2;
	Key[2].Y_Position = 1*NUMBER_SPACE+y+NUMBER_HEIGTH/2;
	Key[2].Value      = 2;
	Key[6].X_Position = z+NUMBER_WIDTH/2;
	Key[6].Y_Position = (3*NUMBER_SPACE+y+1*NUMBER_HEIGTH)+NUMBER_HEIGTH/2;
	Key[6].Value      = 6;
	Key[9].X_Position = z+NUMBER_WIDTH/2;
	Key[9].Y_Position = 5*NUMBER_SPACE+y+2*NUMBER_HEIGTH+NUMBER_HEIGTH/2;
	Key[9].Value      = 9;
   ///////////////////
    z = 7*NUMBER_SPACE+x+3*NUMBER_WIDTH;
	Put_Button_Bar(z,     1*NUMBER_SPACE+y+0*NUMBER_HEIGTH, z+NUMBER_WIDTH,1*NUMBER_SPACE+y+1*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     3*NUMBER_SPACE+y+1*NUMBER_HEIGTH, z+NUMBER_WIDTH,3*NUMBER_SPACE+y+2*NUMBER_HEIGTH,2,COL_WHITE);
	Put_Button_Bar(z,     5*NUMBER_SPACE+y+2*NUMBER_HEIGTH, z+NUMBER_WIDTH,5*NUMBER_SPACE+y+3*NUMBER_HEIGTH,2,COL_WHITE);
	Key[3].X_Position  = z+NUMBER_WIDTH/2;
	Key[3].Y_Position  = 1*NUMBER_SPACE+y+NUMBER_HEIGTH/2;
	Key[3].Value       = 3;
	Key[7].X_Position = z+NUMBER_WIDTH/2;
	Key[7].Y_Position = (3*NUMBER_SPACE+y+1*NUMBER_HEIGTH)+NUMBER_HEIGTH/2;
	Key[7].Value      = 7;
	Key[11].X_Position = z+NUMBER_WIDTH/2;
	Key[11].Y_Position = 5*NUMBER_SPACE+y+2*NUMBER_HEIGTH+NUMBER_HEIGTH/2;
	Key[11].Value      = KEY_OK;
    //////////显示提示字符	 深蓝色+小字体
	Set_Chr_Color(COL_DEEPBLUE,COL_WHITE);
	Set_ASC_Lib(Arial20B_Font);
	for(x=0;x<10;x++)
	{	 
	 Put_Char(Key[x].X_Position-8,Key[x].Y_Position-13,0x30+x);
	}
	Set_Chr_Color(COL_DEEPRED,COL_WHITE);
	Set_ASC_Lib(Arial20B_Font);
	Put_Str(Key[10].X_Position-29,Key[10].Y_Position-10,Language_Str("退格","BACK"));    //ESC
	Set_Chr_Color(COL_DEEPGREEN,COL_WHITE);
	Put_Str(Key[11].X_Position-16,Key[11].Y_Position-10,Language_Str("确认","OK"));		//OK
	
	}
////////////////////////////
//
////////////////////////////
void  GetKeyBuf(Key_Indicator)
	{
	 switch(WinFromMode)
	 {
	 case 1:			       //from MONOCUTWINDOW
      
		 break;
	  case 2:			      //from BiCUTWINDOW
        
		 break;
	  case 3:			      //from MONOCOAGWINDOW
       
		 break;
	  case 4:			      //from BICOAGWINDOW
       
		 break;
	  case 5:			      //from BICOAGWINDOW
       
		 break;
	  case 6:			  //from BICOAGWINDOW
       
		 break;
	  default:break;	
	}
  }
////////////////////////////
////////////////////////////
//
////////////////////////////
void  Action_NumInput(uint16 x,uint16 y)
      {
	   uint32 i,KeyValue,a,b;
	   uint32 Power_buf;
	   struct KeyStruct *p;
	   p = Key;           //point to searcharea
	   KeyValue = 0xAA55;
	   for(i=0;i<12;i++)
	   {
	     if((abs((*p).X_Position-x)< NUMBER_WIDTH/2)&& (abs((*p).Y_Position-y)< NUMBER_HEIGTH/2))
		 {KeyValue = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   if(KeyValue != 0xAA55)
		{                              //update displayarea
 	      //增加反白显示 直到按键松开
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 if(88 != 0x80)
		 {
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
		 	 Set_Chr_Color(COL_DEEPBLUE,COL_THINGRAY);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_THINGRAY);
		     Put_Char(Key[i].X_Position-8,Key[i].Y_Position-13,0x30+i);   
		     break;
            case 10:
			 Set_Chr_Color(COL_DEEPRED,COL_THINGRAY);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_THINGRAY);
		     Put_Str(Key[i].X_Position-29,Key[i].Y_Position-10,Language_Str("退格","BACK"));   
		     break;
		    case 11:
			 Set_Chr_Color(COL_DEEPGREEN,COL_THINGRAY);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_THINGRAY);
		     Put_Str(Key[i].X_Position-16,Key[i].Y_Position-10,Language_Str("确认","OK"));   
		     break;
			 default:break;
		   }
		  }
		 do
		 {
		  //OSTimeDlyHMSM(0, 0, 0, 100);
		 }
		 while(8 != 0x80);		   //等待按键释放，并重新恢复按键颜色
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////复原显示
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
		 	 Set_Chr_Color(COL_DEEPBLUE,COL_WHITE);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_WHITE);
		     Put_Char(Key[i].X_Position-8,Key[i].Y_Position-13,0x30+i);   
		     break;
            case 10:
			 Set_Chr_Color(COL_DEEPRED,COL_WHITE);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_WHITE);
		     Put_Str(Key[i].X_Position-29,Key[i].Y_Position-10,Language_Str("退格","BACK"));   
		     break;
		    case 11:
			 Set_Chr_Color(COL_DEEPGREEN,BUTTON_COL);
		     Set_ASC_Lib(Arial20B_Font);
		     Put_Button_Bar(a,b, a+NUMBER_WIDTH,b+NUMBER_HEIGTH,2,COL_WHITE);
		     Put_Str(Key[i].X_Position-16,Key[i].Y_Position-10,Language_Str("确认","OK"));   
		     break;
			 default:break;
		   }
		if(KeyValue == KEY_ESC)
		  {switch(ShiftNumber)
		   {
		    case 0: Key_Indicator = 0;                              break;
			case 1:	Key_Indicator = Key_Indicator%10; ShiftNumber--;break;
			case 2:	Key_Indicator = Key_Indicator%100;ShiftNumber--;break;
			default:break;
		   }
		  }	
		if(((KeyValue>0) ||(KeyValue==0)) &&(KeyValue<11))
		  {switch(ShiftNumber)
		   {
		    case 0: Key_Indicator = KeyValue;                 ShiftNumber++;break;
			case 1:	Key_Indicator = KeyValue+Key_Indicator*10;ShiftNumber++;break;
			case 2:	 
			         Power_buf =  KeyValue+(Key_Indicator%100)*10;
			         if(Power_buf>300)
					 break;
					 else 
					 Key_Indicator = Power_buf;
					 break;
			default:break;
            }
		  }
////////////////////////////////////////////////////////////display power
        Set_Chr_Color(COL_DEEPBLUE,COL_WHITE);
	      Clr_Bar(Key_IndicatorXY[0]-50,Key_IndicatorXY[1]-5,150,60, COL_WHITE);
			 Set_ASC_Lib(Arial20B_Font);
   	  	if(Key_Indicator > 99)
	         i = Key_IndicatorXY[0];
				else if(Key_Indicator > 9)
					 i = Key_IndicatorXY[0]+20;
				else
					 i = Key_IndicatorXY[0]+35;
			Put_Data(i,Key_IndicatorXY[1], "%d", Key_Indicator);
      
////////////////////////////////////////////////////////////////
	if(KeyValue == KEY_OK)
		  {
		   GetKeyBuf(Key_Indicator);
		 
		  }
	  }
 	}