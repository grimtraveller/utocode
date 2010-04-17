#ifndef JZMEMORY_H
#define JZMEMORY_H
#ifdef __cplusplus
extern "C"
{
#endif //_cplusplus
//jzrmemory macro
#include "jztype.h"
#ifdef DEBUG
#define	JZMALLOC(size)\
	jzmalloc(size, __FILE__, __LINE__)
#else
#define JZMALLOC(size)\
	malloc(size)
#endif	//DEBUG
#define JZMEM_ITEM_FLAG	"jzrt0"
#define JZMEM_ITEM_FLAG_LEN	6
typedef struct _mem_tracer_item_st
{
	char flag[JZMEM_ITEM_FLAG_LEN];
	char file[JZ_MAX_PATH];
	int line;
	void* real_p;
	unsigned int len;
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
	int version;
	jzmem_item_st* next;
} jzmem_header_st;
typedef void (*CHECKLEAKCALLBACK)(char* file, int line, void* p, int len);
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
