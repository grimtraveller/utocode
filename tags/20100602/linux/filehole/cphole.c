#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int
main(int argc, char** argv)
{
	char buf;
	ssize_t n;
	int src, tar;
	if (argc < 3)
	{
		printf("uage:cphole target source\n");
		return 1;
	}
	src = open(argv[1], O_RDONLY);
	if (src < 0)
	{
		close(src);
		perror("open");
		return 1;
	}
	tar = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 000744);
	if (tar < 0)
	{
		perror("open");
		return 1;
	}
	while(1)
	{
		n = read(src, &buf, 1);
		if (0 == n)
		{
			break;
		}
		if (0 == buf)
		{
			if(-1 == lseek(tar,1, SEEK_CUR))
			{
				perror("lseek");
				goto fail;
			}
		}
		else
		{
			if (write(tar, &buf, 1) < 0)
			{
				perror("write");
				goto fail;
			}
		}
	}
	close(tar);
	close(src);
	return 0;
fail:
	close(tar);
	close(src);
	return 1;
}
