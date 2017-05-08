//**********************************************************************
//ÎÄ¼þÃû:ad_acquire.h
//**********************************************************************
#ifndef _AD_ACQUIRE_H_
#define _AD_ACQUIRE_H_

#include "type_def.h"

void  AD7190_Channel_Switch(void);

uint32 Get_Raw_From_Int2(uint8 ch);

void Init_EINT2(void);
void FIQ_Exception(void);
#endif