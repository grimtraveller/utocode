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
	char name[20]; //�õ���������
	char ipbuf[16]; // ����ip��ַ
	struct addrinfo hints,*addr;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_flags=AI_CANONNAME;
	hints.ai_flags=AI_ADDRCONFIG;

	gets(name); //�������Ҫ���ҵ�������
	if((rc=getaddrinfo(name,NULL,&hints,&addr))==0){
	do{
		printf("ip: %s,host: %s\n",inet_ntop(AF_INET,&(((struct sockaddr_in*)addr->ai_addr)->sin_addr),ipbuf,sizeof(ipbuf)),addr->ai_canonname);
	}while((addr=addr->ai_next)!=NULL); //��ӡ�ҵ���������ַ��������
	return 0;
	}
	printf("%d\n",rc);//������ʱ��rcֵ
	return 0;
}
