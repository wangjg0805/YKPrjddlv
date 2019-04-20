
//**********************************************************************
//�ļ�˵��              
//����
//��1������ģʽ�˵��Ĵ���
//��2�����Բ˵��Ĵ���
//�����б�
//1   �����������������ʾ         MoreCfg_Disp_Pro
//2   ����������ʾ                 Brig_Adjust_Disp_Pro                  
//3   ����������ʾ                 WirelessCfg_Disp_Pro
//4   ��ĻУ׼��ʾ                 ScrCal_Disp_Pro
//5   ����ģʽ��ʾ      	       ModeSel_Disp_Pro
//6   ����������������ж� 	       Action_MoreCfg
//7   ���ȵ��������ж�      	   Action_BrightCfg
//8   �������ð����ж�      	   Action_WirelessCfg
//9   ��ĻУ׼��������      	   Action_ScreenCal
//10  ����ģʽ�����ж�      	   Action_ModeSelMenuCfg
//11  ��������������� ��ʼ��      MoreCfgInput_Interface
//12  ���ȵ������� ��ʼ��   	   Brig_Adjust_Interface
//13  �������ý��� ��ʼ��    	   WirelessCfgInput_Interface
//14  ��ĻУ׼���� ��ʼ��    	   ScrCalInput_Interface
//15  ����ģʽ���� ��ʼ��    	   ModeSel_Interface
//16  ����������������Ĵ���  	   MoreCfg_Key_Pro
//17  ���ȵ��� �����Ĵ���     	   Brig_Adjust_Key_Pro
//18  �������� �����Ĵ���     	   WirelessCfg_Key_Pro
//19  ��ĻУ׼ �����Ĵ���     	   ScrCal_Key_Pro
//20  ����ģʽ �����Ĵ���     	   ModeSel_Key_Pro
//21  ����ѡ���ͼ�λ���      	   Display_SelLang ������
//22  ����ѡ����水���ж�    	   Action_LangMenuCfg
//23  ����ѡ����� ��ʼ��    	   LangSel_Interface
//24  ����ѡ��ѡ��İ������� 	   LangSel_Key_Pro
//25  ����ѡ��ѡ�����ʾ����       LangSel_Disp_Pro
//26  ���������������ʾ����       NumberInput_Disp_Pro
//27  ������������� �����ж�      Action_NumInputMenuCfg
//28  ������������ ���� ��ʼ��     NumberInput_Interface
//29  ������������İ�������       NumInput_Key_Pro
//30  С���� �Ĵ���                dot_key_process
//31  �˸�� �Ĵ���                back_key_process
//32  �õ�������������� ��ʱ����  Get_NumInput_Tmp
//33  ������������ ��ʾ          PCSInput_Disp_Pro
//34  �������밴�� ���ж�          Action_PCSInputMenuCfg
//35  ������������ ��ʼ��        PCSInput_Interface
//36  ��������İ�������           PCSInput_Key_Pro
//37  �õ������������ʱ����       Get_PCSInput_Tmp
// 
//**********************************************************************
#include "Font_Lib.h"

#include "lcd2478.h"
#include "graph.h" 
#include "global.h"
#include "normal_operation.h"
#include "type_def.h"
#include "disp.h"
#include "i2c_com.h"
#include "eeprom.h"
#include "ad7843.h"
#include "alarm_pro.h"
#include "speaker.h"

///////////////////////////////
#include "ddlvmenu.h"


static uint32 language_var_tmp,standard_var_tmp,phmv_division_tmp,temp_comp_tmp,temp_unit_tmp,user_id_tmp;

static uint32 factory_do_flag;

static uint32 speak_var_tmp,bright_var_tmp;
static uint32 rec_page;             //��Ҫ��ʾ�ڼ�ҳ
static uint32 current_display_page; //��ǰ��ʾ���ǵڼ�ҳ
static uint32 rec_called_num;       //��ǰ�Ѿ���ʾ�˶�������¼
static uint32 curre_page_num;

////////////////////////////////////////////////////////
static uint32 PCSInput_tmp;

////////////////////////////////////////////////////////

//��ĻУ׼����
static uint32   Screen_CalStep,screen_step_tmp;
///////////////////////////////////////////////////////

void   Get_PCSInput_Tmp(uint32 data);
void   Display_SelLang(void);

//////////////////////////////////////////////////////////
//0A ���빤��У׼״̬ ��ȼ�
//////////////////////////////////////////////////////////
void Enter_SysCal_SDJ(void)
   {
    if(FALSE == point2_cal_start)
     {
      current_mode     = STAT_SYS_CAL;
      point2_cal_start = TRUE;
      point2_cal_step  = SYS_CAL_ZER0A;
      switch_flag=1;
     }
   }

//////////////////////////////////////////////////////////
//0A ���빤��У׼״̬ �絼����
//////////////////////////////////////////////////////////
void Enter_SysCal_DDLV(void)
   {
    if(FALSE == point2_cal_start)
     {
      current_mode     = STAT_SYS_CAL;
      point2_cal_start = TRUE;
      point2_cal_step  = SYS_CAL_RANG1_1;
      
      automode         = 0;
      range_man        = RANGE_200MS;
     }
   }
   
/*///////////////////////////////////////////////////
/*
/*2c �����Ļ����
//////////////////////////////////////////////////*/
void  Clr_Scr_Data(void)
    {
     uint32 i;
     for(i=0;i<10;i++)
     {
      Clr_Bar(115, 135+28*i, 40, 22, COL_DEEPGRAY); //���
      Clr_Bar(170, 135+28*i,155, 22, COL_DEEPGRAY); //����
      Clr_Bar(335, 135+28*i, 90, 22, COL_DEEPGRAY); //PH
      Clr_Bar(435, 135+28*i, 90, 22, COL_DEEPGRAY); //MV
      Clr_Bar(535, 135+28*i, 70, 22, COL_DEEPGRAY); //temp 
      Clr_Bar(615, 135+28*i, 70, 22, COL_DEEPGRAY); //�û���
     }
    }
/*///////////////////////////////////////////////////
/*
/*2b ��ȡ�ƶ���ַǰ��һҳ����
//////////////////////////////////////////////////*/
void Display_Page_Data(uint32 page_num)
    {
     un_rec_data rec_data;
     uint32 i,j,tmp;
     uint8  BufferData[16];
     uint8  check_data;
         
     if((0==rec_real_num)||(0 == page_num)||(page_num > rec_total_pages)) 
      {
       curre_page_num = 0;
       return; //�Ƿ����� ���߿�����
      }
     
     curre_page_num = rec_real_num-rec_called_num;
     if(0 == curre_page_num)   return;

     if(curre_page_num >= RECNUM_PER_PAGE) //����һҳ����һҳ��
       curre_page_num = RECNUM_PER_PAGE;
     
      //curre_page_num  ��ǰ��Ҫ��ʾ�ļ�¼��    
      //page_num        ��ǰҪ��ʾ��ҳ��
      Set_Chr_Color(COL_BLACK,COL_DEEPGRAY); 
      Set_ASC_Lib(Arial16B_Font);           //ʹ��16������
      for(i=0;i<curre_page_num;i++)
      {
       //���ȵõ��ü�¼�ĵ�ַ��ע���ַ�ķ�ת
       tmp = rec_curre_index-1 - (rec_called_num+i);
       if(tmp > rec_curre_index)                    //��ַ��ת��ļ��㷽��
       tmp = (REC_MAX_NUM-1) - (0xffffffff - tmp);  //ȡ�� ����ĩβȡֵ
         
       Read_EEPROM(EEP_REC_START_ADDR+tmp*EEP_BYTE_PER_REC, BufferData,16); //��ȡһ����¼
       check_data = 0;
       for(j=0;j<15;j++)
       {
        check_data += BufferData[j];
       } //�����ۼӺ�
       
       //��֤��¼����Ч��
       //��һ�����
       Put_Data(115, 135+28*i, "%3d", (page_num-1)*RECNUM_PER_PAGE+i+1);//��ʾ���
       if(check_data == BufferData[15])
       {//��֤OK����ʾһ����¼
        rec_data.rec_uint32 = (BufferData[7]<<24)+(BufferData[8]<<16)+(BufferData[9]<<8)+BufferData[10];
        //�ڶ�����ʾʱ�� ������ Сʱ����
        tmp = (BufferData[6]<<16)+(BufferData[5]<<8)+BufferData[4];
        
        //Put_Data(170, 135+28*i, "%02d",      BufferData[2]);
        //Put_Char(192, 135+28*i,'-');
        Put_Data(202-15, 135+28*i, "%02d",      (tmp>>16)&0x0f);
        Put_Char(224-15, 135+28*i,'-');
        Put_Data(234-15, 135+28*i, "%02d",      (tmp>>11)&0x01f);
        
        Put_Data(265-15, 135+28*i, "%02d",      (tmp>>6)&0x1f);
        Put_Char(287-15, 135+28*i,':');
        Put_Data(297-15, 135+28*i, "%02d",       tmp&0x3f);
        //Put_Data(285, 135+28*i, "%02d",      BufferData[7]);
        //������PH
        rec_data.rec_uint32 = (BufferData[7]<<24)+(BufferData[8]<<16)+(BufferData[9]<<8)+BufferData[10];
        Put_Data_Float(350, 135+28*i, "%6.2f",rec_data.rec_float);
        //������MV
        rec_data.rec_uint32 = (BufferData[11]<<24)+(BufferData[12]<<16)+(BufferData[13]<<8)+BufferData[14];
        Put_Data_Float(440, 135+28*i, "%7.2f",rec_data.rec_float);
        //������temp
        Put_Data_Float(550, 135+28*i, "%4.1f",BufferData[2]+BufferData[3]*0.1);
        //������uid
        Put_Data(630, 135+28*i, "%03d",(BufferData[0]<<8)+BufferData[1]);
       
        }
       else
       {//����¼��ȡ�д���ʱ���˳��������ѱ�ҳ�����ļ�¼��ʾ����
         Put_Str(Language_Pos(180,180), 135+28*i,Language_Str("������¼��","BAD RECORDER"));
        //break; 
       }
     } 
      rec_called_num += curre_page_num; //��������ʾ��¼��
   }  
