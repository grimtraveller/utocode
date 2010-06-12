#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#ifndef NULL
#define NULL 0
#endif
int main(int argc, char* argv[])
{
    char path[260] = {0};
    if (NULL == getcwd(path, 260))
    {
	printf("ERROR");
    }
    printf("%s\n", path);

    int ret = chdir("/");
    if (-1 == ret)
    {
	printf("ERROR");
	return 0;
    }
    if (NULL == getcwd(path, 260))
    {
	printf("ERROR");
    }
    printf("%s\n", path);
    return 0;
} 
