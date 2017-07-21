//**************************************************************************************/
//  文件名   AD7843.C
//  功能     对ADS7843进行读写操作，提供的
//  输入接口（1）设置引脚连接
//          （2）设置滤波参数
//          （3）设置屏幕2个对角点
//          （4）设置延时
//  输出接口（1）输出X Y 坐标的AD采样数值
//          （2）输出相对屏幕的X Y数值
//  作者：   王建光
//  版本号： V1.0
//  日期：   2010.09.25
/***************************************************************************************/
#include  "global.h"
#include  "LCD2478.h"
#include  "ad7843.h"

/***************************************************************************************/
 INT32U      x0,y0,xmid,ymid,xmax,ymax;                      //用户原点和最大点设置

//**************************************************************************************/
//  函数名     void Start_SPI(void)
//  输入参数   无
//  输出参数   无
/***************************************************************************************/

void  ADS7843PIN_Init(void)
	{
    SCS = 	SCS | 0x01 ;    //use FAST IO  
	IO_AD7843_IRQDIR  &=  (~AD7843_IRQ) ; 
	IO_AD7843_DIR  |=   AD7843_CLK | AD7843_CS | AD7843_DIN ;
	IO_AD7843_DIR  &=  (~AD7843_BUSY) & (~AD7843_DOUT) ;
	//ADS7843_KeyBufClear();
   }

/***************************************************************************************/
//  函数名     void Start_SPI(void)
//  输入参数   无
//  输出参数   无
/***************************************************************************************/
void Start_SPI(void) //SPI开始
	{
	IO_AD7843_CLR = AD7843_CLK;
    DlyMs(DLYCONST);
    IO_AD7843_SET = AD7843_CS;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_DIN;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_CLK;
    DlyMs(DLYCONST);
    IO_AD7843_CLR = AD7843_CS;
    DlyMs(DLYCONST); 
    }
/***************************************************************************************/
//  函数名     void WriteCharTo7843(INT08U command)
//  输入参数   单字节命令
//  输出参数   无
/***************************************************************************************/
void WriteCharTo7843(INT08U command) 
    {
     INT08U count=0;
     IO_AD7843_CLR = AD7843_CLK;
     DlyMs(DLYCONST);
     for(count=0;count<5;count++)
	  {
	  	  
	  IO_AD7843_CLR = AD7843_DIN;
	  if(command & BIT7)
	  IO_AD7843_SET = AD7843_DIN;
	  
	  DlyMs(DLYCONST);
	  IO_AD7843_SET = AD7843_CLK; 
	  DlyMs(DLYCONST);
	  IO_AD7843_CLR = AD7843_CLK;
	  command<<=1;
	  }
	 for(count=0;count<3;count++)
	  {
	  	  
	  IO_AD7843_CLR = AD7843_DIN;
	  if(command & BIT7)
	  IO_AD7843_SET = AD7843_DIN;
	  
	  DlyMs(DLYCONST);
	  IO_AD7843_SET = AD7843_CLK; 
	  DlyMs(DLYCONST);
	  IO_AD7843_CLR = AD7843_CLK;
	  command<<=1;
	  }
    }
/***************************************************************************************/
//  函数名     UINT16U ReadFromCharFrom7843(void)
//  输入参数   无
//  输出参数   返回测量的X Y 数值（具体宽度由转换模式决定）
//  备注      (1) 函数内部采用查询方式得到采样数据；如不需要此引脚，可使用定时等其它方式
/***************************************************************************************/
INT16U  ReadFromCharFrom7843(void)              //SPI 读数据
	{
     INT08U count=0;
	 INT16U Num=0;
	 
	 IO_AD7843_SET = AD7843_CLK;
	 DlyMs(DLYCONST); 
     IO_AD7843_CLR = AD7843_CLK;
     
  	 for(count=0;count<12;count++)
	    {
   	     Num<<=1;
	     IO_AD7843_CLR = AD7843_CLK; 
		 DlyMs(DLYCONST);
	     IO_AD7843_SET = AD7843_CLK;
		 DlyMs(DLYCONST); 
	     if(IO_AD7843_PIN & AD7843_DOUT) 
	        Num |= BIT0;
 	     }
      return (Num);
	}

/***************************************************************************************/
//  函数名     void DlyMs(INT16U m)
//  输入参数   INT16U型延时数值(根据不同硬件环境调节)
//  输出参数   无
/***************************************************************************************/
void DlyMs(INT32U m)        // adjustable
{
  do
  {
  ;
  }while(m--);
}

