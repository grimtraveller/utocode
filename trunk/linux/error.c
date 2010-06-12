//Note:sterror() and perror() must NOT be used together.
#include "public.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
	printf("errprint strerror or errprint perror\n");
	return 1;
    }
    char* file_name = "/falsefile";
    int fd;
    if (0 > (fd =open(file_name,O_RDONLY)))
    {
	if (0 == strncmp("strerror", argv[1], strlen("strerror")+1))
	{
	    printf("call strerror()\n");
	    int myerr = errno;
	    char* err;
	    printf("errno=%d\n", myerr);
	    err = strerror(myerr);
	    err = strdup(err);
	    printf("%s\n", err);
	    free(err);
	}
	else if (0 == strncmp("perror", argv[1], strlen("perror")+1))
	{
	    printf("call perror()\n");
	    perror("open");
	}
	else
	{
	    printf("argment:error\n");
	    return 1;
	}     
	return 0;
    }
}
