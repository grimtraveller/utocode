#include "pfxlock.h"
#include "pfxerror.h"
#include "../include/public.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void)
{
	pid_t pid;

	int lockid;
	pid_t waitvalue;
	int status;
	int ret;
	//make a semaphore
	ret = lockid = mklock((int)PRSNTLIST_KEY);
	printf("lockid=%d\n", lockid);
	if (lockid < 0)
	{
		showpfxerr();
		perror("mklock");
		return 1;
	}


	if ((pid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (pid == 0)
	{
		printf("lockid=%d\n", lockid);
		//child
		sleep(1);
		printf("child process say: hello sem\n");
		//test semaphore...
		printf("wait_unlock\n");
		ret = wait_unlock(lockid);
		if (ret < 0)
		{
			showpfxerr();
			perror("wait_unlock\n");
			return 0;
		}
		//begin
		printf("begin\n");
		sleep(2);
		printf("finish\n");
		//finish
		return 1;
	}
	else
	{
		//parent
		printf("parent process say: hello sem\n");
		//begin do semaphore...
		printf("lock\n");
		ret = lock(lockid);
		if (ret < 0)
		{
			showpfxerr("lock()");
			perror("lock()");
			return 1;
		}
		sleep(5);
		//finish undo semaphore...
		printf("unlock\n");
		ret = unlock(lockid);
		if (ret < 0)
		{
			showpfxerr("unlock()");
			perror("semop()\n");
			return 1;
		}
		while(1)
		{
			waitvalue = wait(&status);
			if (waitvalue < 0)
			{
				perror("wait()");
				return 0;
			}
			else if (pid == waitvalue)
			{
				if (WIFEXITED(status))
				{
					break;
				}
			}
			else
			{
				continue;
			}

		}
		printf("press any key to continue...\n");
		getchar();
		//remove a semaphore
		ret = rmlock(lockid);
		if (ret < 0)
		{
			showpfxerr();
			perror("rmlock");
		}
		return 1;
	}
	return 0;
}
