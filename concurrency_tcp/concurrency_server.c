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
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#else
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif //WIN32
#define MAXLINE 1000
#define LISTENQ 5
#ifndef WIN32
typedef void (*sighandler_t)(int);
void doChildExit(int sig)
{
	int status;
	if (wait(&status) < 0)
	{
		perror("wait");
	}
	printf("children exit!\n");
	return;
}
#endif //WIN32
int main()
{
#ifndef WIN32
    socklen_t cliaddrlen = 0;
    char straddr[12] = {0};
    pid_t pid;
#else
	int cliaddrlen = 0;
	char* straddr = NULL;
#endif //WIN32
    int listenfd, connfd;
    struct sockaddr_in servaddr,cliaddr;
    char str[MAXLINE];
    int recvlen;
    char* p;
	int n;
#ifdef WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (NO_ERROR != result)
	{
		printf("Error at WSAStartup()\n");
		exit(0);
	}
#endif //WIN32
#ifndef WIN32
	if (SIG_ERR == signal(SIGCHLD, doChildExit))
	{
		perror("signal");
		return 0;
	}
#endif //WIN32
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
#ifndef WIN32
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
#else
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
#endif //WIN32
#ifndef WIN32
        memset(straddr, 0, sizeof(straddr));
        if (0 == inet_ntop(AF_INET, &(cliaddr.sin_addr), straddr,sizeof(cliaddr)))
        {
            perror("inet_ntop");
        }
        else
        {
            printf("IP:%s\n", straddr);
        }
#else
		straddr = inet_ntoa(cliaddr.sin_addr);
		printf("IP:%s\n", straddr);
#endif //WIN32

#ifndef WIN32
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
//////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////
        }
        close(connfd);
#else
	closesocket(connfd);
#endif //WIN32
    }
#ifdef WIN32
	WSACleanup();
#endif //WIN32
    return 0;
}
