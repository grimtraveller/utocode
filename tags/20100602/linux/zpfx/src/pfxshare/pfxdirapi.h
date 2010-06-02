#ifndef PFXDIRAPI_H
#define PFXDIRAPI_H
#define _XOPEN_SOURCE 500
#include <ftw.h>
typedef int (*FTWDO)(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftw);
int pfxfind(const char* path, FTWDO dofile);
#endif	//PFXDIRAPI_H
