/*
 *Name:		memmove_t.c
 *Descr:	test memory copy and move in overlap
 *Usage:	$>memmove_t
 *Author:	zuohaitao
 *Date:		
 */
#include <string.h>
#include <stdio.h>
int
main(int argc, char* argv[])
{
	char buf1[21];
	char buf2[11];
	char* src;
	char* des;
	memset(buf1, 0x20, 20);
	buf1[20] = 0;
	strncpy(buf1, "1234567890", 10);
	printf("%s\n", buf1);
	buf2[10] = 0;

	printf("memcpy()\n");
	src = buf1;
	des = buf1+5;
	memcpy(des, src, 10);
	printf("src:%s\n", src);
	printf("des:%s\n", des);

	printf("memmove()\n");
	strncpy(buf1, "1234567890", 10);
	src = buf1;
	des = buf1+5;
	memmove(des, src, 10);
	printf("src:%s\n", src);
	printf("des:%s\n", des);
	return 0;
}
