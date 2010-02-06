/*
 * Name:	list.c
 * Desc:	some functions about list
 * Usage:	gcc -c list.c -o list.o
 * Author:	zuohaitao 
 * Date:	2009-03-02
 */

#include "list.h"
void InitList(LISTHEAD* L)
{
	L->next = NULL;
}
void TravelList(LISTHEAD* LA, VISITFUN pfunVisit)
{
	LISTITEM* item;
	item = LA->next;
	while(NULL != item)
	{
		pfunVisit(item);
		item = item->next;
	}
}

long GetListElem(LISTHEAD* L, int pos, Element* e)
{
	int i;
	LISTITEM* where;
	where = L->next;
	
	for (i = 0; i < pos; i++)
	{
		if (NULL == where)
		{
			return FALSE;	
		}
		where = where->next;
	}
	*e= where->e;
	return TRUE;
}

BOOLEAN LocateList(LISTHEAD* L, const Element e)
{
	LISTITEM* Litem;
	Litem = L->next;
	while(NULL !=Litem)
	{
		if (e == Litem->e)
		{
			return TRUE;
		}
		Litem = Litem->next;
	}
	return FALSE;
}
void PutListItem(LISTHEAD* L, int pos, LISTITEM* item)
{
	int i;
	LISTITEM* where;
	if (NULL == L->next)
	{
		item->next = L->next; 
		L->next = item;
		return;
	}
	where = L->next;
	
	for (i = 0; i < pos; i++)
	{
		if (NULL == where->next)
		{
			break;
		}
		where = where->next;
	}
	item->next = where->next;
	where->next = item;
}
int GetListLength(LISTHEAD* L)
{
	LISTITEM* Litem;
	int len;
	len = 0;
	Litem = L->next;
	while(NULL !=Litem)
	{
		len++;
		Litem = Litem->next;
	}

	return len;
}

void DestroyList(LISTHEAD* L)
{
	LISTITEM* item;
	item = L->next;
	while(NULL != item)
	{
		LISTITEM* next_item;
		next_item = item->next;
		free(item);
		item = next_item;
	}
	L->next = NULL;
}

void DelListItem(LISTHEAD* L, int pos)
{
	int i;
	LISTITEM* where;
	where = L->next;
	for (i = 0; i < pos-1; i++)
	{
		if (NULL == where->next)
		{
			return;
		}
		where = where->next;
	}
	LISTITEM* item;
	item = where->next;
	if (NULL == item)
	{
		return;
	}
	where->next = where->next->next;
	free(item);
	item = NULL;
	return;
}

