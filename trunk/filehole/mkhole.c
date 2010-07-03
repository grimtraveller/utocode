#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int 
main(int argc, char** argv)
{
	int fd;
	char str[2];
	if (argc < 2)
	{
		printf("USAGE:mkhole filename string\n");
		return 1;
	}

	if ((fd = creat(argv[1], 000744)) < 0)
	{
		perror("creat");
		return 1;
	}
	if (-1 == lseek(fd, 1024*10, SEEK_SET))
	{
		perror("lseek");
		return 1;
	}
	if (write(fd, "a", 1)<0)
	{
		perror("write");
	}
	if (-1 == lseek(fd, 1, SEEK_SET))
	{
		perror("lseek");
		return 1;
	}

	close(fd);

	return 0;
}
