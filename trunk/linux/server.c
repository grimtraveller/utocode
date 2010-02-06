/*
 * Name:	server.c
 * Desc:	
 * Usage:	$>./server
			$>telnet 127.0.0.1
 * Author:	zuohaitao
 * Date:	2009-03-13
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
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
	char buf[1024] = {0};
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
		if (acceptfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen) < 0)
		{
			perror("accept()");
		}

		inet_ntop(AF_INET, &(clientaddr.sin_addr), clientip, INET_ADDRSTRLEN);
		printf("IP:%s PORT:%d\n", clientip, ntohs(clientaddr.sin_port));
		write(accept, buf, strlen(buf));
		memset(buf, 0, 1024);
		read(accept, buf, 1024);
		printf("read()=%s\n", buf);
	}
	return 0;
}

