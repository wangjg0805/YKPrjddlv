//**********************************************************************
//文件名:port.c
//函数列表:
//1.初始化GPIO口:   Init_Port
//2 CPU运行灯取反   LED_CPU_RUN_Cpl
//
//**********************************************************************
#include "port.h"
#include "global.h"
#include "type_def.h"
#include "lpc2400.h"
#include "ADS1100.h"


//**********************************************************************
//函数  名:Init_Port
//函数功能:初始化GPIO口和扩展口的状态
//入口参数:无
//出口参数:无
//**********************************************************************
void Init_Port(void)
{
    SCS |= 0x01;                //io01 is configed as fast mode
    //配置P0   PIN0.01 2  3   456789 10 11 12 13 14 15 
    //                 RX TX                           
    PINSEL0 &= 0xf0003f0f;      //P0.0-P0.15
    PINSEL0 |= 0x0f000050;  
    //配置P0   PIN0.161718 19   202122  232425    26  2728  293031 
    //                nc        x9313       beep      I2C             
    PINSEL1 &= 0xfc3300ff;      //P0.0-P0.15
    PINSEL1 |= 0x01404000;
       
    //FIO0DIR |= 0x00000180;
    FIO0DIR &= 0xfffffcff;
    FIO0DIR |= 0x02000c80;
    //配置P1   PIN1.0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    //         P1.01489 connect GP21    
    //         P1.3     PWM0[2] OUTPUT 1.35KHZ DRIVE SIGNAL                  
    //         P1.7     PWM0[5] TFT BACKLIGHT CONTROL SIGNAL here always 1
    PINSEL2 &= 0xffff330f;      //P1.0-P1.15
    PINSEL2 |= 0x0000CCC0;  
    //SPEAK_PWM_DRI
    //配置P2   PIN2.0      1     23456789 10 11  12   13 14 15 
    //             run                          eint2             
    //             out                           in
    PINSEL4 &= 0xfcfffffc;      //p2.0 p2.1 p2.12
    PINSEL4 |= 0x01000000;      //
    FIO2DIR |= EO2_LED_CPU_S;
    //配置P3   PIN3.0123456789101112131415
    //PINSEL6 = 0x00000000;      //P3.0-P3.15
    
    //配置P3   PIN3.16         17     18       1920212223    24     25262728293031 
    //                       P3_WP  drive_sig             speaker
    //                        OUT     OUT                  pwm1[1]
    PINSEL7 &= 0xfffcffc0;      //P3.18
    PINSEL7 |= 0x00030001;   
    
    FIO3DIR |= (L_DRV_OUT|P3_WP);
    //配置P4   
    //P4 NOT CONFIGED
    FIO0CLR = SPEAKER_PIN;
 }
 
//**********************************************************************
//函数  名:LED_CPU_RUN_Cpl
//函数功能:cpu 运行指示灯取反
//入口参数:无
//出口参数:无
//**********************************************************************  
void  LED_CPU_RUN_Cpl(void)
 {
   if(FIO2PIN & EO2_LED_CPU_S)
     FIO2CLR = EO2_LED_CPU_S;
     else
     FIO2SET = EO2_LED_CPU_S;

 }
 
 void  CD4052_A_Cpl(void)
 {
  
     if(FIO0PIN & CD4052_A)
        FIO0CLR = CD4052_A;
     else
        FIO0SET = CD4052_A;
 }

//**********************************************************************
//**********************************************************************  
void  Speaker_OFF(void)
 {
  FIO0CLR = SPEAKER_PIN;
 }
void  Speaker_ON(void)
 {
  FIO0SET = SPEAKER_PIN;
 } 
void  Speaker_CPL(void)
 {
   if(FIO0PIN & SPEAKER_PIN)
     FIO0CLR = SPEAKER_PIN;
     else
     FIO0SET = SPEAKER_PIN;
 }
 