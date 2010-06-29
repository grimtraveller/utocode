/**
 * @file	jzerror.h 
 * @brief	jz runtime error functions
 * @details	jzerrno
 *			jzstrerror()
 *			jzperror()
 *			jzerror_exit()
 *			jzerror_set()
 *			jzerror_return()
 * @author	zuohaitao
 * @date	2010-06-29 
 * @warning	
 * @bug	
 */
#ifndef JZERROR_H
#define JZERROR_H
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include "jztype.h"
#ifdef WIN32
#define jzerrno GetLastError()
#else
#define jzerrno errno
#endif //WIN32
#define jzerror_exit()	\
	jzperror();	\
	exit(jzerrno);
#define jzerror_set(flag)	\
	flag = FALSE;	\
	jzperror();
#define jzerror_return(val)	\
	jzperror();	\
	return val;
typedef void (*SHOWMSG)(const char*);
extern SHOWMSG g_showfn;
extern void jzperror();

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //JZERROR_H
