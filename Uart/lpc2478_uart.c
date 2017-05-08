/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_uart.c
* Programmer(s)  : Prasad.K.R.S.V
* Version        :
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                           INCLUDE HEADER FILES
**************************************************************************************************************
*/
#include "lpc2478_uart.h"
#include "rtc.h"
#include "global.h"
#include "disp.h"

/*
**************************************************************************************************************
*                                         INITIALIZE UART
*
* Description: This function initializes UART port, setup pin select, clock, parity, stopbits, FIFO etc
*
* Arguments  : baud_rate    UART baud rate (115200)
*
* Returns    : None
*
**************************************************************************************************************
*/
#define   DIVADDVAL   1                                                 
#define   MULVAL      12   

void  UART_Init(INT32U baudrate)
{
    INT32U  Fdiv;
          
   
	PCONP 	|= 1 << 3;						                            /* 使能UART0                    */
	PINSEL0  = (PINSEL0 & 0xFFFFFF0F) | (0x01 << 4) | (0x01 << 6);	    //设置P0.2和P0.3连接到UART0                                    */
   	U0LCR    = 0x83;		                  /* 8 bits, no Parity, 1 Stop bit                              */
    Fdiv     = (((2 * Fpclk) / 16 / baudrate) + 1) / 2;
    
    U0DLM    = (Fdiv>>8)&0xff;      //) / 256;							                                                        
    U0DLL    =      Fdiv&0xff;      // % 256;
	U0LCR    = 0x03;		                  /* DLAB = 0
	                                                   */
    //U0FDR    = (DIVADDVAL << 0)|(MULVAL << 4); 
    
    U0FCR    = 0x07;		                  /* Enable and reset TX and RX FIFO                            */


}

/*
**************************************************************************************************************
*                                         PRINT CHARECTER
*
* Description: This function is used to print a single charecter through UART1.
*
* Arguments  : ch    charecter to be printed
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_PrintChar (uint8 ch)
{

   while (!(U0LSR & 0x20));
   U0THR  = ch;
}

/*
**************************************************************************************************************
*                                         PRINT STRING
*
* Description: This function is used to print a string
*
* Arguments  : str    Pointer to the string
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_CommStr(char *str,uint8 length)
{
 uint8 i;
 for(i=0;i<length;i++)
       UART_PrintChar(*str++);
 
 }
 
 void  UART_PrintStr(char *str)
{
 while(*str)
 {
   UART_PrintChar(*str++);
 }
}
/*
**************************************************************************************************************
*                                    send data to PC
*
**************************************************************************************************************
*/
//通讯协议
//数据开始标志（2个字节）+ 数据长度LEN（1个字节）+ 仪器标志码(1个字节) 
//+ 有效数据（n个字节） + 数据结束标志（2个字节） + 校验码（2个字节）
//
//
#define   FRAME_START_CHAR1               0x7e
#define   FRAME_START_CHAR2               0x7f
#define   FRAME_END_CHAR1                 0xea
#define   FRAME_END_CHAR2                 0xeb


#define   INSTRU_CODE_PH                  0x00
#define   INSTRU_CODE_DDLV                0x01
#define   INSTRU_CODE_RJY                 0x02
#define   INSTRU_CODE_LZJ                 0x03
#define   INSTRU_CODE_BALANCE             0x04

#define   PARA_CODE_PH                    0x00
#define   PARA_CODE_MV                    0x01
#define   PARA_CODE_COND                  0x02
#define   PARA_CODE_RES                   0x03
#define   PARA_CODE_TDS                   0x04
#define   PARA_CODE_SAL                   0x05
#define   PARA_CODE_DO1                   0x06
#define   PARA_CODE_DO2                   0x07
#define   PARA_CODE_TEMP                  0x08

#define   DATA_EXPAND_1                   0x00
#define   DATA_EXPAND_10                  0x01
#define   DATA_EXPAND_100                 0x02
#define   DATA_EXPAND_1000                0x03
#define   DATA_EXPAND_10000               0x04
#define   DATA_EXPAND_0_1                 0x05
#define   DATA_EXPAND_0_01                0x06
#define   DATA_EXPAND_0_001               0x07
#define   DATA_EXPAND_0_0001              0x08

#define   DATA_EXPAND_1_N                 0x09
#define   DATA_EXPAND_10_N                0x0A
#define   DATA_EXPAND_100_N               0x0B
#define   DATA_EXPAND_1000_N              0x0C
#define   DATA_EXPAND_10000_N             0x0D
#define   DATA_EXPAND_0_1_N               0x0E
#define   DATA_EXPAND_0_01_N              0x0F
#define   DATA_EXPAND_0_001_N             0x10
#define   DATA_EXPAND_0_0001_N            0x11

