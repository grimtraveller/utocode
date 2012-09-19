#include "zjperformance.h"
#if defined(WIN32)	//OS
#include <windows.h>
#elif defined(LINUX)
#include <time.h>
#include <stdio.h>
#include <errno.h>
#endif //OS


ZJBOOL zjqueryPerformanceUnit(zjint64* unit)
{
#if defined(WIN32) //OS
	DWORD dwError;
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		return FALSE;
	}
	*unit = frequency.QuadPart;
#elif defined(LINUX)
	*unit = 1;
#endif //OS
	return TRUE;
}

ZJBOOL zjqueryPerformanceCounter(zjint64* counter)
{
#if defined(WIN32) //OS
	LARGE_INTEGER c;
	if (!QueryPerformanceCounter(&c))
	{
		return FALSE;
	}
	*counter = c.QuadPart;
#elif defined(LINUX)
	struct timeval tv;
	if (-1 == gettimeofday(&tv, NULL))
	{
		return FALSE;
	}
	*counter = tv.tv_sec * 1000000 + tv.tv_usec;
#endif //OS
	return TRUE;
}

zjint64 zjcalcPerformance(zjint64 start, zjint64 end, zjint64 unit)
{
#if defined(WIN32)	//OS
	return (((end - start) * (zjint64)1000000)/unit);
#elif defined(LINUX)
	return end - start;
#endif //OS
}

