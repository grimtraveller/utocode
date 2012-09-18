/**
  @file		zjtype.h
  @brief	define crossplatform c inline types
  @details	interger float etc
  @author	zuohaitao
  @date		2010-05-28
  @warning	
  @bug		
			ZTODO line 73 64 bit integer in linux 
			ZTODO line 48 - 53 test MAX_PATH in linux
			add type range
			rename BOOLEAN to ZJBOOL becuase BOOLEAN is used in other frame code
  */
#ifndef ZJTYPE_H
#define ZJTYPE_H
#if defined(WIN32)	//OS
#include <windows.h>
#elif defined(MACOSX)
#elif defined(LINUX)
#include <stdlib.h>	//such as size_t,
#include <limits.h>	//such as PATH_MAX,
#else
#error("unknow OS!")
#endif //OS

/**
 * @brief inalid point
 */
#if defined(MACOSX)
//use nil Nil
#else
#ifndef NULL
#define NULL	0
#endif
#endif //OS
/**
 * @brief value only is 0 (FALSE) or 1 (TRUE)
 */
#if defined(WIN32)	//OS
#  define ZJBOOL	BOOL	
#elif defined(MACOSX)
#  error("ZJBOOL is undefine in Mac OS X")
#elif defined(LINUX)
#  error("ZJBOOL is undefine in linux")
#else
#  error("unknow OS!")
#endif //OS
/**
 * @brief true the value of ZJBOOL
 */
#ifndef TRUE
#  if defined(MACOSX)	//OS
#    define TRUE	YES
#  else
#    define TRUE	1
#  endif //OS
#endif //TRUE
/**
 * @brief false the value of ZJBOOL
 */
#ifndef FALSE
#  if defined(MACOSX)	//OS
#    define FALSE	NO
#  else
#    define FALSE	0
#  endif //OS
#endif //FALSE
/**
 * @brief success the value of status
 */
#define	ZJOK			1
/**
 * @brief error the value of status
 */
#define	ZJERROR		0
/**
 * @brief unable error the value of status
 */
#define	ZJINFEASIBLE	-1
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
#if defined(WIN32)
#  define ZJ_MAX_PATH	MAX_PATH
#elif defined(LINUX)
#  error("ZJ_MAX_PATH is undefine in linux")
#  define ZJ_MAX_PATH PATH_MAX
#elif defined(MACOSX)
#  error("ZJ_MAX_PATH is undefine in mac os x")
#endif
/**
 * @brief zjrt0 types
 */
typedef signed char zjint8;
#define ZJINT8_MIN (-128)
#define ZJINT8_MAX 127
typedef unsigned char zjuint8;
#define ZJUINT8_MAX 0xff

typedef zjuint8 zjbyte;
#define ZJBYTE_MAX 0xff

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


typedef long long zjint64;
typedef unsigned long long zjuint64;
#if defined(WIN32)	//OS
#  define ZJUINT64_MAX 0xffffffffffffffffui64
#  define ZJINT64_MIN (-9223372036854775806i64)
#  define ZJINT64_MAX 9223372036854775807i64
#else
#  define ZJUINT64_MAX 0xffffffffffffffff
#  define ZJINT64_MIN (-9223372036854775806)
#  define ZJINT64_MAX 9223372036854775807
#endif //OS
typedef float zjfloat32;
typedef double zjfloat64;

typedef zjuint32 zjoffset32;
typedef zjuint64 zjoffset64;

typedef size_t zjsize;

#endif //ZJTYPE_H

