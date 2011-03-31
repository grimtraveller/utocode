/*
 *Name:		malloc_t.c
 *Descr:	test the malloc function
			get the malloc max value supported in current system 
 *Usage:	$gcc malloc_t.c -Wall -o ~/bin/malloc_t   
 *Author:	zuo haitao
 *Date:		2008/12/29
 *Date:		2011/03/31
 *
 */


#include <stdio.h>
#include <stdlib.h>
int Tmalloc(unsigned int unlen);
int main()
{
	{
		unsigned int i = 0;
		for (i = 0; i < 0xffffffff; i++)
		{
			if (Tmalloc(i) < 0)
			{
				break;
			}
		}
	}
	Tmalloc(100);
	Tmalloc(0x10000000);
	Tmalloc(0x20000000);
	Tmalloc(0x40000000);
	Tmalloc(0x60000000);
	Tmalloc(0x70000000);
	Tmalloc(0x80000000);
	Tmalloc(0x90000000);
	Tmalloc(0xa0000000);
	Tmalloc(0xb0000000);
	Tmalloc(0xc0000000);
	Tmalloc(0xd0000000);
	Tmalloc(0xe0000000);
	Tmalloc(0xf0000000);
	Tmalloc(0xffffffff);
	printf("\n");


	return 0;
}

int Tmalloc(unsigned int unlen)
{
	int ret = 0;
	char* pbuf = 0;
	printf("size=%u(0x%x):", unlen, unlen);
	fflush(stdout);
	pbuf = malloc(unlen);
	if (0 == pbuf)
	{
		ret = -1;
		perror("malloc");
	}
	else
	{
		ret = 0;
		printf("ok\n");
	}
	free(pbuf);
	return ret;
}