/*///////////////////////////////////////////////////
/*
/*2a������ʾ����
//////////////////////////////////////////////////*/
void Data_Call_Disp_Pro(void)
{
 uint32 i;
 
 if(rec_page != current_display_page)
  {
   current_display_page = rec_page;
   Clr_Scr_Data();             //����Ļ�ϵ��������
   Display_Page_Data(rec_page);
  }   //ȷ����ҳ�����
}     //end

/*///////////////////////////////////////////////////
/*
/*7a ������ʾ�����ж�
//////////////////////////////////////////////////*/
uint32  Action_DataCallCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     //if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 50/2))
	     if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)< 25))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	     //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	      {
	       Key_Status = KEY_DO_OVER; 
	       Key_Valid_Sound();
	      }
	   return(Key_Code);
	}	
	
//**********************************************************************
//17a ������ʾ��������
//**********************************************************************
sint32 Data_Call_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  buffer[8];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_DataCallCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //clear ALL
  	           for(i=0;i<8;i++)
  	           buffer[i] = 0;
  	           Write_EEPROM(EEP_REC_TOTAL_ADDR,buffer, 8);
  	           for(i=0;i<1000;i++) {;}
  	           Log_Alm(&tips_clear_ok,ALM_LEV_TIPS);
  	           rec_total_num   = 0;
  	           rec_real_num    = 0;
		       rec_curre_index = 0;
		       rec_total_pages = 1;
  	           break;
    	case 2://��һҳ
    	       if(1 == rec_page)
    	       {
    	        //��ʾ��Ϣ�ǵ�һҳ
    	         Log_Alm(&tips_first_page,ALM_LEV_TIPS);
    	       }
    	       else if(rec_page>1)
    	       {rec_page--;
    	        rec_called_num = rec_called_num - RECNUM_PER_PAGE-curre_page_num; //������ʾ��¼������ȥ
    	       }
    	       
    	       break;
       	case 3://��һҳ
       	       rec_page++;
       	       if(rec_page > rec_total_pages)
       	       {
       	        rec_page--;
       	        //��ʾ��Ϣ�����һҳ
       	        Log_Alm(&tips_last_page,ALM_LEV_TIPS);
       	       } 
       	       break;
       	case 4:
       	       op_id = MENU_OP_OPER; //���ص����˵�
       	       break;
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}	
//**********************************************************************
//12a������:data_call_Interface
//��������:���ݻ���ʾ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Data_Call_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
     //Create window
     Create_Window_Frame(100,70,590,400,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(110,80,Language_Str("���ݼ�¼","Recorder"));
     Put_Char(210,80,'-');
     Put_Data(225,80,"%03d",rec_real_num);   //�ڱ�������ʾ��¼����
     
     //���ߵ���ɫ 
     Set_Graphic_Color(COL_THINRED,COL_DEEPGRAY);
     for(i=0;i<12;i++) //ÿҳ��ʾ10����¼ 
       Hor_Line(104,693,103+28*i);
   
     Ver_Line(103,411,103); 
     Ver_Line(103,411,160); //���
     Ver_Line(103,411,330); //����
     Ver_Line(103,411,430); //PH
     Ver_Line(103,411,530); //MV
     Ver_Line(103,411,610); //temp
     Ver_Line(103,411,693); //�û���
     
     Set_Chr_Color(COL_BLACK,COL_DEEPGRAY);
     #if SDJ_SOFT
     Put_Line_Str(Language_Pos(110,110), 108,Language_Str("���",       "No."));
     Put_Line_Str(Language_Pos(225,225), 108,Language_Str("����",      "Date"));
     Put_Line_Str(Language_Pos(360,360), 108,Language_Str("PHֵ",        "PH"));
     Put_Line_Str(Language_Pos(460,460), 108,Language_Str("mVֵ",        "mV"));
     Put_Line_Str(Language_Pos(550,550), 108,Language_Str("�¶�",      "Temp"));
     Put_Line_Str(Language_Pos(630,630), 108,Language_Str("UID",        "UID"));
     #else
     Put_Line_Str(Language_Pos(110,110), 108,Language_Str("���",       "No."));
     Put_Line_Str(Language_Pos(225,225), 108,Language_Str("����",      "Date"));
     Put_Line_Str(Language_Pos(350,350), 108,Language_Str("�絼��",    "Cond"));
     Put_Line_Str(Language_Pos(460,460), 108,Language_Str("mVֵ",        "mV"));
     Put_Line_Str(Language_Pos(550,550), 108,Language_Str("�¶�",      "Temp"));
     Put_Line_Str(Language_Pos(630,630), 108,Language_Str("UID",        "UID"));
     #endif
     ////////////////////////////////////////////////////////////////////
     Put_Button_Bar(180, 420, 180+80,  420+45,3,COL_THINBLUE);//clearALL
     Put_Button_Bar(300, 420, 300+80,  420+45,3,COL_THINBLUE);//prepage
     Put_Button_Bar(420, 420, 420+80,  420+45,3,COL_THINBLUE);//nextpage
     Put_Button_Bar(540, 420, 540+80,  420+45,3,COL_THINBLUE);//exit
    
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     Put_Line_Str(Language_Pos(190+5, 190),     435,Language_Str("���",   "DelALL"));
     Put_Line_Str(Language_Pos(300+3, 300+3),   435,Language_Str("��һҳ", "PrePage"));
     Put_Line_Str(Language_Pos(420+3, 420+3),   435,Language_Str("��һҳ", "NexPage"));
     Put_Line_Str(Language_Pos(540+15,540+20),  435,Language_Str("�˳�",   "Exit"));
   
     //create button list
     xy[0][0] =  180 + 80/2;    xy[0][1] = 420+50/2;
     xy[1][0] =  300 + 80/2;    xy[1][1] = 420+50/2;
     xy[2][0] =  420 + 80/2;    xy[2][1] = 420+50/2;
     xy[3][0] =  540 + 80/2;    xy[3][1] = 420+50/2;
                  
     Create_ButtonList(xy,4); 
     curre_page_num = 0;       //��ǰҳ����ʾ�ļ�¼����
     rec_called_num = 0;       //��ǰ��ʾ�ļ�¼��Ϊ0
     current_display_page = 0; 
     rec_page = 1;             //�ӵ�һҳ��ʼ��ʾ
          
}	
///////////////////////////////////////////////////////// 
/*///////////////////////////////////////////////////
/*4 ��ĻУ׼ ��ʾ
//////////////////////////////////////////////////*/
void ScrCal_Disp_Pro(void)
{
 if(Screen_CalStep != screen_step_tmp)
 {
  screen_step_tmp = Screen_CalStep;
  switch(screen_step_tmp)
  {
  case 1:  Draw_Point(            5,            5,     COL_WHITE);break;
  case 2:  Draw_Point(  SCR_XSIZE/2,  SCR_YSIZE/2,     COL_WHITE);break;
  case 3:  Draw_Point(  SCR_XSIZE-6,  SCR_YSIZE-6,     COL_WHITE);break;
  case 4:  Draw_Point(            5,            5, COL_DEEPGREEN);break;
  case 5:  Draw_Point(  SCR_XSIZE/2,  SCR_YSIZE/2, COL_DEEPGREEN);break;
  case 6:  Draw_Point(  SCR_XSIZE-6,  SCR_YSIZE-6, COL_DEEPGREEN);break;
  default:break;
  }
  }      
}

