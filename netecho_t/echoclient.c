/*
 * name:	echoclient.c
 * desc:	write message to server and read message from server and show it
 * usage:	$gcc echoclient.c -o echoclient
			$echoclient 127.0.0.1 1202
			input "bye" to exit
 * author:	zuohaitao
 * date:	2008/08/14
 * useage:	$>cat send.txt|clinet 127.0.0.1 1212
 *
 */
#ifdef WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif //WIN32
#include <stdio.h>
#include <string.h>
void __echo_client(int fd);
int main(int argc, char* argv[])
{
	int connfd;
	char servip[INET_ADDRSTRLEN + 1] = {0};
	short int servport;
	struct sockaddr_in servaddr = {0};
	int servaddrlen;	
	if (argc != 3)
	{
		printf("usage: echoclient ip port\n");
		return 1;
	}
	strncpy(servip, argv[1], INET_ADDRSTRLEN);
	servport = atoi(argv[2]);

	if ((connfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		return 1;
	}
	servaddr.sin_family = AF_INET;
	strcpy(servip, "127.0.0.1");
#ifdef WIN32
	if (INADDR_NONE == inet_addr(servip))
	{
		perror("inet_aton");
	}
#else
	if (inet_aton(servip, (struct in_addr*)&servaddr.sin_addr) < 0)
	{
		perror("inet_aton");
	}
#endif //WIN32
	servport = 1202;
	servaddr.sin_port = htons(servport);
	servaddrlen = sizeof(servaddr);

	if (connect(connfd, (const struct sockaddr*)&servaddr, servaddrlen) < 0)
	{
		perror("connect()");
		return 1;
	}

	__echo_client(connfd);
	close(connfd);
	return 0;
}

void __echo_client(int connfd)
{
	char buf[1024] = {0};
	int nlen = 0;
	int readlen = -1;
#ifdef WIN32
	unsigned int netlong;
#else
	uint32_t netlong;
#endif
	while(1)
	{
		fgets(buf, 1024, stdin);
		if (0 == strncmp(buf, "bye", 3))
		{
			netlong = htonl(0);
#ifdef WIN32
			if (write(connfd, &netlong, sizeof(unsigned int)) < 0)
#else
			if (write(connfd, &netlong, sizeof(uint32_t)) < 0)
#endif //WIN32
			{
				perror("write()");	
			}
			break;
		}

		nlen = strlen(buf);
		netlong = htonl(nlen);
#ifdef WIN32
		if (write(connfd, &netlong, sizeof(unsigned int)) < 0)
#else
		if (write(connfd, &netlong, sizeof(uint32_t)) < 0)
#endif //WIN32
		{
			perror("write()");
			return;
		}
		if ((nlen = write(connfd, buf, nlen)) < 0)
		{
			perror("write()");
			return;
		}
		printf("write \n");
		memset(buf, 0 , 1024);
		if ((readlen = read(connfd, buf, nlen)) < 0)
		{
			perror("read()");
			return;
		}
		/*
		   else if (0 == readlen )
		{
			perror("read()=0");
		}
		*/
		printf("%s\n", buf, nlen);
	}
	return;
}
	


