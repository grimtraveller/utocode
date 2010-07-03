/*
 *name:	tshark.c
 *use:	for test ctl.sh
 *usage:
		linux:
			$gcc tshark.c -o tshark
		win32:
			>cl tshark.c /D "WIN32"
 *auhor:zuohaitao
 *data:	208/07/27
 */
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif
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
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	return 0;
}
