/*
 * Name:	pip_t.c
 * Desc:	parent process write to pipe;
			child process read from pipe.
 * Usage:	
 * Author:	zuohaitao
 * Date:
 */
#include <error.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define MAXLINE 1024
int main(void)
{
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];

	if (pipe(fd) < 0)
	{
		perror("pipe()");
	}

	if ((pid = fork()) < 0)
	{
		perror("fork()");
	}
	else if (pid > 0)
	{
		//parent
		close(fd[0]);
		sleep(10);
		write(fd[1], "hello pipe world\n", 17);
	}
	else 
	{
		//child

		close(fd[1]);
		memset(line, 0, MAXLINE);
		n = read(fd[0], line, MAXLINE);
		printf("%s", line);
	}
	return 0;
}
