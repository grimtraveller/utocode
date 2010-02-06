#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include "pfxthreadapi.h"
#include "pfxthreadapi.h"
int pfxcreatethread(PFXTHREADID* ptid, PFXTHREADDO pfxdo, void* pdata)
{
	return pthread_create(ptid, NULL, pfxdo, pdata);
}
void pfxkillthread(PFXTHREADID tid)
{
	pthread_kill(tid, SIGKILL);
}
int pfxwaitthread(PFXTHREADID tid)
{
	return pthread_join(tid, 0);
}

