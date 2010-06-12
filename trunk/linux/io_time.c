#include "include/public.h"
#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

//read file max= 134784
int null_file(char* filename, unsigned long filesize);
int main(int argc, char* argv[])
{
    struct tms tm;
    clock_t start, end;
    unsigned long size_r = 0;
    char ddd[1024*1024*10];
    size_r = atol(argv[2]);
    if (-1 == start)	return errno;
    if (3 != argc)
    {
	printf("io_time filename size\n");
    }
    
    null_file(argv[1], size_r);
    start = times(&tm);
    FILE* f;
    if (0 == (f = fopen(argv[1], "rb")))
    {
	perror("OPEN");
	return errno;
    }

    char* pBuff = NULL;
   // printf("%d", size_r);
    pBuff = (char*)calloc(size_r,0);
    if (NULL == pBuff)
    {
	perror("CALLOC");
	return errno;
    }
    printf("read start\n");
    printf("errno=%d\n",errno);
    if (size_r != fread(ddd,  sizeof(char),1024*1024, f))
    {
	perror("READ");	
	return errno;
    }
    printf("read end\n");
    fclose(f);
    unsigned int i = 0;
    end = times(&tm);
    if (-1 == end)  return errno; 
    printf("Total(User+Sys) CPU time:%d\n", end - start);
    return 0;
}

int null_file(char* filename, unsigned long filesize)
{
    int fd;
    char* pBuff = NULL;
char bbb[1024*1024];
    if (0 > (fd = open(filename, O_WRONLY|O_CREAT)))
    {
	perror("OPEN");
	return errno;
    }
    pBuff = calloc(0, filesize);
    if (NULL == pBuff)
    {
	perror("CALLOC");
	return errno;
    }
    if (0 == write(fd,bbb, sizeof(bbb)))
    {
	perror("Write");
	return errno;
    }
    close(fd);
    return 0;
}
    
