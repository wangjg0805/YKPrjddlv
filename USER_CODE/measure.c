#include "global.h"
#include "measure.h"
//#include "uart.h"
#include "timer.h"

#include "ADS1100.h"
#include "ads8325.h"

#include "filter.h"


typedef struct tag_rmap_t {
	int m_ain; // read from ADC in specified range_config_t, negtive for end of array
	float m_value; // actual resistor value
} rmap_t;

const uint32 Rmap_AD[6][8] = {
{14699,  10578,   7476,    6227,   3023,   1520,  1532 ,100},
{16327,  13475,  10040,    8010,   3196,   1594,  1326 ,100},
{14972,  12456,   9331,    7458,   3724,   1460,  1210 ,100},

{14510,   9090,   7280,    3650,   2100,   1480,  1250 ,100},
{14000,   7050,   3500,    2450,   1800,   1600,  3030 ,100},
{14650,   7400,   3700,    1600,   1000,    700,   300 ,100},
};

const float Rmap_float[6][8] = {
{1000000.0f /       5.0f,  1000000.0f /        7.0f, 1000000.0f /       9.0f, 1000000.0f /      20.0f,
 1000000.0f /      30.0f,  1000000.0f /       40.0f, 1000000.0f /       50.0f,       0.0f},
 
{1000000.0f /       50.0f, 1000000.0f /       70.0f, 1000000.0f /       90.0f, 1000000.0f /     200.0f,
 1000000.0f /      300.0f, 1000000.0f /      400.0f, 1000000.0f /      500.0f,       0.0f},
 
{1000000.0f /      500.0f, 1000000.0f /      700.0f, 1000000.0f /      900.0f, 1000000.0f /    2000.0f,
 1000000.0f /     3000.0f, 1000000.0f /     4000.0f, 1000000.0f /     5000.0f,       0.0f},
 
{1000000.0f /     5000.0f, 1000000.0f /     7000.0f, 1000000.0f /    9000.0f, 1000000.0f /   20000.0f,
 1000000.0f /    30000.0f, 1000000.0f /    40000.0f, 1000000.0f /    50000.0f,       0.0f},
 
{1000000.0f /    50000.0f, 1000000.0f /    70000.0f, 1000000.0f /   90000.0f, 1000000.0f /  200000.0f,
 1000000.0f /   300000.0f, 1000000.0f /   400000.0f, 1000000.0f /  500000.0f,       0.0f},
 
{1000000.0f /   500000.0f, 1000000.0f /   700000.0f, 1000000.0f /  900000.0f, 1000000.0f / 2000000.0f,
 1000000.0f /  3000000.0f, 1000000.0f /  4000000.0f, 1000000.0f / 5000000.0f,       0.0f},
 
};

 rmap_t Rmap[6][8];
/*
 rmap_t RMAP_20MS[8];
 rmap_t RMAP_2MS[8];
 rmap_t RMAP_200US[8];
 rmap_t RMAP_20US[8];
 rmap_t RMAP_2US[8];
 */
 
