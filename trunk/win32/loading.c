/*
 * Name:		loading.c
 * Descr:		the char in a new line will be change in 1 second  '-' '\' '|' '/' ' looks like "*"
 * Usage:		win32
 *				>cl loading.c -D WIN
 *				>loading.exe
 *				cygwin
 *				$>gcc loading.c -o loading
 *				$>./loading
 * Author:		zuohaitao
 * Date:		2010-02-21 
 */
#include <stdio.h>
#ifdef WIN
#include <windows.h>
#endif
int main()
{
	int i = 0;
	while(1)
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
		i++;
#ifdef WIN
		Sleep(1000);
#else
		sleep(1);
#endif
	}
	return 0;
}
