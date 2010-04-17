#include <stdio.h>
int
main()
{
	system("clear");
	printf("\n");
	printf("\033[1m\\033[1m\033[0m\n");
	printf("\033[4m\\033[4m\033[0m\n");
	printf("\033[5m\\033[5m\033[0m\n");
	printf("\033[7m\\033[7m\033[0m\n");
	printf("\033[8m\\033[8m\033[0m\n");
	printf("\033[10A\\033[10A\033[0m\n");
	sleep(1);
	printf("\033[n10\\033[8m\033[0m\n");
	printf("\033[0m\n");
	return 0;
}
