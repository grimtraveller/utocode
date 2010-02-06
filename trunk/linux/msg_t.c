/*
 *Name:		msg_t.c
 *Desc:		SystemV message
 *Usage:	$>gcc msg_t.c -o msg_rcv
			$>gcc msg_t.c -DSND -o msg_snd
			$>msg_snd
			$ipcs -q
			$>msg_rcv
			$ipcs -q
			ipcrm -Q id //may be useful
 *Date:		2008 12 31
 *Author:	zuohaitao
v1.1
 *when:		2009-03-12
 *where:	#define main()
 *who:		zuohaitao
 *what:		add study ftok() for create key
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
//#define IPC_KEY 1202
#define MAX_PATH		260
int main(int argc, char* argv[])
{
	int msgid;
	char* pdata;
	size_t datalen;
	datalen = 100;
	//add by zuohaitao for v1.1
	struct stat status;
	key_t key;
	char fullname[MAX_PATH];
	if (0 == getcwd(fullname, MAX_PATH))
	{
		perror("getcwd");
		exit(1);
	}
	strcat(fullname, "/");
	//strcat(fullname, argv[0]);
	if (stat(fullname, &status) < 0)
	{
		perror("stat");
		exit(1);
	}
	key = ftok(fullname, IPC_PRIVATE);
	printf("path=%s st_dev=%lx st_ino=%lx key=%lx\n", 
			fullname, (u_long)status.st_dev, (u_long)status.st_ino, key);
	//end add
	pdata = (char*)malloc(100 * sizeof(char));
	strcpy(pdata, "hello System V msgqueue world!\n");
	//add by zuohaitao v1.1
	//if((msgid = msgget(IPC_KEY, IPC_CREAT|0777)) < 0)
	if((msgid = msgget(key, IPC_CREAT|0777)) < 0)
	//end add
	{
		perror("msgget()");
	}
#ifdef SND
	if (msgsnd(msgid, pdata, datalen, IPC_NOWAIT) < 0)
	{
		perror("msgsnd()");
	}
#else
	//modify by zuohaitao for v1.0
	while(1)
	{
		memset(pdata, 0, datalen);
		if (msgrcv(msgid, pdata, datalen, 0, IPC_NOWAIT) < 0)
		{
			if (ENOMSG != errno)
			{
				perror("msgrcv()");
			}
			else
			{
				printf("no message!\n");
			}
			break;
		}
		printf("%s\n", pdata);
	}
	//end mod
	free(pdata);
	if (msgctl(msgid, IPC_RMID, 0) < 0)
	{
		perror("msgctl(IPC_RMID)");
	}
#endif

	return 0;
}
