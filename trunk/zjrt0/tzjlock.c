#include "zjlock.h"
#include "zjsleep.h"
#if defined(WIN32)	//OS
#include <windows.h>
typedef DWORD THREAD_PROC_RETURN;
#define createThread(func, argv)	CreateThread(NULL, 0, func, argv, 0, NULL)
#elif defined(LINUX)
typedef void* THREAD_PROC_RETURN;
#define createThread(func, argv)	pthread_create(0, 0, func, argv)
#define WINAPI
#endif //OS

#define LOOP_PRINT	do	{	\
						int i;	\
						for (i = 0; i < 10; i++)	{	\
							printf("%s %d\n", __FUNCTION__, i);	\
							zjsleep(1);	\
						}				\
					}while(0)

ZJLOCK l;

DWORD WINAPI workThread(void* dummy)
{
	LOOP_PRINT;
	return 0;
}

THREAD_PROC_RETURN WINAPI workLockThread(void* dummy)
{
	zjlock(&l);
	LOOP_PRINT;
	zjunlock(&l);
	return 0;
}

THREAD_PROC_RETURN WINAPI workThread2(void* dummy)
{
	LOOP_PRINT;
	return 0;
}

THREAD_PROC_RETURN WINAPI workLockThread2(void* dummy)
{
	zjlock(&l);
	LOOP_PRINT;
	zjunlock(&l);
	return 0;
}

void wait(int t)
{
	int i = 0;
	while(1)
	{
		if (i >= t)
		{
			break;
		}
		printf("sleep...%d\n", i);
		zjsleep(1);
		i++;
	}
}
int
main(int argc, char** argv)
{
	int i = 0;
	printf("===two thread===\n");
	createThread(workThread, NULL);
	createThread(workThread2, NULL);
	wait(50);
	printf("===two thread with lock===\n");
	zjinitLock(&l);
	createThread(workLockThread, NULL);
	createThread(workLockThread2, NULL);
	wait(50);
	zjuninitLock(&l);
	return 0;
}
