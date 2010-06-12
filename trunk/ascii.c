/*
 * Name:	ascii.c
 * Desc:	print charactor's ascii 
 * Usage:	$>ascii a b c
 * Author:	zuohaitao
 * Date:	2009-03-10
 */
#include <stdio.h>
int main(int argc,char* argv[])
{
	int i;
	if (argc < 2)
	{
		printf("usag: ascii character\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
	{
		printf("%c: 0x%x %d\n", argv[i][0], argv[i][0], argv[i][0]);
	}
	return 0;
}
