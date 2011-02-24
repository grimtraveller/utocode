/**
  @file		zjtype.h
  @brief	define crossplatform c inline types
  @details	interger float etc
  @author	zuohaitao
  @date		2010-05-28
  @warning	
  @bug		
			line 73 64 bit integer in linux 
			line 48 - 53 test MAX_PATH in linux
			add type range
  */
#ifndef ZJTYPE_H
#define ZJTYPE_H
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
#define ZJBOOLEAN		long
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
#define ZJ_MAX_PATH	MAX_PATH
#else
#define ZJ_MAX_PATH PATH_MAX
#endif
/**
 * @brief zjrt0 types
 */
typedef signed char zjint8;
#define ZJINT8_MIN (-128)
#define ZJINT8_MAX 127
typedef unsigned char zjuint8;
#define ZJUINT8_MAX 0xff

typedef signed short zjint16;
#define ZJINT16_MIN (-32768)
#define ZJINT16_MAX 32767
typedef unsigned short zjuint16;
#define ZJUINT16_MAX 0xffff

typedef int zjint32;
#define ZJINT32_MIN (-2147483646)
#define ZJINT32_MAX 2147483647
typedef unsigned int zjuint32;
#define ZJUINT32_MAX 0xffffffff


#ifdef WIN32
typedef long long zjint64;
#define ZJINT64_MIN (-9223372036854775806i64)
#define ZJINT64_MAX 9223372036854775807i64
typedef unsigned long long zjuint64;
#define ZJUINT64_MAX 0xffffffffffffffffui64
#else
typedef long long zjint64;
typedef unsigned long long zjuint64;
#endif
typedef float zjfloat32;
typedef double zjfloat64;
typedef size_t zjsize;
typedef zjuint32 zjoffset32;
typedef zjuint64 zjoffset64;

#endif

