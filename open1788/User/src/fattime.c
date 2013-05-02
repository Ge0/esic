#include "fattime.h"

void rtc_gettime (RTC_TIME_Type *rtc)
{
	RTC_GetFullTime( _LPC_RTC, rtc);	
}


/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
INT_32 get_fattime ()
{
	RTC_TIME_Type rtc;

	/* Get local time */
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return	  ((INT_32)(rtc.YEAR - 1980) << 25)
			| ((INT_32)rtc.MONTH << 21)
			| ((INT_32)rtc.DOM << 16)
			| ((INT_32)rtc.HOUR << 11)
			| ((INT_32)rtc.MIN<< 5)
			| ((INT_32)rtc.SEC>> 1);

}