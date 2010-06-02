#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
int
main()
{
	int fd = creat("abc", 0666);
	//int fd = open("abc", O_WRONLY|O_CREAT|O_TRUNC, 440);
	
	if (-1 == fd)
	{
		perror("");
		return 1;
	}
	printf("succes\n");
	close(fd);
	return 0;
}
