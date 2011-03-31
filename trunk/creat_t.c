/**
 * @file	creat_t.c 
 * @brief	creat and open function
 * @details	
 *			usage
 *			gcc creat_t.c -o creat_t
 *			cl creat_t.c -D "WIN32"
 * @author	zuohaitao
 * @date	2010-07-05 
 * @warning	
 * @bug	
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#ifdef WIN32
#define creat _creat
#define open _open
#define CREAT_MODE	_S_IREAD|_S_IWRITE
#define OPEN_MODE	_S_IREAD|_S_IWRITE
#else
#define CREAT_MODE	0666
#define OPEN_MODE	440
#endif
int
main()
{
	int fd;
	fd = creat("abc", CREAT_MODE);
	if (-1 == fd)
	{
		perror("creat");
		return 1;
	}
	printf("creat success\n");
	close(fd);

	fd = open("ABC", O_WRONLY|O_CREAT|O_TRUNC, OPEN_MODE);
	if (-1 == fd)
	{
		perror("open");
		return 1;
	}
	printf("open success\n");
	close(fd);
	return 0;
}