void rmap_var_init(void)
{
    uint8 i,j;
 
    if(sys_cal_ok) {
        for(j=0;j<6;j++) {
            for(i=0;i<7;i++) {
             Rmap[j][i].m_ain   = ddlv_syscal_buf[j][i];
             Rmap[j][i].m_value = Rmap_float[j][i];
            } 
             Rmap[j][7].m_ain = -1;   
             Rmap[j][7].m_value = 0.0f; 
        }
    } else {
        for(j=0;j<6;j++) {
            for(i=0;i<8;i++) {
                Rmap[j][i].m_ain   = 0;
                Rmap[j][i].m_value = 0;
            }    
        }
    }   
 }
 /*
 rmap_t RMAP_200MS[] = { // 5~50
	{ 15000, 1000000.0f / 5.0f },
	{ 10737, 1000000.0f / 7.0f },
	{ 7575, 1000000.0f / 10.0f },
	{ 6090, 1000000.0f / 12.5f },
	{ 3076, 1000000.0f / 25.0f },
	{ 1552, 1000000.0f / 50.0f },
	{ 1294, 1000000.0f / 60.0f },
	{ -1, 0.0f },
};

 rmap_t RMAP_20MS[] = { // 50~500
	{ 16397, 1000000.0f / 50.0f },
	{ 13526, 1000000.0f / 60.0f },
	{ 10079, 1000000.0f / 80.0f },
	{ 8041, 1000000.0f / 100.0f },
	{ 3208, 1000000.0f / 250.0f },
	{ 1601, 1000000.0f / 500.0f },
	{ 1333, 1000000.0f / 600.0f },
	{ -1, 0.0f },
};

 rmap_t RMAP_2MS[] = { // 500~5k
	{ 15000, 1000000.0f / 500.0f },
	{ 12480, 1000000.0f / 600.0f },
	{ 9351, 1000000.0f / 800.0f },
	{ 7479, 1000000.0f / 1000.0f },
	{ 3736, 1000000.0f / 2000.0f },
	{ 1467, 1000000.0f / 5000.0f },
	{ 1213, 1000000.0f / 6000.0f },
	{ -1, 0.0f },
};

 rmap_t RMAP_200US[] = { // 5k~50k
	{ 14536, 1000000.0f / 5000.0f },
	{ 10536, 1000000.0f / 8000.0f },
	{ 7300, 1000000.0f / 10000.0f },
	{ 3657, 1000000.0f / 20000.0f },
	{ 2657, 1000000.0f / 35000.0f },
	{ 1470, 1000000.0f / 50000.0f },
	{ 1226, 1000000.0f / 60000.0f },
	{ -1, 0.0f },
};

 rmap_t RMAP_20US[] = { // 50k~500k
	{ 14130, 1000000.0f / 50000.0f },
	{ 7100, 1000000.0f / 100000.0f },
	{ 3570, 1000000.0f / 200000.0f },
	{ 2400, 1000000.0f / 300000.0f },
	{ 1820, 1000000.0f / 400000.0f },
	{ 1460, 1000000.0f / 500000.0f },
	{ 1250, 1000000.0f / 600000.0f },
	{ -1, 0.0f },
};

 rmap_t RMAP_2US[] = { // 500k+
	{ 14670, 1000000.0f / 500000.0f },
	{ 7370, 1000000.0f / 1000000.0f },
	{ 3710, 1000000.0f / 2000000.0f },
	{ 1515, 1000000.0f / 5000000.0f },
	{ 790, 1000000.0f /10000000.0f }, // 10M
	{ 590, 1000000.0f /20000000.0f }, // 20M
	{ 390, 1000000.0f /50000000.0f }, // 50M
	{ -1, 0.0f },
};
*/

/*
const rmap_t RMAP_200MS[] = { // 5~50
	{ 15000, 1000000.0f / 5.0f },
	{ 12495, 1000000.0f / 6.0f },
	{ 10737, 1000000.0f / 7.0f },
	{ 7575, 1000000.0f / 10.0f },
	{ 6090, 1000000.0f / 12.5f },
	{ 3076, 1000000.0f / 25.0f },
	{ 1552, 1000000.0f / 50.0f },
	{ 1294, 1000000.0f / 60.0f },
	{ -1, 0.0f },
};

const rmap_t RMAP_20MS[] = { // 50~500
	{ 16397, 1000000.0f / 50.0f },
	{ 13526, 1000000.0f / 60.0f },
	{ 10079, 1000000.0f / 80.0f },
	{ 8041, 1000000.0f / 100.0f },
	{ 3208, 1000000.0f / 250.0f },
	{ 1601, 1000000.0f / 500.0f },
	{ 1333, 1000000.0f / 600.0f },
	{ -1, 0.0f },
};

const rmap_t RMAP_2MS[] = { // 500~5k
	{ 15000, 1000000.0f / 500.0f },
	{ 12480, 1000000.0f / 600.0f },
	{ 9351, 1000000.0f / 800.0f },
	{ 7479, 1000000.0f / 1000.0f },
	{ 3736, 1000000.0f / 2000.0f },
	{ 1467, 1000000.0f / 5000.0f },
	{ 1213, 1000000.0f / 6000.0f },
	{ -1, 0.0f },
};

const rmap_t RMAP_200US[] = { // 5k~50k
	{ 14536, 1000000.0f / 5000.0f },
	{ 7300, 1000000.0f / 10000.0f },
	{ 3657, 1000000.0f / 20000.0f },
	{ 1470, 1000000.0f / 50000.0f },
	{ 1226, 1000000.0f / 60000.0f },
	{ -1, 0.0f },
};

const rmap_t RMAP_20US[] = { // 50k~500k
	{ 14130, 1000000.0f / 50000.0f },
	{ 7100, 1000000.0f / 100000.0f },
	{ 3570, 1000000.0f / 200000.0f },
	{ 2400, 1000000.0f / 300000.0f },
	{ 1820, 1000000.0f / 400000.0f },
	{ 1460, 1000000.0f / 500000.0f },
	{ 1250, 1000000.0f / 600000.0f },
	{ -1, 0.0f },
};

const rmap_t RMAP_2US[] = { // 500k+
	{ 14670, 1000000.0f / 500000.0f },
	{ 7370, 1000000.0f / 1000000.0f },
	{ 3710, 1000000.0f / 2000000.0f },
	{ 1515, 1000000.0f / 5000000.0f },
	{ 790, 1000000.0f / 10000000.0f }, // 10M
	{ 590, 1000000.0f /20000000.0f }, // 20M
	{ 390, 1000000.0f /50000000.0f }, // 50M
	{ 290, 1000000.0f /70000000.0f }, // 70M
	{ 200, 1000000.0f /100000000.0f },// 100M
	{ -1, 0.0f },
};
*/

