/**
  @file		jztype.h
  @brief	define crossplatform c inline types
  @details	interger float etc
  @author	zuohaitao
  @date		2010-05-28
  @warning	
  @bug		
			line 73 64 bit integer in linux 
			line 48 - 53 test MAX_PATH in linux
  */
#ifndef JZTYPE_H
#define JZTYPE_H
#ifdef WIN32
#include <windows.h>
#else 
#include <stdlib.h>	//such as size_t,
#include <limits.h>	//such as PATH_MAX,
#endif
#ifdef __cplusplus
#	define EXTERN_C_BEGIN	extern "C" {
#	define EXTERN_C_END	}
#else	/* !__cplusplus */
#	define EXTERN_C_BEGIN
#	define EXTERN_C_END
#endif	/*__cplusplus */


/**
 * @brief inalid point
 */
#ifndef NULL
#define NULL	0
#endif
/**
 * @brief value only is 0 (FALSE) or 1 (TRUE)
 */
#define BOOLEAN		long
/**
 * @brief true the value of boolean
 */
#define	TRUE		1
/**
 * @brief false the value of boolean
 */
#define FALSE		0
/**
 * @brief success the value of status
 */
#define	OK			1
/**
 * @brief error the value of status
 */
#define	ERROR		0
/**
 * @brief unable error the value of status
 */
#define	INFEASIBLE	-1
/**
 * @brief overflow error the value of status
 */
#define	ZJOVERFLOW	-2
/**
 * @brief logic operation 'and'
 */
#define	AND		&&
/**
 * @brief logic operation 'or'
 */
#define	OR		||
/**
 * @brief function status normally, is OK ERROR 
 */
typedef int status;
#ifdef WIN32
#define JZ_MAX_PATH	MAX_PATH
#else
#define JZ_MAX_PATH PATH_MAX
#endif
/**
 * @brief jzrt0 types
 */
typedef signed char jzint8;
typedef unsigned char jzuint8;

typedef signed short jzint16;
typedef unsigned short jzuint16;

typedef int jzint32;
typedef unsigned int jzuint32;

#ifdef WIN32
typedef long long jzint64;
typedef unsigned long long jzuint64;
#else
typedef long long jzint64;
typedef unsigned long long jzuint64;
#endif
typedef float jzfloat32;
typedef double jzfloat64;
typedef size_t jzsize;
typedef jzuint32 jzoffset32;
typedef jzuint64 jzoffset64;
//typedef ptrdiff_t __PRPtrdiff;
#endif

