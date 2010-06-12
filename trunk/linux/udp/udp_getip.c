/*
 * name:		udp_getip.c
 * descrption:	study socket api about UDP with connect
 * usage:		1 gcc udp_noconn_udp_getip.c -Wall -o udp_noconn_getip
 * author:		zuohaitao
 * date:		2008/07/31
 *
 */
#ifdef WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <Inaddr.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif //WIN32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argv, char* argc[])
{
	char servip[16] = {0};
	char* localip = 0;
	short int port = 0;
	int sockfd;
	struct sockaddr_in servaddr = {0};
	struct sockaddr_in localaddr = {0};
#ifdef WIN32
	int len;
#else
	socklen_t len;
#endif //WIN32
#ifdef WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (NO_ERROR == result)
	{
		printf("Error at WSAStartup()\n");
		exit(0);
	}
#endif

	if (argv < 3)
	{
		printf("getinetip ip port\n");
		return 0;
	}
	strcpy(servip, argc[1]);
	servip[15] = 0;
	port = atoi(argc[2]);
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0) ) < 0)
	{
		perror("socket()");
		return 0;
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(servip);
	servaddr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect()");
		return 0;
	}
	len = sizeof(localaddr);
	if (getsockname(sockfd, (struct sockaddr*)&localaddr, &len) < 0)
	{
		perror("getsockname()");
		return 0;
	}
#ifdef WIN32
	localip = inet_ntoa(localaddr.sin_addr);
#else
	localip = inet_ntoa((struct in_addr)localaddr.sin_addr);
#endif //WIN32
	if (0 == localip )
	{
		perror("inet_ntoa()");
		return 0;
	}
	printf("local address %s\n", localip);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_UNSPEC;
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	//free(localip);
	return 0;
}
