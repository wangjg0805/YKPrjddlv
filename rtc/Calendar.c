#include "Calendar.h"
#include "type_def.h"

uint8 IsLeapYear(uint16 year)
{
    return (!(year%4)&&(year%100)||!(year%400));
}

//0=ĞÇÆÚÈÕ  6=ĞÇÆÚÁù
void GetCalendarWeek(uint16 year,uint8 mon,
					 uint8 day,uint8 *week)
{
	if(mon<3){
	   year--;
	   mon+=12;
	}
	*week=(uint8 )( ((uint16)day+2*(uint16)mon+3*
		((uint16)mon+1)/5+year+year/4-year/100+year/400+1)%7);
}

uint8 GetCalendarDay(uint16 year,uint8 mon)
{
    static uint8 DayOfMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	if((mon > 12)||(mon == 0))
		return 0;
	if(mon==2)
	{
		if(IsLeapYear(year))
			return 29;
	}
	return DayOfMonth[mon-1];
}

uint16 GetCalendarDayOfYear(uint16 year,uint8 mon,uint8 day)
{
	static unsigned short monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334};
	if((mon > 12)||(mon == 0))
	    return 1;
	if(mon > 2)
	{
	    return (monthday[mon-1]+day);
	}
	else
	{
		if(IsLeapYear(year))
		{
		    return (monthday[mon-1]+day+1);
		}
		else
		    return (monthday[mon-1]+day+1);
	}
	return 1;
}