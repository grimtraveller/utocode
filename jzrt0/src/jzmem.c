/**
  @file		jzmem.c	
  @brief	implement memory functions
  @details	
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug	    
  */
#include "jzmem.h"
#include "jztype.h"
#include <stdlib.h>
#include <string.h>
jzmem_header_st* g_pjzmem = NULL;
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
		pjzmem->mgr = JZRT;
	}
	else
	{
		pjzmem->mgr = USER;
	}
	pjzmem->next = NULL;
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
void* jzmalloc(size_t size, const char* file, jzuint32 line)
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

	if (NULL != p->next)
	{
		p->next->prev = p;
	}

	return p->real_p;
}

//////////////////////////////////////////////////////////////////////////////
void* jzrealloc(void* ptr, size_t size, const char* file, jzuint32 line)
{
	jzmem_item_st* p;
	jzmem_item_st* pitem;
	pitem = (jzmem_item_st*)((char*)ptr - sizeof(jzmem_item_st));
	p = realloc(pitem, sizeof(jzmem_item_st) + size);
	strncpy(p->file, file, JZ_MAX_PATH);
	p->line = line;
	p->real_p = (char*)p + sizeof(jzmem_item_st);

	if (p != pitem)
	{
		if (NULL != p->next)
		{
			p->next->prev = p;
		}
		if (NULL != p->prev)
		{
			p->prev->next = p;
		}
		else
		{
			g_pjzmem->next = p;
		}
	}
	return p->real_p;
}

/////////////////////////////////////////////////////////////////////////////
void jzfree(void* ptr)
{
	jzmem_item_st* p;
	p = (jzmem_item_st*)((char*)ptr - sizeof(jzmem_item_st));
	if (NULL != p->prev)
	{
		p->prev->next = p->next;
	}
	else
	{
		g_pjzmem->next = NULL;
	}
	if (NULL != p->next)
	{
		p->next->prev = p->prev;
	}
	free(p);
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
