#include "../include/public.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pfxdatetime.h"
enum pfxerrstyle_t g_pfxerrstyle;
int g_pfxerrno;
char g_pfxerrmsg[COMMONLEN];

void setpfxerr(enum pfxerrstyle_t errstyle, int errno, const char* msg);
void cleanpfxerr();
void showpfxerr();
void logpfxerr(const char* path);
//if how=1 then show
//if how=2 then log
//if how=3 then show and log
void ppfxerr(int how);

void setpfxerr(enum pfxerrstyle_t errstyle, int errno, const char* msg)
{
	g_pfxerrstyle = errstyle;
	g_pfxerrno = errno;
	strncpy(g_pfxerrmsg, msg, COMMONLEN);
}


void cleanpfxerr()
{
	g_pfxerrstyle = noerr;
	g_pfxerrstyle = 0;
	g_pfxerrno = 0;
	memset(g_pfxerrmsg, 0, COMMONLEN);
}

void showpfxerr()
{
	printf("%.4x-%.4x[%s]\n", g_pfxerrstyle, g_pfxerrno, g_pfxerrmsg);
}

void logpfxerr(const char* path)
{
	FILE* fp;
	char datestr[11];	//YYYY-MM-DD
	char defaultpath[MAXPATH] = {0};
	strncpy(datestr, "2008-8-02", 11);
	pfxdate(datestr, 1);
	sprintf(defaultpath, "../log/%s%s", datestr, LOGSUFFIX);
	if (NULL != path)
	{
		strncpy(defaultpath, path, MAXPATH);
	}
	if (NULL == (fp = fopen(defaultpath, "a")))
	{
		perror("fopen");
		exit(1);
	}
	fprintf(fp, "%.4x-%.4x[%s]\n", g_pfxerrstyle, g_pfxerrno, g_pfxerrmsg);
	fclose(fp);
}

void ppfxerr(int how)
{
	switch(how)
	{
	case 1:
		showpfxerr();
		break;
	case 2:
		logpfxerr((void*)NULL);
		break;
	case 3:
		showpfxerr();
		logpfxerr((void*)NULL);
		break;
	default:
		showpfxerr();
		break;
	}
}
