#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#define open _open
#define close _close
#define read _read
#define write _write
#define lseek _lseek
#else
#include <unistd.h>
#endif

int
main(int argc, char** argv)
{
	char buf;
#ifdef WIN32
	int n;
#else
	ssize_t n;
#endif //WIN32
	int src, tar;
	if (argc < 3)
	{
		printf("uage:cphole source target \n");
		return 1;
	}
	src = open(argv[1], O_RDONLY);
	if (src < 0)
	{
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
