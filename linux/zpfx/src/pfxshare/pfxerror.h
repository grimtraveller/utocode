#ifndef PFXERROR_H
#define PFXERROR_H
#include "../include/public.h"

#ifdef __cplusplus
extern "C"
{
#endif
extern enum pfxerrstyle_t g_errstyle;
extern int pfxerrno;
extern char errmsg[COMMONLEN];
extern void setpfxerr(enum pfxerrstyle_t pfxerrstyle, int errno, const char* msg);
extern void cleanpfxerr();
extern void ppfxerr(int how);
extern void showpfxerr();
extern void logpfxerr(const char* path);
#ifdef __cplusplus
}
#endif

#endif	//PFXERR_H
