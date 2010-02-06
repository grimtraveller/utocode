#include "pfxerror.h"
#include "string.h"
#include "../include/public.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int main()
{
	int i;
	char logpath[] = "test";
	//test setpfxerr() and showpfxerr
	for (i = 0; i < 257; i++)
	{
		setpfxerr(syserr, i, strerror(i));
		showpfxerr();
	}
	//test cleanpfxerr()
	cleanpfxerr();
	showpfxerr();

	//test logpfxerr()
	setpfxerr(pfxerr, 0, "success");
	logpfxerr(0);
	logpfxerr("test");
	logpfxerr(logpath);

	//test ppfxerr()
	setpfxerr(2, 1, "fail parse in show[ppfxerr(1)");
	ppfxerr(1);
	setpfxerr(2, 2, "fail parse in log[ppfxerr(2)");
	ppfxerr(2);
	setpfxerr(2, 3, "fail parse in log[ppfxerr(3)");
	ppfxerr(3);

	//test syserr
	int fd;
	fd = open("./noexist",O_CREAT|O_EXCL|0777);
	perror("open()");
	setpfxerr(syserr, errno, strerror(errno));
	showpfxerr();
	return 0;
}
