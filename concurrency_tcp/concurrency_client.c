/**
  @file		concurrency_client.c
  @brief	concurrency tcp communicate client
  @details	
			usage:
				$gcc concurrency_client.c -o concurrency_client
				$concurrency_client
			modify:
				why porting to win32 os
  @author	zuohaitao
  @date		2008/08/15
  @warning	
  @bug		
  */
#include <stdio.h>
#define MAXLINE 1000
#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
int main()
{
    int socketfd;
    struct sockaddr_in servaddr;

    int n;
    char str[MAXLINE];
    char c;
#ifdef WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (NO_ERROR != result)
	{
		printf("Error at WSAStartup()\n");
		exit(0);
	}
#endif
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
#ifndef WIN32
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        return 0;
    }
#else 
	//addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1"); 
	servaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#endif
    servaddr.sin_port = htons(1212);

    if (connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        return 0;
    }
    while(1)
    {
        char* p = str;
        while (1)
        {
            c = getchar();
            if (c == '\n')
            {
                break;
            }
            else
            {
                *(p++) = c;
            }
        }
        *p = 0;
#ifdef WIN32
		if (send(socketfd, str, strlen(str), 0))
#else
        if (0 > write(socketfd,str,strlen(str)))
#endif
        {
            perror("write");
            return 0;
        }
        if (0 == strncmp(str, "bye", 3))
        {
            break;
        }
        //printf("%s", str);
    }
#ifdef WIN32
    close(socketfd);
#else
	closesocket(socketfd);
	WSACleanup();
#endif
    return 0;
}
