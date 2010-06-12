/*
 *Name:		malloc_t.c
 *Descr:	test the malloc function
 *Usage:	$gcc malloc_t.c -Wall -o ~/bin/malloc_t   
 *Author:	zuo haitao
 *Date:		2008/12/29
 *
 */


#include <stdio.h>
#include <stdlib.h>
void Tmalloc(unsigned int unlen);
int main()
{
	Tmalloc(100);
	Tmalloc(0x10000000);
	Tmalloc(0x20000000);
	Tmalloc(0x40000000);
	Tmalloc(0x60000000);
	Tmalloc(0x60000001);
	Tmalloc(0x64444444);
	Tmalloc(0x68888888);
	Tmalloc(0x70000000);
	Tmalloc(0xffffffff);
	printf("\n");


	return 0;
}

void Tmalloc(unsigned int unlen)
{
	char* pbuf = 0;
	printf("size=%u(0x%x):", unlen, unlen);
	fflush(stdout);
	pbuf = malloc(unlen);
	if (0 == pbuf)
	{
		perror("malloc");
	}
	else
	{
		printf("ok\n");
	}
	free(pbuf);
}
