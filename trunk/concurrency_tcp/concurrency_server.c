/*
 * name:	concurrency_server.c
 * desc:	concurrency tcp comunicate server
 * usage:	$gcc concurrency_server.c -o concurrency_server
			$concurrency_server
 * author:	zuohaitao
 * date:	2008/08/15
v1.1
 * what:	avoid zombi child process
 * who:		zuohaitao
 * when:	2009-06-28
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#define MAXLINE 1000
#define LISTENQ 5
typedef void (*sighandler_t)(int);
void doChild(int sig)
{
	int status;
	if (wait(&status) < 0)
	{
		perror("wait");
	}
	printf("children exit!\n");
	return;
}

int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t cliaddrlen = 0;
    char straddr[12] = {0};
    pid_t pid;


    char str[MAXLINE];
    int recvlen;
    char* p;
    int n;
	if (SIG_ERR == signal(SIGCHLD, doChild))
	{
		perror("signal");
		return 0;
	}
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1212);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))< 0)
    {
        perror("bind");
        return 0;
    }

    if (listen(listenfd, LISTENQ) < 0)
    {
        perror("listen");
        return 0;
    }
    while(1)
    {
        cliaddrlen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen))< 0)
        {
			//The system call was interrupted by  a  signal  that  was  caught
            //before a valid connection arrived.
            if (errno == EINTR)//???no block and no connect???
            {
                continue;
            }
            perror("accept");
            return 0;
        }
        memset(straddr, 0, sizeof(straddr));
        if (0 == inet_ntop(AF_INET, &(cliaddr.sin_addr), straddr,sizeof(cliaddr)))
        {
            perror("inet_ntop");
        }
        else
        {
            printf("client IP:%s\n", straddr);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            close(listenfd);
            memset(str, 0, MAXLINE);
            recvlen = 0;
            p = str;
            while(1)
            {
                n = read(connfd, str, MAXLINE);
                if (n < 0)
                {
                    perror("read");
                }
                str[n] = 0;
                printf("%s\n", str);
                if (0 == strncmp(str, "bye", 3))     exit(0);
            }
			close(connfd);
        }
        close(connfd);
    }
    return 0;
}
