/**
  @file		sq_queue_t.c
  @brief	test sqqueue
  @details	
  @author	zuohaitao
  @date		2010-03-28
  @warning	
  @bug		
  */
#include "public.h"
#include "sq_queue.h"
int main()
{
	//Element sqQueue[10];
	SqQueue head;
	//InitSqQueue(&head, 10, sqQueue);
	int i;
	CreateSqQueue(&head, 10);
	if (IsSqQueueEmpty(&head))
	{
		printf("sqQueue is Empty\n");
	}
	else
	{
		printf("sqQueue is not Empty\n");
	}
	printf("Len=%d\n", GetSqQueueLen(&head));
	printf("PUSH 5\n");
	for (i = 0; i < 5; i++)
	{
		PushSqQueueElem(&head, i);
	}
	if (IsSqQueueEmpty(&head))
	{
		printf("sqQueue is Empty\n");
	}
	else
	{
		printf("sqQueue is not Empty\n");
	}
	printf("Len=%d\n", GetSqQueueLen(&head));
	printf("POP 5\n");
	for (i = 0; i < 5; i++)
	{
		int cur;
		PopSqQueueElem(&head, &cur);
		printf("%d ", cur);
	}
	printf("\n");
	if (IsSqQueueEmpty(&head))
	{
		printf("sqQueue is Empty\n");
	}
	else
	{
		printf("sqQueue is not Empty\n");
	}
	printf("POP 10\n");
	for (i = 0; i < 10; i++)
	{
		PushSqQueueElem(&head, i);
	}
	printf("POP AGAIN\n");
	printf("%d\n", (int)PushSqQueueElem(&head, i));
	printf("POP 5\n");
	for (i = 0; i < 5; i++)
	{
		int cur;
		PopSqQueueElem(&head, &cur);
		printf("%d ", cur);
	}
	printf("\n");
	printf("POP 3\n");
	for (i = 10; i < 13; i++)
	{
		PushSqQueueElem(&head, i);
	}
	printf("Len=%d\n", GetSqQueueLen(&head));
	for (i = 0; i < GetSqQueueLen(&head); i++)
	{
		int e;
		GetSqQueueElem(&head, i, &e);
		printf("%d ", e);
	}
	printf("\n");
	DeleteSqQueue(&head);
	return 0;
}

