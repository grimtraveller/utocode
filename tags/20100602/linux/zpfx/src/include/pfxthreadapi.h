#ifndef PFXTHREADAPI_H
#define PFXTHREADAPI_H
#include <pthread.h>
#ifdef __cplusplus
extern "C"
{
#endif	//__cplusplus
#define PFXTHREADID	pthread_t
typedef void* (*PFXTHREADDO)(void* pdata);
extern int pfxcreatethread(PFXTHREADID* ptid, PFXTHREADDO pfxdo, void* pdata);
extern void pfxkillthread(PFXTHREADID tid);
extern int pfxwaitthread(PFXTHREADID tid);
#ifdef __cplusplus
}
#endif
#endif	//PFXTHREADAPI_H
