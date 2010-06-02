/*
 * Name:	rand_matrix.c
 * Desc:	print rand number in console or tty
 * Usage:	gcc rand_matrix.c -o rand_matrix
 * Author:	zuohaitao
 * Date:	2009-03-07
 */
#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int nRand;
	int i;
#ifdef WIN32
	HANDLE hStdout;
#endif

	srand( (unsigned)time( NULL ) );
	while(1)
	{
		for (i = 0; i < 16 * 10; i++)
		{
			nRand = rand();
#ifdef WIN32
			hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
			SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN);

			printf("%04x ", nRand);
#else
			printf("\33[32m%08x \33[40;37m", nRand);
			if (0 == i%5)
			{
				printf("\n");
			}
#endif
		}
		//_sleep(100);
	}
	return 0;
}


