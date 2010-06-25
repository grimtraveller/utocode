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
int make_rand(int max);
void easy_test();
void 
show_leak(char* file, int line, void* p, int len)
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
	boolean isRealloc = FALSE;
	int resize = 0;
	char* pold = NULL;
	jzint64 rtime;

	//easy_test();
	START_JZTIMER;
	srand((unsigned)time(NULL));
	count = make_rand(ALLOC_COUNT_MAX);
	printf("malloc count = %d\n", count);
	JZMEMINIT(NULL);
	
	pp = JZMALLOC(count * sizeof(char*));
	for (i = 0; i < count; i++)
	{
		isRealloc = FALSE;//(BOOLEAN)make_rand(1);

		size = make_rand(MEMSIZE_MAX);
		STOP_JZTIMER(rtime);
		*(pp + i) = JZMALLOC(size);
		STOP_JZTIMER(rtime);
		printf("JZMALLOC time = %lld\n", rtime);
		if (!isRealloc)
		{
			printf("[%d] malloc size = %d ", i, size);
		}
		else
		{
			resize = make_rand(MEMSIZE_MAX);
			*(pp + i) = JZREALLOC(*(pp+i), resize+size);
			printf("[%d] realloc size = %d ", i, resize+size);
		}
		
		printf("address = %x", (unsigned int)*(pp + i));
		memset(*(pp+i), 0x20, size);
		printf("---[ok]\n");
	}
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
		printf("free [%d]\n", i);
		JZFREE(*(pp+i));
	}
	printf("memory leak\n");
	JZCHECKLEAK(show_leak);
	JZMEMUNINIT();
	STOP_JZTIMER(rtime);
	printf("time = %lld ms\n", rtime);
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