/*
INT32U KeyAD_Proc(INT32U x,INT32U y)
{
    INT32U i,x_tmp,y_tmp;
    
    key_bufx[buf_index] = x;
    key_bufy[buf_index] = y;
    buf_index++;
    if(32 == buf_index) {
        buf_index = 31;
    }
    x_tmp = 0;
    y_tmp = 0;
    for(i=0;i<buf_index;i++) {
        x_tmp += key_bufx[i];
        y_tmp += key_bufy[i];
    }
    x_tmp = x_tmp/buf_index;
    y_tmp = y_tmp/buf_index;
    return(((x_tmp<<16)&0xffff0000) + (y_tmp&0x0000ffff));
}
*/
/***************************************************************************************/
//  函数名     Get_PositionAD(void)
//  输入参数   指针变量，用于存储AD数值:高16位：X;低16位：Y 
//  输出参数   位指示当前数值是否有效
/***************************************************************************************/
BOOLEAN  Get_PositionAD(INT32U *PositionAD)
	{  
     BOOLEAN valid;
     INT32U i,X1,Y1,X2,Y2,x,y;
         
     static INT32U key_data,key_data_last;
    
     /////////////////////////////////////////
     X1 = 0; Y1 = 0;
     X2 = 0; Y2 = 0;
     /////////////////////////////////////////
	 Start_SPI();
	 if(0 == (IO_AD7843_IRQPIN&AD7843_IRQ))
	   {//有键按下
	         //Start_SPI();
	         if(Key_Status == KEY_PRESS){
                  valid = OK; 
		          *PositionAD = key_data_last;		 
		     } else {
                do {
	                X2 = X1;
	                Y2 = Y1;
	                DlyMs(5000);
	                WriteCharTo7843(STARTBIT|MX|CONVER12BIT|SINGL_END|PD_ENABLE); //X坐标
	                X1 = ReadFromCharFrom7843();
                    WriteCharTo7843(STARTBIT|MY|CONVER12BIT|SINGL_END|PD_ENABLE); //Y
                    Y1 = ReadFromCharFrom7843();
                    x = abs(X1-X2);
                    y = abs(Y1-Y2);     
	            } while(!((x<ACCP_VALUE)&&(y<ACCP_VALUE)));
	    
	         if(0 == (IO_AD7843_IRQPIN&AD7843_IRQ)) {
	              if((KEY_DO_OVER+KEY_RELEASE) == Key_Status) {
	                Key_Status = KEY_PRESS;
	                valid = OK;
	                key_data_last= ((X1<<16)&0xffff0000) + (Y1&0x0000ffff);
	                *PositionAD = key_data_last;
	              }     
	         } else {
               valid = FALSE;
               *PositionAD = 0;
            }
         }
     }
      ////////////////////////////////////////////////////////////////  
	 else
	 {  
	   if(Key_Status == KEY_PRESS)
		 {  //刚刚无键按下 或者 按键按下后还没有执行  
		  valid = OK; 
		  *PositionAD = key_data_last;		 
		  Key_Status  = KEY_DO_START;     //发送执行指令 
		  key_data_last = 0;   
	     }
       else if(Key_Status ==KEY_DO_OVER) {
           valid = FALSE;
           *PositionAD = 0;
		   Key_Status += KEY_RELEASE;
       }
       else {
           valid = FALSE;
           *PositionAD = 0;     
       }    
    } 
       
    /////////////////////////////////////////////////  
    return(valid);
  } 
/***************************************************************************************/
//  函数名      Set_Originalx0y0(INT32U a,INT32U b)
//  输入参数    用户原点AD数值
//  输出参数    无
/***************************************************************************************/
BOOLEAN  Set_Originalx0y0(INT32U a,INT32U b)
    { 
   	 if((a>X0_MIN) & (a<X0_MAX) & (b>Y0_MIN) & (b<Y0_MAX)) {
   	    x0 = a;
   	    y0 = b;
		return(0); 
     } else
	    return(1);
	}

BOOLEAN  Set_XmidYmid(INT32U a,INT32U b)
   	 { 
   	  //if ((a>XMID_MIN) & (a<X0_DEFAULT_MAX) & (b>Y0_DEFAULT_MIN) & (b<Y0_DEFAULT_MAX))
   	    xmid = a;
   	    ymid = b;
		return(0); 
     }
/***************************************************************************************/
//  函数名      Set_XmaxYmax(INT32U c,INT32U d)
//  输入参数   用户原点对角点的AD数值
//  输出参数   无
/***************************************************************************************/
BOOLEAN  Set_XmaxYmax(INT32U c,INT32U d)
   	 { 
   	  if((c>XMAX_MIN) & (c<XMAX_MAX) & (d>YMAX_MIN) & (d<YMAX_MAX)) {
   	      xmax = c;
   	      ymax = d;
		  return(0); 
      } else
	      return(1);
     }
      
/***************************************************************************************/
//  函数名     Get_PositionXY(void)
//  输入参数   无
//  输出参数   获取用户屏幕坐标点；高16位：X;低16位：Y 
/***************************************************************************************/
INT32U  Get_PositionXY(void)
	{
	 BOOLEAN x;
	 INT32U  y,a,b,xx,yy;
    
	 if(Get_PositionAD(&y))
	   {
	     if(MACHINE_SCREENCAL_MODE == machine_mode)
		     return(y);
		 else {
		 xx = (y>>16)&0x0000ffff;
		 yy =  y&0x0000ffff;
		 //////////////////////////////////////////////
		 if(xx>xmid) {
		     if(xx>xmax)
		         a = SCR_XSIZE;
		     else
		         a = SCR_XSIZE/2 + (xx-xmid)*(SCR_XSIZE/2)/(xmax-xmid);
		 }else {
		     if(xx<x0)
		         a = 0;
		     else
		         a = (xx-x0)*(SCR_XSIZE/2)/(xmid-x0);
         }
         ////////////////////////////////////////////////  
         if(yy>ymid) {
		     if(yy>ymax)
		         b = SCR_YSIZE;
		     else
		         b = SCR_YSIZE/2 + (yy-ymid)*(SCR_YSIZE/2)/(ymax-ymid);
		 }else {
		     if(yy<y0)
		         b = 0;
		     else
		         b = (yy-y0)*(SCR_YSIZE/2)/(ymid-y0);
	  	} 
	  	 /////////////////////////////////////////////////  
	  	 return (a<<16 | b);
	   }	   
     } 
	 else
	  return(0);
  }

///////////////////////////////////////END OF THE FILE
