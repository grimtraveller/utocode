#ifndef ZJLOCK_H
#define ZJLOCK_H
#if defined(WIN32)
#include <windows.h>
typedef CRITICAL_SECTION ZJLOCK;
#define zjinitLock(x)	InitializeCriticalSection(x)
#define zjuninitLock(x)	DeleteCriticalSection(x)
#define zjlock(x)	EnterCriticalSection(x)
#define zjunlock(x)	LeaveCriticalSection(x)
#elif defined(LINUX)
#include <pthread.h>
typedef pthread_mutex_t ZJLOCK;
#define zjinitLock(x)	pthread_mutex_init(x, NULL)
#define zjuninitLock(x)	pthread_mutex_destroy(x)
#define zjlock(x)	pthread_mutex_lock(x)
#define zjunlock(x)	pthread_mutex_unlock(x)
#endif //OS

#endif //ZJLOCK_H
