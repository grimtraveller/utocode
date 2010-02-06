#include <time.h>
#include <stdio.h>
/*
 * name:	pfxdatetime.c
 * desc:	the functions about date time
 * author:	zuohaitao
 * date:	2008/08/09
 */
#include "../include/public.h"
void pfxdate(char* buf, int flag);

void pfxdate(char* buf, int flag)
{
	struct tm *tm;
	long t;
	//date format:2008-08-03
	t = time(NULL);
	tm = localtime(&t);
	if (0 == flag)
	{
		sprintf(buf, "%04d%02d%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
	}
	else
	{
		sprintf(buf, "%04d-%02d-%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
	}
}


