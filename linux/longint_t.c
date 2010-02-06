/*
 * Name:		longint_t.c
 * Desc:		test the "long int" type
 * Usage:		longint_t
 * Author:		zuohaitao
 * Date:
 */
#include <unistd.h>
#include <stdio.h>
int main()
{
	long int a;
	ssize_t b;
	printf("sizeof(ssize_t)=%d", sizeof(ssize_t));
	printf("sizeof(long int)=%d", sizeof(long int));
	return 0;
}
