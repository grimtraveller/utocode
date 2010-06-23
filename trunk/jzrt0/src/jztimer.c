/**
 * @file	jztimer.c 
 * @brief	implement timer function
 * @details	
 * @author	zuohaitao
 * @date	2010-06-22 
 * @warning	
 * @bug	
 */
#include "jztimer.h"
#include "jztype.h"

void init_jztimer(struct jztimer_st* ptimer)
{
#ifdef WIN32
	DWORD dwError;
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		dwError = GetLastError();
		ptimer->bOK = FALSE;
	}
	else
	{
		ptimer->bOK = TRUE;
	}
	ptimer->unit = frequency.QuadPart;
#else
	ptimer->unit = 1;
#endif //WIN32
	ptimer->start = 0;
	ptimer->stop = 0;
	ptimer->rtime = 0;
}

void start_jztimer(struct jztimer_st* ptimer)
{
#ifdef WIN32
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);
	ptimer->start = startTime.QuadPart;
#else
	
#endif //WIN32
}

void stop_jztimer(struct jztimer_st* ptimer)
{
#ifdef WIN32
	LARGE_INTEGER stopTime;
	QueryPerformanceCounter(&stopTime);
	ptimer->stop = stopTime.QuadPart;
#else
	
#endif
	ptimer->rtime = ((ptimer->stop - ptimer->start) * (jzint64)1000 )/ ptimer->unit;
	ptimer->start = ptimer->stop;
}