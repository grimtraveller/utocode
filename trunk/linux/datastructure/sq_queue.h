#ifndef SQ_QUEUE_H
#define SQ_QUEUE_H
#include "public.h"
typedef int Element;
typedef struct tagSqQueueHead
{
	int maxSize;
	Element* pBase;
	BOOLEAN bFull;
	int front;
	int rear;	//alway point a empty item

} SqQueue;

extern void CreateSqQueue(SqQueue* head, int maxSize);
extern void DeleteSqQueue(SqQueue* head);
extern void InitSqQueue(SqQueue* head, int maxSize, Element* pMem);
extern BOOLEAN IsSqQueueEmpty(SqQueue* head);
extern int GetSqQueueLen(SqQueue* head);
extern void ClearSqQueue(SqQueue* head);
extern BOOLEAN PushSqQueueElem(SqQueue* head, Element e);
extern BOOLEAN PopSqQueueElem(SqQueue* head, Element* e);
extern BOOLEAN GetSqQueueElem(SqQueue* head, int index, Element* e);
#endif	//SQ_QUEUE_H
