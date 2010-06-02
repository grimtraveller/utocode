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
#include "jztype.h"
#ifdef JZDEBUG
#define JZINIT(p)\
	construct_jzmem(p)
#define	JZMALLOC(size)\
	jzmalloc(size, __FILE__, __LINE__)
#define JZREALLOC(p, size)\
	jzrealloc(p, size)
#else
#define JZMALLOC(size)\
	malloc(size)
#endif	//JZDEBUG
#define JZMEM_ITEM_FLAG	"jzrt0"
#define JZMEM_ITEM_FLAG_LEN	6
typedef struct _mem_tracer_item_st
{
	char flag[JZMEM_ITEM_FLAG_LEN];
	char file[JZ_MAX_PATH];
	jzuint32 line;
	void* real_p;
	jzuint32 len;
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
typedef void (*CHECKLEAKCALLBACK)(char* file, jzint32 line, void* p, int len);
//memory alloc
extern status construct_jzmem(jzmem_header_st* pjzmem);
extern void* jzmalloc(unsigned int size, const char* file, int line);
extern void* jzrealloc(void* ptr, int size);
extern void jzfree(void* ptr);
extern void destruct_jzmem();
extern void jzcheck_leak(CHECKLEAKCALLBACK fclbck);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //JZMEMORY_H
