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
#ifndef WIN32
	ssize_t b;
	printf("sizeof(ssize_t)=%d", sizeof(ssize_t));
#endif
	printf("sizeof(long int)=%d", sizeof(long int));
	return 0;
}
