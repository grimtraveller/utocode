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
/**********Log API**********/
extern void z_perror(const char* s);
/**********Memory Operation API**********/
extern void* z_malloc(size_t size);
extern void* z_realloc(void* ptr, size_t size);
extern void z_free(void* ptr);
#endif
