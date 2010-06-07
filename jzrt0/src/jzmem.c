/**
  @file		jzmem.c	
  @brief	implement memory functions
  @details	
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug	    jzfree() exception	
  */
#include "jzmem.h"
#include "jztype.h"
#include <stdlib.h>
#include <string.h>
jzmem_header_st* g_pjzmem;
///////////////////////////////////////////////////////////////////////////////
status construct_jzmem(jzmem_header_st* pjzmem)
{
	if (NULL == pjzmem)
	{
		pjzmem = (jzmem_header_st*)malloc(sizeof(jzmem_header_st));
		if (NULL == pjzmem)
		{
			return ERROR;
		}
        pjzmem->next = NULL;
		pjzmem->mgr = JZRT;
	}
	else
	{
		pjzmem->mgr = USER;
	}
	g_pjzmem = pjzmem;
	return OK;
}

///////////////////////////////////////////////////////////////////////////////
void destruct_jzmem()
{
	if (JZRT == g_pjzmem->mgr)
	{
		free(g_pjzmem);
	}
	g_pjzmem = NULL;
	return;
}

///////////////////////////////////////////////////////////////////////////////
void* jzmalloc(unsigned int size, const char* file, int line)
{
	jzmem_item_st* p;
	p = malloc(sizeof(jzmem_item_st) + size);
	if (NULL == p)
	{
		perror("jzmalloc");
		exit(1);
	}
	memcpy(p->flag, JZMEM_ITEM_FLAG, JZMEM_ITEM_FLAG_LEN);
	strncpy(p->file, file, JZ_MAX_PATH);
	p->line = line;
	p->real_p = (char*)p + sizeof(jzmem_item_st);
	p->len = size;
	p->prev = NULL;
	p->next = g_pjzmem->next;
	g_pjzmem->next = p;
	return p->real_p;
}

//////////////////////////////////////////////////////////////////////////////
void* jrealloc(void* ptr, size_t size)
{
	jzmem_item_st item;
	jzmem_item_st* p;
	memcpy(item.flag, ((jzmem_item_st*)ptr)->flag, JZMEM_ITEM_FLAG_LEN);
	strncpy(item.file, ((jzmem_item_st*)ptr)->file, JZ_MAX_PATH);
	item.line = ((jzmem_item_st*)ptr)->line;
	item.prev = ((jzmem_item_st*)ptr)->prev;
	item.next = ((jzmem_item_st*)ptr)->next;
	p = realloc(ptr, sizeof(jzmem_item_st) + size);
	memcpy(p->flag, item.flag, JZMEM_ITEM_FLAG_LEN);
	strncpy(p->file, ((jzmem_item_st*)ptr)->file, JZ_MAX_PATH);
	p->line = item.line;
	p->real_p = p + sizeof(jzmem_item_st);
	p->len = size;
	p->prev = item.prev;
	p->next = item.next;
	(item.prev)->next = p;
	return p->real_p;
}

/////////////////////////////////////////////////////////////////////////////
void jzfree(void* ptr)
{
	jzmem_item_st* p;
	p = (char*)ptr - sizeof(jzmem_item_st);
	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(ptr);
	return;
}

/////////////////////////////////////////////////////////////////////////////
void jzcheck_leak(CHECKLEAKCALLBACK fclbk)
{
	jzmem_item_st* p;
	p = g_pjzmem->next;
	while(NULL != p)
	{
		fclbk(p->file, p->line, p->real_p, p->len);
		p = p->next;
	}
}
