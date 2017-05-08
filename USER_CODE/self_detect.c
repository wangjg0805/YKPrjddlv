//**********************************************************************
//文件  名:self_detect.c
//函数列表:
//1.显示自检错误的信息:Put_SelfDetect_Err
//2.检查CPU部件:Check_CPU_Part
//3.自检LCD:LCD_Selfdetect
//**********************************************************************
#include "self_detect.h"
#include "FontLib.H"
#include "disp.h"
#include "lcd2478.h"
//#include "lcd2478_cilib.h"

#include "stdio.h"
#include "port.h"
#include "global.h"

#if (LCDTYPE == TFTLCD)
#include "lpc2400.h"
#else
#include "lpc2294.h"
#endif 

#define MAX_COUNT_MP_OK                3        //检测CPU端口最大无错次数3次

//**********************************************************************
//函数  名:Put_SelfDetect_Err
//函数功能:显示自检错误的信息
//入口参数:err--自检的结果
//出口参数:无
//**********************************************************************
void Put_SelfDetect_Err(uint32 err)
{
    sint32 di;
    uint8* str;
    Set_Graphic_Color(COL_DEEPRED,COL_WHITE);//设置背景色
    //Set_Write_Mode(WRITE_OR);
    Set_HZ_Lib(&ST_11);        //设置字库
    Set_ASC_Lib(Arial10B_Font);
    Clr_Screen(DISP_NOR);    //用背景色清零
    Set_Chr_Color(COL_DEEPRED,COL_WHITE);//设置错误字体颜色：红色
    if (err&MCU_RAM_ERR_FLAG_BIT)//RAM出错
    {
        str = Language_Str("RAM出错!","RAM ERROR!");
        di = 160-Get_Str_Width(str)/2;
        Put_Line_Str(di,110,str);
    }
    else if (err&MCU_ROM_ERR_FLAG_BIT)//ROM出错
    {
        str = Language_Str("ROM出错!","ROM ERROR!");
        di = 160-Get_Str_Width(str)/2;                    
        Put_Line_Str(di,110,str);                    
    }
    else if (err&MCU_PORT_ERR_FLAG_BIT)//端口错误
    {
        str = Language_Str("CPU端口出错!","CPU Port Error!");
        di = 160-Get_Str_Width(str)/2;                        
        Put_Line_Str(di,110,str);                            
    }
    else if (err&LCD_ERR_FLAG_BIT)//LCD自检错误
    {
        str = Language_Str("LCD出错!","LCD ERROR!");
        di = 160-Get_Str_Width(str)/2;                        
        Put_Line_Str(di,110,str);            
    }
}    

/****************************************************************************
* 名称：Check_CPU_Part()
* 功能：检测CPU部件
* 入口参数：无
* 出口参数：无
****************************************************************************/
#define RAM_SIZE    0x8000        //内部RAM为32K
#define RAM_ARRAY    ((volatile sint32 *) 0x40000000)    //内部RAM数组指针
#define ROM_ARRAY    ((volatile sint32 *) 0x0)        //内部ROM数组指针
//extern sint32 Image$$RO$$Limit;    //代码段末尾(由编译器产生)
//extern sint32 Image$$RW$$Base;    //RAM段起始(由编译器产生)
//extern sint32 Image$$ZI$$Base;    //变量段起始(由编译器产生)

