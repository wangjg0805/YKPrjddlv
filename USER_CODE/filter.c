#include "filter.h"
#include "global.h"


const  float ntc_table[]=
{ //-5     0     5      10     15
 41.033,29.884,25.121,19.805,15.693,
 //20     25    30      35     40
 12.496,10.000,8.0413,6.4972,5.2743,
 //45     50    55      60     65
 4.3013,3.4970,2.8993,2.3959,1.9882,
 //70     75    80      85     90
 1.6567,1.3850,1.1641,0.9814,0.8304,
 //95    100    105
 0.7052,0.5840,0.5140,0.0001
};

const  uint8 ntc_start[]=
{0,//无效数据
 0 ,5, 10,15,20,25,30,35,
 40,45,50,55,60,65,70,75,
 80,85,90,95,100,105
};

static uint16 ntc_buf[8];

/////////////////////////////////////////////////////////////////////
void filter_init_lpc2478ad(void)
 {
  uint8 i;
  for(i=0;i<8;i++)
    {
     ntc_buf[i] = 0;
    
    }
  
 }

//********************************************************
float  filter_ad_NTC(uint16 ntc_data)
         { //当前缓存已满，则取除了最大最小值后的平均值
           //如果没有满  ，则取 所有数据的平均值
           uint32  sum;
           float   x;
           uint8   i;
           static  uint8 index = 0;
           ntc_buf[index] = ntc_data;
           index++;
           if(8 == index)
               index = 0;
           ////////////////////////////////////////
	       sum = 0;
	       for(i=0;i<8;i++)
	           sum+=ntc_buf[i];  
	       sum = sum/8;
	       /////////////////////////////////根据此数据倒推出电阻式多大
	      
	       x = (10*sum)/(1024.001-sum); 
	       ////////////////////////////////根据此电阻值计算出温度
	       if(x>=ntc_table[0]) //温度太小
	         {
	          return(TEMP_SENSOR_NC_VALUE);
	         }
	       else if(x<=ntc_table[22]) 
	         {
	          return(TEMP_SENSOR_NC_VALUE);
	         } 
	       else 
	         {
	          i = 1;
	          while(1)
	           {
	            if(x>ntc_table[i])
	               break;
	            else
	             i++;   
	           }
	          //////////////////计算温度 
	          x =ntc_start[i]-(x-ntc_table[i])/(ntc_table[i-1]-ntc_table[i])*5;
	          if(x>2)
	          return(x-2);
	          else
	          return(x); 
	          } 
	 }       














void ma_init(ma_t *ma, int count)
{
	ma->m_count = count;
	
	ma_reset(ma);
}

void ma_reset(ma_t *ma)
{
	int i;

	ma->m_sum = 0.0f;
	ma->m_ptr = 0;
	ma->m_sum = 0;
	for(i = 0; i < ma->m_count; i++)
	{
		ma->m_val[i] = 0.0f;
	};
}

float ma_push(ma_t *ma, float v)
{
	ma->m_sum -= ma->m_val[ma->m_ptr];
	ma->m_sum += v;

	ma->m_val[ma->m_ptr] = v;
	if(++ma->m_ptr >= ma->m_count)
		ma->m_ptr = 0;
	
	return ma->m_sum / ma->m_count;
}
