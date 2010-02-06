/*
 *Name:		nfun_t.c
 *Descr:	test snprint() and strncpy()
 *Usage:
 *Date:		2009 01 02
 *Author:	zuohaitao
 */
#include <stdio.h>
#include <string.h>
int main()
{
	char hello[6]={0};
	snprintf(hello, 6, "hello");
	printf("%s\n", hello);
	memset(hello, '$', 6);
	snprintf(hello, 6, "hello_world");
	printf("%s\n", hello);
	memset(hello, '$', 6);
	strncpy(hello, "hello world", 6);
	//strncpy last is NOT '\0'
	printf("%s\n", hello);
	return 0;
}
