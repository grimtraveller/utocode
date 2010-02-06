/*
 * usage:	make a zombie child process
 *			1 run it
			2 $ps -x
				6992 pts/0    R+     0:07 zombie
				6993 pts/0    Z+     0:00 [zombie] <defunct>
			3 enjoy it?
 *
 */
#include <stdio.h>
#include <unistd.h>
int main(int argc,char **argv)    
{
	pid_t pid;    
	pid=fork();
	if (0 > pid)
	{
		printf("pid=%d\n", pid);
		perror("fork()");
	}
	if (0 ==pid)
	{
		printf("childern process exit and change a zombie process\n");
		return 0;
	}
	else
	{
		printf("father process!\n");
	}
	while(1)
	{
		;
	}    
	return 0;    
} 
