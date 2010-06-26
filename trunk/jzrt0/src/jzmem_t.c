#define JZDEBUG
#include "jzmem.h"
#include <stdio.h>
#include <time.h>
#include "jztimer.h"

#define MEMSIZE_MAX		(1024*1024*1)
#define MEMSIZE_MIN		(1)
#define ALLOC_COUNT_MAX (100)
#define ALLOC_COUNT_MIN (1)
#define _CRT_SECURE_NO_WARNINGS
BOOLEAN make_boolean();
int make_rand(int min, int max);
void easy_test();
void 
show_leak(char* file, jzuint32 line, void* p, size_t len)
{
	printf("%s %10d : 0x%08x %10d\n", file, line, (int)p, len);
} 
int
main(int argc, char** argv)
{
	//! malloc count
	jzuint32 malloc_count = 0;
	//! realloc count
	jzuint32 realloc_count = 0;
	//! free count
	jzuint32 free_count = 0;
	//! point array
	char** pp = NULL;
	//! malloc length
	jzsize size = 0;
	//! index
	jzuint32 i = 0;
	//BOOLEAN isRealloc = FALSE;
	//! realloc size
	jzsize resize = 0;
	//! performace time
	jzint64 rtime = 0;
	//! jzmem_header point
	jzmem_header_st* pjzmem_header = NULL;
	//! test case total time
	jztimer_st total_timer = {0};

	START_JZTIMER();
	//test total jztimer	
	init_jztimer(&total_timer);
	start_jztimer(&total_timer);

	//initial random
	srand((unsigned)time(NULL));
	
	//initial jztimer
	STOP_JZTIMER(rtime);

	//initial jzmem
	if (make_boolean())
	{
		JZMEMINIT(NULL);
	}
	else
	{
		pjzmem_header = malloc(sizeof(jzmem_header_st));
		JZMEMINIT(pjzmem_header);
	}

	STOP_JZTIMER(rtime);
	printf("JZMEMINIT time = %.2f ms \n", ((double)rtime/1000.00));

	//! malloc test case
	malloc_count = make_rand(ALLOC_COUNT_MIN, ALLOC_COUNT_MAX);
	printf("[malloc test case:] malloc count = %d\n", malloc_count);
	pp = JZMALLOC(malloc_count * sizeof(char*));
	for (i = 0; i < malloc_count; i++)
	{
		size = make_rand(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp + i) = JZMALLOC(size);
		STOP_JZTIMER(rtime);
		printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
		printf("malloc size = %7d ", size);
		printf("address = 0x%08x", (unsigned int)*(pp + i));
		memset(*(pp+i), 0x20, size);
		printf("---[ok]\n");
	}

	//! realloc test case
	realloc_count = make_rand(ALLOC_COUNT_MIN, malloc_count);
	printf("[realloc test case:] realloc count = %d\n", realloc_count);

	for (i = 0; i < realloc_count; i++)
	{
		resize = make_rand(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp + i) = JZREALLOC(*(pp+i), resize);
		STOP_JZTIMER(rtime);
		printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
		printf("realloc size = %7d ", resize);
		printf("address = 0x%08x", (unsigned int)*(pp + i));
		memset(*(pp+i), 0x20, resize);
		printf("---[ok]\n");
	}

	//! realloc the first in jzmem test case
	if (make_boolean())
	{
		resize = make_rand(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp) = JZREALLOC(*(pp), resize);
		STOP_JZTIMER(rtime);
		printf("[%2d] time = %.2f ms ", 0, ((double)rtime/1000.00));
		printf("realloc size = %7d ", resize);
		printf("address = 0x%08x", (unsigned int)*(pp));
		memset(*(pp), 0x20, resize);
		printf("---[ok]\n");
	}

	//! realloc the last in jzmem test case
	if (make_boolean())
	{
		resize = make_rand(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp+malloc_count-1) = JZREALLOC(*(pp+malloc_count-1), resize);
		STOP_JZTIMER(rtime);
		printf("[%2d] time = %.2f ms ", malloc_count-1, ((double)rtime/1000.00));
		printf("realloc size = %7d ", resize);
		printf("address = 0x%08x", (unsigned int)*(pp+malloc_count-1));
		memset(*(pp+malloc_count-1), 0x20, resize);
		printf("---[ok]\n");
	}

	//free test case
	if(make_boolean())
	{
		free_count = make_rand(0, malloc_count);
	}
	else
	{
		free_count = malloc_count;
	}
	printf("[last realloc test case:] free count = %d\n", free_count);
	for (i = 0; i < free_count; i++)
	{
		JZFREE(*(pp+i));
	}
	printf("\n");
	printf("memory leak\n");
	JZCHECKLEAK(show_leak);
	JZFREE(pp);
	JZMEMUNINIT();
	stop_jztimer(&total_timer);
	printf("time = %.2f ms \n", ((double)total_timer.rtime/1000.00));
	return 0;
}

int 
make_rand(int min, int max)
{
	int n;
	while(1)
	{
		n = rand();
		n = n%max;
		if ((n >= min))
		{
			break;
		}
	}
	return n;
}
BOOLEAN
make_boolean()
{
	int n;
	n = rand();
	return (BOOLEAN)(n%2);
}
