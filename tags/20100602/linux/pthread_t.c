/*
 * Name:	pthread_t.c
 * Desc:	test create threads
 * Usage:	$>gcc src/pthread_t.c -lpthread -o bin/pthread_t
 * Author:	zuohaitao
 * Date:	2009-03-13
 */
#include<stdio.h>
#include<pthread.h>
#define NUM 5
main()
{
	pthread_t t1,t2;
	void *print_msg(void *);
	char hello[] = "Hello";
	char world[] = "Thread World\n";
	pthread_create(&t1,NULL,print_msg,(void *)hello);
	pthread_create(&t2,NULL,print_msg,(void *)world);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}

void *print_msg(void * m)
{
	char *cp=(char *)m ;
	int i;
	for(i=0;i<NUM;i++){
		printf("%s",m);
		fflush(stdout);
		sleep(1);
	}
	return NULL;

}
