// hibaidu_editor.cpp : 定义控制台应用程序的入口点。
//
/*
 * Name:	hibaidu_editor.cpp
 * Descr:	Editor for baidu space(blog) [http://hi.baiud.com]
			
 * Usage:	building hibaidu_editor.sln in m$ vs2008
 * Author:	this is a demo code, and a lots of bugs is in it.
			so, if it run well, its author is zuohaitao.otherwise, i DON'T know who write it!
 * Date:	2009 02 28
 */
#include "stdafx.h"
#include "winsock2.h"
#include <sys/types.h>
#include <sys/stat.h>
#define _CRT_SECURE_NO_WARNING
#pragma comment(lib,"Ws2_32.lib")
typedef int status ;
#define	SUCCESS	0
#define	FAIL	-1
#define TITLE_MAX	1024
#define CONTENT_MAX	30000
/* the two functions is important*/
//login http://passport.baidu.com/?login
//const user	[in]user name
//int ulen		[in]user name length
//const passwd	[in]password
//int plen		[in]password length
//char* baiduid	[out]cookie:BAIDUID
//char* bduss	[out]cookie:BDUSS
//char* ptoken	[out]cookie:PTOKEN
//char* stoken	[out]cookie:STOKEN
status passport_bd(const char* user, int ulen,const char* passwd, int plen, char* baiduid, char* bduss, char* ptoken, char* stoken);
//publish blog http://hi.baidu.com/
//const char* title		[in] article 's title
//int tlen				[in] title length
//const char* content	[in] article 's content
//int clen				[in] content length
//const char* usr		[in] user anme
//int ulen				[in] user length
//char* baiduid		[in] cookie:BAIDUID
//char* bduss		[in] cookie:BDUSS
//char* ptoken		[in] cookie:PTOKEN
//char* stoken			[in] cookie:STOKEN
status commit_blog(const char* title, int tlen,
				   const char* content, int clen,
				   const char* usr, int ulen,
				   char* baiduid, char* bduss, char* ptoken, char* stoken);

/*Wait*/
void loading(int sec);

/*read article from file*/
/*TODO:FIX BUG*/
status get_article(const TCHAR* file, char* &title, char* &content);

int _tmain(int argc, _TCHAR* argv[])
{
/*
	if (2 != argc)
	{
	printf("Usage:hibaidu_editor.exe [filename]\n");
	return -1;
	}
*/
	printf("╭(　▔▽▔)╮HI~我是百度空间离线发布系统！\n"
		   "     ╜　╙                                \n");
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("Error at WSAStartup()\n");
		return -1;
	}

	char baiduid[47] = {0};
	char bduss[200] = {0};
	char ptoken[41] = {0};
	char stoken[41] = {0};

	char user[] = "c3alice";
	char passwd[] = "password";
	status result;
	result = passport_bd(user, strlen(user), passwd, strlen(passwd), baiduid, bduss, ptoken, stoken);
	char title[] = "%7A%75%6F";
	char content[] = "%7A%75%6F%68%61%69%74%61%6F";
	/*
	char* title = 0;
	char* content = 0;
	result = get_article(argv[1], title, content);
	*/
	result = commit_blog(title, strlen(title), content, strlen(content), user, strlen(user), baiduid, bduss, ptoken, stoken);
	/*
	free(title);
	free(content);
	*/
	WSACleanup();
	printf("input any key to quit...");
	getchar();
	return 0;
}

