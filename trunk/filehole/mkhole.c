#ifdef WIN32
#include <io.h>
#include <SYS/Stat.h>
#define creat _creat
#define lseek	_lseek
#define write	_write
#define close	_close
#else
#include <sys/types.h>
#include <unistd.h>
#endif //WIN32
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
#ifdef WIN32
	if ((fd = creat(argv[1], _S_IREAD | _S_IWRITE)) < 0)
#else
	if ((fd = creat(argv[1], 000744)) < 0)
#endif
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
