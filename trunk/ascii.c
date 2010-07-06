/*
 * Name:	ascii.c
 * Desc:	print charactor's ascii 
 * Usage:	$>ascii a b c
 * Author:	zuohaitao
 * Date:	2009-03-10
 * Modify:
 *		what:	scape char convert
 *		who:	zuohaitao
 *		date:	2010-07-07
 */
#include <stdio.h>
int main(int argc,char* argv[])
{
	int i;
	if (argc < 2)
	{
		printf("usage: ascii character\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
	{

		if (0 == strcmp("\'\\n\'", argv[i]))
		{
			printf("%s:\t0x%02x %03d\n", "\'\\n\'", '\n', '\n');
			continue;
		}
		if (0 == strcmp("\'\\b\'", argv[i]))
		{
			printf("%s:\t0x%02x %03d\n", "\'\\b\'", '\b', '\b');
			continue;
		}
		printf("\'%c\':\t0x%02x %03d\n", argv[i][0], argv[i][0], argv[i][0]);
	}
	return 0;
}
