/*
 * Name:		sigaction_t.c
 * Desc:		
 * Usage:
 * Author:	
 * Date:
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void sig_handler(int signo, siginfo_t * info, void * p);

int main(char** argc, int argv)
{
	printf("pid = %d\n", getpid());
	struct sigaction act;
       
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = sig_handler;
       
	act.sa_flags = SA_SIGINFO;
	/*
	for (int i = 1; i <= 32; i++)
	{       
		if (i == 2 || i == 9 || i == 19 || i == 32) continue;
	       
		if(sigaction(i, &act, NULL)<0)
		{
			printf("Install SIG[%d] Action Error:%s\n\a", i, strerror(errno));
		       
			return (1);
		}
	}
	*/
	sigaction(SIGALRM, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
       
	if (alarm(3) != 0)
	{
		printf("Set Alarm Error:%s\n\a", strerror(errno));
	       
		return (1);
	}
	printf("set alarm in 3 sec\n");
    printf("SIGALARM=%d SIGUSR1=%d", SIGALRM, SIGUSR1);   
	//sleep(-1); //这里已经无限期等待了
	while(1)
	{
		;
	}
	return 0;
}

void sig_handler(int signo, siginfo_t * info, void * p)
{
	printf("[Signal]::sig_handler(): SIG [%d] received !\n", signo);
       
	if (signo == SIGALRM)
	{
		printf("sig_handler: recv signal = %d\n", signo);
		pid_t pid = getpid();
		printf("sig_handler: pid = %d\n", pid);
		int sig = SIGUSR1;
		char pdata[16] = {0};
		union sigval sigv;
		strcpy(pdata, "Hello, Signals");
		sigv.sival_ptr = (void*)pdata;
		/*
		   if (0 != sigqueue(getpid(), sig, sigv))		//向本进程发送信号，并传递附加信息
		{
			printf("sigqueue error! pid = %d :%d %s\n\a", getpid(), errno, strerror(errno));
		}
		*/
		return ;
	}

	if (signo == SIGUSR1)
	{
		printf("sig_handler: recv signal = %d\n", signo);
		if (!info)
		{
			printf("sig_handler: recv info = NULL\n");
			return;
		}
		printf("sig_handler: recv [%s] \n", (char*)(info->si_ptr));
	}       
}

