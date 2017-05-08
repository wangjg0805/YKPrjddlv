//**********************************************************************
//ÎÄ¼þÃû:data_pro.h
//**********************************************************************
#ifndef _DATA_PRO_H
#define _DATA_PRO_H

#include "type_def.h"
sint32 Cal_Paw(uint16 ad);
sint32 Cal_Batt_V(uint16 ad);
sint32 Cal_Batt_I(uint16 ad);
sint32 Cal_Vref_Test(uint16 ad);
sint32 Cal_O2_Sen_Vol(uint16 ad);
sint32 Cal_O2_Thickness(uint16 ad,uint8* o2_units);
sint32 Cal_Vt(sint32 lap,uint8 *vt_units);
sint32 Cal_Vol(uint16 ad);
void Update_Paw_Curve(uint16 ad,sint8 point);
sint32 Check_Pipeline_P_AD(uint8 point,uint16 ad);
 uint8 Check_EEP_Pipeline_P(void);
sint32 Check_O2_AD(uint8 point,uint16 ad);
sint32 Check_Vt_Laps(uint8 point,sint32 lps);
 uint8 Check_EEP_Vte(void);
void Update_O2_Sen_Curve(uint16 ad,sint8 point);
void Update_Vt_Cur(sint32 laps,sint8 point);
void Read_Vt_Dem_EEP(void);
void Read_Paw_Dem_EEP(void);
void Read_O2_Dem_EEP(void);
 uint8 Check_O2_Curve(void);
uint8 Check_Curve_Data_Valid(sint16 *cdp, sint16 *ydp, sint16 len, sint16 min, sint16 max, sint16 reldt);
uint8 Check_Sen_Curve_AD(uint16 *ad_buf,uint16 len,const uint16 *ad_min_buf,const uint16 *ad_max_buf);
uint8 Check_Sen_Curve_K(uint16 len,sint32 *k_buf,sint32* k_min_buf,sint32 *k_max_buf);
#endif