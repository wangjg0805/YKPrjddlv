#ifndef _MEASURE_H_
#define _MEASURE_H_

#include <stdint.h>
#include "type_def.h"

void measure_init(void);

void rmap_var_init(void);

uint32 measure(void);
float filter(uint32 v);
void reset_filter(void);
float calibrate(float v);
float adjust(float v);

uint8 autorange(float val);
uint8 autorange2(float val);

float kvalue(void);
void kstart(void);
void kcalibrate(float stdv, float readv);

#endif
