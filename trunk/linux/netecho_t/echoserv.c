/*
 * name:	echoserv.c
 * descr:	read message from client and write message to client
			for studing net programming in like-unix os.
 * usage:	$gcc echoserv.c -o echoserv
			$serv
 * author:	zuohaitao 
 * date:	2008/08/14
 * modify:	add serv show information from client
 *			$serv|/bin/sh
 *			$serv > recv.txt
 * date:	2008 12 31
 * author:	zuohaitao
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
void __echo_serv(int accept);
void doSIGCHLD(int xsignal);
int main()
{
	int ret;
	int listenfd;
	int acceptfd;
	struct sockaddr_in servaddr = {0};
	struct sockaddr_in clientaddr;
	char servip[] = "127.0.0.1";
	int servport = 1202;
	char clientip[INET_ADDRSTRLEN+1] = {0};
	int clientaddrlen;
	if (SIG_ERR == signal(SIGCHLD, doSIGCHLD))
	{
		perror("signal");
		return 0;
	}
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		return  0;
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*
	if ((ret =inet_pton(AF_INET, servip, &(servaddr.sin_addr))) <= 0)
	{
		perror("inet_pton()");
		return 0;
	}
	*/
	servaddr.sin_port = htons(servport);

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind()");
		return 0;
	}

	if (listen(listenfd, 5) < 0)
	{
		perror("listen()");
		return 0;
	}
	while(1)
	{
		clientaddrlen = sizeof(clientaddr);
		if ((acceptfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen)) < 0)
		{
			perror("accept()");
		}

		inet_ntop(AF_INET, &(clientaddr.sin_addr), clientip, INET_ADDRSTRLEN);
		printf("IP:%s PORT:%d\n", clientip, ntohs(clientaddr.sin_port));
		//write(accept, buf, strlen(buf));
		__echo_serv(acceptfd);
		//memset(buf, 0, 1024);
		//read(accept, buf, 1024);
		//printf("read()=%s\n", buf);
	}
	close(acceptfd);
	return 0;
}

void __echo_serv(int accept)
{
	long nlen;
	uint32_t netlong;
	char* pbuf = 0;
	char buf[1024] = {0};
	while(1)
	{
		if ((nlen = read(accept, &netlong, sizeof(uint32_t))) < 0)
		{
			perror("read()");
			break;
		}
		nlen = ntohl(netlong);
		//printf("%d\n", nlen);
		if (0 == nlen)
		{
			break;
		}
		pbuf = (char*)malloc(nlen);
		if (0 == pbuf)
		{
			perror("malloc");
			return;
		}
		if (read(accept, pbuf, nlen) < 0)
		{
			free(pbuf);
			pbuf = 0;
			perror("read()");
			return;
		}
		if(write(fileno(stdout), pbuf, nlen) < 0)
		{
			free(pbuf);
			pbuf = 0;
			perror("write()");
		}
		//printf("%s\n", pbuf);
		if (write(accept, pbuf, nlen) < 0)
		{
			free(pbuf);
			pbuf = 0;
			perror("write()");
		}
		free(pbuf);
		pbuf = 0;
	}
}
void doSIGCHLD(int xsignal)
{
}