status passport_bd(const char* user, int ulen,
				   const char* passwd, int plen,
				   char* baiduid,
				   char* bduss, 
				   char* ptoken, 
				   char* stoken)
{
	SOCKET s;
	const char head_1[] = {
		0x50, 0x4F, 0x53, 0x54, 0x20, 0x2F, 0x3F, 0x6C, 0x6F, 0x67, 0x69, 0x6E, 0x20, 0x48, 0x54, 0x54,	//POST /?login HTT
		0x50, 0x2F, 0x31, 0x2E, 0x31, 0x0D, 0x0A, 0x43, 0x6F, 0x6E, 0x6E, 0x65, 0x63, 0x74, 0x69, 0x6F,	//P/1.1..Connectio
		0x6E, 0x3A, 0x20, 0x6B, 0x65, 0x65, 0x70, 0x2D, 0x61, 0x6C, 0x69, 0x76, 0x65, 0x0D, 0x0A, 0x43,	//n: keep-alive..C
		0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2D, 0x54, 0x79, 0x70, 0x65, 0x3A, 0x20, 0x61, 0x70, 0x70,	//ontent-Type: app
		0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x78, 0x2D, 0x77, 0x77, 0x77, 0x2D, 0x66,	//lication/x-www-f
		0x6F, 0x72, 0x6D, 0x2D, 0x75, 0x72, 0x6C, 0x65, 0x6E, 0x63, 0x6F, 0x64, 0x65, 0x64, 0x0D, 0x0A,	//orm-urlencoded..
		0x43, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2D, 0x4C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x3A, 0x20	//Content-Length:
	};

	const char head_2[] = {
		0x0D, 0x0A, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2D, 0x63, 0x6F, 0x6E, 0x74, 0x72,					//..Cache-contr
		0x6F, 0x6C, 0x3A, 0x20, 0x6E, 0x6F, 0x2D, 0x63, 0x61, 0x63, 0x68, 0x65, 0x0D, 0x0A, 0x48, 0x6F,	//ol: no-cache..Ho
		0x73, 0x74, 0x3A, 0x20, 0x70, 0x61, 0x73, 0x73, 0x70, 0x6F, 0x72, 0x74, 0x2E, 0x62, 0x61, 0x69,	//st: passport.bai
		0x64, 0x75, 0x2E, 0x63, 0x6F, 0x6D, 0x0D, 0x0A, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x3A, 0x20,	//du.com..Accept: 
		0x74, 0x65, 0x78, 0x74, 0x2F, 0x68, 0x74, 0x6D, 0x6C, 0x2C, 0x20, 0x2A, 0x2F, 0x2A, 0x0D, 0x0A,	//text/html, */*..
		0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x2D, 0x45, 0x6E, 0x63, 0x6F, 0x64, 0x69, 0x6E, 0x67, 0x3A,	//Accept-Encoding:
		0x20, 0x67, 0x7A, 0x69, 0x70, 0x2C, 0x20, 0x64, 0x65, 0x66, 0x6C, 0x61, 0x74, 0x65, 0x2C, 0x20,	// gzip, deflate, 
		0x69, 0x64, 0x65, 0x6E, 0x74, 0x69, 0x74, 0x79, 0x0D, 0x0A, 0x55, 0x73, 0x65, 0x72, 0x2D, 0x41,	//identity..User-A
		0x67, 0x65, 0x6E, 0x74, 0x3A, 0x20, 0x43, 0x68, 0x72, 0x79, 0x73, 0x61, 0x6E, 0x74, 0x68, 0x20,	//gent: Chrysanth 
		0x57, 0x65, 0x62, 0x73, 0x74, 0x6F, 0x72, 0x79, 0x28, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x74, 0x69,	//Webstory(compati
		0x62, 0x6C, 0x65, 0x3B, 0x20, 0x4D, 0x53, 0x49, 0x45, 0x20, 0x36, 0x2E, 0x30, 0x3B, 0x20, 0x57,	//ble; MSIE 6.0; W
		0x69, 0x6E, 0x64, 0x6F, 0x77, 0x73, 0x20, 0x4E, 0x54, 0x20, 0x35, 0x2E, 0x31, 0x29, 0x0D, 0x0A,	//indows NT 5.1)..
		0x0D, 0x0A	//..
	};

	const char content_1[] = {
		0x74, 0x70, 0x6C, 0x5F, 0x6F, 0x6B, 0x3D, 0x26, 0x6E, 0x65, 0x78, 0x74, 0x5F, 0x74, 0x61, 0x72,	//tpl_ok=&next_tar
		0x67, 0x65, 0x74, 0x3D, 0x26, 0x74, 0x70, 0x6C, 0x3D, 0x6D, 0x6E, 0x26, 0x73, 0x6B, 0x69, 0x70,	//get=&tpl=mn&skip
		0x5F, 0x6F, 0x6B, 0x3D, 0x26, 0x61, 0x69, 0x64, 0x3D, 0x26, 0x6E, 0x65, 0x65, 0x64, 0x5F, 0x70,	//_ok=&aid=&need_p
		0x61, 0x79, 0x3D, 0x26, 0x6E, 0x65, 0x65, 0x64, 0x5F, 0x63, 0x6F, 0x69, 0x6E, 0x3D, 0x26, 0x70,	//ay=&need_coin=&p
		0x61, 0x79, 0x5F, 0x6D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x3D, 0x26, 0x75, 0x3D, 0x68, 0x74, 0x74,	//ay_method=&u=htt
		0x70, 0x25, 0x33, 0x41, 0x25, 0x32, 0x46, 0x25, 0x32, 0x46, 0x77, 0x77, 0x77, 0x2E, 0x62, 0x61,	//p%3A%2F%2Fwww.ba
		0x69, 0x64, 0x75, 0x2E, 0x63, 0x6F, 0x6D, 0x25, 0x32, 0x46, 0x26, 0x72, 0x65, 0x74, 0x75, 0x72,	//idu.com%2F&retur
		0x6E, 0x5F, 0x6D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x3D, 0x67, 0x65, 0x74, 0x26, 0x6D, 0x6F, 0x72,	//n_method=get&mor
		0x65, 0x5F, 0x70, 0x61, 0x72, 0x61, 0x6D, 0x3D, 0x26, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x5F,	//e_param=&return_
		0x74, 0x79, 0x70, 0x65, 0x3D, 0x26, 0x70, 0x73, 0x70, 0x5F, 0x74, 0x74, 0x3D, 0x30, 0x26, 0x70,	//type=&psp_tt=0&p
		0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x3D													//assword=
	};

	const char content_2[] = {
		0x26, 0x73, 0x61, 0x66, 0x65, 0x66, 0x6C, 0x67,	0x3D, 0x30, 0x26, 0x75, 0x73, 0x65, 0x72, 0x6E,	//&safeflg=0&usern
		0x61, 0x6D, 0x65, 0x3D																			//ame=
	};

	const char content_3[] = {
		0x26, 0x76, 0x65, 0x72, 0x69, 0x66, 0x79, 0x63, 0x6F, 0x64, 0x65, 0x3D//&verifycode=
	};

	char head[306] = {0};
	char rcvbuf[1420] = {0};
	int ret;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		return FAIL;
	}

	sockaddr_in loginserv; 
	loginserv.sin_family = AF_INET;
	loginserv.sin_addr.s_addr = inet_addr( "220.181.43.127" );
	loginserv.sin_port = htons( 80 );

	if ( connect( s, (SOCKADDR*) &loginserv, sizeof(loginserv) ) == SOCKET_ERROR) {
		printf( "Failed to connect.\n" );
		return FAIL;
	}

	memcpy(head, head_1, sizeof(head_1));

	char len[4] = {0};
	int content_len = sizeof(content_1)+plen+sizeof(content_2)+ulen+sizeof(content_3);
	sprintf(len, "%d", content_len);
	memcpy(head+sizeof(head_1), len, 3);
	memcpy(head+sizeof(head_1)+3, head_2, sizeof(head_2));

	char* content = 0;
	content = (char*)malloc(content_len);
	if (0 == content)
	{
		return FAIL;
	}
	memcpy(content, content_1, sizeof(content_1));
	memcpy(content+sizeof(content_1), passwd, plen);
	memcpy(content+sizeof(content_1)+plen, content_2, sizeof(content_2));
	memcpy(content+sizeof(content_1)+plen+sizeof(content_2), user, ulen);
	memcpy(content+sizeof(content_1)+plen+sizeof(content_2)+ulen, content_3, sizeof(content_3));


	ret = send(s, head, 306, 0);
	ret = send(s, content, content_len, 0);
	free(content);
	while(1)
	{
		ret = recv(s, rcvbuf, 1420, 0);
		if (1420 == ret)
		{
			break;
		}
		Sleep(1000);
	}

	//TO DO:verify login success or not

	//save session
	memcpy(baiduid, rcvbuf+0x166, 47);
	memcpy(bduss, rcvbuf+0x01ff, 200);
	memcpy(ptoken, rcvbuf+0x02ee, 41);
	memcpy(stoken, rcvbuf+0x0346, 41);


	ret = recv(s, rcvbuf, 547, 0);

	closesocket(s);

	return SUCCESS;
}


