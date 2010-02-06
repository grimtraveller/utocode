/*
 * name: signal_t.c
 * desc: test signal and know the  Portablility of signal
 * usage:
		 1. compile
			$gcc signal_t.c -o signal_t
			$gcc signal_t.c -DSYSV signal_t_sysv
		 2. run
			$signal_t
			$^c(ctrl+c)
			input "ctrl+c" 10 times
			
			$signal_t_sysv
			$^c(ctrl+c)
			input "ctrl+c" 2 times
		 
		 3. termilate process
			$> kill `ps -a|grep "signal_t" | awk '{print $1}'`
		 
		 4. result
			bsd unix is only answer 2 times sigal,the other signals is ignore
			sysv unix is answer signal is SIG_DEF at the second times
 * author: zuohaitao
 * date: 2008/08/19
 *
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
void doSIGQUIT(int xsignal)
{
	if (SIGINT == xsignal)
	{
		printf("SIGINT\n");
		sleep(10);
		//doSIGQUIT(xsignal);
		//exit(0);
	}
}
int main()
{
#ifdef SYSV
	sysv_signal(SIGINT, doSIGQUIT);
#else
	signal(SIGINT, doSIGQUIT);
#endif
	while(1)
	{
		sleep(1);
		printf("wait SIGINT...\n");
	}
	return 0;
}
