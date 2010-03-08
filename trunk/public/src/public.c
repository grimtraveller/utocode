/*
 * Name:	public.c
 * Desc:	pack base system call
 * Usage:	$>gcc -c public.c -o public.o
 * Author:	zuohaitao
 * Date:	2009-03-02
 *
 */
#include "public.h"
void z_perror(const char* s)
{
	perror(s);
}

void* z_malloc(size_t size)
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

void* z_realloc(void* ptr, size_t size)
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

void z_free(void* ptr)
{
	if (NULL != ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}




