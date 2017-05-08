/***************************************************************************

 Author        : Neil Zhao - CAST

 Date          : Feb 20th, 2009

 File          : AD7190_IO.c

 Hardware      : ADuC7026 and AD7190

 Description   : Use the GPIO to simulate the SPI communication of AD7190
	
***************************************************************************/
#include "global.h"
#include "config.h"
#include "AD7190_IO.h"

static void delay (int length)
	{
	 while (length >0)
    	 length--;
	 }
//**************************************************************************************/
//  函数名     void Start_SPI(void)
//  输入参数   无
//  输出参数   无
/***************************************************************************************/
void  AD7190PIN_Init(void)
	{

	 //配置P1   PIN1.0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
     //         P1.01489 AD7190USE   
     SCS = 	SCS | 0x01 ;    //use FAST IO 
          	
	 FIO0DIR  |=   AD7190_CS;
	 FIO0DIR  |=  AD7190_DIN;
	 
	 FIO2DIR  |=   AD7190_CLK;
	 AD7190_DOUT_INT_MODE;
   
    }
//---------------------------------
//void WriteToAD7190(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the AD7190 via the SPI port. 
//--------------------------------------------------------------------------------
void WriteToAD7190(uint8 count,uint8 *buf)
{
	uint8	ValueToWrite = 0;
    uint8	i = 0;
	uint8	j = 0;
		
	AD7190_CLK_H;//SET_SCL();
	//delay(1);
	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
		  AD7190_CLK_L;    //CLR_SCL();
		  if(0x80 == (ValueToWrite & 0x80))
		   AD7190_DIN_H; // SET_SDO();	  //Send one to SDO pin
	 	  else
	 	   AD7190_DIN_L; //CLR_SDO();	  //Send zero to SDO pin
		   //delay(1);
		   AD7190_CLK_H;	//SET_SCL();
		
		   ValueToWrite <<= 1;	//Rotate data
		}
	 }	 
}


//---------------------------------
//void ReadFromAD7190(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the AD7190 via the SPI port. 
//--------------------------------------------------------------------------------
void ReadFromAD7190(uint8 count, uint8 *buf)
{
	uint8	i = 0;
	uint8	j = 0;
	uint8  	RotateData = 0;
        
    AD7190_CLK_H;//SET_SCL();
	//delay(1);
	for(j=count; j>0; j--)
	{
	   for(i=0; i<8; i++)
	     {
	       AD7190_CLK_L; //CLR_SCL();
	       RotateData <<= 1;	   //Rotate data
	       //delay(1);
	       if(FIO2PIN & AD7190_DOUT)
		   RotateData |= 1;	
		   AD7190_CLK_H; //SET_SCL();	
		   //delay(1);
	      }
	    *(buf + j - 1)= RotateData;
	}	 	
} 

//---------------------------------
//void SOFTWARERESET()
//---------------------------------
void  AD7190SoftwareReset(void)
     {
     	uint8 ResetQueue[]={0xff,0xff,0xff,0xff,0xff};
     	
     	AD7190_CS_H; //SET_CS();
	    delay(100);
	    AD7190_CS_L; //CLR_CS();
	    delay(100);
     	WriteToAD7190(5,ResetQueue);
     	delay(2000);   //access reg must be 500us after rst 
      }
//---------------------------------
//void internalZeroScaleCal()
//--------------------------------- 
void AD7190InternalZeroScaleCalibration()      
     {
     	uint8 WriteBuf[4];
     	     
        WriteBuf[3] = WEN|RW_W|(REG_CONF<<3)|CREAD_DIS;
        WriteBuf[2] = CHOP_DIS|REF_IN1;
        WriteBuf[1] = AIN1_COM;
	    WriteBuf[0] = BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;
	    WriteToAD7190(4,WriteBuf);   //write config reg
	
	    WriteBuf[3] = WEN|RW_W|(REG_MODE<<3)|CREAD_DIS;
	    WriteBuf[2] = MODE_INZCL|DAT_STA_EN|INCLK_MCLK2EN;
	    WriteBuf[1] = SINC_4|ENPAR_EN|SINGLECYCLE_DIS|REJ60_DIS;
     	WriteBuf[0] = 0x60;                                     // FS0-FS9;
    	WriteToAD7190(4,WriteBuf);   //write mode reg
        
        AD7190_CS_L;    //CLR_CS();
	    while(FIO2PIN & AD7190_DOUT){;}			//	wait until RDY = 0;
	    AD7190_CS_H;    //SET_CS();
       }
       
