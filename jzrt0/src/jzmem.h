/**
  @file		jzmem.h
  @brief	declare memory functions
  @details	
			usage 
				JZINIT()
				JZMALLOC()
				JZREALLOC()
				JZFREE()
				JZUNINIT()
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug		
  */
#ifndef JZMEMORY_H
#define JZMEMORY_H
#ifdef __cplusplus
extern "C"
{
#endif //_cplusplus
//jzrmemory macro
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "jztype.h"
#include <stdlib.h>
#ifdef JZDEBUG
#define JZMEMINIT(p) construct_jzmem(p)
#define	JZMALLOC(size) jzmalloc(size, __FILE__, __LINE__)
#define JZREALLOC(p, size) jzrealloc(p, size, __FILE__, __LINE__)
#define JZFREE(p) jzfree(p)
#define JZMEMUNINIT destruct_jzmem
#define JZCHECKLEAK(f)   jzcheck_leak(f) 
#else
#define JZMEMINIT(p)
#define JZMALLOC(size) malloc(size)
#define JZREALLOC(p, size) realloc(p, size)
#define JZFREE(p) free(p)
#define JZMEMUNINIT
#define JZCHECKLEAK(f)
#endif	//JZDEBUG
#define JZMEM_ITEM_FLAG	"jzrt0"
#define JZMEM_ITEM_FLAG_LEN	6
typedef struct _mem_tracer_item_st
{
	char flag[JZMEM_ITEM_FLAG_LEN];
	char file[JZ_MAX_PATH];
	jzuint32 line;
	void* real_p;
	size_t len;
	struct _mem_tracer_item_st* prev;
	struct _mem_tracer_item_st* next;
} jzmem_item_st;
typedef enum
{
	JZRT = 0,
	USER = 1
} JZMEM_MGR_E;
typedef struct _mem_tracer_header_st
{
	char mgr;
	jzuint32 version;
	jzmem_item_st* next;
} jzmem_header_st;
typedef void (*CHECKLEAKCALLBACK)(char* file, jzuint32 line, void* p, size_t len);
//memory alloc
extern status construct_jzmem(jzmem_header_st* pjzmem);
extern void* jzmalloc(size_t size, const char* file, jzuint32 line);
extern void* jzrealloc(void* ptr, size_t size, const char* file, jzuint32 line);
extern void jzfree(void* ptr);
extern void destruct_jzmem();
extern void jzcheck_leak(CHECKLEAKCALLBACK fclbck);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //JZMEMORY_H
