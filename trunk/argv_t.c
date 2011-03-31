/*
 *Name:		argv_t.c
 *Desc:		test the argment of the main function
 *Usage:	$gcc argv.c -o ~/bin/argv
			argv a b c d e f -g
 *Date:		
 */
#include <stdio.h>
int main(int argc, char* argv[])
{
	int i = 0;
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d]=%s\n", i, argv[i]);
	}
	return 0;
}