status commit_blog(const char* title, int tlen, 
				   const char* nr, int nrlen, 
				   const char* usr, int ulen,
				   char* baiduid, 
				   char* bduss, 
				   char* ptoken, 
				   char* stoken)
{
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	sockaddr_in blogserv; 
	blogserv.sin_family = AF_INET;
	blogserv.sin_addr.s_addr = inet_addr( "220.181.37.151" );
	blogserv.sin_port = htons( 80 );

	if ( connect(s, (SOCKADDR*) &blogserv, sizeof(blogserv) ) == SOCKET_ERROR) {
		printf( "Failed to connect.\n" );
		WSACleanup();
		return 0;
	}

	char hicookie[550];

	const char hicookie_1[278] = {
		0x47, 0x45, 0x54, 0x20, 0x2F, 0x20, 0x48, 0x54, 0x54, 0x50, 0x2F, 0x31, 0x2E, 0x31, 0x0D, 0x0A,	//GET / HTTP/1.1..
		0x43, 0x6F, 0x6E, 0x6E, 0x65, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x3A, 0x20, 0x6B, 0x65, 0x65, 0x70,	//Connection: keep
		0x2D, 0x61, 0x6C, 0x69, 0x76, 0x65, 0x0D, 0x0A, 0x43, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2D,	//-alive..Content-
		0x54, 0x79, 0x70, 0x65, 0x3A, 0x20, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F,	//Type: applicatio
		0x6E, 0x2F, 0x78, 0x2D, 0x77, 0x77, 0x77, 0x2D, 0x66, 0x6F, 0x72, 0x6D, 0x2D, 0x75, 0x72, 0x6C,	//n/x-www-form-url
		0x65, 0x6E, 0x63, 0x6F, 0x64, 0x65, 0x64, 0x0D, 0x0A, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2D, 0x63,	//encoded..Cache-c
		0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x3A, 0x20, 0x6E, 0x6F, 0x2D, 0x63, 0x61, 0x63, 0x68, 0x65,	//ontrol: no-cache
		0x0D, 0x0A, 0x48, 0x6F, 0x73, 0x74, 0x3A, 0x20, 0x68, 0x69, 0x2E, 0x62, 0x61, 0x69, 0x64, 0x75,	//..Host: hi.baidu
		0x2E, 0x63, 0x6F, 0x6D, 0x0D, 0x0A, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x3A, 0x20, 0x74, 0x65,	//.com..Accept: te
		0x78, 0x74, 0x2F, 0x68, 0x74, 0x6D, 0x6C, 0x2C, 0x20, 0x2A, 0x2F, 0x2A, 0x0D, 0x0A, 0x41, 0x63,	//xt/html, */*..Ac
		0x63, 0x65, 0x70, 0x74, 0x2D, 0x45, 0x6E, 0x63, 0x6F, 0x64, 0x69, 0x6E, 0x67, 0x3A, 0x20, 0x67,	//cept-Encoding: g
		0x7A, 0x69, 0x70, 0x2C, 0x20, 0x64, 0x65, 0x66, 0x6C, 0x61, 0x74, 0x65, 0x2C, 0x20, 0x69, 0x64,	//zip, deflate, id
		0x65, 0x6E, 0x74, 0x69, 0x74, 0x79, 0x0D, 0x0A, 0x55, 0x73, 0x65, 0x72, 0x2D, 0x41, 0x67, 0x65,	//entity..User-Age
		0x6E, 0x74, 0x3A, 0x20, 0x43, 0x68, 0x72, 0x79, 0x73, 0x61, 0x6E, 0x74, 0x68, 0x20, 0x57, 0x65,	//nt: Chrysanth We
		0x62, 0x73, 0x74, 0x6F, 0x72, 0x79, 0x28, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x74, 0x69, 0x62, 0x6C,	//bstory(compatibl
		0x65, 0x3B, 0x20, 0x4D, 0x53, 0x49, 0x45, 0x20, 0x36, 0x2E, 0x30, 0x3B, 0x20, 0x57, 0x69, 0x6E,	//e; MSIE 6.0; Win
		0x64, 0x6F, 0x77, 0x73, 0x20, 0x4E, 0x54, 0x20, 0x35, 0x2E, 0x31, 0x29, 0x0D, 0x0A, 0x43, 0x6F,	//dows NT 5.1)..Co
		0x6F, 0x6B, 0x69, 0x65, 0x3A, 0x20																//okie: 
	};
	const char hicookie_2[27] = {
		0x0A, 0x0D, 0x43, 0x6F, 0x6F, 0x6B, 0x69, 0x65, 0x32, 0x3A, 0x20, 0x24, 0x56, 0x65, 0x72, 0x73,	//..Cookie2: $Versi
		0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x22, 0x0D, 0x0A, 0x0D, 0x0A								//on="1"....
	};
	memcpy(hicookie, hicookie_1, 278);
	memcpy(hicookie+278, baiduid, 47);
	memcpy(hicookie+278+47,bduss, 198);
	memcpy(hicookie+278+47+198, hicookie_2, 27);
	int ret;
	ret = send(s, hicookie, 550, 0);

	char rcvbuf[1420] = {0};
	char bdsp[207];
	char bdstat[72];
	int bdxpinfo = 9+24+1+ulen+1+ulen+1+32+1+1;
	char* bdspinfo = 0;
	bdspinfo = (char*)malloc(bdxpinfo);
	char* bdopinfo = 0;
	bdopinfo = (char*)malloc(bdxpinfo);
	while(1)
	{
		ret = recv(s, rcvbuf, 1420, 0);
		if (1420 == ret)
		{
			break;
		}
		Sleep(1000);
	}
	memcpy(bdsp, rcvbuf+0x005b, 207);
	bdsp[206] = 0x20;
	memcpy(bdstat, rcvbuf+0x152, 72);
	memcpy(bdspinfo, rcvbuf+0x0299, bdxpinfo);
	bdspinfo[bdxpinfo-1] = 0x20;
	memcpy(bdopinfo, rcvbuf+665+bdxpinfo-1+41, bdxpinfo);
	bdopinfo[bdxpinfo-1] = 0x20;


	/*
	int i = 1;
	while(i < 13)
	{
	ret = recv(s, rcvbuf, 1420, 0);
	if (1420 == ret)
	{
	i++;
	}
	else
	{
	Sleep(1000);
	}
	}
	ret = recv(s, rcvbuf, 1305, 0);
	*/

	const char head_0[6+1] = "POST /";

	const char head_1[18+1] = "/commit HTTP/1.1\r\n";

	const char head_2[24+1] = "Connection: keep-alive\r\n";
	const char head_3[49+1] = "Content-Type: application/x-www-form-urlencoded\r\n";
	const char head_4[16+1] = "Content-Length: ";

	const char head_5[25+1] = "Cache-control: no-cache\r\n";
	const char head_6[20+1] = "Host: hi.baidu.com\r\n";
	const char head_7[24+1] = "Accept: text/html, */*\r\n";
	const char head_8[42+1] = "Accept-Encoding: gzip, deflate, identity\r\n";
	const char head_9[70+1] = "User-Agent: Chrysanth Webstory(compatible; MSIE 6.0; Windows NT 5.1)\r\n";
	const char head_10[8+1] = "Cookie: ";
	const char head_11[27+1] = "\r\nCookie2: $Version=\"1\"\r\n\r\n";
	const char content_0[39+1] = "ct=1&cm=1&spRefURL=http://hi.baidu.com/";
	const char content_1[31+1] = "%2Fcreat%2Fblog%2F&spBlogTitle=";
	const char content_2[] = "&spBlogText=";
	const char content_3[] = "&spBlogCatName=%C4%AC%C8%CF%B7%D6%C0%E0&spIsCmtAllow=1&spBlogPower=0&spVcode=&spVerifyKey=&tj=+%B7%A2%B1%ED%CE%C4%D5%C2+";


	char head[1034] = {0};
	memcpy(head, head_0, 6);
	memcpy(head+6, usr, ulen);
	memcpy(head+6+ulen, head_1, 18);
	memcpy(head+6+ulen+18, head_2, 24);
	memcpy(head+6+ulen+18+24, head_3, 49);
	memcpy(head+6+ulen+18+24+49, head_4, 16);
	char* c_len = 0;
	int content_len = strlen(content_1) + tlen + strlen(content_2) + nrlen + strlen(content_3);

	int l;
	if (content_len > 999)
	{
		l = 6;
	}
	else if (content_len > 9999)
	{
		l = 7;
	}
	else
	{
		l = 5;
	}
	c_len = (char*)malloc(l+1);

	sprintf(c_len, "%d\r\n", content_len);
	memcpy(head+6+ulen+18+24+49+16, c_len, l);
	free(c_len);
	memcpy(head+6+ulen+18+24+49+16+l, head_5, 25);
	memcpy(head+6+ulen+18+24+49+16+l+25, head_6, 20);
	memcpy(head+6+ulen+18+24+49+16+l+25+20, head_7, 24);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24, head_8, 42);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42, head_9, 70);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70, head_10, 8);

	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8, bdsp, 207);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207, bdstat, 72);
	*(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72) = 0x20;
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1, bdspinfo, bdxpinfo);
	free(bdspinfo);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1+bdxpinfo, bdopinfo, bdxpinfo);
	free(bdopinfo);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1+bdxpinfo+bdxpinfo, baiduid, 47);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1+bdxpinfo+bdxpinfo+47, bduss, 198);
	memcpy(head+6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1+bdxpinfo+bdxpinfo+47+198, head_11, 27);


	char* content = 0;
	content = (char*)malloc(content_len);
	if (0 == content)
	{
		return FAIL;
	}

	memcpy(content, content_0, strlen(content_0));
	memcpy(content+strlen(content_0), usr, ulen);
	memcpy(content+strlen(content_0)+ulen, content_1, strlen(content_1));
	memcpy(content+strlen(content_0)+ulen+strlen(content_1), title, tlen);
	memcpy(content+strlen(content_0)+ulen+strlen(content_1)+tlen, content_2, strlen(content_2));
	memcpy(content+strlen(content_0)+ulen+strlen(content_1)+tlen+strlen(content_2), nr, nrlen);
	memcpy(content+strlen(content_0)+ulen+strlen(content_1)+tlen+strlen(content_2)+nrlen, content_3, strlen(content_3));

	ret = send(s, head, 6+ulen+18+24+49+16+l+25+20+24+42+70+8+207+72+1+bdxpinfo+bdxpinfo+47+198+27, 0);

	ret = send(s, content, strlen(content_0)+ulen+strlen(content_1)+tlen+strlen(content_2)+nrlen+strlen(content_3), 0);

	loading(30);

	closesocket(s);
	return SUCCESS;
}

