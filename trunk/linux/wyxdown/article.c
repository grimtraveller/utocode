
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <regex.h>
#endif //WIN32
#include <fcntl.h>
#ifdef DS
#include "../datastructure/public.h"
#else
#define DSmalloc malloc
#define DSfree free
#define DSrealloc realloc
#define BOOLEAN long
#define TRUE 1
#define FALSE 0
#endif	//DS
#define TMPSIZE 512
int
getline(int fd, char** ppbuf)
{
	int size = 0;
	int i = 0;
	char* pbuf = 0;
	pbuf = (char*)DSmalloc(sizeof(char)*TMPSIZE);
	do
	{
		if (TMPSIZE == i)
		{
			pbuf = (char*)DSrealloc(pbuf, sizeof(char)*(TMPSIZE+size));
			i = 0;
		
		}
		if (1 != read(fd, pbuf+size, 1))
		{
			break;
		}
		i++;
		size++;
	}
	while ('\n' != *(pbuf+size-1));
	*(pbuf+size) = '\0';
	*ppbuf = pbuf;
	return size;
}
#ifndef WIN32
int
regex(char* r,char* pbuf)
{
	char errmsg[128];
	regex_t reg;
	int ret;
	int nmatch = 1;
	regmatch_t pm[1];
	if (0 != (ret = regcomp(&reg, r, 0)))
	{
		printf("1");
		goto MERR;
	}
	if (0 != (ret = regexec(&reg, pbuf, nmatch, pm, 0)))
	{
		if (ret != REG_NOMATCH)
		{
			printf("2");
			goto MERR;
		}
	}
	regfree(&reg);
	return ret;
MERR:

	regerror(ret, &reg, errmsg, sizeof(errmsg));
	regfree(&reg);
	return ret;
	return 0;
}
#endif //WIN32
int main(int argc, char* argv[])
{
	char* file =  argv[1];
	int fd;
	char* pbuf;
#ifdef WIN32
	char begin[] = "<!-- Article Begin -->";
#else
	char begin[] = ".*<!-- Article Begin -->.*";
#endif //WIN32
	char end[] = "<!-- Article End -->";
	BOOLEAN b_safe = FALSE;
	if ((fd = open(file, O_RDONLY)) < 0)
	{
		perror("");
		return 1;
	}
	while(1)
	{
		
		if (0 == getline(fd, &pbuf))
		{
			break;
		}
		
		if (b_safe)
		{
#ifdef WIN32
			if (0 != strstr(pbuf, end))
#else 
			if (0 == regex(end, pbuf))
#endif //WIN32
			{
				DSfree(pbuf);
				break;
			}
		}
		else
		{
#ifdef WIN32
			if (0 != strstr(pbuf, begin))
#else
			if (0 == regex(begin, pbuf))
#endif //WIN32
			{
				printf("============%s\n", pbuf);
				b_safe = TRUE;
			}
			continue;
		}
		//save
		printf("\n%s\n", pbuf);
		DSfree(pbuf);
	}
	close(fd);
	return 0;
}
