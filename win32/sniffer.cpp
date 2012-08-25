#include <winsock2.h> 
#pragma comment(lib,"ws2_32")
#include <ws2tcpip.h>
#include <mstcpip.h> 
#define _TOSTR(x) #x
#define TOSTR(x) _TOSTR(x)
#include <stdio.h>

typedef struct ip_hdr 
{ 
	unsigned char h_verlen; // 4位首部长度,4位IP版本号 
	unsigned char tos;    // 8位服务类型TOS 
	unsigned short total_len;       // 16位总长度（字节） 
	unsigned short ident;      // 16位标识 
	unsigned short frag_and_flags;  // 3位标志位 
	unsigned char ttl;       // 8位生存时间 TTL 
	unsigned char proto;      // 8位协议 (TCP, UDP 或其他) 
	unsigned short checksum;  // 16位IP首部校验和 
	unsigned int sourceIP;   // 32位源IP地址 
	unsigned int destIP;   // 32位目的IP地址 
} IpHeader;

typedef struct tcp_hdr 
{ 
	USHORT th_sport;     // 16位源端口 
	USHORT th_dport;     // 16位目的端口 
	unsigned int th_seq;    // 32位序列号 
	unsigned int th_ack;    // 32位确认号 
	unsigned char th_lenres;   // 4位首部长度/6位保留字 
	unsigned char th_flag;    // 6位标志位 
	USHORT th_win;      // 16位窗口大小 
	USHORT th_sum;      // 16位校验和 
	USHORT th_urp;      // 16位紧急数据偏移量 
}TcpHeader;


typedef struct icmp_hdr
{
	unsigned char  i_type;
	unsigned char  i_code;
	unsigned short i_cksum;
	unsigned short i_id;
	unsigned short i_seq;
	unsigned long  timestamp;
}IcmpHeader;    

typedef struct udp_hdr  // 8 Bytes
{
	unsigned short uh_sport;         
	unsigned short uh_dport;
	unsigned short uh_len;
	unsigned short uh_sum;
} UdpHeader;


USHORT checksum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	if (size) 
	{ 
		cksum += *(UCHAR*)buffer; 
	} 

	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum); 
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
	{
		printf("%s:%d[%d]", __FILE__, __LINE__, GetLastError());
		return -1;
	}
	SOCKET s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	int flag = 1;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, (char*)&flag, sizeof(flag)) != 0)
	{
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		return -1;
	}

	char  name[16];
	struct hostent* host;


	printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
	// 获取本机名 
	if (gethostname(name, sizeof(name)-1) == SOCKET_ERROR) 
	{
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		return -1;
	}

	// 获取本地 IP 地址
	printf("name = %s\n", name);
	host = gethostbyname((char*)name);
	if (0 == host) 
	{
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		return -1;
	}
	printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());

	SOCKADDR_IN addr_in;
	addr_in.sin_addr  = *(in_addr *)host->h_addr_list[0]; // IP 
	addr_in.sin_family = AF_INET; 
	addr_in.sin_port  = htons(57274);
	printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
	if( bind(s, (struct sockaddr *)&addr_in, sizeof(addr_in)) != 0)
	{
		printf("%s:%d[%d]", __FILE__, __LINE__, GetLastError());
		return -1;
	}
	printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
	// 设置网卡的I/O行为，接收网络上所有的数据包
	DWORD dwValue = 1; 
	if(ioctlsocket(s, SIO_RCVALL, &dwValue) != 0)
	{
		printf("%s:%d[%d]", __FILE__, __LINE__, GetLastError());
		return -1;
	} 
	printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
	const int MAX_RECEIVEBUF = 1024;
	char recv_buf[MAX_RECEIVEBUF] = {0};
	char msg[MAX_RECEIVEBUF] = {0};

	char *ptr = NULL;

	while(1)
	{
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		int ret = recv(s, recv_buf, MAX_RECEIVEBUF, 0);
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		if(ret == SOCKET_ERROR)
		{
			printf("%s:%d[%d]", __FILE__, __LINE__, GetLastError());
		}
		printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
		if(ret > 0)
		{
			printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
			IpHeader* ipheader = (IpHeader*)recv_buf;
			printf("%s:%d[%d]\n", __FILE__, __LINE__, GetLastError());
			// 取得正确的IP头长度
			int ipheader_len = sizeof(unsigned long) * (ipheader->h_verlen & 0xf);
			printf("ip header length=%d\n", ipheader_len);
			in_addr* temp = (in_addr*)&ipheader->sourceIP;
			printf("source ip:%s\n", inet_ntoa(*temp));
			temp = (in_addr*)&ipheader->destIP;
			printf("source ip:%s\n", inet_ntoa(*temp));
			printf("date package length:%d\n", ntohs(ipheader->total_len));
			if(ipheader->proto == IPPROTO_TCP)
			{
				printf("protocol:"TOSTR(IPPROTO_TCP)"\n");
				TcpHeader* tcpheader =  (TcpHeader*)(recv_buf + ipheader_len);
				printf("source port:%d\n",tcpheader->th_sport);
				printf("destination port:%d\n", tcpheader->th_dport);
				
				char* ptr = recv_buf+ipheader_len+(4 * ((tcpheader->th_lenres & 0xf0)>>4|0));
			}
			else if(ipheader->proto == IPPROTO_ICMP)
			{
				printf("protocol:"TOSTR(IPPROTO_ICMP)"\n");
				IcmpHeader* icmpheader = (IcmpHeader*)(recv_buf + ipheader_len);
				printf("icmp return type:%d\n", icmpheader->i_type);
				printf("icmp return code:%d\n", icmpheader->i_code);
			}
			else if(ipheader->proto == IPPROTO_UDP)
			{
				printf("protocol:"TOSTR(IPPROTO_UDP)"\n");
				UdpHeader* udpheader = (UdpHeader*)(recv_buf + ipheader_len);
				printf("source port:%d\n",udpheader->uh_sport);
				printf("destination port:%d\n", udpheader->uh_dport);
				char* ptr = recv_buf + ipheader_len + 8;
			}
			else
			{
				printf("protocol:%d\n", ipheader->proto);
			}
	}

	shutdown(s, SD_BOTH);
	closesocket(s);
	return 0;
}

