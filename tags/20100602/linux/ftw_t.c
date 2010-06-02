/*
 *name:		ftw_t.c
 *use:		study ftw
 *usage:	ftw_t dirname
 *author:	zuohaitao
 *date:		2008/08/07
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#define MAX_PATH	260
typedef int (*fn)(const char* fpath, const struct stat* sb, int typeflag);
int display(const char* fpath, const struct stat* sb, int typeflag);
int main(int argc, char* argv[])
{
	int nopenfd = 1;
	char dirpath[MAX_PATH] = {0};
	fn dispfunc;
	dispfunc = display;
	if (argc != 2)
	{
		printf("usage ftw_t dirname\n");
		return 0;
	}
	strncpy(dirpath, argv[1], MAX_PATH);

	if (ftw(dirpath, dispfunc, 0) < 0)
	{
		perror("ftw()");
	}
	return 0;
}
int display(const char* fpath, const struct stat* sb, int typeflag)
{
	char usrname[MAX_PATH] = {0};
	struct passwd* pw;
	struct group* grp;
	struct tm *tm;
	char tmbuf[20] = {0};
	/*
	if (FTW_NS == typeflag)
	{
		printf("typeflag == %d\n", typeflag);
		return 0;
	}
	*/
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