/*///////////////////////////////////////////////////
/*
/*9 ��ĻУ׼ ���̴���
//////////////////////////////////////////////////*/
void  Action_ScreenCal(uint16 x,uint16 y)
	{
	   static uint8  ScreenCalData[13];
	          uint8  CheckSum;
	   static uint16 Temp_x0,Temp_y0,Temp_xmid,Temp_ymid,Temp_xmax,Temp_ymax;
              uint32 i;
		
		switch(Screen_CalStep)
		 {
		 case 1:
		     if ((x>10) && (x<500) && (y>10) && (y<500))
			   {
			    Temp_x0 = x;Temp_y0 = y;
			    Screen_CalStep++;
			   }
			  break;
		 case 2:
		     if ((abs(x-XMID_DEFAULT)<300) && (abs(y-YMID_DEFAULT)<300))
			   {
			    Temp_xmid = x;Temp_ymid = y;
			    Screen_CalStep++;
			   }
			  break;
	     case 3:
		     if ((x>XMAX_DEFAULT-400) && (x<XMAX_DEFAULT+400) && (y>YMAX_DEFAULT-400) && (y<YMAX_DEFAULT+400))
			   {
			    Temp_xmax = x;Temp_ymax = y;
			    Screen_CalStep++;
			   }
			  break;
		 case 4:
		     if ((x>10) && (x<500) && (y>10) && (y<500))
			   {
			   Temp_x0 = (Temp_x0+x)/2;Temp_y0 = (Temp_y0+y)/2;
			   Screen_CalStep++;
			   }
			  break;
		 case 5:
		     if ((abs(x-XMID_DEFAULT)<300) && (abs(y-YMID_DEFAULT)<300))
			   {
			    Temp_xmid = (Temp_xmid+x)/2;Temp_ymid = (Temp_ymid+y)/2;
			    Screen_CalStep++;
			   }
			  break;
		 case 6:
		     if ((x>XMAX_DEFAULT-400) && (x<XMAX_DEFAULT+400) && (y>YMAX_DEFAULT-400) && (y<YMAX_DEFAULT+400))
			   {
			   Temp_xmax = (Temp_xmax+x)/2;Temp_ymax = (Temp_ymax+y)/2;
			   ScreenCalData[0]=  Temp_x0>>8;
			   ScreenCalData[1]=  Temp_x0&0xff; 
			   ScreenCalData[2]=  Temp_y0>>8;
			   ScreenCalData[3]=  Temp_y0&0xff; 
			   ScreenCalData[4]=  Temp_xmid>>8;
			   ScreenCalData[5]=  Temp_xmid&0xff; 
			   ScreenCalData[6]=  Temp_ymid>>8;
			   ScreenCalData[7]=  Temp_ymid&0xff; 
			   ScreenCalData[8]=  Temp_xmax>>8;
			   ScreenCalData[9]=  Temp_xmax&0xff; 
			   ScreenCalData[10]= Temp_ymax>>8;
			   ScreenCalData[11]= Temp_ymax&0xff; 
			   ScreenCalData[12]= 0;  
			   for(i=0;i<12;i++)
			   ScreenCalData[12] += ScreenCalData[i]; 
			   Write_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);   /* ����ʼ��ַ0x00��ʼд��10������*/
			   for(i=0;i<200000;i++)	{;}	                 //delay
			   for(i=0;i<13;i++) ScreenCalData[i] = 0;	 //clear buf
			   Read_EEPROM(EEP_SCREEN_ADDR, ScreenCalData, 13);	
			   for(i=0;i<1000;i++)	{;}					 //delay
               CheckSum = 0;							 //clear checksum
			   for(i=0;i<12;i++)
			   CheckSum += ScreenCalData[i];			 //get Pre_checksum
			   //////////////////////////////////////////////////
			   Clr_Bar(0,0,SCR_XSIZE,SCR_YSIZE,COL_BLACK);
			   Set_ASC_Lib(Arial16B_Font);
			   if(CheckSum != ScreenCalData[12])
			      {
			       Set_Chr_Color(COL_THINRED,COL_BLACK);
			       Put_Line_Str(350,  220,Language_Str("��ĻУ׼ʧ��","Screen Cal Failed!"));
			       Put_Line_Str(350,  250,Language_Str("��������������","Please Restart Machine!"));
				   while(1){;}
				  }
				  else
				  {
				   Set_Chr_Color(COL_DEEPGREEN,COL_BLACK);
				   Put_Line_Str(350,  220,Language_Str("��ĻУ׼�ɹ�","Screen Cal Successed!"));
				   Put_Line_Str(350,  250,Language_Str("��������������","Please Restart Machine!"));
				   while(1){;}
			      }
			  }
			  break;
		  default: break;
		 }
	}
//**********************************************************************
//19  ��ĻУ׼
//**********************************************************************
sint32 ScrCal_Key_Pro(uint32 key_c,sint32 op)
{   
    sint32 op_id;
    op_id = op;     
    if(KEY_DO_START == Key_Status)
     {
      Action_ScreenCal((key_c>>16)&0xffff,key_c&0xffff); 
   	  Key_Status = KEY_DO_OVER;
   	  Key_Valid_Sound();
     }
     return(op_id);
}
//**********************************************************************
//14����  ��:ScrCalInput_Interface
//��������:��ĻУ׼ ����ĳ�ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void ScrCalInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
      
     //Create window
      machine_mode = MACHINE_SCREENCAL_MODE;
      Screen_CalStep = 1;
      screen_step_tmp = 0;
      
}


/*///////////////////////////////////////////////////
/* 21  ����ѡ��ѡ�� �� ���Ż���
//////////////////////////////////////////////////*/
void Display_SelStandard(void)
{
  
   Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   Clr_Bar(430-10,150-10,30,150,COL_DEEPGRAY);
   switch(standard_var_tmp)
   {case CHINESE_S:
                 Circle(430,150,9);
                 Circle(430,150,8);
                 Circle(430,150,7);
                 Fill_Circle(430,150,4);
                break;
    case EUROPE_S:
                 Circle(430,200,9);
                 Circle(430,200,8);
                 Circle(430,200,7);
                 Fill_Circle(430,200,4);
                 break;
    case AMERICAN_S:
                 Circle(430,250,9);
                 Circle(430,250,8);
                 Circle(430,250,7);
                 Fill_Circle(430,250,4);
                 break;
    default:break;
    }
 }  
 ///////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*
/*2����������ʾ
//////////////////////////////////////////////////*/ 
void Standard_Sel_Disp_Pro(void)
{
  Display_SelStandard();
}

/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_StandardCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     //if((abs((*p).X_Position-x)< 130/2) && (abs((*p).Y_Position-y)< 40/2))
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   
	    //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	      {
	       Key_Status = KEY_DO_OVER; 
	       Key_Valid_Sound();
	      }
	   return(Key_Code);
	}
	

//**********************************************************************
//17 ���ȵ����µİ�������
//**********************************************************************
sint32 Standard_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_StandardCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           standard_var_tmp = CHINESE_S;
  	           break;
    	case 2://
    	       standard_var_tmp = EUROPE_S;
    	       break;
       	case 3:
       	       standard_var_tmp = AMERICAN_S;
       	       break;
       	case 4:
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       op_id =  MENU_OP_OPER;
       	       break;       
       	case 5://save to eeprom
       	       which_standard = standard_var_tmp; 
       	       tx_buf[0] = (uint8)(standard_var_tmp);
  	           tx_buf[1] = 0;
  	           tx_buf[2] = 0;
  	           tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_STANDARD_ADDR,tx_buf, 4);
  	           ////////////////////////////////
       	       need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	      
       	       break;
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void StandardSelInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
//Create window
    Create_Window_Frame(256,80,200,270,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("��׼ѡ��","STANDARD Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
    Bar(260+10,130,260+10+130,130+40);    //STAND1
    Bar(260+10,180,260+10+130,180+40);    //STAND2
    Bar(260+10,230,260+10+130,230+40);    //STAND3
    
    Put_Button_Bar(270, 300,270+80, 300+40,3,COL_BLACK);//esc
    Put_Button_Bar(360, 300,360+80, 300+40,3,COL_BLACK);//ok
  
    Set_Chr_Color(COL_THINGRAY,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270+40,  130+10,Language_Str("����",   "CH-S"));
    Put_Line_Str(270+40,  180+10,Language_Str("ŷ��",   "EU-S"));
    Put_Line_Str(270+40,  230+10,Language_Str("����",   "AM-S"));
   
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(290,280),  300+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  300+10,Language_Str("ȷ��",   "OK"));
    //create button list
    xy[0][0] = 270+130/2;    xy[0][1] = 130+40/2;
    xy[1][0] = 270+130/2;    xy[1][1] = 180+40/2;
    xy[2][0] = 270+130/2;    xy[2][1] = 230+40/2;
    xy[3][0] = 270+80/2;     xy[3][1] = 300+40/2;
    xy[4][0] = 360+80/2;     xy[4][1] = 300+40/2;
    
    Create_ButtonList(xy,5); 
    ///////////////////////////////
    standard_var_tmp = which_standard;
    Display_SelStandard();   
}

////////////////////////////////////////////////////////////
void  Brig_Adjust_Disp_Pro(void)
{
 uint32 i;
 if(bright_var_tmp != lcd_brightness)
  {
   bright_var_tmp = lcd_brightness;
   
   Clr_Bar(328,180,140,40,COL_BLACK);    //������ʾ��
   for(i=0;i<lcd_brightness;i++)
    {
     Clr_Bar(328+7*i,180,5,40,COL_DEEPGREEN); //4
     //Clr_Bar(352+10*i,180,2,40,COL_BLACK); //2
     }
   }
}
 /*///////////////////////////////////////////////////
/*
/*7 ���ȵ��������ж�
//////////////////////////////////////////////////*/
uint32  Action_BrightCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<3;i++)
	   {
	     //if((abs((*p).X_Position-x)< 60/2) && (abs((*p).Y_Position-y)< 50/2))
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	  //���ӽ���10��У׼�İ�����
	   for(;i<4;i++)
	   {
	     if((abs((*p).X_Position-x)< 25) && (abs((*p).Y_Position-y)< 25))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	      {
	       Key_Status = KEY_DO_OVER; 
	       Key_Valid_Sound();
	      }
	   return(Key_Code);
	}

//**********************************************************************
//17 ���ȵ����µİ�������
//**********************************************************************
sint32 Brig_Adjust_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  brightness_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_BrightCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 2: //ADD
  	           lcd_brightness++;
  	           if(lcd_brightness > BRIGHT_CLASS_TOTAL)
  	              lcd_brightness = BRIGHT_CLASS_TOTAL;
  	           Set_LCD_Brightness(lcd_brightness);
  	           break;
    	case 1://SUB
    	       if(lcd_brightness>1)
    	       lcd_brightness--;
    	       Set_LCD_Brightness(lcd_brightness);
    	       break;
       	case 3://save to eeprom
       	       brightness_tx_buf[0] = (uint8)(lcd_brightness);
  	           brightness_tx_buf[1] = 0;
  	           brightness_tx_buf[2] = 0;
  	           brightness_tx_buf[3] = brightness_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_BRIGHT_ADDR,brightness_tx_buf, 4);
  	           ////////////////////////////////
       	       need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	 
       	       break;
       	case 4:
       	       //���빤��У׼ģʽ
       	       Enter_SysCal_DDLV();
       	       
       	       op_id = MENU_OP_OPER; //���ص����ò˵�
       	       break;
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
	
