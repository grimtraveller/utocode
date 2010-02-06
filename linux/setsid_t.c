/*
 * Name:	setid_t.c
 * Desc:	if child process call setsid(), its pgroupid will change.
 * Usage:	
 * Author:	zuohaitao
 * Date:
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
	char c;
	pid_t sid;
	pid_t pid;
	int status;
	pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		return 1;
	}
	else if (pid > 0)
	{
		//parent
		printf("[before]parent:pid=%d,grpid=%d\n", getpid(), getpgrp());
		wait(&status);
		printf("[after]parent:pid=%d,grpid=%d\n", getpid(), getpgrp());
		return 0;
	}
	//child
	printf("[before]child:pid=%d,grpid=%d\n",getpid(), getpgrp());
	sid = setsid();
	printf("[after]child:pid=%d,grpid=%d\n",getpid(), getpgrp());
	if (sid < 0)
	{
		perror("setsid()");
		return 1;
	}
	printf("sid=%d\n", sid);
	scanf("%c", &c);
	printf("%c\n", c);
	return 0;
}
