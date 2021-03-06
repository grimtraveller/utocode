/*
 * Name:		endian_t.c
 * Purpose:		test os style, big-endian or little-endian 
 * Usage:		in linux, $>gcc src/endian.c -o endian_t
 *				in win32, 
 * Author:		zuohaitao
 * Date:		2009-06-28
 */
#include <stdio.h>
#ifdef WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#endif
int main()
{
	int dian = 0x12345678;
	if (0x56780000 == (dian<<16))
	{
		printf("little-endian\n");
	}
	else
	{
		printf("big-endian\n");
	}

	if (dian == htonl(dian))
	{
		printf("big-endian\n");
	}
	else
	{
		printf("little-endian\n");
	}
	return 0;
}