//**********************************************************************
//12������:Brig_Adjust_Interface
//��������:���ȵ��� �����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void Brig_Adjust_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
     //Create window
     Create_Window_Frame(260+6,80,260,220,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(290,85,Language_Str("���ȵ���","Brightness Adjust"));
    
     Set_Chr_Color(COL_THINCYAN,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font); 
     Put_Line_Str(Language_Pos(345,330), 130,Language_Str("��ǰ����", "Current Bri"));    
    
     Clr_Bar(320+8,180,140,40,COL_BLACK);    //������ʾ��
      
     Put_Button_Bar(260+6, 170,260+6+60,   170+60,3,COL_THINBLUE);//add
     Put_Button_Bar(460+10,170,460+10+60,  170+60,3,COL_THINBLUE);//sub
     Put_Button_Bar(340+6, 250,340+6+110,  250+50,3,COL_BLACK);//ok
     
     Set_Chr_Color(COL_BLACK,COL_THINBLUE);
     Put_Line_Str(Language_Pos(305-20,300-20), 190,Language_Str("��","SUB"));
     Put_Line_Str(Language_Pos(480+10,475+10), 190,Language_Str("��","ADD"));
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     Put_Line_Str(Language_Pos(380,375), 265,Language_Str("ȷ��"," OK"));
   
    //create button list
    xy[0][0] =  266 + 60/2;    xy[0][1] = 170+60/2;
    xy[1][0] =  470 + 60/2;    xy[1][1] = 170+60/2;
    xy[2][0] =  346 + 110/2;   xy[2][1] = 240+60/2;
    xy[3][0] =         100;    xy[3][1] =      100;   //����һ���� ���̣����Խ���10��У׼
                  
    Create_ButtonList(xy,4);   
    
    bright_var_tmp = lcd_brightness+1; //��Ϊ���첻һ��             
}	



 ///////////////////////////////////////////////////////
 /*///////////////////////////////////////////////////
/* 21  ����ѡ��ѡ�� �� ���Ż���
//////////////////////////////////////////////////*/
void Display_SelDivision(void)
{
  
   Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPBLUE);
   
   Fill_Circle(430-0,140,10);
   Fill_Circle(430-0,190,10);
   Fill_Circle(430-0,240,10);
   Fill_Circle(430-0,290,10);
   //////////////////////////ph
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   if(phmv_division_tmp&0x01)
      {  
                 Circle(430,140,9);
                 Circle(430,140,8);
                 Circle(430,140,7);
                 Fill_Circle(430,140,4);
      }
    else
      {
                 Circle(430,190,9);
                 Circle(430,190,8);
                 Circle(430,190,7);
                 Fill_Circle(430,190,4);
      }
   ////////////////////mv    
   if(phmv_division_tmp&0x04)
      {  
                 Circle(430,240,9);
                 Circle(430,240,8);
                 Circle(430,240,7);
                 Fill_Circle(430,240,4);
      }
    else
      {
                 Circle(430,290,9);
                 Circle(430,290,8);
                 Circle(430,290,7);
                 Fill_Circle(430,290,4);
      }     
   
 }  
/*///////////////////////////////////////////////////
/*
//////////////////////////////////////////////////*/ 
void DivisionSel_Disp_Pro(void)
{
 Display_SelDivision();
}

/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_DivisionCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<6;i++)
	   {
	     //if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 40/2))
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 40))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	    //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER;  
	      Key_Valid_Sound();
	     } 
	   return(Key_Code);
	}
	

//**********************************************************************
//17 ���ȵ����µİ�������
//**********************************************************************
sint32 DivisionSel_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_DivisionCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           phmv_division_tmp = (phmv_division_tmp&0xfc)|0x01;
  	           break;
    	case 2://
  	           phmv_division_tmp = (phmv_division_tmp&0xfc)|0x02;
  	          break;
       	case 3:
  	           phmv_division_tmp = (phmv_division_tmp&0xf3)|0x04;
  	           break;
       	case 4:
  	           phmv_division_tmp = (phmv_division_tmp&0xf3)|0x08;
  	           break;
       	case 5:
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       //op_id =  MENU_OP_OPER;
       	       break;       
       	case 6://save to eeprom
       	       phmv_division = phmv_division_tmp; 
       	       tx_buf[0] = (uint8)(phmv_division_tmp);
  	           tx_buf[1] = 0;
  	           tx_buf[2] = 0;
  	           tx_buf[3] = tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_DIV_ADDR,tx_buf, 4);
  	           ////////////////////////////////
  	           need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       
       	       break;
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void DivisionSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[6][2];
    
//Create window
    Create_Window_Frame(256,80,200,280,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("�ֱ���ѡ��","DIVISION Select")); 
    //����
    Clr_Bar(260,    210,     200,       3, COL_THINBLUE);
    Clr_Bar(260+50, 210-50,  200-50,    3, COL_THINBLUE);
    Clr_Bar(260+50, 210+50,  200-50,    3, COL_THINBLUE);
    Clr_Bar(260,    210+100, 200,       3, COL_THINBLUE);
    
    Clr_Bar(260+50, 115-2,    3,       200, COL_THINBLUE);
    
    Set_Graphic_Color(COL_BLACK,COL_DEEPBLUE); 
    for(i=0;i<4;i++) 
      Bar(320  ,120+50*i,320+80,120+50*i+30);    //STAND1
   
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    if(1 == version_811)
    {
     Put_Data_Float(335,   125,      "%4.2f",   0.01);
     Put_Data_Float(335,   125+50,   "%5.3f",  0.001);
     Put_Data_Float(335,   125+100,  "%2.1f",    0.1);
     Put_Data_Float(335,   125+150,  "%3.2f",   0.01);
    }
    else
    {
     Put_Data_Float(335,   125,      "%3.1f",   0.1);
     Put_Data_Float(335,   125+50,   "%4.2f",  0.01);
     Put_Data(      335,   125+100,  "%d",        1);
     Put_Data_Float(335,   125+150,  "%3.1f",   0.1);
    }
    Set_Chr_Color(COL_DEEPBLUE,COL_THINGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270,  130+15,Language_Str("PH",   "PH"));
    Put_Line_Str(270,  230+15,Language_Str("mV",   "mV"));
   
    Put_Button_Bar(270, 320,270+80, 320+40,3,COL_BLACK);//esc
    Put_Button_Bar(360, 320,360+80, 320+40,3,COL_BLACK);//ok
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(290,280),  320+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  320+10,Language_Str("ȷ��",   "OK"));
    //create button list
    xy[0][0] = 320+80/2;     xy[0][1] = 120+30/2;
    xy[1][0] = 320+80/2;     xy[1][1] = 170+30/2;
    xy[2][0] = 320+80/2;     xy[2][1] = 220+30/2;
    xy[3][0] = 320+80/2;     xy[3][1] = 270+30/2;
    xy[4][0] = 270+80/2;     xy[4][1] = 320+40/2;
    xy[5][0] = 360+80/2;     xy[5][1] = 320+40/2;
    Create_ButtonList(xy,6); 
    ///////////////////////////////
    phmv_division_tmp = phmv_division;
    Display_SelDivision();   
}

////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*
//////////////////////////////////////////////////*/ 
#define  TEMP_UNIT_C     0x01
#define  TEMP_UNIT_F     0x02

void TempcompCfg_Disp_Pro(void)
{
  static uint32 temp_tmp;
 
  //��ʾ�¶�ѡ��ť
  Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
  if(TEMP_UNIT_C == temp_unit_tmp)
     {
      Fill_Circle(420,265,9);
      Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
      Circle(420,265,6);
      Circle(420,265,5);
      Circle(420,265,4);
      ///////////////////////////
      Clr_Bar(410,305,20,20,COL_DEEPGRAY);
     }
  else
     {
      Fill_Circle(420,315,9);
      Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
      Circle(420,315,6);
      Circle(420,315,5);
      Circle(420,315,4);
      ///////////////////////////
      Clr_Bar(410,265-10,20,20,COL_DEEPGRAY);
     }   
 }


/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_TempcompCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<5;i++)
	   {
	     //if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 40/2))
	     if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	    //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER;   
	      Key_Valid_Sound(); 
	     }
	   return(Key_Code);
	}
	