//---------------------------------
//void internalZeroScaleCal()
//--------------------------------- 
void AD7190InternalFullScaleCalibration()      
     {
     	uint8 WriteBuf[4];
     	     
        WriteBuf[3] = WEN|RW_W|(REG_CONF<<3)|CREAD_DIS;
        WriteBuf[2] = CHOP_DIS|REF_IN1;
        WriteBuf[1] = AIN1_COM;
	    WriteBuf[0] = BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;
	    WriteToAD7190(4,WriteBuf);   //write config reg
	
		WriteBuf[3] = WEN|RW_W|(REG_MODE<<3)|CREAD_DIS;
		WriteBuf[2] = MODE_INFCL|DAT_STA_EN|INCLK_MCLK2EN;
		WriteBuf[1] = SINC_4|ENPAR_EN|SINGLECYCLE_DIS|REJ60_DIS;
		WriteBuf[0] = 0x60;                                     // FS0-FS9;
		WriteToAD7190(4,WriteBuf);   //write mode reg
        
        AD7190_CS_L;    //CLR_CS();
		while(FIO2PIN & AD7190_DOUT){;}			//	wait until RDY = 0;
		AD7190_CS_H;    //SET_CS();
       }   
       
////////////////////////////////////////////////////////////
void AD7190_Set_Channel(uint32 ch)
      {
         uint8 buf[4];
         
         buf[3] = WEN|RW_W|(REG_CONF<<3)|CREAD_DIS;          //配置寄存器
         buf[2] = CHOP_EN|REF_IN1;                           //斩波使能 选择参考源
         buf[1] = ch<<4 ;                                    //CH左移三位即：配置字
	     buf[0] = BURN_DIS|REFDET_DIS|BUF_DIS|UB_UNI|GAIN_1; //杂项设置
         WriteToAD7190(4,buf);                               //write config reg
      }
            
       
void AD7190Configuration(uint32 ch)
{
    uint32 i;
	uint8 WriteBuf[4];
    //AD7190PIN_Init();
    VICIntEnable &= 0xFFFEFFFF;
     
	AD7190SoftwareReset();
	
	AD7190_Set_Channel(ch);	     
   	delay(1000);
	WriteBuf[3] = WEN|RW_W|(REG_MODE<<3)|CREAD_DIS;              //模式寄存器
	WriteBuf[2] = MODE_CONT | DAT_STA_EN | INCLK_MCLK2EN;            //连续转换 状态使能 选择时钟源   
	WriteBuf[1] = SINC_4|ENPAR_EN|SINGLECYCLE_DIS|REJ60_EN|0x0; //
	WriteBuf[0] = 0x80;     // FS0-FS9 96 80 32 16
	//WriteBuf[0] = 0x01; 
                          //FS9-0 数字越小，输出速率越快，有效位数变少
	WriteToAD7190(4,WriteBuf);        //write mode reg
	
	i = 0;
	do
	{
     if(0 == (FIO2PIN & AD7190_DOUT)) //等待转换完成，写入连续读取指令，然后交给中断
        break;
     delay(200000);
     i++;
    }
    while(i!=5);
     
    WriteBuf[0] = WEN|RW_R|(REG_DATA<<3)|CREAD_EN;
    WriteToAD7190(1,WriteBuf);   //write config reg
    ReadFromAD7190(4,WriteBuf);  //READ DATA 
   
    Init_EINT2();           //初始化外部测量引脚中断 
}           