#include <stdio.h>
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
		goto MERR;
	}
	if (0 != (ret = regexec(&reg, pbuf, nmatch, pm, 0)))
	{
		if (ret != REG_NOMATCH)
		{
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
	int i = 0;
	int size;
	char* file =  argv[1];
	char* file_tmp[TMPSIZE];

	int fdin;
	int fdout;
	char* pbuf;
#ifdef WIN32
	char begin[] = "<!-- Article Begin -->";
#else
	char begin[] = ".*<!-- Article Begin -->.*";
#endif //WIN32
	char end[] = "<!-- Article End -->";
	BOOLEAN b_safe = FALSE;
	strcpy(file_tmp, file);
	strcat(file_tmp, ".tmp");
	if ((fdin = open(file, O_RDONLY)) < 0)
	{
		perror("");
		return 1;
	}
	if ((fdout = open((const char*)file_tmp, O_WRONLY|O_CREAT)) == 0)
	{
		perror("fopen");
		return 1;
	}
	while(1)
	{
		size = getline(fdin, &pbuf);
		if (0 == size)
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
				b_safe = TRUE;
			}
			continue;
		}
		//save
		for (i = 0; i < size; i++)
		{
			if (pbuf[i] == '>')
			{
				pbuf[i] = '\n';
			}
		}
		//printf("%s", pbuf);
		write(fdout, pbuf, size);
		DSfree(pbuf);
	}
	close(fdin);
	close(fdout);



	if ((fdin = open(file_tmp, O_RDONLY)) < 0)
	{
		perror("");
		return 1;
	}
	if ((fdout = open(file, O_WRONLY|O_TRUNC)) < 0)
	{
		perror("");
		return 1;
	}

	while(1)
	{
		size = getline(fdin, &pbuf);
		if (0 == size)
		{
			b_safe = FALSE;
			continue;
		}
		
		//save
		b_safe = TRUE;
		for (i = 0; i < size; i++)
		{
			if ((pbuf[i] != ' ') && (pbuf[i] != '\t'))
			{
				if (pbuf[i] == '<')
				{
					b_safe = FALSE;	
					break;
				}
				else
				{
					b_safe = TRUE;
				}
				break;
			}
			else
			{
			}
		}
		printf("%s=%d\n", pbuf, b_safe);
		if (b_safe)
		{
			//printf("%s\n", pbuf);
			write(fdout, pbuf, size);
		}

		DSfree(pbuf);
	}
	close(fdin);
	close(fdout);
	return 0;
}
