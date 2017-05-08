//**********************************************************************
//文件名:eeprom.c
//函数列表:
//1.从EEPROM中读取一块数据:Read_EEPROM
//2.从EEPROM指定地址读取一字节数据:Read_EEPROM_1B
//3.从EEPROM指定地址中读取2字节:Read_EEPROM_2B
//4.从EEPROM指定地址中读取4字节:Read_EEPROM_4B
//5.向EEPROM写一块数据:Write_EEPROM
//6.向EEPROM写1字节数据:Write_EEPROM_1B
//7.向EEPROM写2字节数据:Write_EEPROM_2B
//8.向EEPROM写4字节数据:Write_EEPROM_4B
//9.查询是否有EEPROM读写错误:Check_EEP_Wr_ERR
//10.测试EEPROM的读写是否正常:EEPROM_Rw_Test
//**********************************************************************
#include "eeprom.h"
#include "port.h"
#include "stdio.h"
#include "global.h"
#include "i2c_com.h"
#include "timer.h"
#include "type_def.h"
#include "stdlib.h"
#include "lpc2400.h"

static uint8 flag_eep_wr_error = FALSE;        //写EEPROM时发生错误的标志

/****************************************************************************
* 名称: Read_EEPROM()
* 功能: 从EEPROM读取一块数据
* 入口参数: addr地址(11bit--EEPROM的存储地址,不是器件地址), rptr接收缓冲区指
*            针, rlen顺序读的字节数
* 出口参数: 0为正常,非0则表示读错误
****************************************************************************/
uint8 Read_EEPROM(uint16 addr,  uint8 *rptr, uint16 rlen)
{
    uint16 end_addr;    //写的结束地址(最后一个字节的地址)
    uint16 bound_addr;    //组的边界地址
    uint16 len;
    uint8 di;
    uint8 *rbuf_ptr;
    
    if (NULL == rptr) return(TRUE);
    if (!rlen)    return(TRUE);
    
    rbuf_ptr = rptr;
    end_addr = addr+rlen-1;
    while (addr <= end_addr)
    {
        bound_addr = addr|0x0f;
        if (end_addr > bound_addr)    //越界,分多次读取
        {
            len = bound_addr-addr+1;
        }
        else
        {
            len = end_addr-addr+1;
        }
         di = (I2C_Rcv_Str(((addr>>7)&0x0E)|SYS24C08, addr, rbuf_ptr, len));
         if (di) break;
         addr += len;
         rbuf_ptr += len;
     }
     
     flag_eep_wr_error = di;    
     return (di);
}    

/****************************************************************************
* 名称: Read_EEPROM_1B()
* 功能: 从EEPROM读取1字节(字节)
* 入口参数: addr地址(11bit), r1ptr字节指针
* 出口参数: 0为正常,非0则表示读错误
****************************************************************************/
 uint8 Read_EEPROM_1B(uint16 addr,  uint8 *r1ptr)
{
    return(Read_EEPROM(addr, r1ptr, 1));
}

/****************************************************************************
* 名称: Read_EEPROM_2B()
* 功能: 从EEPROM读取2字节(半字)
* 入口参数: addr地址(11bit), r2ptr半字指针
* 出口参数: 0为正常,非0则表示读错误
****************************************************************************/
 uint8 Read_EEPROM_2B(uint16 addr, uint16 *r2ptr)
{
    return(Read_EEPROM(addr, ( uint8 *)r2ptr, 2));
}

/****************************************************************************
* 名称: Read_EEPROM_4B()
* 功能: 从EEPROM读取4字节(字)
* 入口参数: addr地址(11bit), r4ptr字指针
* 出口参数: 0为正常,非0则表示读错误
****************************************************************************/
 uint8 Read_EEPROM_4B(uint16 addr, uint32 *r4ptr)
{
    return(Read_EEPROM(addr, ( uint8 *)r4ptr, 4));
}

