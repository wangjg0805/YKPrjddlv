//**********************************************************************
//文件名:speaker.c
//修改历史：
//**********************************************************************
#include "config.h"
#include "speaker.h"
#include "global.h"

//定义音量调节电组匹配表
const uint8 VOLUME_TABLE[16]={1,2,3,4,5,6,7,8,9,10,11,13,16,20,25,31};


static volatile sint16  time_key_sound;      //按键音定时器
static           uint8  key_sound_type   = SOUND_TYPE_NONE;    //按键音的类型

static volatile sint16  time_alarm_sound;    //报警音计时器
static           uint8  alarm_sound_type = SOUND_TYPE_NONE;    //报警音的类型

/****************************************************************************
* 函数名: X9313管脚 初始化
****************************************************************************/
void X9313_Pin_Init(void)
{
    //配置P0   PIN0.161718 19202122    23242526  2728  293031 
    //                            //P0.23 24 25                  
    PINSEL1 &= 0xfff03fff;      
    PINSEL1 |= 0x00000000;
       
    //全部为输出
    //FIO0DIR &= 0xff8fffff;
    FIO0DIR |= (SPK_X9313_CS+SPK_X9313_INC+SPK_X9313_UD);
}

/****************************************************************************
* 名称：Set_Config_Bri(uint16 Brightness)
* 功能：配置用户设置的亮度
* 入口参数：亮度等级（分8级别）
* 出口参数
****************************************************************************/
void Set_LCD_Brightness(uint8 brightness)
{
	//PWM0MR0 = Fpclk/1000 ; //亮度调整区间:duty from 5%-40%
    if((brightness>0) && (brightness<=BRIGHT_CLASS_TOTAL))     
      PWM0MR5 = (PWM0MR0 * brightness)/20;  //20等分
    else
      PWM0MR5 =  PWM0MR0/2;
    
    //PWM0LER = 0x21;            // PWMMR0, PWMMR5锁存
    PWM0LER = 0x2d;
	}
/*********************************************************************************************************
* 名称: Set_LCD_Brightness
* 功能: 设置LCD背光亮度(PWM方式调节)
* 入口参数: 亮度(0~15)
* 出口参数: 无
*********************************************************************************************************/
void  Set_Default_Bri(void)   //占空比大于50%亮度调整已经无意义
 	{                         //此款液晶屏PWM允许范围是200HZ-30kHZ,此处取1KHZ
 	                          //只能在低区调整
 	PWM0MR5 = PWM0MR0/2 ;     // 设置默认PWM占空比50%
 	PWM0MR2 = PWM0MR0*8/10;   // 设置无线驱动信号1占空比
 	PWM0MR3 = PWM0MR0*8/10;   // 设置无线驱动信号2占空比
    //PWM0LER = 0x21;            //PWM0MR5
    PWM0LER = 0x2d; 
	}
/****************************************************************************
* 名称：Init_BRIGHT_PWM()
* 功能：初始化声音PWM
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Init_Brightness_PWM(void)
{
                                 // 允许PWM0[5]输出, 单边PWM
    PCONP   |=     1 << 5 ;
    PWM0TCR = 0x02;              //reset
    PWM0PR = 0x00;               // 不分频, 计数频率为Fpclk
    PWM0MCR = 0x02;              // 设置PWM0MR5匹配时复位PWMTC
    
    PWM0PCR = 0;
    PWM0MR0 = Fpclk/200 ;        // 设置PWM频率200HZ
    Set_Default_Bri();           // 黑屏
  
    PWM0PCR = 0x2C00;            // 允许PWM5 PWM2PWM3输出, 单边PWM
    PWM0TCR = 0x09;            // 启动定时器，PWM使能
}

/****************************************************************************
* 名称：Init_BRIGHT_PWM()
* 功能：初始化声音PWM
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Init_PWM0_DRIVER(void)
{
    PCONP   |=     1 << 5 ;
   	PWM0TCR = 0x02;			                                            /* PWM1的计数器复位             */
	PWM0PR 	= 0x00;			                                            /* 不分频,PWM1计数器的          */
	                                                                    /* 计数频率即Fpclk              */
	PWM0MCR = 0x02;			                                            /* 计数器里的值和匹配寄存器里   */
	                                                                    /* 的匹配值相等时复位           */
		                                            /* 设置PWM1输出为单边沿PWM      */
    PWM0MR0 = Fpclk / 1350;		/* 1.3500000kHZ                        */
    PWM0MR1 = PWM0MR0 / 2;	    /* 设置PWM0[1]输出为50%占空比      */
	PWM0MR2 = PWM0MR0-0x33b0;   //pwm0[2] 占空比是可以调节的  	                                            /* 设置PWM1输出为单边沿PWM      */
    
    //PWM0MR0 = Fpclk / 1349;												/* 500HZ                        */
 	//PWM0MR1 = PWM0MR0 / 2;	    /* 设置PWM0[1]输出为50%占空比      */
	//PWM0MR2 = 0x33B0;////////////////////////////(PWM0MR0 / 100)*99;      
	      
	
	PWM0LER = 0x07;			                                            /* PWM0匹配MR0 MR1 MR2  */
	PWM0PCR = 0x0600; 		                                            /* PWM0输出使能                 */
   	PWM0TCR = 0x09;			                                            /* PWM和计数器都使能            */	
   	
}
///////////////////////////////以下为声音PWM函数相关

