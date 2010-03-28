/**
  @file		sq_queue.c
  @brief	squeue functions
  @details	
  @author	zuohaitao
  @date		2009-03-28
  @warning	
  @bug		
  */

#include "public.h"
#include "sq_queue.h"
void CreateSqQueue(SqQueue* head, int maxSize)
{
	head->maxSize = maxSize;
	Element* pMem;
	pMem = (Element*)DSmalloc(sizeof(Element)*maxSize);
	InitSqQueue(head, maxSize, pMem);
}

void InitSqQueue(SqQueue* head, int maxSize, Element* pMem)
{
	head->pBase = pMem;
	head->maxSize = maxSize;
	head->front = 0;
	head->rear = 0;
	head->bFull = 0;
}

extern void DeleteSqQueue(SqQueue* head)
{
	free(head->pBase);
}


BOOLEAN IsSqQueueEmpty(SqQueue* head)
{
	return ((!head->bFull)&&(head->rear == head->front));
}

int GetSqQueueLen(SqQueue* head)
{
	/*
	   if (rear > front) len=rear-front+1=maxSize*0+rear-front;
	   else if (rear < front) len=maxSize-(front-rear)=maxSize*1+(rear-front)
	   so...
	*/
	return ((head->maxSize+head->rear-head->front)%head->maxSize);
}

void ClearSqQueue(SqQueue* head)
{
	int i;
	for (i = 0; i < head->maxSize; i++)
	{
		memset(head->pBase+i, 0, sizeof(Element));
	}
	head->front = head->rear = 0;
}

BOOLEAN PushSqQueueElem(SqQueue* head, Element e)
{
	if (!head->bFull)
	{
		memcpy(head->pBase+head->rear, &e, sizeof(Element));
		head->rear = (head->rear+1)%head->maxSize;
		if (head->front == head->rear)
		{
			head->bFull = TRUE;
		}
		return TRUE;
	}
	return FALSE;
}
BOOLEAN PopSqQueueElem(SqQueue* head, Element* e)
{
	if (IsSqQueueEmpty(head))
	{
		return FALSE;
	}
	memcpy(e, head->pBase+head->front, sizeof(Element));
	head->front = (head->front+1)%head->maxSize;
	if (head->bFull)
	{
		head->bFull = FALSE;
	}
	return TRUE;
}

BOOLEAN GetSqQueueElem(SqQueue* head, int index, Element* e)
{
	if (index > GetSqQueueLen(head))
	{
		return FALSE;
	}
	index = (index+head->front)%head->maxSize;
	memcpy(e, head->pBase+index, sizeof(Element));
	return TRUE;
}

