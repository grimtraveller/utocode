/**
  @file		zjmem.h
  @brief	declare memory functions
  @details	
			usage 
				ZJINIT()
				ZJMALLOC()
				ZJREALLOC()
				ZJFREE()
				ZJCHECKLEAK()
				ZJUNINIT()
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug		
  */
#ifndef ZJMEMORY_H
#define ZJMEMORY_H
#ifdef __cplusplus
extern "C"
{
#endif //_cplusplus
//zjrmemory macro
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdlib.h>
#include <string.h> //such as memset
#include "zjtuxlist.h"
#ifdef ZJDEBUG
#define ZJMEMINIT(p) construct_zjmem(p)
#define	ZJMALLOC(size) zjmalloc(size, __FILE__, __LINE__)
#define ZJREALLOC(p, size) zjrealloc(p, size, __FILE__, __LINE__)
#define ZJFREE(p) zjfree(p)
#define ZJMEMUNINIT() destruct_zjmem()
#define ZJCHECKLEAK(f)   zjcheck_leak(f) 
#else
#define ZJMEMINIT(p)
#define ZJMALLOC(size) malloc(size)
#define ZJREALLOC(p, size) realloc(p, size)
#define ZJFREE(p) free(p)
#define ZJMEMUNINIT()
#define ZJCHECKLEAK(f)
#endif	//ZJDEBUG
typedef struct MEM_TRACER_ITEM
{
	TuxListHead list;
	char file[ZJ_MAX_PATH];
	zjuint32 line;
	void* real_p;
	size_t len;
} ZJMemTracerItem;
typedef void (*CheckLeakCallBack)(char* file, zjuint32 line, void* p, size_t len);
//memory alloc
extern status zjcreateMem(zjmem_header_st* pzjmem);
extern void* zjmalloc(size_t size, const char* file, zjuint32 line);
extern void* zjrealloc(void* ptr, size_t size, const char* file, zjuint32 line);
extern void zjfree(void* ptr);
extern void zjdestroyMem();
extern void zjcheckLeakage(CHECKLEAKCALLBACK fclbck);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //ZJMEMORY_H
