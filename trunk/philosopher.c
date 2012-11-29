#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif //WIN32
#define N		5
#define L(p)	(p)
#define R(p)	(((p)+1)%N)

#ifdef WIN32
//#pragma comment( lib, "Kernel32.lib") 
#endif //WIN32
#ifdef WIN32
CRITICAL_SECTION chopstick[N];
HANDLE max_into_room;
#else
pthread_mutex_t	chopstick[N];
sem_t max_into_room;
#endif //WIN32

void get_stick(int which)
{
#ifdef WIN32
	EnterCriticalSection(&chopstick[L(which)]);
	EnterCriticalSection(&chopstick[R(which)]);
#else
	pthread_mutex_lock(&chopstick[L(which)]);
	pthread_mutex_lock(&chopstick[R(which)]);
#endif //WIN32
}

void put_stick(int which)
{
#ifdef WIN32
	LeaveCriticalSection(&chopstick[L(which)]);
	LeaveCriticalSection(&chopstick[R(which)]);
#else
	pthread_mutex_unlock(&chopstick[L(which)]);
	pthread_mutex_unlock(&chopstick[R(which)]);
#endif //WIN32
}

#ifdef WIN32
unsigned int WINAPI philosopher(void* arg)
#else
void *philosopher(void *arg)
#endif //WIN32
{
	int me = *(int*)arg;
	while(1)
	{
		printf("%s %d", __FUNCTION__, __LINE__);
#ifdef WIN32
		 WaitForSingleObject(max_into_room, INFINITE);
		 printf("%s %d", __FUNCTION__, __LINE__);
#else
		sem_wait(&max_into_room);
#endif //WIN32
		get_stick(me);
		printf("philosopher %d eating\n", me);
#ifdef WIN32
		Sleep(100000);
#else
		usleep(100000);
#endif //WIN32
		put_stick(me);
#ifdef WIN32
		ReleaseSemaphore(max_into_room, 1, NULL);
#else
		sem_post(&max_into_room);
#endif //WIN32
		printf("philosopher %d thinking\n", me);
#ifdef WIN32
		Sleep(100000);
#else
		usleep(100000);
#endif //WIN32
	}
#ifdef WIN32
	return 0;
#endif //WIN32
}
int main(int argc, char*argv[])
{
	int i,arg[N];
#ifdef WIN32
	HANDLE pid[N];
#else
	pthread_t pid[N];
#endif //WIN32
#ifdef WIN32
	max_into_room = CreateSemaphore(NULL, 0, N-1, NULL);
#else
	sem_init(&max_into_room, 0, N-1);
#endif //WIN32
	for(i = 0; i < N; i+=1)
#ifdef WIN32
		InitializeCriticalSection(&chopstick[i]);
#else
		pthread_mutex_init(&chopstick[i], NULL);
#endif //WIN32
	for(i = 0; i < N; i+=1)
	{
		arg[i] = i;
#ifdef WIN32
		pid[i] = CreateThread(NULL, 0, philosopher, &arg[i], 0/*CREATE_SUSPENDED*/, NULL);
#else
		pthread_create(&pid[i], NULL, philosopher, &arg[i]);
#endif //WIN32
	}


	for(i = 0; i < N; i+=1)
#ifdef WIN32
		ResumeThread(pid[i]);
#else
		pthread_join(pid[i], NULL);
#endif //WIN32

#ifdef WIN32
	while(1)
	{
		Sleep(100000);
	}
#endif //WIN32
#ifdef WIN32
	CloseHandle(max_into_room);
	
#else
	sem_destroy(&max_into_room);
#endif //WIN32
#ifdef WIN32
	for (i = 0; i < N; i += 1)
	{
		DeleteCriticalSection(&chopstick[i]);
	}
#endif //WIN32
}
