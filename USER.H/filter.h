#ifndef _MA_H_
#define _MA_H_

#include "type_def.h"


#define MA_COUNT_MAX    256
#define FILTER_STAGE    24


typedef struct tag_ma_t
{
	float m_val[MA_COUNT_MAX];
	float m_sum;
	int m_count;
	int m_ptr;
} ma_t;



void ma_init(ma_t *ma, int count);

void ma_reset(ma_t *ma);
float ma_push(ma_t *ma, float v);

void filter_init_lpc2478ad(void);
float filter_ad_NTC(uint16 ntc_data);

#endif
