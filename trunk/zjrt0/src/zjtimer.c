/**
 * @file	zjtimer.c 
 * @brief	implement timer function
 * @details	
 * @author	zuohaitao
 * @date	2010-06-22 
 * @warning	
 * @bug	
 */
#include "zjtimer.h"
#include "zjtype.h"
#ifndef WIN32
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#endif

void init_zjtimer(zjtimer_st* ptimer)
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

void start_zjtimer(zjtimer_st* ptimer)
{
#ifdef WIN32
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);
	ptimer->start = startTime.QuadPart;
#else
	struct timeval tv;
	if (-1 == gettimeofday(&tv, NULL))
	{
		ptimer->bOK = FALSE;
		perror("start_zjtimer");
		exit(errno);
	}
	ptimer->start = tv.tv_sec * 1000000 + tv.tv_usec;
#endif //WIN32
}

void stop_zjtimer(zjtimer_st* ptimer)
{
#ifdef WIN32
	LARGE_INTEGER stopTime;
	QueryPerformanceCounter(&stopTime);
	ptimer->stop = stopTime.QuadPart;
	ptimer->rtime = ((ptimer->stop - ptimer->start) * (zjint64)1000000 )/ ptimer->unit;
#else
	struct timeval tv;
	if (-1 == gettimeofday(&tv, NULL))
	{
		ptimer->bOK = FALSE;
		exit(errno);
	}
	ptimer->stop = tv.tv_sec * 1000000 + tv.tv_usec;
	ptimer->rtime = ptimer->stop - ptimer->start;
#endif
	ptimer->start = ptimer->stop;
}

