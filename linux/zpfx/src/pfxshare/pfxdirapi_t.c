/*
 *name:		pfxdirapi_t.c
 *use:		test pfxdirapi
 *usage:	pfxdirapi_t
 *author:	zuohaitao
 *date:		2008/08/07
 *
 */
#define _XOPEN_SOURCE	500
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "pfxdirapi.h"
#include "pfxerror.h"
#include <errno.h>
#define PATH_MAX 260
int display(const char* fpath, const struct stat* sb, int typeflag,struct FTW* ftwbuf);
int main(int argc, char* argv[])
{
	int nopenfd = 1;
	char dirpath[PATH_MAX] = {0};
	FTWDO dispfunc;
	int nftwflags;
	nftwflags |= FTW_PHYS;
	dispfunc = display;
	if (argc != 2)
	{
		printf("usage ftw_t dirname\n");
		return 0;
	}
	strncpy(dirpath, argv[1], PATH_MAX);
	if (pfxfind(dirpath, dispfunc) < 0)
	{
		setpfxerr(syserr, errno, strerror(errno));
		showpfxerr();
		return 1;
	}
	return 0;
}
int display(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf)
{
	char usrname[PATH_MAX] = {0};
	struct passwd* pw;
	struct group* grp;
	struct tm *tm;
	char tmbuf[20] = {0};
	//printf("name\tmode\tusr\tgrp\tsize\ttime\n");
	pw = getpwuid(sb->st_uid);
	grp = getgrgid(sb->st_gid);
	tm = localtime(&(sb->st_mtime));
	sprintf(tmbuf, "%04d-%02d-%02d %02d:%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	printf("%o %s %s %10d %s %s\n", 
			sb->st_mode,
			pw->pw_name,
			grp->gr_name,
			sb->st_size,
			tmbuf,
			fpath);
	/*printf("%s\t%.3o\t%d\t%d\t%d\%s\n,
			fpath, sb->st_mode, sb->st_uid, sb->st_grpid, sb->st_size, sb->st_mtime);*/
	return 0;
}
