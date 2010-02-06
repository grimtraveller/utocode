/*
 * Name:	printPATH.c
 * Desc:	printf SYSTEM environment PATH
 * Usage:	
 *			In Windows:
 *			>cl printPATH.c -D WIN32
 *			>printPATH
 *			In Linux:
 *			$>gcc printPATH.c -o printPATH
 *			$>./printPATH
 * Author:	zuohaitao
 * Date:	2009/02/21
 *
 */
#include <stdlib.h>
#include <stdio.h>
#define		NAME	"PATH"
#ifdef		WIN32
#define		SPLIT	";"
#else
#define		SPLIT	":"
#endif	//WIN32

int main()
{
	char* psz;
	char* token;
	psz = getenv(NAME);
	if (0 == psz)
	{
#ifdef WIN32
		printf("getenv() error\n");
#else
		perror("getenv() error");
#endif
		exit(1);
	}
	token = strtok(psz, SPLIT);
	while(0 != token)
	{
		printf("%s\n", token);
		token = strtok(0, SPLIT);
	}
	return 0;
}
