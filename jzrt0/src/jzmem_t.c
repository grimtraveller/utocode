#define JZDEBUG
#include "jzmem.h"
#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include "jztype.h"
#include "jztimer.h"

#define MEMSIZE_MAX		(1024*1024*1)
#define ALLOC_COUNT_MAX (100)
#define _CRT_SECURE_NO_WARNINGS
int make_rand(int max);
void easy_test();
void 
show_leak(char* file, jzuint32 line, void* p, size_t len)
{
	printf("%s %d : 0x%x %d\n", file, line, (int)p, len);
} 
int
main()
{
	int count = 0;
	int free_count = 0;
	char** pp = NULL;
	int size = 0;
	int i = 0;
	BOOLEAN isRealloc = FALSE;
	int resize = 0;
	char* pold = NULL;
	jzint64 rtime;
	jzmem_header_st* pjzmem_header = NULL;

	//easy_test();
	START_JZTIMER;
	srand((unsigned)time(NULL));
	count = make_rand(ALLOC_COUNT_MAX);
	printf("malloc count = %d\n", count);
	if ((BOOLEAN)make_rand(1))
	{
		JZMEMINIT(NULL);
	}
	else
	{
		pjzmem_header = malloc(sizeof(jzmem_header_st));
		JZMEMINIT(pjzmem_header);
	}
	
	pp = JZMALLOC(count * sizeof(char*));
	for (i = 0; i < count; i++)
	{
		isRealloc = (BOOLEAN)make_rand(1);
		size = make_rand(MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp + i) = JZMALLOC(size);
		STOP_JZTIMER(rtime);
		if (!isRealloc)
		{
			printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
			printf("malloc size = %7d ", size);
		}
		else
		{
			resize = make_rand(MEMSIZE_MAX);
			STOP_JZTIMER(rtime);
			*(pp + i) = JZREALLOC(*(pp+i), resize+size);
			STOP_JZTIMER(rtime);
			printf("[%2d] time = %.2f ms ", i, ((double)rtime/1000.00));
			printf("realloc size = %7d ", resize);
		}	
		printf("address = 0x%08x", (unsigned int)*(pp + i));
		memset(*(pp+i), 0x20, size);
		printf("---[ok]\n");
	}
	return 0;
	if((BOOLEAN)make_rand(1))
	{
		free_count = make_rand(count);
	}
	else
	{
		free_count = count;
	}
	printf("free count = %d\n", free_count);
	for (i = 0; i < free_count; i++)
	{
		switch(i%4)
		{
			case 0:
				printf("\b-");
				fflush(stdout);
				break;
			case 1:
				printf("\b\\");
				fflush(stdout);
				break;
			case 2:
				printf("\b|");
				fflush(stdout);
				break;
			case 3:
				printf("\b/");
				fflush(stdout);
				break;
			default:
				break;
		}
		JZFREE(*(pp+i));
	}
	printf("\n");
	printf("memory leak\n");
	JZCHECKLEAK(show_leak);
	JZMEMUNINIT();
	STOP_JZTIMER(rtime);
	printf("time = %.2f ms \n", ((double)rtime/1000.00));
	return 0;
}

void base_test()
{
	char* p = NULL;
	p = JZMALLOC(100);
	memset(p, 0x20, 100);
}
void easy_test()
{
	size_t size = 1803298383;
	char* p = (char*)malloc(size);
	perror("");
	memset(p, 0x20, size);
}
int 
make_rand(int max)
{
	int n;
	while(1)
	{
		n = rand();
		if (n <= max)
		{
			break;
		}
	}
	return n;
}