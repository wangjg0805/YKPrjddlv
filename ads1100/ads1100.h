#ifndef  _ADS1100_H
#define  _ADS1100_H

#include "type_def.h"
#include "global.h"

/**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/
#define ADS1100_CS                  (1<<17) //P0[1]
#define ADS1100_CLK                 (1<<1) //P0[1]
#define ADS1100_DOUT                (1<<0) //P0[0]

#define ADS1100_CLK_L               FIO0CLR=ADS1100_CLK
#define ADS1100_CLK_H               FIO0SET=ADS1100_CLK
#define ADS1100_DOUT_L              FIO0CLR=ADS1100_DOUT
#define ADS1100_DOUT_H              FIO0SET=ADS1100_DOUT

#define ADS1100_DOUT_TEST           (FIO0PIN & ADS1100_DOUT)

#define ADS1100_DATA_MODE_O         FIO0DIR |= ADS1100_DOUT
#define ADS1100_DATA_MODE_I         FIO0DIR &= 0xfffffffe

///////////////////////////////////////以下配置选择驱动电源 和 反馈电阻
//驱动电源选择
#define CD4052_POW_PIN               (1<<7) //P0[7]
#define CD4052_A                    (1<<15) //P0[15]
#define CD4052_B                    (1<<16) //P0[16]

#define CD4052_POW_EN                 FIO0CLR=CD4052_POW_PIN
#define CD4052_POW_DIS                FIO0SET=CD4052_POW_PIN


#define CD4052_A_SET                  FIO0SET=CD4052_A
#define CD4052_A_CLR                  FIO0CLR=CD4052_A
#define CD4052_B_SET                  FIO0SET=CD4052_B
#define CD4052_B_CLR                  FIO0CLR=CD4052_B

//#define CD4052_a(v)            do { (v) ? CD4052_A_SET : CD4052_A_CLR; } while(0)
//#define CD4052_b(v)            do { (v) ? CD4052_B_SET : CD4052_B_CLR; } while(0)


#define POWER_FRE_NULL                   0
#define POWER_FRE_4K                     1
#define POWER_FRE_2K                     2
#define POWER_FRE_1K                     3
#define POWER_FRE_500                    4
#define POWER_FRE_200                    5

#define CD4052_POWER_SEL_1        CD4052_POW_EN;FIO0SET=CD4052_B;power_fre = POWER_FRE_4K
#define CD4052_POWER_SEL_2        CD4052_POW_EN;FIO0CLR=CD4052_B;power_fre = POWER_FRE_2K
#define CD4052_POWER_SEL_3        CD4052_POW_EN;FIO0CLR=CD4052_B;power_fre = POWER_FRE_1K
#define CD4052_POWER_SEL_4        CD4052_POW_EN;FIO0CLR=CD4052_B;power_fre = POWER_FRE_500
#define CD4052_POWER_SEL_5        CD4052_POW_EN;FIO0SET=CD4052_B;power_fre = POWER_FRE_200
#define CD4052_POWER_SEL_6        CD4052_POW_EN;FIO0CLR=CD4052_B;power_fre = POWER_FRE_200


//反馈电阻选择
#define CD4053_EN                    (1<<5)  //P0[5]
#define CD4053_A                    (1<<20)  //P0[20]
#define CD4053_B                    (1<<19)  //P0[19]
#define CD4053_C                    (1<<18)  //P0[18]

#define CD4053_EN_CLR           FIO0CLR=CD4053_EN
#define CD4053_EN_SET           FIO0SET=CD4053_EN

#define CD4053_A_CLR           FIO0CLR=CD4053_A
#define CD4053_A_SET           FIO0SET=CD4053_A
#define CD4053_B_CLR           FIO0CLR=CD4053_B
#define CD4053_B_SET           FIO0SET=CD4053_B
#define CD4053_C_CLR           FIO0CLR=CD4053_C
#define CD4053_C_SET           FIO0SET=CD4053_C

/*
#define CD4053_mode(v)      do { \
	if(v >= 0) { \
		(v & 0x01) ? CD4053_A_SET : CD4053_A_CLR; \
		(v & 0x02) ? CD4053_B_SET : CD4053_B_CLR; \
		(v & 0x04) ? CD4053_C_SET : CD4053_C_CLR; \
		CD4053_EN_CLR; \
	} else { \
		CD4053_EN_SET; \
	} \
} while(0)
*/

#define CD4053_SEL_RES_1        FIO0CLR=CD4053_C;FIO0CLR=CD4053_B;FIO0CLR=CD4053_A
#define CD4053_SEL_RES_2        FIO0CLR=CD4053_C;FIO0CLR=CD4053_B;FIO0SET=CD4053_A
#define CD4053_SEL_RES_3        FIO0CLR=CD4053_C;FIO0SET=CD4053_B;FIO0CLR=CD4053_A
#define CD4053_SEL_RES_4        FIO0CLR=CD4053_C;FIO0SET=CD4053_B;FIO0SET=CD4053_A
#define CD4053_SEL_RES_5        FIO0SET=CD4053_C;FIO0CLR=CD4053_B;FIO0CLR=CD4053_A
#define CD4053_SEL_RES_6        FIO0SET=CD4053_C;FIO0CLR=CD4053_B;FIO0SET=CD4053_A
#define CD4053_SEL_RES_7        FIO0SET=CD4053_C;FIO0SET=CD4053_B;FIO0CLR=CD4053_A
#define CD4053_SEL_RES_8        FIO0SET=CD4053_C;FIO0SET=CD4053_B;FIO0SET=CD4053_A
#define CD4053_SEL_RES_9        FIO0CLR=CD4053_C;FIO0SET=CD4053_B;FIO0CLR=CD4053_A

#define CD4053_SEL_RES_200R     CD4053_SEL_RES_5       
#define CD4053_SEL_RES_2K       CD4053_SEL_RES_4  
#define CD4053_SEL_RES_200K     CD4053_SEL_RES_3  
#define CD4053_SEL_RES_2M       CD4053_SEL_RES_9  

#define MATCH_ITEM_A            CD4052_POWER_SEL_1;CD4053_SEL_RES_200R       
#define MATCH_ITEM_B            CD4052_POWER_SEL_2;CD4053_SEL_RES_200R 
#define MATCH_ITEM_C            CD4052_POWER_SEL_3;CD4053_SEL_RES_2K 
#define MATCH_ITEM_D            CD4052_POWER_SEL_4;CD4053_SEL_RES_200K  //切换档位 变化
#define MATCH_ITEM_E            CD4052_POWER_SEL_5;CD4053_SEL_RES_2M
#define MATCH_ITEM_F            CD4052_POWER_SEL_6;CD4053_SEL_RES_2M

#define MATCH_ITEM_1            1     
#define MATCH_ITEM_2            2 
#define MATCH_ITEM_3            3
#define MATCH_ITEM_4            4
#define MATCH_ITEM_5            5
#define MATCH_ITEM_6            6

///////////////////////////////////////////////////
void      ADS1100Pin_Init(void);
void      ADS1100_Init(void);
void      CD4053_mode(uint8 v);
uint32    rdfromIIC(void);
#endif