/**
 * Name:		sigtstp_t.c
 * Descr:		test SIGTSTP and signal
 *				when the process is hangiing by SIGTSTP, send SIGCONT to the 
 *				process.the process continue to run from stop position.
 * usage:		$>gcc sigtstp_t.c -o sigtstp_t
 *				$>./sigtstp_t
 *				$><ctrl><z> to send SIGTSTP signal
 *				$>kill -18 `ps -A|grep "sigtstp_t"|awk '{print $1}'` to send
 *				SIGCONT signal
 *				$>kill `ps -A|grep "sigtstp_t"|awk '{print $1}'` to send to kill process
 * Date:		2010-03-08
 * Author:		zuohaitao
 **/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void handler(int sig_num)
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	if (SIGTSTP == sig_num)
	{
		printf("\n [SIGNAL]get SIGTSTP \n");
		sa.sa_handler = SIG_DFL;
		sigaction(SIGTSTP, &sa, NULL);
		kill(getpid(), SIGTSTP);
	}
	else
	{
		printf("\n [SIGNAL]get SIGCONT \n");
		sa.sa_handler = handler;	
		sigaction(SIGTSTP, &sa, NULL);
	}
	return;
}
int
main(int argc, char* argv[])
{
	struct sigaction sa;
	struct sigaction old;
	int i = 0;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = handler;
	sigaction(SIGTSTP, &sa, &old);
	sigaction(SIGCONT, &sa, &old);
	while(1)
	{
		printf("%d ", i);
		fflush(0);
		sleep(1);
		i++;
	}
	return 0;
}