/*
#define   DATA_EXPAND_10                  0x01
#define   DATA_EXPAND_100                 0x02
#define   DATA_EXPAND_1000                0x03
#define   DATA_EXPAND_10000               0x04
/*
void  UART_SendData (void)
{
  static uint8  send_index = 0;
  uint16  check_value;
  uint32  i,j;
  char send_tmp[18];
  //////////////////////////////
  check_value = 0;
  i = 0;
  
  send_tmp[i++] = FRAME_START_CHAR1;
  send_tmp[i++] = FRAME_START_CHAR2;
  send_tmp[i++] = 0x0b;             //长度 固定
  #if SDJ_SOFT  //酸度计程序 传送三个参数 
  // ph_negative_flag ph_data;
  // mv_negative_flag mv_data;
  // temp_float_ptc
  //以下是根据INDEX 内容 构建帧数据
  send_tmp[i++] = INSTRU_CODE_PH;   //PH   仪器型号
  switch(send_index++) 
  {
   case 0: 
          send_tmp[i++] = PARA_CODE_PH; //PH值 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   case 1: 
          send_tmp[i++] = PARA_CODE_MV; //MV值 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   case 2: 
          send_tmp[i++] = PARA_CODE_TEMP; //温度 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   default:break;        
  }
  if(3 == send_index)
     send_index = 0;
  #else
  send_tmp[i++] = INSTRU_CODE_DDLV;   //PH   仪器型号
  switch(send_index++) 
  {
   case 0: 
          send_tmp[i++] = PARA_CODE_COND; //COND 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   case 1: 
          send_tmp[i++] = PARA_CODE_RES; //RES 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   case 2: 
          send_tmp[i++] = PARA_CODE_TDS; //TDS 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
   case 3: 
          send_tmp[i++] = PARA_CODE_SAL; //SAL 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;
          
   case 4: 
          send_tmp[i++] = PARA_CODE_TEMP; //温度 
          send_tmp[i++] = 0xAD;
          send_tmp[i++] = 0x02;
          send_tmp[i++] = 0x02; //倍数
          break;         
   default:break;        
  }
  if(5 == send_index)
     send_index = 0;
  #endif 
  send_tmp[i++] = 0x00;
  send_tmp[i++] = 0x00;
  send_tmp[i++] = FRAME_END_CHAR1;
  send_tmp[i++] = FRAME_END_CHAR1;
  for(j=2;j<8;j++)
   check_value += send_tmp[j]; //累加和
  send_tmp[i++] = check_value&0xff;
  send_tmp[i++] = (check_value>>8)&0xff;
 
  UART_CommStr((char *)send_tmp,send_tmp[2]+7);                                                    

}
*/