sint16 Check_CPU_Part(void)
{
uint8 flag_mp_err;
sint32 i,d,cnt;
#ifndef _DEBUG
sint32 m;
#endif
sint16 part_err_flag = 0;
uint32 int_bak;

    int_bak = VICIntEnable;                //读中断使能状态
    VICIntEnClr = int_bak;                //禁中
/*    
    cnt = 0;
    for(d=0; d<10; d++)
    {//检测10次
        flag_mp_err = FALSE;
        // 检测打到138的作为片选的3根线 
        for(i=0; i<8; i++)
        {
            Macro_Wr_EO_Addr(i);
            if (EXT_OUTP1_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport1);    //防止检查改变外部扩展口的状态
            }
            else if (EXT_OUTP2_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport2);    //防止检查改变外部扩展口的状态
            }
            else if (EXT_OUTP3_ADDR == i)
            {
                Macro_Wr_EO_Bus(ext_outport3);    //防止检查改变外部扩展口的状态
            }            
            Macro_Wr_EO_Addr(i);
            Macro_Wr_Drv_Key(i);
            Macro_Wr_Drv_Key(i);
            if((Macro_Rd_EO_Addr() != i) || (Macro_Rd_Drv_Key() != i))
                flag_mp_err = TRUE;
            IO0SET = P0_EXT_BUS_ADDR | P0_SCAN_DRV_OUT;//初始化为高电平
            IO0SET = P0_EXT_BUS_DATA;//初始化为高电平
        }
        // 检测8位数据总线 
        IO0DIR |= P0_EXT_BUS_DATA;        //总线口设为输出
        for(i=0; i<256; i++)
        {
            Macro_Wr_EO_Bus(i);
            Macro_Wr_EO_Bus(i);
            Macro_Wr_EO_Bus(i);
            if(Macro_Rd_EO_Bus()!=i) flag_mp_err = TRUE;
            IO0SET = P0_EXT_BUS_DATA;//初始化为高电平
        }
        // 检测其他输出线 
        //if(Check_P0_Pin(P0_HC164_DATA1)) flag_mp_err = TRUE;    //单输出口,不测试
        if(Check_P1_Pin(P1_WP)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_LCD_EN)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_LCD_CS)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_INLET_V)) flag_mp_err = TRUE;
        if(Check_P1_Pin(P1_EXP_V)) flag_mp_err = TRUE;
        if(flag_mp_err) cnt++;
    }
    if(cnt > MAX_COUNT_MP_OK) part_err_flag |= MCU_PORT_ERR_FLAG_BIT;*/
    
/* 初始时为呼气状态 */
    Valve_Action_in_Exp();
/* 检测片内RAM */
    for(i=0; i<RAM_SIZE/4; i++)
    {
        d = RAM_ARRAY[i];        //保存原来内容
        RAM_ARRAY[i] = 0x55AA55AA;
        if(RAM_ARRAY[i] != 0x55AA55AA) part_err_flag |= MCU_RAM_ERR_FLAG_BIT;
        RAM_ARRAY[i] = 0xAA55AA55;
        if(RAM_ARRAY[i] != 0xAA55AA55) part_err_flag |= MCU_RAM_ERR_FLAG_BIT;
        RAM_ARRAY[i] = d;        //恢复原来内容
        if(part_err_flag&MCU_RAM_ERR_FLAG_BIT) break;
    }
    
/* 检测片内ROM程序 */
/*#ifndef _DEBUG
    m = ((sint32)(&(Image$$RO$$Limit)) +
        (sint32)(&(Image$$ZI$$Base)) -
        (sint32)(&(Image$$RW$$Base))) / 4;    //求程序段长度(字)
    d = 0;
    //for(i=0; i<m; i++) d += ROM_ARRAY[i];
    //if(d != ROM_ARRAY[m]) flag_mcu_rom_error = TRUE;
#endif*/

    VICIntEnable = int_bak;        //允中  
return (part_err_flag);
}

//**********************************************************************
//函数  名:LCD_Selfdetect
//函数功能:自检LCD，从LCD读取数据与显示缓冲区的数据对比.
//入口参数:无
//出口参数:自检结果
//**********************************************************************
sint16 LCD_Selfdetect(void)
{
    sint32 di,count=0;
   #if (LCDTYPE == TFTLCD)
    return (0);
   #else  
    Set_Panel_Addr(SEC_LAY_HPTR+0);        //设置显存光标地址,本系统的显示层采用第二层
    LCDBus_Write(CMD_TO_LCD,CRSDIR_INC_BY_ONE_CMD);    //设置光标地址的增长方式        
    LCDBus_Write(CMD_TO_LCD,MREAD_CMD);    //读显存
    for (di=0; di<DISP_BUF_BYTES; di++)
    {
        if (LCDBus_Read(DATA_ADDR_TO_MCU) != disp_buffer[di])//比较显示缓冲区和显存
        {
            if(DISP_BUF_BYTES/16 == ++count) //"1/16"的数据错误
            {
                return(LCD_ERR_FLAG_BIT);
            }
        }    
    }
    return(0);    
  #endif
}