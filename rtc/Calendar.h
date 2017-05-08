#ifndef __CALENDAR_H__
#define __CALENDAR_H__
#include "type_def.h"

#define CALENDAR_START_YEAR    2009
#define CALENDAR_END_YEAR      2100

uint8 IsLeapYear(uint16 year);
void  GetCalendarWeek(uint16 year,uint8 mon,
					 uint8 day,uint8 *week);
uint8  GetCalendarDay(uint16 year,uint8 mon);
uint16 GetCalendarDayOfYear(uint16 year,uint8 mon,uint8 day);
#endif