typedef struct tag_range_config_t {
	int m_v; // 4053.CBA
	int m_b; // 4052.B
	int m_period; // timer period, low level should be half of that
	int m_low; // low level time
	int m_highw; // high level time before sampling
	int m_highs; // high level time before each sampling
	int m_nsample; // count of sampling, high level should be split into nsample part
	int m_count1; // count for dummy sampling
	int m_count2; // count for saved sampling
	rmap_t* m_rmap;
	float m_min;
	float m_max;
} range_config_t;

// to check the values, the standard device also have error:
// 1. read water at the standard device
// 2. find a resistor with the same value at the standard device
// 3. read water at our device, and compare the value with the resistor
/*
const range_config_t range_configs[] = {
	// 124/41/73
	{ m_v = 4,  m_b = 1, m_period = 250, m_low = 124, m_highw = 50, m_highs = 73, m_nsample = 1, m_count1 = 160, m_count2 = 256, RMAP_200MS, m_min = 1000000/55.0f, m_max = 1000000/4.5f },
	{ m_v = 4,  m_b = 0, m_period = 500, m_low = 239, m_highw = 74, m_highs = 73, m_nsample = 2, m_count1 = 80, m_count2 = 128, RMAP_20MS, m_min = 1000000/550.0f, m_max = 1000000/50.0f },
	{ m_v = 3,  m_b = 0, m_period = 1000, m_low = 468, m_highw = 157, m_highs = 73, m_nsample = 4, m_count1 = 40, m_count2 = 64, RMAP_2MS, m_min = 1000000/5500.0f, m_max = 1000000/500.0f },
	{ m_v = 2,  m_b = 0, m_period = 2000, m_low = 925, m_highw = 325, m_highs = 73, m_nsample = 8, m_count1 = 20, m_count2 = 32, RMAP_200US, m_min = 1000000/55000.0f, m_max = 1000000/5000.0f },
	{ m_v = -1, m_b = 1, m_period = 5000, m_low = 2301, m_highw = 1114, m_highs = 73, m_nsample = 16, m_count1 = 10, m_count2 = 16, RMAP_20US, m_min = 1000000/550000.0f, m_max = 1000000/50000.0f },
	{ m_v = -1, m_b = 0, m_period = 5000, m_low = 2301, m_highw = 1114, m_highs = 73, m_nsample = 16, m_count1 = 10, m_count2 = 16, RMAP_2US, m_min = 1000000/5500000.0f, m_max = 1000000/500000.0f },
};
*/
const range_config_t range_configs[] = {
	// 124/41/73
	{  4,   1,   250,   124,    50,  73,   1,   160,  256, Rmap[0],     1000000/55.0f,        1000000/4.5f },
	{  4,   0,   500,   239,    74,  73,   2,    80,  128, Rmap[1],     1000000/550.0f,       1000000/50.0f },
	{  3,   0,  1000,   468,   157,  73,   4,    40,   64, Rmap[2],     1000000/5500.0f,      1000000/500.0f },
	{  2,   0,  2000,   925,   325,  73,   8,    20,   32, Rmap[3],     1000000/55000.0f,     1000000/5000.0f },
	{  0,   1,  5000,  2301,  1114,  73,  16,    10,   16, Rmap[4],     1000000/550000.0f,    1000000/50000.0f },
	{  0,   0,  5000,  2301,  1114,  73,  16,    10,   16, Rmap[5],     1000000/5500000.0f,   1000000/500000.0f },
};


static ma_t ma;

void measure_init(void)
{
	ma_init(&ma, FILTER_STAGE);
}

