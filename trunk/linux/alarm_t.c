/*
 * Name:	alarm_t.c
 * Desc:	study SIGXXX(SIGARLM, SIGINT)
 * Usage:	
			$>gcc alarm_t.c -o ~/bin/alarm_t
			$>~/bin/alarm_t
			
			ctrl+c to do or undo SIGARLM
			ctrl+z to kill
 * Author:	zuohaitao
 * Date:	2008/12/28
 *
 */
#include <signal.h>
#include <stdio.h>
typedef void (*Sigfunc)(int);
void do_alarm(int signalno);
void undo_alarm();
void do_sigint(int signalno);
Sigfunc sigfunc;
int nHow = 0;	//0 if catch; 1 if not catch
int main()
{
	int i = 0;
	signal(SIGINT, do_sigint);
	do_alarm(SIGALRM);
	nHow = 0;
	while(1)
	{
		sleep(1);
		//[NOTE]printf is not safe!!!
		printf("Sleep...%d\n", i++);
	}
	return 0;
}
void do_alarm(int signalno)
{
	int nsec;
	nsec = 2;
	if ((sigfunc =(Sigfunc)signal(SIGALRM, do_alarm) )< 0)
	{
		perror("signal()");
	}
	if (0 != alarm(nsec))
	{
		perror("alarm was already set!");
	}

	printf("do_alarm\n");
	return;
}

void do_sigint(int signo)
{
	if (0 == nHow)
	{
		do_alarm(SIGALRM);
	}
	else
	{
		undo_alarm();
	}
	nHow = ~nHow;
}

void undo_alarm()
{
	alarm(0);
	signal(SIGALRM, sigfunc);
}