//**********************************************************************
//17 ���ȵ����µİ�������
//**********************************************************************
sint32 TempcompCfg_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_TempcompCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           from_menu = MENU_TEMP_COMP_OPER; 
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_UINT_INPUT_OPER;;
  	           break;
      	case 2://ѡ�����϶ȵ�λ
       	       temp_unit_tmp = TEMP_UNIT_C;
       	       break;
      	case 3://ѡ���϶ȵ�λ
      	       temp_unit_tmp = TEMP_UNIT_F;
       	       break;  
        
       	case 5://save to eeprom
       	       temp_comp_manual = temp_comp_tmp; 
       	       temp_unit        = temp_unit_tmp;
       	       tx_buf[0] = (uint8)temp_comp_manual;
       	       tx_buf[1] = temp_unit_tmp;
       	       tx_buf[2] = 0;
       	       tx_buf[3] = tx_buf[0] + tx_buf[1] + REC_CHECK_DATA1;
       	       Write_EEPROM(EEP_TEMP_COMP_ADDR,tx_buf, 4);
  	           ////////////////////////////////
  	           
  	    case 4://�ص����ý��� 
  	           need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	      
       	       break;
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void TempcompCfg_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[5][2];
    
    //Create window
    if(1==need_recover)
    {
     need_recover = 0;
     Recover_Area(0,80,600,400,save_buf);
    }
    else
   {           
    Create_Window_Frame(256,80,200,320,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(260,85,Language_Str("�¶Ȳ�������","TempComp Config")); 

    Set_Chr_Color(COL_DEEPBLUE,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270,  100+20,Language_Str("�¶Ȳ���ֵ:",  "Temp_Comp:"));
    Put_Line_Str(270,  200+20,Language_Str("�¶ȵ�λ:",    "Temp_Unit:"));
    Set_Graphic_Color(COL_BLACK,COL_DEEPBLUE); 
    Bar(300, 150, 300+100,150+40);  //�¶Ȳ�����ֵ����
    //-------------------------------------------------------
    Set_Graphic_Color(COL_WHITE,COL_DEEPBLUE);
    Bar(260, 205, 260+200,205+2);   //���ָ���
    //-------------------------------------------------------
    Set_Graphic_Color(COL_BLACK,COL_DEEPBLUE);
    Bar(300, 250, 300+90,250+30);
    Bar(300, 300, 300+90,300+30);
    //-------------------------------------------------------
    Set_Graphic_Color(COL_WHITE,COL_DEEPBLUE);
    Bar(260, 340, 260+200,340+2);   //���ָ���
   
   
    Put_Button_Bar(270, 360,270+80, 360+40,3,COL_BLACK);//esc
    Put_Button_Bar(360, 360,360+80, 360+40,3,COL_BLACK);//ok
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(290,280),  360+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  360+10,Language_Str("ȷ��",   "OK"));
    //
    Put_Line_Str(Language_Pos(310,330),  255,Language_Str("���϶�", " C "));
    Put_Line_Str(Language_Pos(310,330),  305,Language_Str("���϶�", " F "));
   
    temp_comp_tmp = temp_comp_manual;
   }
    //create button list
    xy[0][0] = 300+100/2;    xy[0][1] = 150+40/2;
    xy[1][0] = 300+90/2;     xy[1][1] = 250+30/2;
    xy[2][0] = 300+90/2;     xy[2][1] = 300+30/2;
    xy[3][0] = 270+80/2;     xy[3][1] = 360+40/2;
    xy[4][0] = 360+80/2;     xy[4][1] = 360+40/2;
    Create_ButtonList(xy,5); 
    ///////////////////////////////
    
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(340, 150+10, "%02d", temp_comp_tmp); 
    temp_unit_tmp = temp_unit; 
    
}
///////////////////////////////////////////////////
 ///////////////////////////////////////////////////////
/*///////////////////////////////////////////////////
/*
//////////////////////////////////////////////////*/ 


void UserID_Disp_Pro(void)
 {
  /*
  if(display_tmp != factory_cfg_tmp)
    {
     display_tmp = factory_cfg_tmp;
     if(0 == (factory_cfg_tmp&0x01))
      {
       Clr_Bar(270,     260,   180,   50,COL_BLACK);
       Clr_Bar(270+3, 260+3, 180-6, 50-6,COL_THINYELLOW);
       /////////////////////////////////////////////////////////////////////    
       Set_Chr_Color(COL_BLACK,COL_THINYELLOW);
       Put_Line_Str(Language_Pos(290,275),  265+10,Language_Str("������������",   "FACTORY PARAM"));
      } 
     else
      {
       Clr_Bar(270,     260,   180,   50,COL_BLACK);
       Clr_Bar(270+3, 260+3, 180-6, 50-6,COL_THINGREEN);
       /////////////////////////////////////////////////////////////////////    
       Set_Chr_Color(COL_BLACK,COL_THINGREEN);
       Put_Line_Str(Language_Pos(290,275),  265+10,Language_Str("������������",   "FACTORY PARAM"));
      }
    }*/
 }

/*///////////////////////////////////////////////////
/*
/*8 �������ð����ж�
//////////////////////////////////////////////////*/
uint32  Action_UserIDCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     //if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 40/2))
	     if((abs((*p).X_Position-x)< 60) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   //////////////////////////////////////////////////////////�а�������
	   if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER;   
	      Key_Valid_Sound(); 
	     } 
	   return(Key_Code);
	}
	

