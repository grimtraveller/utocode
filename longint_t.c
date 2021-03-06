/*
 * Name:		longint_t.c
 * Desc:		test the "long int" type
 * Usage:		longint_t
 * Author:		zuohaitao
 * Date:
 */
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
int main()
{
	long int a;
#if defined(MACOSX)
	printf("sizeof(ssize_t)=%ld", sizeof(ssize_t));
	printf("sizeof(long int)=%ld", sizeof(long int));
#else 
#	ifndef WIN32
	ssize_t b;
	printf("sizeof(ssize_t)=%d", sizeof(ssize_t));
#	endif
	printf("sizeof(long int)=%d", sizeof(long int));
#endif
#
	return 0;
}
