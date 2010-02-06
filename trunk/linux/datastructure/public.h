#ifndef PUBLIC_H
#define PUBLIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NULL
#define NULL	0
#endif
#define BOOLEAN		long
#define	TRUE		1
#define FALSE		0
#define	OK			1
#define	ERROR		0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	AND		&&
#define	OR		||
typedef int Status;
//LOG
extern void DSperror(const char* s);
//内存分配
extern void* DSmalloc(size_t size);
extern void* DSrealloc(void* ptr, size_t size);
extern void DSfree(void* ptr);
#endif