uint32 measure(void)
{
	const range_config_t* config = &range_configs[range];
	int i, j;
	uint32 sum;
	uint32 now;
	
	CD4053_EN_CLR;             //ls4053_enable();
	CD4053_mode(config->m_v); //ls4053_mode(config->m_v);
	
	CD4052_POW_EN;          //ls4052_enable();
	if(1 == config->m_b)
	    CD4052_B_SET;
	else
	    CD4052_B_CLR;    
	//CD4052_b(config->m_b);   //ls4052_b(config->m_b);
	
	now = timer_now();

	sum = 0;
	for(i = config->m_count1; i > 0; i--)
	{
		// low
		CD4052_A_CLR; //CD4052_a(0);

		// read the last sample in the last cycle
		if(_ads8325_sampled)
		sum += ads8325_get();

		now = timer_wait(now + config->m_low);
		// high0
		CD4052_A_SET; //CD4052_a(1);
		now = timer_wait(now + config->m_highw);
		// high for sampling
		for(j = config->m_nsample; j > 0; j--)
		{
			if(_ads8325_sampled)
				sum += ads8325_get();
			now = timer_wait(now + config->m_highs);
			//sum += ads8325_read();
			ads8325_sample();
		};
		//now = timer_wait(now + config->m_highe);
	};
	ads8325_drop();

	sum = 0;
	for(i = config->m_count2; i > 0; i--)
	{
		// low
		CD4052_A_CLR; //CD4052_a(0);

		// read the last sample in the last cycle
		if(_ads8325_sampled)
			sum += ads8325_get();

		now = timer_wait(now + config->m_low);
		// high0
		CD4052_A_SET; //CD4052_a(1);
		now = timer_wait(now + config->m_highw);
		// high for sampling
		for(j = config->m_nsample; j > 0; j--)
		{
			if(_ads8325_sampled)
				sum += ads8325_get();
			now = timer_wait(now + config->m_highs);
			//sum += ads8325_read();
			ads8325_sample();
		};
		//now = timer_wait(now + config->m_highe);
	};

	CD4052_POW_DIS; //ls4052_disable();
	
	sum += ads8325_get();

	return sum;
}

float filter(uint32_t v)
{
	return ma_push(&ma, (float) v);
}

void reset_filter(void)
{
	ma_reset(&ma);
}

float calibrate(float v)
{
	const range_config_t *config = &range_configs[range];
	const rmap_t *rmap = config->m_rmap;
	int i = 0;
	
	while(rmap[i].m_ain >= 0 && rmap[i+1].m_ain >= 0)
	{
		if((i > 0 || rmap[i].m_ain >= v) && (rmap[i+2].m_ain < 0 || v > rmap[i+1].m_ain))
		{
			// find range
			float result = (v - rmap[i+1].m_ain) * (rmap[i].m_value - rmap[i+1].m_value)
												/ (rmap[i].m_ain - rmap[i+1].m_ain) + rmap[i+1].m_value;
			return result < 0.001f ? 0.001f : result;
		};
		
		i++;
	};
	
	return 0.0f;
}

float adjust(float v)
{
    static float lastdata= 0.1;
    static uint32 cnt = 0;
    
    float errf; // error for autorange
    return(v);
 	
 	errf = (v - lastdata) / v;
	if(errf < 0)
		errf = -errf;
	
	if(errf < 0.05) {
	    cnt++;
	    if(cnt > 5)
	        stable_flag = 1;
	    else 
	        stable_flag = 0;
	        
	    if(cnt > 25) {
	        return(lastdata);   
		} else {
		    lastdata = v;
		    return(v);
	    }
	} else {
	    stable_flag = 0;
	    cnt = 0;
	    lastdata = v;
	    return(v);
	}	
}

uint8 autorange2(float val)
{

    uint8 flag;
    flag = 0;	// adjust range

    if(val < Rmap[range][6].m_ain - 50) {
		if(range != RANGE_2US) {
		    range++;
		     flag = 1;
		}    
	}
	else if(val > Rmap[range][0].m_ain + 50)
	{
		if(range != RANGE_200MS) {
		    range--;
		    flag = 1;
		}    
	}
	
	return(flag);
}

/////////////added 
void measure_proc(void)
{
        uint32 val_raw; // raw
		float val_filter; // filtered
		float val_cali; // calibrated
		float val_adjust; // adjusted
		float last_v;

static uint32 range_changed_cnt = 0;
	/////////////////////////////////
	    LED_CPU_RUN_Cpl();
		val_raw = measure();

		val_filter = filter(val_raw) / 1000; ////first filter

		range_changed_cnt++;
		if(automode) {
		    if(range_changed_cnt> 0) { //ÇÐ»»ºóµÈ´ý5´Î
		        if(val_filter < 20) {
		            range++;
		            range_changed_cnt = 0;
		            reset_filter();
		            if(range > RANGE_2US)
                        range = RANGE_200MS;
     	        } else {
			        if(autorange2(val_filter)) {
			            range_changed_cnt = 0;
			            reset_filter();
			        }  
		        }
		        
		        last_v = val_filter;
		        
		    } else {
		    
		        last_v = val_filter;
		    }
		}
		else {
			//int new_range = range_man;
			if(range_man != range) {
				reset_filter();
				range = range_man;
			}
			last_v = val_filter;
		}
	    
		ddlv_raw_data1 = last_v;
		// do calibration
		val_cali = calibrate(last_v);
		//ddlv_raw_data2 = val_cali;
		// do adjust for stable display
		val_adjust = adjust(val_cali);
		ddlv_raw_data2 = val_adjust; 
}