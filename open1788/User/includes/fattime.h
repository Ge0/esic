#ifndef _FATTIME_H_
#define _FATTIME_H_

#include "LPC177x_8x.h"
#include "lpc177x_8x_rtc.h"


#define _LPC_RTC		(LPC_RTC)
void rtc_gettime (RTC_TIME_Type *rtc);
INT_32 get_fattime ();

#endif /* _FATTIME_H_ */