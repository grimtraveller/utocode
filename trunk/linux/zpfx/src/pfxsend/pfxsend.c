#include "../include/pfxdirapi.h"
#include "../include/public.h"
#include "../include/pfxthreadapi.h"
#include "../include/pfxlock.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
struct threadinfo_t
{
	PFXTHREADID tid;
	int is_work;	/*	0 = resting 1 = working	*/ 
	int is_send;	/*	0 = waiting 1 = sending */
	int sendfile[260];
};
struct present
{
	char filename[260];
};
struct present g_presentlist[100];
struct threadinfo_t* g_pcheckthread;
//threadinfo_t* g_sendthreadlist[5];
//threadinfo_t* g_recvthreadlist[5];
//threadinfo_t* g_
void* checksendboxbytime(void* pdata);
int getfile(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftw);
int main(int argc, char* argv[])
{
	int i = 0;	//
	int sendthreadcount = 0;
	int recvthreadcount = 0;
	//load config 
	sendthreadcount = 5;
	recvthreadcount = 5;
	//start thread
	g_pcheckthread = (struct threadinfo_t*)malloc(sizeof(struct threadinfo_t));
	/*
	g_sendthreadlist = (threadinfo_t*)malloc(sizeof(threadinfo_t)*sendthreadcount);
	for (i = 0; i < sendthreadcount; i++)
	{
		g_sendthreadlist[i] = g_sendthreadlist + i;
	}
	
	for (i = sendthreadcount; i < 5; i++)
	{
		g_sendthreadlist[i] = NULL;
	}
	*/
	g_pcheckthread->is_work = 1;
	if (pfxcreatethread(&(g_pcheckthread->tid), checksendboxbytime, g_pcheckthread))
	{
		perror("pfxcreatethread");
	}
	pfxwaitthread(g_pcheckthread->tid);
	return 0;
}
void* checksendboxbytime(void* pdata)
{
	FTWDO func;
	func = getfile;
	struct threadinfo_t* ptinfo;
	ptinfo = (struct threadinfo_t*)pdata;
	while(1)
	{
		printf("check\n");
		if (0 == ptinfo->is_work)
		{
			break;
		}
		if (pfxfind("../sendbox", func) < 0)
		{
			perror("pfxfind");
		}
		sleep(1);
	}
	return 0;
}

int getfile(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftw)
{
	const char szflag[] = ".ok";
	char szsuffix[4] = {0};
	char szfile[260] = {0};
	strncpy(szsuffix, fpath + (strlen(fpath) - 3), 3);
	printf("%s\n", szsuffix);
	if (0 == strncmp(szsuffix, szflag, 3))
	{
		memcpy(szfile, fpath, strlen(fpath) - 3);
		printf("%s\n", szfile);
	}
	return 0;

}

