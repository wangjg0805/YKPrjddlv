#include "filter.h"
#include "global.h"


const  float ntc_table[]=
{ //-5     0     5      10     15
 42.326,32.650,25.391,19.898, 15.71,
 //20     25    30      35     40
 12.491,10.000,8.057, 6.531,  5.327,
 //45     50    55      60     65
 4.369, 3.603, 2.9866,2.4822, 2.08284,
 //70     75    80      85     90
 1.751, 1.479, 1.255, 1.069,  0.915,
 //95    100    105
 0.786, 0.678, 0.514, 0.0001
};
const  uint8 ntc_start[]=
{0,//无效数据
 0 ,5, 10,15,20,25,30,35,
 40,45,50,55,60,65,70,75,
 80,85,90,95,100,105
};

#define FILTER_LENGTH 4
static uint16 ntc_buf[FILTER_LENGTH];
static uint8 index1 = 0;

/////////////////////////////////////////////////////////////////////
void filter_init_lpc2478ad(void)
 {
  uint8 i;
  for(i=0;i<FILTER_LENGTH;i++)
    {
     ntc_buf[i] = 0;
    }
  
 }

//********************************************************
void  filter_ad_NTC(uint16 ntc_data)
       { 
           static uint32 Cnt = 0;
           uint32  sum;
           float   x,y;
           uint8   i;
          
           Cnt++;
           
           ntc_buf[index1++] = ntc_data;      
           if(FILTER_LENGTH == index1)
               index1 = 0;
        
           ////////////////////////////////////////
           sum = 0;
           if(Cnt > FILTER_LENGTH) {
               for(i=0;i<FILTER_LENGTH;i++)
	              sum += ntc_buf[i];
	       } else {
	           for(i=0;i<index1;i++)
	              sum += ntc_buf[i];
           }
	        
	       sum = sum / FILTER_LENGTH;
	          
	       x = (10*sum)/(1023.001-sum); 
	       ////////////////////////////////根据此电阻值计算出温度
	       if(Cnt > FILTER_LENGTH) {
	       
	           if((x>=ntc_table[0])||(x<=ntc_table[22])) //温度太小
	               temp_float_ptc = TEMP_SENSOR_NC_VALUE;
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
	          
	          if(temp_float_ptc > x)
	              y = temp_float_ptc - x;
	          else 
	              y = x - temp_float_ptc;
	              
	          if(y>0.5)
	              temp_float_ptc = x; 
	          else if(y>0.2)
	              temp_float_ptc = temp_float_ptc*0.7+x*0.3;
	          
	          }
	 }
}











 static uint32 buf_full_flag = 0;

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
	}
	
	buf_full_flag = 0;
}

float ma_push(ma_t *ma, float v)
{
   
    
	ma->m_sum -= ma->m_val[ma->m_ptr];
	ma->m_sum += v;

	ma->m_val[ma->m_ptr] = v;
	if(++ma->m_ptr >= ma->m_count) {
	    buf_full_flag = 1;
		ma->m_ptr = 0;
    }
	
	if(1 == buf_full_flag)
 	    return ma->m_sum / ma->m_count;
    else
        return ma->m_sum / ma->m_ptr;
}
