#include <stdio.h>
#include <stddef.h>
typedef struct
{
	int i;
	char* pc;
	char s[100];
	int* pi;
}S;

/*
 * type member type
 * s struct
 * m member
 */
#define MEMBER_OFFSET(s, m)	((size_t)(&(((s*)0)->m)))

int
main(int argc, char* argv[])
{
	printf("%d\n", (int)MEMBER_OFFSET(S, pi));
	printf("offsetof(S, pi)=%d\n", (int)offsetof(S, pi));
	return 0;
}
