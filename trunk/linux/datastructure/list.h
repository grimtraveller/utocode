#ifndef LIST_H
#define LIST_H
#include "public.h"
typedef int Element;
typedef struct ListItem
{
	Element e;
	struct ListItem* next;
} LISTITEM;
typedef struct ListHead
{
	struct ListItem* next;
} LISTHEAD;
typedef struct ListHead LIST;
typedef void (*VISITFUN)(LISTITEM* item);
extern void InitList(LISTHEAD* L);
extern void DestroyList(LISTHEAD* L);
//void ClearList(LISTHEAD* L);
//BOOLEAN IsListEmpty(LISTHEAD* L);
//GetListPriorListElem(L, cur_e, &pre_e);
//GetListNextListElem(L, cur_e, &pre_e);
extern BOOLEAN NextListElem(LISTHEAD* L, LISTITEM* cur_item, LISTITEM* next_item);
extern BOOLEAN LocateList(LISTHEAD* L, const Element e);

extern int GetListLength(LISTHEAD* L);
extern void TravelList(LISTHEAD* L, VISITFUN visit);
extern void PutListItem(LISTHEAD* L, int pos, LISTITEM* item);
extern long GetListElem(LISTHEAD* L, int pos, Element* e);
extern void DelListItem(LISTHEAD* L, int pos);
//InsertListElem()
//DeleteListElem()
#endif
