/*
 * name:		udp_noconn_recv.c
 * descrption:	study socket api about UDP
 * usage:		1 gcc udp_noconn_recv.c -Wall -o udp_noconn_recv
 * author:		zuohaitao
 * date:		2008/07/31
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
int main()
{
	int sock;
	struct sockaddr_in serv = {0};
	struct sockaddr_in from = {0};
	socklen_t fromlen = 0;
	int servport = 1202;
	char recvbuf[100] = {0};

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
	}
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htons(INADDR_ANY);
	serv.sin_port = htons(servport);
	if (bind(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0)
	{
		perror("bind()");
	}
	fromlen = sizeof(from);
	while(1)
	{
		if (recvfrom(sock, recvbuf, 100, 0, (struct sockaddr*)&from, &fromlen) < 0)
		{
			perror("recvfrom()");
		}
		printf("from:%s:%d say:\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		printf("\t%s\n", recvbuf);
		memset(&from, 0, fromlen);
	}
	return 0;
}