void  UART_SendData (void)
{
  uint16  check_value;
  uint32  i,tmp;
  char send_tmp[50];
  //////////////////////////////
  check_value = 0;
  i = 0;
  
  send_tmp[i++] = FRAME_START_CHAR1;
  send_tmp[i++] = FRAME_START_CHAR2;
  #if 0  //酸度计程序 传送三个参数 
  send_tmp[i++] = 22;             //长度 固定 4+6x 
  // ph_negative_flag ph_data;
  // mv_negative_flag mv_data;
  // temp_float_ptc
  send_tmp[i++] = INSTRU_CODE_PH;   //PH   仪器型号
  /////////////////////////////////////////////////////////////
  //存放PH数据
  send_tmp[i++] = PARA_CODE_PH;       //PH值 
  tmp = (uint32)(ph_data*1000 + 0.5);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //倍数
  if(TRUE == ph_negative_flag)
  send_tmp[i++] = DATA_EXPAND_1000_N; //倍数
  else
  send_tmp[i++] = DATA_EXPAND_1000;   //倍数
  ////////////////////////////////////////////////////////////
  //存放MV数据
  send_tmp[i++] = PARA_CODE_MV;       //MV值 
  tmp = (uint32)(mv_data*1000 + 0.5);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //倍数
  if(TRUE == mv_negative_flag)
  send_tmp[i++] = DATA_EXPAND_1000_N; //倍数
  else
  send_tmp[i++] = DATA_EXPAND_1000;   //倍数
  ////////////////////////////////////////////////////////////
  //存放温度数据
  send_tmp[i++] = PARA_CODE_TEMP;     //MV值 
  if(TRUE == manual_temp_comp_flag)
  tmp = (uint32)(temp_comp_manual*10);
  else
  tmp = (uint32)(temp_float_ptc*10);
  send_tmp[i++] =  tmp&0xff;
  send_tmp[i++] = (tmp>>8)&0xff;
  send_tmp[i++] = (tmp>>16)&0xff;
  send_tmp[i++] = (tmp>>24)&0xff;
  //存放倍数
  send_tmp[i++] = DATA_EXPAND_10;    //倍数
 
  #else
  send_tmp[i++] = INSTRU_CODE_DDLV;   //PH   仪器型号
  send_tmp[i++] = PARA_CODE_COND;     //COND 
  ////////////////////////////////////////////////////////////
  //存放温度数据
  /*
  send_tmp[i++] =  ddlv_data      & 0xff;
  send_tmp[i++] = (ddlv_data>>8)  & 0xff;
  send_tmp[i++] = (ddlv_data>>16) & 0xff;
  send_tmp[i++] = (ddlv_data>>24) & 0xff;
  */
  send_tmp[i++] = 0x11;
  send_tmp[i++] = 0x22;
  send_tmp[i++] = 0x33;
  send_tmp[i++] = 0x44;
  send_tmp[i++] = 0x55;
  send_tmp[i++] = 0x66;
  send_tmp[i++] = 0x77;
  send_tmp[i++] = 0x88;
  
  //存放倍数
  send_tmp[i++] = 0x02; //倍数
  #endif 
  send_tmp[i++] = 0x00;
  send_tmp[i++] = 0x00;
  send_tmp[i++] = FRAME_END_CHAR1;
  send_tmp[i++] = FRAME_END_CHAR2;
  for(tmp=2;tmp<(i-2);tmp++)
   check_value += send_tmp[tmp]; //累加和
  send_tmp[i++] = check_value&0xff;
  send_tmp[i++] = (check_value>>8)&0xff;
 
  UART_CommStr((char *)send_tmp,i);                                                    

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static void  Printer_enter(void)
  {
   //UART_PrintChar(0x0d);
   UART_PrintChar(0x0a);
  }
static void  Printer_CH_MODE(void)
  {
   UART_PrintChar(0x1C);
   UART_PrintChar(0x26);  //进入中文打印
  }
static void  Printer_EN_MODE(void)
  { 
   UART_PrintChar(0x1C);
   UART_PrintChar(0x2E);  //退出中文打印
  }

void UART_Print_Char1(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x36);  //选择字符集 1
 }  
void UART_Print_Char2(void)
 {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x37);  //选择字符集 2
 }    
//////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////  
static void  Printer_TITLE(void)
{
  uint8  i;
  uint8  send_tmp[20];
  i = 0;
  /////////////////////////
  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //横向纵向都放大一倍
  /////////////////////////////////////////////
  Printer_EN_MODE();
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = 'P';
  send_tmp[i++]  = '8';
  send_tmp[i++]  = '1';
  send_tmp[i++]  = '1';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
  if(CHINESE == Get_Language_Type())
   {
    Printer_CH_MODE();
    UART_PrintStr("型酸度计");
   }
  else
    UART_PrintStr("ACID METER");
  
  Printer_enter();
  Printer_EN_MODE();
  /////////////////////////////////////////////
  /*
  UART_PrintChar(0x1b);
  UART_PrintChar(0x55);  
  UART_PrintChar(0x01);  //横向

  UART_PrintChar(0x1b);
  UART_PrintChar(0x56);  
  UART_PrintChar(0x02);  //纵向都放大一倍
  */
  ////////////////////////////////////////////
}

