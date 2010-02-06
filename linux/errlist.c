/*
 * Name:	errlist.c
 * Desc:	input errno print error infomation like lookup in win32
 * Usage:	
			input errno and print error info
			$>./errlist errno
			print all error information
			$>./errlist
 * Author:	zuohaitao
 * Date:	2009-03-15
 */
#include<stdio.h>
#include<errno.h>
#include<string.h>
int main(int argc, char* argv[])
{
	int i;
	if (2 == argc)
	{
		i = atoi(argv[1]);
		printf("#%d: %s\n", i, strerror(i));
		return 0;
	}
	for (i=0; i<257; i++)
	{
		printf("#%d: %s\n", i, strerror(i));
	}
}