/****************************************************************************
* 名称：Start_Sound_PWM()
* 功能：启动声音PWM
* 入口参数：frq 声音频率(Hz)
* 出口参数：无
****************************************************************************/
void Start_Sound_PWM(uint16 frq)
{    //PWM1[4]
    
    PWM1MR0 = Fpclk / frq;    // 设置PWM周期(声音频率)
    PWM1MR4 = PWM1MR0 / 2;    // 设置PWM占空比50%
    PWM1LER = 0x11;            // PWMMR0, PWMMR2锁存   
}
/****************************************************************************
* 名称：Stop_Sound_PWM()
* 功能：停止声音PWM
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Stop_Sound_PWM(void)
{
    
    PWM1MR0 = Fpclk / 65536;
    PWM1MR4 = 0;        // 使PWM1一直输出低电平
    PWM1LER = 0x11;    // PWMMR0, PWMMR1锁存  
}
/****************************************************************************
* 名称：Init_Sound_PWM()
* 功能：初始化声音PWM
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Init_Sound_PWM(void)
{    
    //use p3[27]
    //配置P3[27]
    PINSEL7 &= 0xff3fffff;      
    PINSEL7 |= 0x00c00000;
       
    //使用PWM1[1]
    PCONP   |=     1 << 6 ;
    
    PWM1TCR = 0x02;          //reset
    PWM1PR  = 0x00;          // 不分频, 计数频率为Fpclk
    PWM1MCR = 0x02;          // 设置PWM1MR0匹配时复位PWMTC
    
    PWM1PCR = 0;
    Stop_Sound_PWM();        // 停止声音PWM
    PWM1PCR = 0x1000;         // 允许PWM4输出, 单边PWM
    PWM1TCR = 0x09;          // 启动定时器，PWM使能
    
    //X9313_Pin_Init();
 
}
/****************************************************************************
* 函数名: Key_Valid_Sound()
* 功能  : 按键有效时发出声音的处理
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Key_Valid_Sound(void)
{
 
    if (OFF == sound_switch)    return;       //按键音关
    time_key_sound = 10;    //总共200ms
    key_sound_type = SOUND_TYPE_KEY_VALID;        //设置按键声音类型

}

/****************************************************************************
* 函数名: Key_No_Valid_Sound()
* 功能  : 按键无效时发出声音的处理
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Key_No_Valid_Sound(void)
{
  
    if (OFF == sound_switch)    return;      //按键音关
    time_key_sound = 40;    //总共400ms
    key_sound_type = SOUND_TYPE_KEY_NO_VALID;    //设置按键声音类型
  
}

/****************************************************************************
* 函数名: Key_Sound_Pro()
* 功能  : 键声音的处理(放入定时器中断中,每10ms处理一下)
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Key_Sound_Pro(void)
{
     switch(key_sound_type)
        {
         case SOUND_TYPE_KEY_VALID:    //按键有效时发出声音
              Speaker_ON();
              break;
         case SOUND_TYPE_KEY_NO_VALID: //按键无效时发出声音的处理
         
              Speaker_ON();
              break; 
         case SOUND_TYPE_SER_ON:    //维修模式下一直发出声音
              Speaker_ON();
              break;    
         default:   //无键声音则退出
            return;
        }
        
     if(time_key_sound == 0)
       {  
        Speaker_OFF();                     //停止发声
        key_sound_type = SOUND_TYPE_NONE;  //按键发声结束
       }
    else
       {
        time_key_sound--;        //每10ms减1.
       }   
}

///////////////
void Key_Sound_Pro_TP(void)
{
     switch(key_sound_type)
        {
         case SOUND_TYPE_KEY_VALID:    //按键有效时发出声音
            if(time_key_sound > 10) Start_Sound_PWM(300);    //发420Hz声音"3"
            else Start_Sound_PWM(200);                        //发333Hz声音"1"
            break;
         case SOUND_TYPE_KEY_NO_VALID: //按键无效时发出声音的处理
            if(time_key_sound > 27) Start_Sound_PWM(500);    //发500Hz声音"5"
            else if(time_key_sound > 13) Stop_Sound_PWM();    //停止发声
            else Start_Sound_PWM(500);
            break; 
         case SOUND_TYPE_SER_ON:    //维修模式下一直发出声音
            time_key_sound = 2;
            Start_Sound_PWM(333);
            break;    
         default://无键声音则退出
            return;
        }
        
     if(time_key_sound == 0)
       {
        Stop_Sound_PWM();                  //停止发声
        key_sound_type = SOUND_TYPE_NONE;  //按键发声结束
       }
    else
       {
        time_key_sound--;        //每10ms减1.
       }   
}

/****************************************************************************
* 名称：Alarm_Sound_Pro()
* 功能：报警的发声音处理(放入定时器中断中,每10ms处理一下)
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Alarm_Sound_Pro(void)
{
    if(SOUND_TYPE_NONE != key_sound_type) return;        //有按键声则退出不处理报警声
   
     switch(alarm_sound_type)
     {
        case SOUND_TYPE_WARNING://发报警类声音(10声,周期10秒,3长2短,频率666Hz)
            if(time_alarm_sound < 16) Start_Sound_PWM(666);            //发160ms
            else if(time_alarm_sound < 24) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 40) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 48) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 64) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 88) Stop_Sound_PWM();        //停240ms
            else if(time_alarm_sound < 104) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 112) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 128) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 208) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 224) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 232) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 248) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 256) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 272) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 296) Stop_Sound_PWM();        //停240ms
            else if(time_alarm_sound < 312) Start_Sound_PWM(666);    //发160ms
            else if(time_alarm_sound < 320) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 336) Start_Sound_PWM(666);    //发160ms
            else Stop_Sound_PWM();
            if(++time_alarm_sound >= 1000) time_alarm_sound = 0;    //10秒循环
            break;
        case SOUND_TYPE_TIPS://发提示类报警声(10声,周期10秒,3长2短,频率333Hz)
            if(time_alarm_sound < 16) Start_Sound_PWM(333);            //发160ms
            else if(time_alarm_sound < 24) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 40) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 48) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 64) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 88) Stop_Sound_PWM();        //停240ms
            else if(time_alarm_sound < 104) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 112) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 128) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 208) Stop_Sound_PWM();        //停800ms
            else if(time_alarm_sound < 224) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 232) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 248) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 256) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 272) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 296) Stop_Sound_PWM();        //停240ms
            else if(time_alarm_sound < 312) Start_Sound_PWM(333);    //发160ms
            else if(time_alarm_sound < 320) Stop_Sound_PWM();        //停80ms
            else if(time_alarm_sound < 336) Start_Sound_PWM(333);    //发160ms
            else Stop_Sound_PWM();
            if(++time_alarm_sound >= 1000) time_alarm_sound = 0;    //10秒循环
            break;
      
        default://无报警声音
            Stop_Sound_PWM();        //停止发声
            break;
    }
}

//**********************************************************************
//函数  名:Get_Key_Sound_Type
//函数功能:获取当前的按键音类型设置
//入口参数:无
//出口参数:按键音类型
//**********************************************************************
uint8 Get_Key_Sound_Type(void)
{
    return (key_sound_type);
}

//**********************************************************************
//函数  名:Set_Key_Sound_Type
//函数功能:设置按键音类型
//入口参数:s_type--声音类型
//出口参数:无
//**********************************************************************
void Set_Key_Sound_Type(uint8 s_type)
{
    key_sound_type = s_type;
}

//**********************************************************************
//函数  名:Set_Key_Sound_Time
//函数功能:设置按键音的时间
//入口参数：t--时间
//出口参数:无
//**********************************************************************
void Set_Key_Sound_Time(sint16 t)
{
    time_key_sound = t;
}

//**********************************************************************
//函数  名:Get_Alm_Sound_Type
//函数功能:获取当前的报警音类型设置
//入口参数:无
//出口参数:报警音类型
//**********************************************************************
uint8 Get_Alm_Sound_Type(void)
{
    return(alarm_sound_type);
}

//**********************************************************************
//函数  名:Set_Alm_Sound_Type
//函数功能:设置报警音类型
//入口参数:s_type--声音类型
//出口参数:无
//**********************************************************************
void Set_Alm_Sound_Type(uint8 s_type)
{
    alarm_sound_type = s_type;
    time_alarm_sound = 0;       //重新开始
}

/////////////////////////////////////////////////////////////////
//以下函数式对X9313的操作
//用于调整音量

/****************************************************************************
* 函数名: Inc_Speaker_Volume
* 功能: 增加喇叭音量
* 入口参数: 音量级数
* 出口参数: 无
****************************************************************************/
void Inc_Speaker_Volume(uint8 step)
{
	Set_Ext_OutPort1(SPK_X9313_UD);		//音量增加
	Clr_Ext_OutPort1(SPK_X9313_CS);
	while(step--)
	{
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
	}
	Set_Ext_OutPort1(SPK_X9313_CS);
}

