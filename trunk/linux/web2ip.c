#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>


int main(void){
	int rc;
	char name[20]; //得到的主机名
	char ipbuf[16]; // 保存ip地址
	struct addrinfo hints,*addr;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_flags=AI_CANONNAME;
	hints.ai_flags=AI_ADDRCONFIG;

	gets(name); //从输入得要查找的主机名
	if((rc=getaddrinfo(name,NULL,&hints,&addr))==0){
	do{
		printf("ip: %s,host: %s\n",inet_ntop(AF_INET,&(((struct sockaddr_in*)addr->ai_addr)->sin_addr),ipbuf,sizeof(ipbuf)),addr->ai_canonname);
	}while((addr=addr->ai_next)!=NULL); //打印找到的主机地址和主机名
	return 0;
	}
	printf("%d\n",rc);//检查出错时的rc值
	return 0;
}
