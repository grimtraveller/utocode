#include "jzmem.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
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
	//easy_test();
	srand((unsigned)time(NULL));
	count = make_rand(ALLOC_COUNT_MAX);
	printf("malloc count = %d\n", count);
	construct_jzmem(NULL);
	pp = JZMALLOC(count * sizeof(char*));
	for (i = 0; i < count; i++)
	{
		size = make_rand(MEMSIZE_MAX);
		printf("[%d] malloc size = %d ", i, size);
		*(pp + i) = JZMALLOC(size);
		printf("address = %x", (unsigned int)*(pp + i));
		memset(*(pp+i), 0x20, size);
		printf("---[ok]\n");
	}
	free_count = make_rand(count);
	printf("free count = %d\n", free_count);
	for (i = 0; i <= free_count; i++)
	{
		//JZFREE(*(pp+i));
	}
	jzcheck_leak(show_leak);
	destruct_jzmem();
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
