/*
 * Name:	lock_t.c
 * Descr:	test record lock
			usurally, it is a advisory lock
 * Usage:	$>touch  touch lock_t_use.lock
			$>gcc gcc lock_t.c -o lock_t
			$>./lock_t
 * thanks:	<<APUE>>
 * Date:	2008 12 31
 */
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;
	return fcntl(fd, cmd, &lock);
}
#define read_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)

#define readw_lock(fd, offset, whence, len)	\
	lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)

#define write_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)

#define writew_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)

#define un_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;

	if (fcntl(fd, F_GETLK, &lock) < 0)
	{
		perror("fcntl()");
		exit(0);
	}
	if (F_UNLCK == lock.l_type )
	{
		return 0;
	}
	return lock.l_pid;
}
#define is_readlock(fd, offset, whence, len)\
	lock_test(fd, F_RDLCK, offset, whence, len)
#define is_writelock(fd, offset, whence, len)\
	lock_test(fd, F_WRLCK, offset, whence, len)
int main(int argv, char* argc[])
{
	int fd;
	pid_t pid, parentpid, childpid;
	int status;
	char t = 0;
	parentpid = getpid();
	printf("parent 0x%x\n",parentpid);
	if ((fd = open("lock_t_use.lock", O_RDWR)) < 0)
	{
		perror("open()");
		return 0;
	}
	if ((pid = fork()) < 0)
	{
		perror("fork()\n");
	}
	else if (0 == pid)
	{
		//Record locks are not inherited by a child created via fork(2), but are preserved across an execve(2).
		printf("child 0x%x\n", (childpid=getpid()));
		/*
		while(1)
		{
			if ((lockpid = is_readlock(fd, 0, SEEK_SET, 1)) < 0)
			{
				perror("is_readlock\n");
			}
			else if (0 == lockpid)
			{
				printf("===unlock===\n");	
				break;
			}
			else
			{
				printf("child:0x%x is locking\n", lockpid);
				sleep(1);
			}
		}
		*/
		while(1)
		{
			sleep(3);
			if (write_lock(fd, 0, SEEK_SET, 1) < 0)
			{
				if (errno == EACCES || errno == EAGAIN)
				{
					printf("0x%x say:Already locked by another process\n", childpid);
					sleep(1);
					continue;
				}
				else
				{
					perror("write_lock()\n");
					exit(1);
				}
			}
			else
			{
				printf("0x%x say:Get Lock!!\n", childpid);
				break;
			}
		}
	}
	else
	{
		if (write_lock(fd, 0, SEEK_SET, 1) < 0)
		{
			perror("write_lock");
		}
		else
		{
			printf("0x%x say: Get Lock!\n", parentpid);
		}

		sleep(10);
		if (un_lock(fd, SEEK_SET, 0, 0) < 0)
		{
			perror("un_lock()");
		}
		else
		{
			printf("0x%x say:release lock!\n", parentpid);
		}
		wait(&status);
	}
	close(fd);
	return 0;
}

