#include "pfxdirapi.h"
int pfxfind(const char* path, FTWDO dofile)
{
	int nftwflags = 0;
	nftwflags |= FTW_PHYS;

	int nopenfd = 1;
	return nftw(path, dofile, nopenfd, nftwflags);
}
