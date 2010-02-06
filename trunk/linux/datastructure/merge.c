/*
 * Name:	merge.c
 * Desc:	merge two list
 * Usage:	$>make merge
 *			$>merge
 * Author:	zuohaitao
 * date:	2009-03-02
 */
#include "public.h"
#include "list.h"

//#include <mcheck.h>
void ShowItem(LISTITEM* item);
void MergeList();
int main()
{
	//mtrace();
	LISTHEAD LA;
	LISTHEAD LB;
	int i;
	InitList(&LA);
	InitList(&LB);
	for (i = 0; i< 10; i++)
	{
		LISTITEM* item;
		item = (LISTITEM*)DSmalloc((size_t)sizeof(LISTITEM));
		item->e = i;
		item->next = NULL;
		PutListItem(&LA, 0, item);
	}
	printf("LA:");
	TravelList(&LA, ShowItem);
	printf("\n");
	for (i = 0; i< 10; i++)
	{
		LISTITEM* item;
		item = (LISTITEM*)DSmalloc((size_t)sizeof(LISTITEM));
		item->e = i+7;
		PutListItem(&LB, 0, item);
	}
	printf("LB:");
	TravelList(&LB, ShowItem);
	printf("\n");
	printf("Merge...\n");
	MergeList(&LA, &LB);
	printf("LA:");
	TravelList(&LA, ShowItem);
	printf("\n");
	DestroyList(&LA);
	DestroyList(&LB);
	return 0;
}

void ShowItem(LISTITEM* item)
{
	printf("%d ", item->e);
}

void MergeList(LISTHEAD* LA, LISTHEAD* LB)
{
	int i;
	int LBlen;
	Element e;
	LBlen = GetListLength(LB);
	for (i = 0; i < LBlen; i++)
	{
		GetListElem(LB, i, &e);
		if (!LocateList(LA, e))
		{
			LISTITEM* item;
			item = (LISTITEM*)DSmalloc((size_t)sizeof(LISTITEM));
			item->e = e;
			PutListItem(LA, GetListLength(LA), item);
		}
	}
	return;
}
