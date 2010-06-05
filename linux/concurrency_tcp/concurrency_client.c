/*
 * name:	concurrency_client.c
 * desc:	concurrency tcp communicate client 
 * usage:	$gcc concurrency_client.c -o concurrency_client
			$concurrency_client
 * author:	zuohaitao
 * date:	2008/08/15
 *
 */

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
#ifdef WIN32
#else
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#include <stdio.h>
#define MAXLINE 1000
int main()
{
    int socketfd;
    struct sockaddr_in servaddr;

    int n;
    char str[MAXLINE];
    char c;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        return 0;
    }
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
        if (0 > write(socketfd,str,strlen(str)))
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
    close(socketfd);
    return 0;
}