/****************************************************************************
* 函数名: Dec_Speaker_Volume
* 功能: 减小喇叭音量
* 入口参数: 音量级数
* 出口参数: 无
****************************************************************************/
void Dec_Speaker_Volume(uint8 step)
{
	Clr_Ext_OutPort1(SPK_X9313_UD);		//音量减小
	Clr_Ext_OutPort1(SPK_X9313_CS);
	while(step--)
	{
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Clr_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
		Set_Ext_OutPort1(SPK_X9313_INC);
	}
	Set_Ext_OutPort1(SPK_X9313_CS);
}

/****************************************************************************
* 函数名: Clr_Speaker_Volume
* 功能: 清除喇叭音量
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void Clr_Speaker_Volume(void)
{
	Dec_Speaker_Volume(32);		//将9313抽头移到0
}

/****************************************************************************
* 函数名: Set_Speaker_Volume
* 功能: 设置报警音量
* 入口参数: 音量(1~16级)
* 出口参数: 无
****************************************************************************/
void Set_Speaker_Volume(uint8 volume)
{
	/*//先将9313抽头移到0
	Clr_Speaker_Volume();
	//再移到需要的位置
	if(volume<1) 
	   volume = 1;
	else if(volume>SPEAK_CLASS_TOTAL) 
	        volume = SPEAK_CLASS_TOTAL;
	Inc_Speaker_Volume(VOLUME_TABLE[volume-1]);*/
}

/****************************************************************************
* 函数名: Calc_Sound_Change_Step
* 功能: 根据当前音量计算渐变步长(共9级)
* 入口参数: 当前音量和渐变步长数组
* 出口参数: 无
****************************************************************************/
void Calc_Sound_Change_Step(uint8 volume, uint8 *scsarray)
 {
 /*
  uint8 res,soundstep[10];
  uint32 i;
	if(volume<1)
	   volume = 1;
	else if(volume>SPEAK_CLASS_TOTAL) 
	        volume = SPEAK_CLASS_TOTAL;
	res = VOLUME_TABLE[volume-1];
	for(i=0; i<10; i++)
	{
		soundstep[i] = (res * i + 4) / 9;
		if(i>0)
		scsarray[i-1] = soundstep[i] - soundstep[i-1];
	}
 */	
 }