//**********************************************************************
//17 
//**********************************************************************
sint32 UserID_Key_Pro(uint32 key_c,sint32 op)
{   
    uint8  tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_UserIDCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 1: //
  	           from_menu = MENU_USER_ID_OPER;
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_UINT_INPUT_OPER;
  	           break;
  	    case 2: //
  	           Save_Area(0,80,600,400,save_buf);
  	           op_id = MENU_OPER_CONFIRM_OPER;
  	           break;
  	           	           
       	case 4://save to eeprom
       	       if(0 == factory_do_flag)  //��ǰû�лָ���������
       	       {
       	        user_id = user_id_tmp; 
       	        tx_buf[0] = (uint8)(user_id_tmp&0xff);
  	            tx_buf[1] = (uint8)((user_id_tmp>>8)&0xff);
  	            tx_buf[2] = 0;
  	            tx_buf[3] = tx_buf[0]+tx_buf[1]+tx_buf[2]; 
  	            Write_EEPROM(EEP_USER_ID_ADDR,tx_buf, 4);
  	           }
  	           ////////////////////////////////
  	    case 3:
  	           need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	      
       	       break;
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void UserIDInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
   
   if(1==need_recover) //�����ֲ˵�����
    {
     need_recover = 0;
     Recover_Area(0,80,600,400,save_buf);
    
     if(1 == factory_do_flag) //�� �ѻָ���־
       {
        Clr_Bar(280,     260,   170,   50,COL_BLACK);
        Clr_Bar(280+3, 260+3, 170-6, 50-6,COL_THINGREEN);
        /////////////////////////////////////////////////////////////////////    
        Set_Chr_Color(COL_BLACK,COL_THINGREEN);
        Put_Line_Str(Language_Pos(290,290),  265+10,Language_Str("�ָ����óɹ�", "FAC-CFG CALLED"));
       }
    }
   else
    {           
     //Create window
     Create_Window_Frame(256,80,200,310,3,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
     Put_Str(260,85,Language_Str("�û�����","User Config")); 

     Set_Chr_Color(COL_DEEPBLUE,COL_DEEPGRAY);
     Set_ASC_Lib(Arial16B_Font);
     Put_Line_Str(270,  100+35,Language_Str("��ǰ�û���:",  "USER ID:"));
     
     Set_Graphic_Color(COL_BLACK,COL_DEEPBLUE); 
     Bar(310, 170, 310+100,170+40);
    
     //------------------------------
     Clr_Bar(260, 230, 200, 3,COL_THINGRAY);
    
     //����ʾ��ˢ��
    
     Clr_Bar(280,     260,   170,   50,COL_BLACK);
     Clr_Bar(280+3, 260+3, 170-6, 50-6,COL_THINYELLOW);
     /////////////////////////////////////////////////////////////////////    
     Set_Chr_Color(COL_BLACK,COL_THINYELLOW);
     
     Put_Line_Str(Language_Pos(290,290),  265+10,Language_Str("�ָ���������",   "CALL FAC-CFG"));
   
     Put_Button_Bar(270, 340,270+80, 340+40,3,COL_BLACK);//esc
     Put_Button_Bar(365, 340,365+80, 340+40,3,COL_BLACK);//ok
     Set_Chr_Color(COL_WHITE,COL_BLACK);
     Put_Line_Str(Language_Pos(290,275),  340+10,Language_Str("ȡ��",   "CANCEL"));
     Put_Line_Str(Language_Pos(385,395),  340+10,Language_Str("ȷ��",   "OK"));
    
     user_id_tmp = user_id;
     factory_do_flag = 0;
    }
  
    //create button list
    xy[0][0] = 310+100/2;    xy[0][1] = 170+40/2;
    
    xy[1][0] = 280+170/2;    xy[1][1] = 260+50/2;
     
    xy[2][0] = 270+80/2;     xy[2][1] = 340+40/2;
    xy[3][0] = 365+80/2;     xy[3][1] = 340+40/2;
    
    Create_ButtonList(xy,4);
     
    ///////////////////////////////
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Data(345, 150+30, "%03d", user_id_tmp); 
}
/*///////////////////////////////////////////////////
/* 21  ����ѡ��ѡ�� �� ���Ż���
//////////////////////////////////////////////////*/
void Display_SelLang(void)
{
  
   Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
   switch(language_var_tmp)
   {case CHINESE:
                 Fill_Circle(430,150,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(430,150,6);
                 Circle(430,150,5);
                 Circle(430,150,4);
          
                 Clr_Bar(430-10,200-10,20,20,COL_DEEPGRAY);
                 break;
    case ENGLISH:
                 Fill_Circle(430,200,9);
                 Set_Graphic_Color(COL_DEEPGRAY,COL_DEEPGRAY);
                 Circle(430,200,6);
                 Circle(430,200,5);
                 Circle(430,200,4);
          
                 Clr_Bar(430-10,150-10,20,20,COL_DEEPGRAY);
                 break;
    default:break;
    }
 }  
 
 //**********************************************************************
//25 ����  ��:LangSel_Disp_Pro               
//��������:����ѡ�� �˵���ʾ����
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void LangSel_Disp_Pro(void)
{
   Display_SelLang();
  }
	
/*///////////////////////////////////////////////////
//  22  ����ѡ��� �����ж�
//////////////////////////////////////////////////*/
uint32  Action_LangMenuCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<4;i++)
	   {
	     //if((abs((*p).X_Position-x)< 150/2) && (abs((*p).Y_Position-y)< 40/2))
		 if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	      {
	       Key_Status = KEY_DO_OVER; 
	       Key_Valid_Sound();
	      }
	   return(Key_Code);
	}
	
//**********************************************************************
//24 ����  ��:LangSel_Key_Pro
//��������:����ѡ�� �˵��µİ�������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 LangSel_Key_Pro(uint32 key_c,sint32 op)
{
    uint8  eeprom_tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
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
       	case 4://OK
       	       Set_Language_Type(language_var_tmp);
       	       eeprom_tx_buf[0] = (uint8)(language_var_tmp);
  	           eeprom_tx_buf[1] = 0;
  	           eeprom_tx_buf[2] = 0;
  	           eeprom_tx_buf[3] = eeprom_tx_buf[0] + REC_CHECK_DATA1; 
  	           Write_EEPROM(EEP_LANGUAGE_ADDR,eeprom_tx_buf, 4);
  	           ////////////////////////////////
  	  	case 3://Esc
  	           need_recover2 = 1;
       	       op_id = MENU_SYSTEM_CONF_OPER; //�ص����ý���
       	       break;       
   		default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

//**********************************************************************
//23 ����  ��:LangSel_Interface
//��������:����ѡ���ʼ�� ����
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************	
void LangSel_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[4][2];
    
//Create window
    Create_Window_Frame(256,80,200,220,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(270,85,Language_Str("����ѡ��","Language Select")); 
    
    Set_Graphic_Color(COL_DEEPBLUE,COL_DEEPBLUE);
    Bar(260+10,130,260+10+130,130+40);    //chinese
    Bar(260+10,180,260+10+130,180+40);    //english
    
    Put_Button_Bar(270, 250,270+80, 250+40,3,COL_BLACK);//esc
    Put_Button_Bar(360, 250,360+80, 250+40,3,COL_BLACK);//ok
  
    Set_Chr_Color(COL_THINGRAY,COL_DEEPBLUE);
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(270+40,  130+10,Language_Str("����",   "����"));
    Put_Line_Str(270+25,  190+ 0,Language_Str("ENGLISH","ENGLISH"));
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(290,280),  250+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(380,390),  250+10,Language_Str("ȷ��",   "OK"));
    //create button list
    xy[0][0] = 270+130/2;    xy[0][1] = 130+40/2;
    xy[1][0] = 270+130/2;    xy[1][1] = 180+40/2;
    xy[2][0] = 270+80/2;     xy[2][1] = 250+40/2;
    xy[3][0] = 360+80/2;     xy[3][1] = 250+40/2;
                  
    Create_ButtonList(xy,4); 
    ///////////////////////////////
    language_var_tmp = Get_Language_Type();
    Display_SelLang();   
}

/////////////////////////////////////////4444444444444444444444444444444
////////////////////////////////////////////////33333
/*///////////////////////////////////////////////////
/* 33 �������� ��ʾ����
//////////////////////////////////////////////////*/
void UINTInput_Disp_Pro(void)
{
   static uint32 display_tmp;
   if(PCSInput_tmp != display_tmp)
   { 
    display_tmp = PCSInput_tmp;
    Clr_Bar(260,120,100,30,COL_THINGRAY);
    Set_Chr_Color(COL_BLACK,COL_THINGRAY);
    Set_ASC_Lib(Arial20B_Font);
    Put_Data(260,     125, "%4d",PCSInput_tmp);
   }
 }
/*///////////////////////////////////////////////////
/*34  �������밴�����ж�
/*
//////////////////////////////////////////////////*/
uint32  Action_PCSInputMenuCfg(uint16 x,uint16 y)
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
//35����  ��:PCSInput_Interface
//��������:������������ ��ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void UINTInput_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[15][2];
    //static  const uint32 display_code[] = {};
    //Create window
    Create_Window_Frame(150-100,80,500,300,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    
    if(MENU_MODE_SEL_OPER == from_menu)
       Put_Str(110,85,Language_Str("�����붨ʱʱ��","Please Input Timer"));
    else if(MENU_BALANCE_SEL_OPER == from_menu)
       Put_Str(110,85,Language_Str("�������ȶ�ʱ��","Please Input Stable_Time"));
    else if(MENU_USER_ID_OPER == from_menu)
       Put_Str(110,85,Language_Str("�������û���","Please Input User_ID"));
    else if(MENU_TEMP_COMP_OPER == from_menu)
       Put_Str(110,85,Language_Str("�����벹���¶�ֵ","Please Input Temp_Comp"));
    else
       Put_Str(110,85,Language_Str("������","Please Input"));
    
    Clr_Bar(300-100,120,200,32,COL_THINGRAY);    //������ʾ��
    
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,160,160-100+100*i+80,160+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,230,160-100+100*i+80,230+60,3,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Button_Bar(160-100+100*i,300,160-100+100*i+80,300+60,3,COL_BLACK);
    
    Set_ASC_Lib(Arial20B_Font);
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    for(i=0;i<5;i++)
    Put_Data( 190-100+100*i, 180,"%d",i);
    for(i=0;i<5;i++)
    Put_Data( 190-100+100*i, 250,"%d",5+i);
    
    Set_ASC_Lib(Arial16B_Font);
    Put_Line_Str(175-100, 320,Language_Str("�˸�", "BACK"));
    //Put_Char(280, 250,'.');
    //Put_Line_Str(280, 270,Language_Str("��", "NULL"));
    //Put_Line_Str(380, 270,Language_Str("��", "NULL"));
    Put_Line_Str(Language_Pos(480-100,480-100), 320,Language_Str("����", "ESC"));
    Put_Line_Str(580-100,                   320,Language_Str("ȷ��", " OK"));
    //create button list
    for(j=0;j<3;j++)
     { for(i=0;i<5;i++)  
        {                //start+off+
         xy[i+5*j][0] =   160-100 + 100*i + 80/2;   
         xy[i+5*j][1] =   160 +  70*j + 60/2;
        }
     }
    ////////////////////init variable 
    PCSInput_tmp = 0;
               
    Create_ButtonList(xy,15);               
}

/*///////////////////////////////////////////////////
/*
/*
//////////////////////////////////////////////////*/
#define  NUMBER_WIDTH       80
#define  NUMBER_HEIGTH      60
#define  NUM_WEI_INPUT      01
#define  NUM_PCS_INPUT      02
/*///////////////////////////////////////////////////
/* 27  �������� �İ��� ��Ӧ
//////////////////////////////////////////////////*/
uint32  Action_UINTInputMenuCfg(uint16 x,uint16 y,uint32 from_window)
	{
       uint32 i,Key_Code,a,b;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	  
	    for(i=0;i<15;i++)
	    {
	     //if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 60/2))
	     if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)< 30))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	    }
	  
	   //////////////////////////////////////////////////////////�а�������
	   if((i<15)&&(Key_Status == KEY_PRESS))
		{                              //��ס����ʾ
 	      //���ӷ�����ʾ ֱ�������ɿ�
		 a = Key[i].X_Position - NUMBER_WIDTH/2;
		 b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		 Set_Chr_Color(COL_BLACK,COL_THINGRAY);
		 Set_ASC_Lib(Arial16B_Font);
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
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			       
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		                
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }
		///////////////////////////////////////////
		 else if(Key_Status == KEY_DO_START)
		{ 
		 Key_Status = KEY_DO_OVER; 
		 Key_Valid_Sound();                          
 	     //���ӷ�����ʾ ֱ�������ɿ� 
		 if(i<15)
		 { 
		  a = Key[i].X_Position - NUMBER_WIDTH/2;
		  b = Key[i].Y_Position - NUMBER_HEIGTH/2;
		 
		  Set_Chr_Color(COL_WHITE,COL_BLACK);
		  Set_ASC_Lib(Arial16B_Font);
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
			 Set_ASC_Lib(Arial20B_Font);
		 	 Put_Char(Key[i].X_Position-8,Key[i].Y_Position-10,0x30+i);   
		     break;
            case 10:  //�˸�
			
		     Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("�˸�","BACK"));   
		     break;
		    case 11:  //.
			      if(NUM_WEI_INPUT == from_window)
			       {
			         Set_ASC_Lib(Arial20B_Font);
		             Put_Char(190, 310,'.'); 
		            }    
		     break;
		    case 12: 
			   //Put_Str(Key[i].X_Position-25,Key[i].Y_Position-5,Language_Str("�˸�","BACK"));   
		     break;
		    case 13:  //����
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("����","ESC"));   
		     break;
		    case 14:  //ȷ��
			 Put_Str(Key[i].X_Position-20,Key[i].Y_Position-10,Language_Str("ȷ��","OK"));   
		     break;
		    default:break;
		   }
		 }  
	   } 
	   return(Key_Code);
	}
