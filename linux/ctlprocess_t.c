/*
 * usage:	test processs control in linux(unix)
 * author:	zuohaitao
 * date:	2008/07/19
 * thx:		
 *			the example for testing atexit() from man
 * modify:	fix exec_t() bug
 *			add switch test in main()
 * date:	2008 12 31
 * author:	zuohaitao
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
void fork_t();
void atexit_t();
void wait_t();
void exec_t();
void signal_t();
int main(int argc, char* argv[])
{
	int nWhich;
	printf("############################\n");
	printf("Which Test?					\n");
	printf("# 1)test fork()             \n");
	printf("# 2)test wait()				\n");
	printf("# 3)test atexit()           \n");
	printf("# 4)test exec()             \n");
	printf("# 5)test signal()           \n");
	printf("############################\n");
	nWhich = getchar();
	switch(nWhich)
	{
		case '1':
			fork_t();
			break;
		case '2':
			wait_t();
			break;
		case '3':
			atexit_t();
			break;
		case '4':
			exec_t();
			break;
		case '5':
			signal_t();
			break;
		default:
			printf("bad input!\n");
	}
	return 0;
}
void fork_t()
{
	pid_t fatherpid;
	pid_t childpid;
	if ((childpid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (0 == childpid)
	{
		printf("this is child!\n");
	}
	else
	{
		printf("this is father!\n");
		//wait child exit!!
		sleep(2);
	}
	return ;
}
void bye(void)
{
	printf("bye!\n");
}
void atexit_t()
{
	int i;
	i = atexit(bye);
    if (i != 0)
	{
		perror("ateexit()");
        return;
    }

}

void wait_t()
{
	pid_t fatherpid;
	pid_t childpid;
	int i;
	int status;
	if ((childpid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (0 == childpid)
	{
		printf("this is child!\n");
		for (i = 0; i < 100; i++)
		{
			printf("lession %d in school\n", i);
		}
	}
	else
	{
		printf("this is father in home!\n");
		//wait child exit!!
		
		if (waitpid(childpid, &status, 0) < 0)
		{
			exit(0);
		}
		if (WIFEXITED(status))
		{
			printf("wait child go home!\n");
		}
		printf("father cooks!\n");
	}
	return ;
}

void exec_t()
{
	pid_t fatherpid;
	pid_t childpid;
	int i;
	int status;
	if ((childpid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (0 == childpid)
	{
		printf("this is child!\n");
		if (execl("/bin/ls", "ls", "-l", ".") < 0)
		{
			perror("execl");
			exit(1);
		}
	}
	else
	{
		printf("this is father!\n");
		//wait child exit!!
		
		if (waitpid(childpid, &status, 0) < 0)
		{
			exit(0);
		}
		if (WIFEXITED(status))
		{
			printf("wait child exit!\n");
		}
		printf("father exit!\n");
	}
	return ;
}


typedef void (*SIGNALFUN)(int);
void tel(int value)
{
	printf("signal!\n");
	if (SIGUSR1 == value)
	{
		printf("father to child\n");
	}
	if (SIGCHLD == value)
	{
		printf("child to father\n");
		int status;
		if (wait(&status) < 0)
		{
			perror("wait()\n");
			exit(0);
		}
		if (WIFEXITED(status))
		{
			printf("child finishes his lessons!\n");
		}
		printf("father cooks!\n");
		exit(0);
	}
}
void signal_t()
{
	pid_t fatherpid;
	pid_t childpid;
	int i;
	int status;
	printf("this is a rich family and they have telephones!\n");
	if (SIG_ERR == signal(SIGCHLD, tel))
	{
		perror("signal() telephone is broken!\n");
		exit(1);
	}
	if (SIG_ERR == signal(SIGUSR1, tel))
	{
		perror("signal() telephone is broken!\n");
		exit(1);
	}
	if ((childpid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (0 == childpid)
	{
		printf("this is child in school!\n");
		pause();
		for (i = 0; i < 10; i++)
		{
			printf("lession %d in school\n", i);
			sleep(1);
		}

	}
	else
	{
		printf("this is father in home!\n");
		//tell child that father is cooking and good good study day day up!
		if (kill(childpid, SIGUSR1))
		{
			perror("kill\n");
			exit(1);
		}
		//wait child exit!!
		while(1)
		{
			printf("father watches TV! DO NOT WATCH CCTV CHANNEL:)\n");
			//check telephone!
			//pause();
			sleep(2);
		}
	}
	return ;
}



