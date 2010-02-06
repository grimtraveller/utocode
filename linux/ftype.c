#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	int i;
	struct stat buf;
	for (i = 0; i < argc; i++)
	{
		printf("%s - ", argv[i]);
		if (-1 == lstat(argv[i],buf))
		{
			perror("argv[i]");