//**********************************************************************
//36����  ��:PCSInput_Key_Pro
//��������:�������� ��������
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
extern  uint32      timer_tmp,balance_time_tmp;    //��ʱʱ�� ƽ��ʱ�� �����������ڴ˵���
sint32 UINTInput_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i;
    float  x,y;
    op_id = op;         //first
    i = Action_UINTInputMenuCfg((key_c>>16)&0xffff,key_c&0xffff,NUM_PCS_INPUT); 
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
   	    case 14:
   	            if(PASSWORD_INPUT_ID == op_id)  //������ɼ��׶�
                 {;}
                else
                 {
                   if((MENU_MODE_SEL_OPER==from_menu)||((MENU_MODE_SEL_OPER+0x1111)==from_menu))    
                        {
                         op_id = MENU_MODE_SEL_OPER; 
                         need_recover = 1;
                        }
                   else    
                        {
                         op_id = from_menu; 
                         need_recover = 1;
                        }
                 }
               break;  
   		case 15:   //return
   		        if(PASSWORD_INPUT_ID == op_id)  //������ɼ��׶�
                 {;}
                else
                 { 
                    if(MENU_TEMP_COMP_OPER==from_menu)
                        {//�ֶ��¶Ȳ��� �ɼ�                         
                         //if(((0==temp_unit)&&(PCSInput_tmp>99))||((1==temp_unit)&&(PCSInput_tmp>210)))
                         if(PCSInput_tmp > 99)
                         temp_comp_tmp = temp_comp_manual;
                         else
                         temp_comp_tmp = PCSInput_tmp;
                         
                         op_id = MENU_TEMP_COMP_OPER;
                         need_recover = 1;
                        }
                     else if(MENU_USER_ID_OPER==from_menu)    
                        {
                         if(PCSInput_tmp>999)
                         user_id_tmp = user_id;
                         else
                         user_id_tmp = PCSInput_tmp;
                         
                         op_id = MENU_USER_ID_OPER;
                         need_recover = 1;
                        }
                     else if(MENU_MODE_SEL_OPER==from_menu)    
                        {
                         if(PCSInput_tmp < 1000) //��ʱʱ�� ����� 999��
                           timer_tmp = PCSInput_tmp;
                           
                         op_id = MENU_MODE_SEL_OPER;
                         need_recover = 1;
                        }
                     else  if((MENU_MODE_SEL_OPER+0x1111)==from_menu)
                        {
                         //ƽ�� ʱ�� ����� 60��
                         if(PCSInput_tmp < 60) 
                           balance_time_tmp = PCSInput_tmp;
                           
                         op_id = MENU_MODE_SEL_OPER;
                         need_recover = 1;
                        }
                     else //����һ������״̬�����Է��ص�������
                         op_id = MENU_OP_OPER;
                 }
   		       break;
   		 default:break;
   	   }
   	    //Key_Status = KEY_DO_OVER;
    }
