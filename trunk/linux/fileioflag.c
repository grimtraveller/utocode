/*
 * Name:	fileioflag.c
 * Desc:	print file system call
 * Usage:	$>gcc fileioflag.c -o fileioflag
 *			$>touch hd
 *			$>./fileioflag
 * Author:	zuohaitao
 * Date:
 */

#include<fcntl.h>
#include<stdio.h>
int
main()
{
	int fd;
	int flag = 0;
	printf("flag=0x%x\n", flag);
	printf("O_WRONLY=0x%x\n", O_WRONLY);
	fd = open("hd", O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return 1;
	}
#if defined(O_SYNC)
	printf("O_SYNC=0x%x\n", O_SYNC);
	flag = fcntl(fd, F_GETFL, 0);
	flag |= O_SYNC;
	fcntl(fd, F_SETFL, flag);
	flag = fcntl(fd, F_GETFL, 0);
	printf("0x%x\n", flag);
#else
	printf("undefine O_SYNC\n");
#endif
}