static void  Printer_TABLE(void)
{
  uint32 temp_tmp;
  uint8  send_tmp[20];
  RTCTime time_tmp = RTCGetTime();
  if(TRUE == manual_temp_comp_flag)
    temp_tmp = (uint32)(temp_comp_manual*10);
  else
    temp_tmp = (uint32)(temp_float_ptc*10);
  ////////////////////////////////////////用户编号 
  if(CHINESE == Get_Language_Type())
  {
   Printer_CH_MODE();
   UART_PrintStr("用户编号");
  } 
  else
   UART_PrintStr("USER ID");
   
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+user_id/100;
  send_tmp[3]  = 0x30+(user_id%100)/10;
  send_tmp[4]  = 0x30+user_id%10;
  send_tmp[5]  = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  ///////////////////////////////////////打印日期
  if(CHINESE == Get_Language_Type())
  {
   Printer_CH_MODE();
   UART_PrintStr("打印日期");
  }
  else
   UART_PrintStr("PRINT TIME");
   
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+time_tmp.RTC_Mon/10;
  send_tmp[3]  = 0x30+time_tmp.RTC_Mon%10;
  send_tmp[4]  = '-';
  send_tmp[5]  = 0x30+time_tmp.RTC_Day/10;
  send_tmp[6]  = 0x30+time_tmp.RTC_Day%10;
  send_tmp[7]  = ' ';
  send_tmp[8]  = 0x30+time_tmp.RTC_Hour/10;
  send_tmp[9]  = 0x30+time_tmp.RTC_Hour%10;
  send_tmp[10] = ':';
  send_tmp[11] = 0x30+time_tmp.RTC_Min/10;
  send_tmp[12] = 0x30+time_tmp.RTC_Min%10;
  send_tmp[13] = ':';
  send_tmp[14] = 0x30+time_tmp.RTC_Sec/10;
  send_tmp[15] = 0x30+time_tmp.RTC_Sec%10;
  send_tmp[16] = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  ///////////////////////////////////////环境温度
  if(CHINESE == Get_Language_Type())
  {
   Printer_CH_MODE();
   UART_PrintStr("补偿温度");
  }
  else
   UART_PrintStr("COMP_TEMP");
  
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  send_tmp[1]  = ' ';
  send_tmp[2]  = 0x30+temp_tmp/100;
  send_tmp[3]  = 0x30+(temp_tmp%100)/10;
  send_tmp[4]  = '.';
  send_tmp[5]  = 0x30+temp_tmp%10; 
  send_tmp[6]  = ' '; 
  send_tmp[7]  = 0x00;
  UART_PrintStr(send_tmp);
  
  /*UART_Print_Char1();
  send_tmp[0]  = 0x24;
  send_tmp[1]  = 0x00;
  UART_PrintStr(send_tmp);
  UART_Print_Char2(); 
  */
  Printer_enter();
}

static void  Printer_SIGN(void)
{
  uint8  send_tmp[20];
  uint8  i;
  if(CHINESE == Get_Language_Type())
  {
   Printer_CH_MODE();
   UART_PrintStr("签名");
  }
  else
   UART_PrintStr("Signature");
  
  Printer_EN_MODE();
  send_tmp[0]  = ':';
  for(i=1;i<20;i++)
  send_tmp[i]  = '_';
  send_tmp[15] = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter(); 
  Printer_enter();
  Printer_enter();
}

static void  Printer_data(void)
{
  uint8  i;
  uint8  send_tmp[20];
  i = 0;
  Printer_EN_MODE();
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = ' ';
  
  send_tmp[i++]  = 'P';
  send_tmp[i++]  = 'H';
  send_tmp[i++]  = ':';
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = 0x00;
  
  UART_PrintStr(send_tmp);
  i = 0;
  if(TRUE == ph_negative_flag)
  send_tmp[i++] = '-';
  else
  send_tmp[i++] = ' ';
  
  sprintf(&send_tmp[i],"%6.1f", ph_data);
  i+=8;
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
 
  if(CHINESE == Get_Language_Type())
  {
   Printer_CH_MODE();
   UART_PrintStr("    电位");
  }
  else
   UART_PrintStr("  Volt");
 
  i = 0;
  send_tmp[i++]  = ':';
  if(TRUE == mv_negative_flag)
  send_tmp[i++]  = '-';
  else
  send_tmp[i++]  = ' ';
  
  sprintf(&send_tmp[i],"%7.2f", mv_data);
  i+=7;
  send_tmp[i++]  = ' ';
  send_tmp[i++]  = 'm';
  send_tmp[i++]  = 'V';
  send_tmp[i++]  = 0x00;
  UART_PrintStr(send_tmp);
  Printer_enter();
  Printer_enter();
}  
void  UART_PrintData(void)
{
  Printer_TITLE();
  Printer_TABLE();
  Printer_data(); 
  Printer_SIGN();
  print_busy = 0;
  
 }
 
void  Print_Init(void)
  {
    UART_PrintChar(0x1b);
    UART_PrintChar(0x40);  //初始化打印机
    
    UART_PrintChar(0x1b);
    UART_PrintChar(0x63);
    UART_PrintChar(0x00);  //从左到右打印
    
    UART_Print_Char1();
  }   
    