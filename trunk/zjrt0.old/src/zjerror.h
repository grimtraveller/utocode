/**
 * @file	zjerror.h 
 * @brief	zj runtime error functions
 * @details	zjerrno
 *			zjstrerror()
 *			zjperror()
 *			zjerror_exit()
 *			zjerror_set()
 *			zjerror_return()
 * @author	zuohaitao
 * @date	2010-06-29 
 * @warning	
 *			///todo zjwin32err
 * @bug	
 */
#ifndef ZJERROR_H
#define ZJERROR_H
#include "zjtype.h"
#ifndef WIN32
#include <errno.h>
#endif //WIN32
EXTERN_C_BEGIN
#ifdef WIN32
#define zjerrno GetLastError()
#else
#define zjerrno errno
#endif //WIN32
#define zjerror_exit()	zjperror();	\
						exit(zjerrno);
#define zjerror_set(flag)	flag = FALSE;	\
							zjperror();
#define zjerror_return(val)	zjperror();	\
							return val;
typedef void (*SHOWMSG_FUNC)(const char*);
extern SHOWMSG_FUNC g_showfn;
extern void zjperror();
EXTERN_C_END
#endif //ZJERROR_H
