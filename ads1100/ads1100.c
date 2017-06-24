#include "global.h"
#include "config.h"
#include "ads1100.h"

static uint32   ad_sample_channel; 

void CD4053_mode(uint8 v)
{
    if(v==9) {
        CD4053_EN_SET;
    }
    else {
    
        if(v&0x01)
            CD4053_A_SET;
        else
            CD4053_A_CLR;
        
        if(v&0x02)
            CD4053_B_SET;
        else
            CD4053_B_CLR;
        
        if(v&0x04)
            CD4053_C_SET;
        else
            CD4053_C_CLR;             
            
        CD4053_EN_CLR;
    }
}

//////////////////////////////////////////////
static void delay_us(unsigned int x)
{
   unsigned int a;
   for(a=0; a<x; a++)
	{
	;
	}
}
//////////////////////////////////////////
uint32  Get_AD_Channel(void)
{
  return(ad_sample_channel);
}

//**************************************************************************************/
//  函数名     void Start_SPI(void)
//  输入参数   无
//  输出参数   无
/***************************************************************************************/
void  ADS1100Pin_Init(void)
	{

	 SCS = 	SCS | 0x01 ;    //use FAST IO 
	 
	 FIO0DIR |= ADS1100_CS | ADS1100_CLK | ADS1100_DOUT;
	 
	 FIO0DIR |= CD4052_POW_PIN | CD4052_A | CD4052_B | CD4053_A | CD4053_B | CD4053_C | CD4053_EN; 
   
	}

///////////////////////////////////////
//以下代码摘自网络，请测试后使用

void start(void)       //开始
{
   ADS1100_DOUT_H;
   delay_us(2);   
   ADS1100_CLK_H;
   delay_us(5);
   ADS1100_DOUT_L;
   delay_us(2);
   ADS1100_CLK_L;
   delay_us(2);
}

void stop(void)             //结束
   { 
     ADS1100_DOUT_L; 
     delay_us(2);
     ADS1100_CLK_H;
     delay_us(5);
     ADS1100_DOUT_H;
     delay_us(2);
     ADS1100_CLK_L;
     delay_us(2);
    }
/////////////////////////////////////
uint32 i2cWaitAck(void)
    {
     uint8 ErrTime=255;
     ADS1100_DOUT_H;
     ADS1100_DATA_MODE_I;
     delay_us(2);
     ADS1100_CLK_H;
     
     while(ADS1100_DOUT_TEST)
     {
       ErrTime--;
       if(0==ErrTime)
         {
          stop();
          return(1);
         }
      }
     ADS1100_CLK_L;
     ADS1100_DATA_MODE_O;
     return(0);
    }    
///////////////////////////////////////////    
 void ack(void)          //  发送连续读信号
{ 
    //TRISD0=0;
    //TRISD1=0;
    ADS1100_DOUT_L;
    delay_us(2);
    //ADS1100_CLK_L;
    ADS1100_CLK_H;
    delay_us(2);
    ADS1100_CLK_L;
    delay_us(2);
}
    
  void nack(void)        //  发送不连续读信号
  { 
    //TRISD0=0;
    //TRISD1=0;
    ADS1100_DOUT_H;
    delay_us(2);
    //ADS1100_CLK_L;
    ADS1100_CLK_H;
    delay_us(2);
    ADS1100_CLK_L;
    delay_us(2);
  }      
///////////////////////////////////////    
uint8  check(void)    //  检查应答信号
     {  
        uint8 check;
        
        ADS1100_CLK_L;
        delay_us(1);  
        ADS1100_CLK_H;
   
        if (ADS1100_DOUT_TEST)
          {
            check = 1;
          }
        else
          {
            check = 0;
          }
         
         ADS1100_CLK_L;
         return(check);
     }
     
////////////////////////////////////////////////     
 void sendBYTE(uint8 send_data)   //  发送一个字节
    {
     uint8 bitcount=8;               //发送8位

        do
          { 
           
           if((send_data&0x80)==0x80)   
              {
               ADS1100_DOUT_H;        //写 1
              }
           else  
              {
               ADS1100_DOUT_L;        //写 0
              }
           ADS1100_CLK_L;       //在时钟大于4u秒期间写数据
           delay_us(2);
           ADS1100_CLK_H;
           delay_us(2);
           ADS1100_CLK_L;
           send_data=send_data<<1;
           bitcount--;
          }while(bitcount);
          
          //TRISD1=1 ;                  //释放总线等待应答
          delay_us(2);       
}   
////////////////////////////////////////////////
uint8 reciveBYTE(void)             //接受一个字节
  {
    uint8  temp1    = 0;
    uint8  bitcount = 8;
    ADS1100_DOUT_H;
     
    ADS1100_DATA_MODE_I;
    //TRISD0=0;
    //TRISD1=1;               //置输入
    do
    {
     ADS1100_CLK_L;           //在时钟大于4u秒期间读数据
     delay_us(2);
     ADS1100_CLK_H;
     delay_us(2);
  
     if(ADS1100_DOUT_TEST)    //读 1
       { 
        temp1=temp1|0x01;    
       }
     else                      //读 0
       {
        temp1=temp1&0xfe;
       }
      ADS1100_CLK_L;
      
      if(bitcount-1)
      {
         temp1=temp1<<1;
         
      }
       bitcount--;  
      }while(bitcount);
      
      ADS1100_DATA_MODE_O;
     
      return(temp1);
  }      
///////////////////////////////////////////////////   
void wrtoIIC_Cfg(uint8 cmd)     //写器件地址和配置字

  { 

    
     start();                  //开始写
     sendBYTE(0x90);           //写器件地址（写）
         
     do{
        ;
        }while(check());       //等待应答
        
     sendBYTE(cmd);            //写配置字
     do{;
        }while(check());
        
     stop();                   //结束写
        
}     
///////////////////////////////////////////////////////
uint32 rdfromIIC(void)         //从ADS1100中读出数据
   { 
     uint32 tmp = 0; 
     
     start();                  //开始读
     sendBYTE(0x91);           //写器件地址（读）
     do{
        ;
       }while(check());        //等待应答
        
      tmp = reciveBYTE();      // 读高8位
      ack();                   //连续读
      tmp = ((tmp<<8)&0xff00) + reciveBYTE();     //读低8位
      //ack();                 //连续读
      //tmp = ((tmp<<8)&0xff00) + reciveBYTE();   //读配置字
      nack();
      stop();                  //结束读
      //delay();
      return(tmp);
   } 
   
//////////////////////////////////
void ADS1100_Init(void)
{
    //wrtoIIC_Cfg(0x8c);
    wrtoIIC_Cfg(0x83);

}   