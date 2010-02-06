/*
 *	name:	pfxlock.h
 *	desc:	lock interface
 *	author:	zuohaitao [at] shxy
 *	date:	2008/08/03
 *	usage:	see pfxshare_t.h
 */
#ifndef PFXLOCK_H
#define PFXLOCK_H
#include <sys/types.h>
#ifdef __cplusplus
extern "C"{
#endif
extern int mklock(key_t key);
extern int lock(int lockid);
extern int unlock(int lockid);
extern int wait_unlock(int lockid);
extern int rmlock(int lockid);
#ifdef __cplusplus
}
#endif
#endif	//PFXLOCK_H
