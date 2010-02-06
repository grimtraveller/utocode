/*
 * Name:	public.c
 * Desc:	pack base system call
 * Usage:	$>gcc -c public.c -o public.o
 * Author:	zuohaitao
 * Date:	2009-03-02
 *
 */
#include "public.h"
void DSperror(const char* s)
{
	perror(s);
}

void* DSmalloc(size_t size)
{
	void* ptr;
	ptr = malloc(size);
	if (NULL == ptr)
	{
		perror("malloc");
		exit(1);
	}
	return ptr;
}

void* DSrealloc(void* ptr, size_t size)
{
	void* newPtr;
	newPtr = realloc(ptr, size);
	if (NULL == newPtr)
	{
		perror("realloc");
		exit(1);
	}
	return newPtr;
}

void DSfree(void* ptr)
{
	if (NULL != ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}




