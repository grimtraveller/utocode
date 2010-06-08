/*
 * Name:	merge.c
 * Descr:	merge closed interval
			like
			[5,6]
			[1,4]
			[10,10]
			[6,9]
			[8,10]
 * Usage:
 * Author:	zuohaitao
 * Date:	2009/02/21
 */
#include<stdio.h>
struct tagInterval
{
	int min;
	int max;
	struct tagInterval* next;
};

struct tagHead
{
	int nCount;
	struct tagInterval* next
};

int ReadFrom(struct tagHead* pHead)
{
	int i;
	tagInterval* pItem;
	fscanf(stdin, "%d", &(pHead->nCount));
	for (i = 0; i < pHead->nCount; i++)
	{
		fgets(stdin, line, MAX_LINE);
		split_line(line, min, max);
		pItem = (tagInterval*)malloc(sizeof(tagInterval));
		pItem->min = min;
		pItem->max = max;
		pItem->next = pHead->next;
		pHead->next = pItem;
	}
	return pHead->nCount;
}

int WriteTo(struct tagHead* pHead)
{
}

int main()
{
	int i,j;
	char* 
	fget(
	for (i = 
	return 0;

