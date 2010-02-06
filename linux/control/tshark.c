/*
 *name:	tshark.c
 *use:	for test ctl.sh
 *usage:
		$gcc tshark.c -o tshark
 *auhor:zuohaitao
 *data:	208/07/27
 */
#include <stdio.h>
int main()
{
	int fd;
	while(1)
	{
		char str[] = "doing someting...\n";
		fd = fileno(stdout);
		printf("printf:%s", str);
		fflush(stdout);
		write(fd, "write:", sizeof("write:") - 1);
		write(fd, str, sizeof(str) - 1);
		//break;
		sleep(1);
	}
	return 0;
}
