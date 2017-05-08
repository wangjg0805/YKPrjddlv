#include "global.h"
#include "config.h"
#include "ads8325.h"

uint8 _ads8325_sampled;


void ads8325_drop(void)
{
	ADS8325_CS_H;
	_ads8325_sampled = 0;
}

void ads8325_sample(void)
 {
  uint32 i;
  
  
  ADS8325_CS_L;
  
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  ADS8325_CLK_L;
  ADS8325_CLK_H;
  
  _ads8325_sampled = 1;
  
 }

uint32  ads8325_get(void)
  {
   uint32 i;
   uint32 data=0;
   
   ADS8325_DATA_MODE_I; 
   //SampleCLK();
   
   ADS8325_CLK_L;
   for(i=0;i<16;i++)
   {
    ADS8325_CLK_L;
    data<<=1;
    ADS8325_CLK_H;
    if(ADS8325_DOUT_TEST)
       data |= 1;
    else
       data |= 0;
   }
   
   ADS8325_CS_H;
   _ads8325_sampled = 0;
   return data;
}