return(op_id);
}
///////////////////////////////////////////////////
//37  �õ������������ʱ����
///////////////////////////////////////////////////
void  Get_PCSInput_Tmp(uint32 i)
{
 if((10==i)&&(PCSInput_tmp>0)) //back
    {
     PCSInput_tmp = PCSInput_tmp/10; 
    }
 else if((i<10)&&(PCSInput_tmp<1000)) //���ּ������������������9999
        {
         PCSInput_tmp =PCSInput_tmp*10+i;
        }
}
///////////////////////////////////////////system configuration
////////////////////////////////////////////////////////////////
//**********************************************************************
//15����  ��:ModeSel_Interface
//��������:����ģʽ �Ľ����ʼ��
//��ڲ���:op--ѡ��
//���ڲ���:��
//**********************************************************************
void SystemConf_Interface(sint32 cur_menu_op)
{
    uint32 i,j;
    uint32 xy[9][2];
    
    //Create window
    if(1 == need_recover2)
     {
      need_recover2 = 0;
      Recover_Area(0,80,600,400,save_buf2);
     }
    else 
     {
    Create_Window_Frame(0,80,300,270,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(30,85,Language_Str("ϵͳ����","SYSTEM CONFIGURATION")); 
       
    Clr_Bar(    3,100+90,300,   3,COL_THINGREEN);//���߳�Ϊ�Ź��� 
    Clr_Bar(    3,180+90,300,   3,COL_THINGREEN); 
    Clr_Bar(100+3,   113,  3, 240,COL_THINGREEN); 
    Clr_Bar(200+3,   113,  3, 240,COL_THINGREEN); 
    ///////////////////////////////////////
  
    //ʹ��ͼƬ����
     if (Get_Language_Type() == CHINESE)
    {
     Put_Bmp( 10, 120,        time9060,0);  //
     Put_Bmp(110, 120,    language9060,0);  //
     Put_Bmp(210, 120,   brightcfg9060,0);  //
    
     #if SDJ_SOFT
     Put_Bmp( 10, 200,    standsel9060,0);  //
     Put_Bmp(110, 200, divisionsel9060,0);  //
     #else
     Put_Bmp( 10, 200,    consetupC9060,0);  //
     Put_Bmp(110, 200,    modesetupC9060,0);  //
     #endif
     Put_Bmp(210, 200,     tempcfg9060,0);  //
     
     Put_Bmp( 10, 280,      userid9060,0);  //
     Put_Bmp(110, 280,   screencal9060,0);  //
     Put_Bmp(210, 280,        exit9060,0);  //
    }
    else
    {
     Put_Bmp( 10, 120,        timeen9060,0);  //
     Put_Bmp(110, 120,    languageen9060,0);  //
     Put_Bmp(210, 120,   brightcfgen9060,0);  //
    
     #if SDJ_SOFT
     Put_Bmp( 10, 200,    standselen9060,0);  //
     Put_Bmp(110, 200, divisionselen9060,0);  //
     #else
     Put_Bmp( 10, 200,    consetupE9060,0);  //
     Put_Bmp(110, 200,    modesetupE9060,0);  //
     #endif
     Put_Bmp(210, 200,     tempcfgen9060,0);  //
      
     Put_Bmp( 10, 280,      useriden9060,0);  //
     Put_Bmp(110, 280,   screencalen9060,0);  //
     Put_Bmp(210, 280,        exit9060,0);  //
    }
    }
    //create button list
    xy[0][0] =  10+90/2;    xy[0][1] = 120+60/2;
    xy[1][0] = 110+90/2;    xy[1][1] = 120+60/2;
    xy[2][0] = 210+90/2;    xy[2][1] = 120+60/2;
    xy[3][0] =  10+90/2;    xy[3][1] = 200+60/2;
    xy[4][0] = 110+90/2;    xy[4][1] = 200+60/2;
    xy[5][0] = 210+90/2;    xy[5][1] = 200+60/2;
    xy[6][0] =  10+90/2;    xy[6][1] = 280+60/2;
    xy[7][0] = 110+90/2;    xy[7][1] = 280+60/2;
    xy[8][0] = 210+90/2;    xy[8][1] = 280+60/2;
                
    Create_ButtonList(xy,9);               
}

/*///////////////////////////////////////////////////
/*
/*10 ����ģʽѡ�� �����ж�
//////////////////////////////////////////////////*/
uint32  Action_SystemConfMenu(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<9;i++)
	   {
	     //if((abs((*p).X_Position-x)< 80/2) && (abs((*p).Y_Position-y)< 50/2))
	     if((abs((*p).X_Position-x)< 30) && (abs((*p).Y_Position-y)< 20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	      //////////////////////////////////////////////////////////�а�������
	    if(Key_Status == KEY_PRESS)
	     {
	      if(Get_Language_Type() == CHINESE)
	       {
	        switch(Key_Code)
	        {
	         case  1:    Put_Bmp( 10, 120,         time9060,1); break; //
             case  2:    Put_Bmp(110, 120,     language9060,1); break; //
             case  3:    Put_Bmp(210, 120,    brightcfg9060,1); break; //
             #if SDJ_SOFT
             case  4:    Put_Bmp( 10, 200,     standsel9060,1); break; //
             case  5:    Put_Bmp(110, 200,  divisionsel9060,1); break; //
             #else
             case  4:    Put_Bmp( 10, 200,    consetupC9060,1); break; //
             case  5:    Put_Bmp(110, 200,    modesetupC9060,1); break; //
             #endif
             case  6:    Put_Bmp(210, 200,      tempcfg9060,1); break; //
           
             case  7:    Put_Bmp( 10, 280,       userid9060,1); break; //
             case  8:    Put_Bmp(110, 280,    screencal9060,1); break; //
             case  9:    Put_Bmp(210, 280,         exit9060,1); break; //
             default:break;
            }
           }
           else
           {
            switch(Key_Code)
	        {
	         case  1:    Put_Bmp( 10, 120,         timeen9060,1); break; //
             case  2:    Put_Bmp(110, 120,     languageen9060,1); break; //
             case  3:    Put_Bmp(210, 120,    brightcfgen9060,1); break; //
             #if SDJ_SOFT
             case  4:    Put_Bmp( 10, 200,     standselen9060,1); break; //
             case  5:    Put_Bmp(110, 200,  divisionselen9060,1); break; //
             #else
             case  4:    Put_Bmp( 10, 200,     consetupE9060,1); break; //
             case  5:    Put_Bmp(110, 200,     modesetupE9060,1); break; //
             #endif
             case  6:    Put_Bmp(210, 200,      tempcfgen9060,1); break; //
             
             case  7:    Put_Bmp( 10, 280,       useriden9060,1); break; //
             case  8:    Put_Bmp(110, 280,    screencalen9060,1); break; //
             case  9:    Put_Bmp(210, 280,         exit9060,1); break; //
             default:break;
            }
           }  
          Key_Code = 0xAA55;
	     }
 	  //���ӷ�����ʾ ֱ�������ɿ� 
	  if(Key_Status == KEY_DO_START)  
	    {
	     Key_Status = KEY_DO_OVER; 
	     Key_Valid_Sound();
	     if(Get_Language_Type() == CHINESE)
	      {
	       switch(Key_Code)
	       {
	        case  1:    Put_Bmp( 10, 120,         time9060,0); break; //
            case  2:    Put_Bmp(110, 120,     language9060,0); break; //
            case  3:    Put_Bmp(210, 120,    brightcfg9060,0); break; //
            #if SDJ_SOFT
            case  4:    Put_Bmp( 10, 200,     standsel9060,0); break; //
            case  5:    Put_Bmp(110, 200,  divisionsel9060,0); break; //
            #else
            case  4:    Put_Bmp( 10, 200,     consetupC9060,0); break; //
            case  5:    Put_Bmp(110, 200,     modesetupC9060,0); break; //
            #endif
            case  6:    Put_Bmp(210, 200,      tempcfg9060,0); break; //
            
            case  7:    Put_Bmp( 10, 280,       userid9060,0); break; //
            case  8:    Put_Bmp(110, 280,    screencal9060,0); break; //
            case  9:    Put_Bmp(210, 280,         exit9060,0); break; //
            default:break;
           } 
          }
          else
          {
           switch(Key_Code)
	       {
	        case  1:    Put_Bmp( 10, 120,         timeen9060,0); break; //
            case  2:    Put_Bmp(110, 120,     languageen9060,0); break; //
            case  3:    Put_Bmp(210, 120,    brightcfgen9060,0); break; //
            #if SDJ_SOFT
            case  4:    Put_Bmp( 10, 200,     standselen9060,0); break; //
            case  5:    Put_Bmp(110, 200,  divisionselen9060,0); break; //
            #else
            case  4:    Put_Bmp( 10, 200,     consetupE9060,0); break; //
            case  5:    Put_Bmp(110, 200,     modesetupE9060,0); break; //
            #endif
            case  6:    Put_Bmp(210, 200,      tempcfgen9060,0); break; //
            
            case  7:    Put_Bmp( 10, 280,       useriden9060,0); break; //
            case  8:    Put_Bmp(110, 280,    screencalen9060,0); break; //
            case  9:    Put_Bmp(210, 280,         exit9060,0); break; //
            default:break;
           } 
          } 
	     }
	   return(Key_Code);
	}		
//**********************************************************************
//20����  ��:ModeSel_Key_Pro
//��������:����ģʽ�µ�ѡ���
//��ڲ���:key_c--����
//           op--��ǰѡ��
//���ڲ���:���������ѡ���ID
//**********************************************************************
sint32 SystemConf_Key_Pro(uint32 key_c,sint32 op)
{
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_SystemConfMenu((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
        case 1: //
                Save_Area(0,80,600,400,save_buf2);
  	            op_id = MENU_TIME_CFG_OPER;
  	            break;
    	case 2://
    	        Save_Area(0,80,600,400,save_buf2);
    	        op_id = MENU_LANG_SEL_OPER;
    	        break;
       	case 3://
       	        Save_Area(0,80,600,400,save_buf2);
       	        op_id = MENU_BRIG_CFG_OPER;
       	        break;
   		#if SDJ_SOFT
   		case 4://
   		        Save_Area(0,80,600,400,save_buf2);
   		        op_id = MENU_STAND_SEL_OPER;   //��׼ѡ��
   		        break;
  	    case 5: //
  	            Save_Area(0,80,600,400,save_buf2);
  	            op_id = MENU_DIVISION_SEL_OPER;//�ֱ���ѡ��
  	            break;
       	#else
       	case 4://
       	        Save_Area(0,80,600,400,save_buf2);
   		        op_id = MENU_DDLV_CONS_OPER;  //���� ����(�缫 TDS)
   		        break;
  	    case 5: //
  	            Save_Area(0,80,600,400,save_buf2);
  	            op_id = MENU_MODE_SEL_OPER;  //����ģʽѡ��
  	            break;
        #endif
        case 6://
    	        Save_Area(0,80,600,400,save_buf2);
    	        op_id = MENU_TEMP_COMP_OPER; //�¶Ȳ�������
    	        break;
       	
       	case 7://
       	        Save_Area(0,80,600,400,save_buf2);
       	        op_id = MENU_USER_ID_OPER;
       	        break;
   		case 8://
   		        op_id = MENU_SCR_CAL_OPER;
   		        break;
   		case 9://
   		        op_id = MENU_OP_OPER;
   		        break;
   		        
   	  	default:break;
   	   }
   	  
   	   // Key_Status = KEY_DO_OVER;
    }
return(op_id);
}

/*///////////////////////////////////////////////////
/*
/*5 ����ģʽ ������ʾ
//////////////////////////////////////////////////*/
void SystemConf_Disp_Pro(void)
{
      
 }
 
///////////////////////////////////////////////////////////////////////////
void OperConfirmInput_Interface(sint32 cur_menu_op)
  {
    uint32 i,j;
    uint32 xy[2][2];
  
    //Create window
    Create_Window_Frame(350,80,200,300,3,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);
    Set_Chr_Color(COL_DEEPYELLOW,COL_DEEPBLUE);
    Put_Str(370,85,Language_Str("����ȷ��","Confirm")); 

    Set_Chr_Color(COL_THINYELLOW,COL_DEEPGRAY);
    Set_ASC_Lib(Arial16B_Font);    
    //Put_Line_Str(370,  110+40,Language_Str("����ȷ��",         "Please Attention"));
    Put_Line_Str(370,  150+40,Language_Str("��ȷ����",         "Press OK"));
    Put_Line_Str(370,  190+40,Language_Str("���ָ���������",   "Call Fac-cfg"));
     
    Put_Button_Bar(360, 320,360+80, 320+40,3,COL_BLACK);//esc
    Put_Button_Bar(460, 320,460+80, 320+40,3,COL_BLACK);//ok
    Set_Chr_Color(COL_WHITE,COL_BLACK);
    Put_Line_Str(Language_Pos(380,370),  320+10,Language_Str("ȡ��",   "CANCEL"));
    Put_Line_Str(Language_Pos(480,490),  320+10,Language_Str("ȷ��",   "OK"));
    
    //create button list
    xy[0][0] = 360 + 80/2;    xy[0][1] = 320+40/2;
    xy[1][0] = 460 + 80/2;    xy[1][1] = 320+40/2;
    Create_ButtonList(xy,2);
  }
////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////  
uint32  Action_ConfirmCfg(uint16 x,uint16 y)
	{
       uint32 i,Key_Code;
	   struct KeyStruct *p;
	   p = Key;   //point to searcharea
	   Key_Code = 0xAA55;
	   for(i=0;i<2;i++)
	   {
	     //if((abs((*p).X_Position-x)< 100/2) && (abs((*p).Y_Position-y)< 40/2))
	     if((abs((*p).X_Position-x)< 40) && (abs((*p).Y_Position-y)<20))
		 {Key_Code = (uint16)((*p).Value);break;}
		 else
		 p++;
	   }
	   //////////////////////////////////////////////////////////�а�������
	   if(Key_Status == KEY_PRESS)
	     Key_Code = 0xAA55;
	    if(Key_Status == KEY_DO_START)  
	     {
	      Key_Status = KEY_DO_OVER;   
	      Key_Valid_Sound(); 
	     } 
	   return(Key_Code);
	}
////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////  
sint32 Confirm_Key_Pro(uint32 key_c,sint32 op)
  {
  
   uint8  tx_buf[4];
    sint32 op_id;
    uint32 i,j;
    op_id = op;         //first
    // if(Key_Status != KEY_DO_START)
    //return(op_id);
    i = Action_ConfirmCfg((key_c>>16)&0xffff,key_c&0xffff); 
   if(i != 0xaa55)
    {
      switch(i)
       {
  	    case 2: //
  	           do_factory_call();
  	           factory_do_flag = 1;
  	                        
  	    case 1: //
  	           need_recover = 1;
   		       op_id =  MENU_USER_ID_OPER; //�ص����ý���
  	           break;
        default:break;
   	   }
   	   // Key_Status = KEY_DO_OVER;
    }
    return(op_id);
  }

void  Confirm_Disp_Pro(void)
  {
 
  }
 
  
