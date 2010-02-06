/*
 * name:	thread_t.c
 * desc:	study use thread in linux
 * usage:	gcc thread_t.c -lpthread -o thread_t
 * author:	zuohaitao
 * date:	2008/08/31
 * modify:	fix bug pthreadkill(tid, SIGKILL)
 * author:	zuohaitao
 * date:	2008/08/08
 */
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
struct pthreadinfo_t
{
	pthread_t tid;
	int is_work;
};
void* work_thread(void* pdata);
int main()
{
	pthread_t tid;
	struct pthreadinfo_t tinfo[5];
	char ctl;
	int exit;
	int i;
	int num;
	for (i = 0; i < 5; i++)
	{
		tinfo[i].is_work = 1;
		if (pthread_create(&tid, NULL, work_thread, (void*)(&tinfo[i])) < 0)
		{
			perror("pthread_create");
			return 1;
		}
	}
	exit = 0;
	while(exit < 5)
	{
		ctl = getchar();
		printf("%d,%d,%d\n", '0', '6', ctl);
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
					printf("%x\n", tinfo[i].tid);
					pthread_kill(tinfo[i].tid, SIGKILL);
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
		if (pthread_join(tinfo[i].tid, 0))
		{
			perror("pthread_join");
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
