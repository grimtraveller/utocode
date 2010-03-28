#ifndef PUBLIC_H
#define PUBLIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NULL
#define NULL	0				//!< NULL POINT
#endif
#define BOOLEAN		long		//!< BOOLEAN TYPE
#define	TRUE		1			//!< BOOLEAN TRUE
#define FALSE		0			//!< BOOLEAN FALSE
#define	OK			1			//!< Status Success
#define	ERROR		0			//!< Status ERROR
#define	INFEASIBLE	-1			//!< Status INFEASIBLE
#define	OVERFLOW	-2			//!< Status OVERFLOW
#define	AND		&&				//!< Logic AND
#define	OR		||				//!< Logic OR
typedef int Status;				//!< Function Status
//Buffer Size
#define	SMALLBUFLEN	512
#define	BUFLEN		1024
#define BIGBUFLEN	(1024*128)
//LOG
extern void DSperror(const char* s);
//Memory Functions
extern void* DSmalloc(size_t size);
extern void* DSrealloc(void* ptr, size_t size);
extern void DSfree(void* ptr);
#endif	//PUBLIC_H
