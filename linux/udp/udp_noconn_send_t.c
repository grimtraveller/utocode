/*
 * name:		udp_noconn_send.c
 * descrption:	study socket api about UDP
 * usage:		1 gcc udp_noconn_send.c -Wall -o udp_noconn_send
 * author:		zuohaitao
 * date:		2008/07/31
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in to = {0};
	int toport = 1202;
	char toip[] = "127.0.0.1";
	char msg[100]= {0};
	if (1 == argc)
	{
		strcpy(msg, "hello udp no connect!\n");
	}
	else
	{
		if (strlen(argv[1]) > 100)
		{
			printf("msg too long!\n");
			return 0;
		}
		else
		{
			strncpy(msg, argv[1], 100);
		}
	}
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
	}

	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr(toip);
	to.sin_port = htons(toport);
	if (sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&to, sizeof(to)) < 0)
	{
		perror("sendto()");
	}
	
	return 0;
}