/****************************************************************************
* 名称：Write_EEPROM()
* 功能：向EEPROM写入一块数据
* 入口参数：addr地址(11bit--EEPROM存储空间的地址,不是器件地址), wptr写缓冲区
*            指针, wlen顺序写的字节数
* 出口参数：写正确与否(非0表示写错误)
****************************************************************************/
 uint8 Write_EEPROM(uint16 addr,  uint8 *wptr, uint16 wlen)
{
    uint8 *rbuf,*wptr_temp;
    sint32 i,j;
    sint32 time;
    uint16 addr_temp,end_addr;    //写的结束地址(最后一个字节的地址)
    uint16 bound_addr;    //组的边界地址
    uint16 len;
    
    if (NULL == wptr) return(TRUE);
    if (!wlen) return(TRUE);
    
    //if(wlen>128) wlen=128;
    rbuf = ( uint8*)malloc(wlen);    //开辟内存空间
    if (rbuf != NULL)    //内存开辟成功
    {
        if(Read_EEPROM(addr, rbuf, wlen)) 
        {    flag_eep_wr_error = 1;
            return(1);    //出错
        }    
        for(i=0; i<wlen; i++)
        {//从头开始比较即将写入和读出的内容,如果相同则当前字节不用再写入了
            if(rbuf[i]!=wptr[i]) break;
        }
        for(j=wlen-1; j>=0; j--)
        {//从尾开始比较即将写入和读出的内容,如果相同则当前字节不用再写入了
            if(rbuf[j]!=wptr[j]) break;
        }
        if(i>j) 
        {
            free(rbuf);                //释放内存
            flag_eep_wr_error = 0;
            return(0);                //都相同则不用再写入了
        }    
        addr += i;                        //重新计算写入地址和长度
        wptr += i;
        wlen = j - i + 1;
    }
    
    Macro_En_EEPROM();                    //禁止写保护

    wptr_temp = wptr;                    //从该位置开始写EEPROM
    addr_temp = addr;
    end_addr = addr+wlen-1;
    while (addr <= end_addr)
    {
        bound_addr = addr|0x0f;
        if (end_addr > bound_addr)    //越界,分多次读取
        {
            len = bound_addr-addr+1;
        }
        else
        {
            len = end_addr-addr+1;
        }
        if(I2C_Send_Str(((addr>>7)&0x0E)|SYS24C08, addr, wptr, len))
        {//出错
            Macro_Dis_EEPROM();                //允许写保护
            if (rbuf != NULL)
            {
                free(rbuf);                //释放内存
            }
            flag_eep_wr_error = 1;
            return(1);
        }
        addr += len;
         wptr += len;
        //等待写操作(5ms)
        time = Get_Cur_Time();
        for (i=0; i<60000; i++)
        {
            if (Cal_Time_Dif(time)>=5) break;
        }

     }        
    
    Macro_Dis_EEPROM();                    //允许写保护
    
    if (rbuf != NULL)
    {
        if(Read_EEPROM(addr_temp, rbuf, wlen)) 
        {
            free(rbuf);                //释放内存
            return(1);        //出错
        }        
        for(i=0; i<wlen; i++)
        {//比较写入和读出的内容,如果不同则返回错误
            if(rbuf[i]!=wptr_temp[i]) 
            {
                free(rbuf);                //释放内存
                return(1);
            }    
        }
    }
    
    if (rbuf != NULL)
    {
        free(rbuf);                //释放内存
    }
    flag_eep_wr_error = 0;    
    return(0);
}

/****************************************************************************
* 名称：Write_EEPROM_1B()
* 功能：向EEPROM写入1字节(字节)
* 入口参数：addr地址(11bit), w1b写入字节
* 出口参数：写正确与否(非0表示写错误)
****************************************************************************/
 uint8 Write_EEPROM_1B(uint16 addr,  uint8 w1b)
{
    return(Write_EEPROM(addr, &w1b, 1));
}

/****************************************************************************
* 名称：Write_EEPROM_2B()
* 功能：向EEPROM写入2字节(半字)
* 入口参数：addr地址(11bit), w2b写入半字
* 出口参数：写正确与否(非0表示写错误)
****************************************************************************/
 uint8 Write_EEPROM_2B(uint16 addr, uint16 w2b)
{
    return(Write_EEPROM(addr, ( uint8 *)(&w2b), 2));
}

/****************************************************************************
* 名称：Write_EEPROM_4B()
* 功能：向EEPROM写入4字节(字)
* 入口参数：addr地址(11bit), w4b写入字
* 出口参数：写正确与否(非0表示写错误)
****************************************************************************/
 uint8 Write_EEPROM_4B(uint16 addr, uint32 w4b)
{
    return(Write_EEPROM(addr, ( uint8 *)(&w4b), 4));
}

//**********************************************************************
//函数  名:Check_EEP_Wr_ERR
//函数功能:查询EEPROM读写是否有错误
//入口参数:无
//出口参数:TRUE-- 有误
//            FALSE--无误
//**********************************************************************
uint8 Check_EEP_Wr_ERR(void)
{
    return (flag_eep_wr_error);
}

//**********************************************************************
//函数  名:EEPROM_Rw_Test
//函数功能:EEPROM读写功能测试
//入口参数:无
//出口参数:OK--正常
//            ERROR--错误
//**********************************************************************
uint8 EEPROM_para_config(uint8 addr,uint8 x)
{
    uint8  test_tx_buf[4];
    uint32 i,j;
    test_tx_buf[0] = REC_CHECK_DATA1; 
    test_tx_buf[1] = REC_CHECK_DATA2;
    test_tx_buf[2] = x;
    test_tx_buf[3] = test_tx_buf[0]+ test_tx_buf[1]+ test_tx_buf[2];
    
    I2C_Send_Str(SYS24C08, addr, test_tx_buf, 4);
    for(i=0;i<800000;i++)	{;}	                 //delay
	
	for(i=0;i<4;i++) test_tx_buf[i] = 0;	     //clear buf
	
	I2C_Rcv_Str(SYS24C08,  addr, test_tx_buf, 4);	
	for(i=0;i<80000;i++)	{;}					 //delay
    
    if(test_tx_buf[3] == (test_tx_buf[0]+test_tx_buf[1]+test_tx_buf[2]))
    return(OK);
    else
    return(ERROR);              
}
