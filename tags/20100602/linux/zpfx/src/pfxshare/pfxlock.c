/*
 *	name:	pfxlock.c
 *	desc:	lock shared resource
 *	author:	zuohaitao [at] shxy
 *	date:	2008/08/03
 *	usage:	see pfxshare_t.c
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "pfxerror.h"
#include <error.h>
#include <string.h>
#include <errno.h>
int mklock(key_t key);
int lock(int lockid);
int unlock(int lockid);
int wait_unlock(int lockid);
//flg=1 lock
//flg= -1 unlock
int ctllock(int lockid, int flg);
int rmlock(int lockid);


int mklock(key_t key)
{
	int semid;
	if ((semid = semget(key, 1, IPC_CREAT|0777)) < 0)
	{
		setpfxerr(syserr, semid, strerror(semid));
	}
	return semid;
}

int wait_unlock(int lockid)
{
	int ret;
	struct sembuf sem = {0};
	sem.sem_num = 0;
	sem.sem_op = 0;
	sem.sem_flg = ~IPC_NOWAIT;
	ret = semop(lockid, &sem, 1);
	if (ret < 0)
	{
		setpfxerr(syserr, errno, strerror(errno));
	}
	return ret;
}

int cltlock(int lockid, int flg)
{
	int ret;
	struct sembuf sem = {0};
	if ((1 != flg) &&(-1 != flg))
	{
		setpfxerr(pfxerr, 1, "parament err");
		ret = -1;
	}
	sem.sem_num = 0;
	sem.sem_op = flg;
	sem.sem_flg = SEM_UNDO;
	ret = semop(lockid, &sem, 1) ;
	if (ret < 0)
	{
		setpfxerr(syserr, errno, strerror(errno));
	}
	return ret;
}
int lock(int lockid)
{
	return cltlock(lockid, 1);
}

int unlock(int lockid)
{
	return cltlock(lockid, -1);
}

int rmlock(int lockid)
{
	int ret;
	union semun
	{
		int val;
		struct semid_ds* buf;
		unsigned short* array;
		struct seminfo* __buf;
	};
	union semun semun;
	ret = semctl(lockid, 1, IPC_RMID, semun);
	if (ret < 0)
	{
		setpfxerr(syserr, errno, strerror(errno));
	}
	return ret;
}
