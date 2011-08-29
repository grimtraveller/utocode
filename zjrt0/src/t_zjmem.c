#define ZJDEBUG
#include "zjmem.h"
#include <stdio.h>
#include <time.h>
#include "zjtimer.h"
#include "zjrand.h"
#define MEMSIZE_MAX		(1024*1024*1)
#define MEMSIZE_MIN		(1)
#define ALLOC_COUNT_MAX (100)
#define ALLOC_COUNT_MIN (1)
#define _CRT_SECURE_NO_WARNINGS
void easy_test();
void 
show_leak(char* file, zjuint32 line, void* p, size_t len)
{
#if defined(MAC64)
	printf("%s %10d : 0x%08lx %10d\n", file, line, (long)p, (int)len);
#elif defined(MAC64)
	printf("%s %10d : 0x%08x %10d\n", file, line, (int)p, (int)len);
#endif 
} 
int
main(int argc, char** argv)
{
	//! malloc count
	zjuint32 malloc_count = 0;
	//! realloc count
	zjuint32 realloc_count = 0;
	//! free count
	zjuint32 free_count = 0;
	//! point array
	char** pp = NULL;
	//! malloc length
	zjsize size = 0;
	//! index
	zjuint32 i = 0;
	//BOOLEAN isRealloc = FALSE;
	//! realloc size
	zjsize resize = 0;
	//! performace time
	zjint64 rtime = 0;
	//! zjmem_header point
	zjmem_header_st* pzjmem_header = NULL;
	//! test case total time
	zjtimer_st total_timer = {0};

	START_ZJTIMER();
	//test total zjtimer	
	init_zjtimer(&total_timer);
	start_zjtimer(&total_timer);

	//initial random
	srand((unsigned)time(NULL));
	
	//initial zjtimer
	STOP_ZJTIMER(rtime);

	//initial zjmem
	//if (make_boolean())
	if (zjrand_boolean())
	{
		ZJMEMINIT(NULL);
	}
	else
	{
		pzjmem_header = malloc(sizeof(zjmem_header_st));
		ZJMEMINIT(pzjmem_header);
	}

	STOP_ZJTIMER(rtime);
	printf("ZJMEMINIT time = %.2f ms \n", ((double)rtime/1000.00));

	//! malloc test case
	//malloc_count = make_rand(ALLOC_COUNT_MIN, ALLOC_COUNT_MAX);
	malloc_count = zjrand_scope(ALLOC_COUNT_MIN, ALLOC_COUNT_MAX);
	printf("[malloc test case:] malloc count = %d\n", malloc_count);
	pp = ZJMALLOC(malloc_count * sizeof(char*));
	for (i = 0; i < malloc_count; i++)
	{
		//size = make_rand(MEMSIZE_MIN, MEMSIZE_MAX);
		size = zjrand_scope(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_ZJTIMER(rtime);
		*(pp + i) = ZJMALLOC(size);
		STOP_ZJTIMER(rtime);
		printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
#if defined(MAC64)
		printf("malloc size = %7ld ", size);
#else
		printf("malloc size = %7d ", size);
#endif
#if defined(MAC64)
		printf("address = 0x%08lx", (long)*(pp + i));
#else
		printf("address = 0x%08x", (unsigned int)*(pp + i));
#endif
		memset(*(pp+i), 0x20, size);
		printf("---[ok]\n");
	}

	//! realloc test case
	//realloc_count = make_rand(ALLOC_COUNT_MIN, malloc_count);
	realloc_count = zjrand_scope(ALLOC_COUNT_MIN, malloc_count);
	printf("[realloc test case:] realloc count = %d\n", realloc_count);

	for (i = 0; i < realloc_count; i++)
	{
		resize = zjrand_scope(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_ZJTIMER(rtime);
		*(pp + i) = ZJREALLOC(*(pp+i), resize);
		STOP_ZJTIMER(rtime);
		printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
#if defined(MAC64)
		printf("realloc size = %16ld ", resize);
#else
		printf("realloc size = %7d ", resize);
#endif
#if defined(MAC64)
		printf("address = 0x%16lx", (unsigned long)*(pp + i));
#else
		printf("address = 0x%08x", (unsigned int)*(pp + i));
#endif
		memset(*(pp+i), 0x20, resize);
		printf("---[ok]\n");
	}

	//! realloc the first in zjmem test case
	//if (make_boolean())
	if (zjrand_boolean())
	{
		resize = zjrand_scope(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_ZJTIMER(rtime);
		*(pp) = ZJREALLOC(*(pp), resize);
		STOP_ZJTIMER(rtime);
		printf("[%2d] time = %.2f ms ", 0, ((double)rtime/1000.00));
#if defined(MAC64)
		printf("realloc size = %16lu ", resize);
#else
		printf("realloc size = %7d ", resize);
#endif
#if defined(MAC64)
		printf("address = 0x%16lx", (unsigned long)*(pp));
#else
		printf("address = 0x%08x", (unsigned int)*(pp));
#endif 
		memset(*(pp), 0x20, resize);
		printf("---[ok]\n");
	}

	//! realloc the last in zjmem test case
	//if (make_boolean())
	if (zjrand_boolean())
	{
		resize = zjrand_scope(MEMSIZE_MIN, MEMSIZE_MAX);
		STOP_ZJTIMER(rtime);
		*(pp+malloc_count-1) = ZJREALLOC(*(pp+malloc_count-1), resize);
		STOP_ZJTIMER(rtime);
		printf("[%2d] time = %.2f ms ", malloc_count-1, ((double)rtime/1000.00));
#if defined(MAC64)
		printf("realloc size = %16ld ", resize);
#else
		printf("realloc size = %7d ", resize);
#endif
#if defined(MAC64)
		printf("address = 0x%16lx", (unsigned long)*(pp+malloc_count-1));
#else
		printf("address = 0x%08x", (unsigned int)*(pp+malloc_count-1));
#endif
		memset(*(pp+malloc_count-1), 0x20, resize);
		printf("---[ok]\n");
	}

	//free test case
	//if(make_boolean())
	if(zjrand_boolean())
	{
		//free_count = make_rand(0, malloc_count);
		free_count = zjrand_scope(0, malloc_count);
	}
	else
	{
		free_count = malloc_count;
	}
	printf("[last realloc test case:] free count = %d\n", free_count);
	for (i = 0; i < free_count; i++)
	{
		ZJFREE(*(pp+i));
	}
	printf("\n");
	printf("memory leak\n");
	ZJCHECKLEAK(show_leak);
	ZJFREE(pp);
	ZJMEMUNINIT();
	stop_zjtimer(&total_timer);
	printf("time = %.2f ms \n", ((double)total_timer.rtime/1000.00));
	return 0;
}
