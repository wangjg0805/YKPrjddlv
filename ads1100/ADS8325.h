#ifndef  _ADS8325_H
#define  _ADS8325_H

#include "type_def.h"
#include "global.h"

/**************************************************************************************************************
*                                           PIN ASSIGNMENT
**************************************************************************************************************
*/

#define ADS8325_CS                  (1<<17) //P0[17]
#define ADS8325_CLK                 (1<<1) //P0[1]
#define ADS8325_DOUT                (1<<0) //P0[0]

#define ADS8325_CS_L               FIO0CLR=ADS8325_CS
#define ADS8325_CS_H               FIO0SET=ADS8325_CS

#define ADS8325_CLK_L               FIO0CLR=ADS8325_CLK
#define ADS8325_CLK_H               FIO0SET=ADS8325_CLK
#define ADS8325_DOUT_L              FIO0CLR=ADS8325_DOUT
#define ADS8325_DOUT_H              FIO0SET=ADS8325_DOUT

#define ADS8325_DOUT_TEST           (FIO0PIN & ADS8325_DOUT)

#define ADS8325_DATA_MODE_O         FIO0DIR |= ADS8325_DOUT
#define ADS8325_DATA_MODE_I         FIO0DIR &= 0xfffffffe

extern uint8  _ads8325_sampled;

///////////////////////////////////////////////////
void      ADS8325_Init(void);

void ads8325_sample(void);
uint32 ads8325_get(void);
void ads8325_drop(void);

#endif