void loading(int sec)
{
	for (int i = 0; i < sec; i++)
	{
		switch(i%4)
		{
		case 0:
			printf("\b-");
			fflush(stdout);
			break;
		case 1:
			printf("\b\\");
			fflush(stdout);
			break;
		case 2:
			printf("\b|");
			fflush(stdout);
			break;
		case 3:
			printf("\b/");
			fflush(stdout);
			break;
		default:
			break;
		}
		Sleep(1000);

	}
}	

status get_article(const TCHAR* file, char* &title, char* &content)
{
	char letter[8+1] = {0};
	FILE* fp;
	fp = _tfopen(file, _T("r"));
	if (0 == fp)
	{
		return FAIL;
	}
	char buf[TITLE_MAX];
	fgets(buf, TITLE_MAX, fp);

	title = (char*)malloc(strlen(buf)*3+1);
	if (0 == title)
	{
		return FAIL;
	}
	memset(title, '%', strlen(buf)*3+1);
	*(title+strlen(buf)*3)= '\0';
	int i = 0;
	for (i = 0; ((i < TITLE_MAX)&&('\0' != buf[i])); i++)
	{
		sprintf(letter, "%8x", (unsigned int)buf[i]);
		title[1+i*3] = letter[6];
		title[1+i*3+1] = letter[7];
	}
	printf("%s", title);

	struct _stat statbuf;
	_tstat(file, &statbuf);

	content = (char*)malloc(3*statbuf.st_size+1);
	if (0 == content)
	{
		return FAIL;
	}
	memset(content, '%', 3*statbuf.st_size+1);
	*(content + 3*statbuf.st_size) = '\0';
	i = 0;
	while(!feof(fp))
	{
		fread(buf, 1, 1, fp);
		sprintf(letter, "%8x", (unsigned int)buf[0]);
		content[1+i*3] = letter[6];
		content[1+i*3+1] = letter[7];
		i++;
	}
	printf("%s", title);
	return SUCCESS;
}