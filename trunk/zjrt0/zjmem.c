/**
  @file		zjmem.c	
  @brief	implement memory functions
  @details	
  @author	zuohaitao
  @date		2010-04-18
  @warning	
  @bug	    
  */
#include "zjmem.h"
#include "zjtype.h"
#include "zjerror.h"
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <errno.h>
#include <stdio.h>
#endif
zjmem_header_st* g_pzjmem = NULL;
///////////////////////////////////////////////////////////////////////////////
status construct_zjmem(zjmem_header_st* pzjmem)
{

	if (NULL == pzjmem)
	{
		pzjmem = (zjmem_header_st*)malloc(sizeof(zjmem_header_st));
		if (NULL == pzjmem)
		{
			return ZJERROR;
		}
		pzjmem->mgr = ZJRT;
	}
	else
	{
		pzjmem->mgr = USER;
	}
	pzjmem->next = NULL;
	g_pzjmem = pzjmem;
	return ZJOK;
}

///////////////////////////////////////////////////////////////////////////////
void destruct_zjmem()
{
	if (ZJRT == g_pzjmem->mgr)
	{
		free(g_pzjmem);
	}
	g_pzjmem = NULL;
	return;
}

///////////////////////////////////////////////////////////////////////////////
void* zjmalloc(size_t size, const char* file, zjuint32 line)
{
	zjmem_item_st* p;

	if (NULL == g_pzjmem)
	{
		construct_zjmem(NULL);
	}
	p = malloc(sizeof(zjmem_item_st) + size);
	if (NULL == p)
	{
		return NULL;
	}
	strncpy(p->file, file, ZJ_MAX_PATH);
	p->line = line;
	p->real_p = (char*)p + sizeof(zjmem_item_st);
	
	p->len = size;
	p->prev = NULL;
	p->next = g_pzjmem->next;
	
	g_pzjmem->next = p;

	if (NULL != p->next)
	{
		p->next->prev = p;
	}

	return p->real_p;
}

//////////////////////////////////////////////////////////////////////////////
void* zjrealloc(void* ptr, size_t size, const char* file, zjuint32 line)
{
	zjmem_item_st* p;
	zjmem_item_st* pitem;
	pitem = (zjmem_item_st*)((char*)ptr - sizeof(zjmem_item_st));
	p = realloc(pitem, sizeof(zjmem_item_st) + size);
	strncpy(p->file, file, ZJ_MAX_PATH);
	p->line = line;
	p->real_p = (char*)p + sizeof(zjmem_item_st);

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
			g_pzjmem->next = p;
		}
	}
	return p->real_p;
}

/////////////////////////////////////////////////////////////////////////////
void zjfree(void* ptr)
{
	zjmem_item_st* p;
	if (NULL == ptr)
	{
		return;
	}
	p = (zjmem_item_st*)((char*)ptr - sizeof(zjmem_item_st));

	if (NULL != p->prev)
	{
		p->prev->next = p->next;
	}
	else
	{
		g_pzjmem->next = p->next;
	}
	
	if (NULL != p->next)
	{
		p->next->prev = p->prev;
	}
	free(p);
	p = NULL;
	return;
}

/////////////////////////////////////////////////////////////////////////////
void zjcheck_leak(CHECKLEAKCALLBACK fclbk)
{
	zjmem_item_st* p;
	p = g_pzjmem->next;
	while(NULL != p)
	{
		fclbk(p->file, p->line, p->real_p, p->len);
		p = p->next;
	}
}
