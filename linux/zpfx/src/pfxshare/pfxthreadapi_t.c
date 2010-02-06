/*
 * name:	pfxthreadapi_t.c
 * desc:	test pfxthreadapi
 * usage:	$gcc thread_t.c -lpthread -o thread_t
			$thread_t
 * author:	zuohaitao
 * date:	2008/08/08
 */
#include "pfxerror.h"
#include "pfxthreadapi.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
struct pthreadinfo_t
{
	PFXTHREADID tid;
	int is_work;
};
void* work_thread(void* pdata);
int main()
{
	PFXTHREADID tid;
	struct pthreadinfo_t tinfo[5];
	char ctl;
	int exit;
	int i;
	int num;
	for (i = 0; i < 5; i++)
	{
		tinfo[i].is_work = 1;
		if (pfxcreatethread(&tid, work_thread, (void*)(&tinfo[i])) < 0)
		{
			setpfxerr(syserr, errno, strerror(errno));
			showpfxerr();
			return 1;
		}
	}
	exit = 0;
	while(exit < 5)
	{
		ctl = getchar();
		//printf("%d,%d,%d\n", '0', '6', ctl);
		if ('\n' == ctl)
		{
			continue;
		}
		if (('0' < ctl) && (ctl < '6'))
		{
			printf("control[%c]...\n", ctl);
			tinfo[atoi(&ctl) - 1].is_work = 0;
			exit++;
			//break;
		}
		else if ('q' == ctl)
		{
			printf("kill all\n");
			for (i = 0; i < 5; i++)
			{
				if (0 != tinfo[i].is_work)
				{
					pfxkillthread(tinfo[i].tid);
				}
			}
		}
		else
		{
			printf("%c\n", ctl);
		}
	}
	for (i = 0; i < 5; i++)
	{
		if (0 != pfxwaitthread(tinfo[i].tid))
		{
			setpfxerr(syserr, errno, strerror(errno));
			showpfxerr();
			return 1;
		}
	}
	return 0;
}

void* work_thread(void* pdata)
{
	struct pthreadinfo_t* ptinfo = (struct pthreadinfo_t*) pdata;
	ptinfo->tid = pthread_self();
	printf("=threadid:0x%x=\n", ptinfo->tid);	
	while(1)
	{
		//if not '=' then main thread getchar() will be a wrong!
		//printf("=0x%x say:work work...\n", ptinfo->tid);

		sleep(5);
		if (0 == ptinfo->is_work)
		{
			printf("=0x%x say: bye!\n", ptinfo->tid);
			break;
		}
	}
}
