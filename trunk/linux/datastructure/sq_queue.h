/**
  @file		sq_squeue.h
  @brief	declaration sq_squeue structure and functions
  @details	
  @author	zuohaitao
  @date		2009-03-28
  @warning	
  @bug		
  */
#ifndef SQ_QUEUE_H
#define SQ_QUEUE_H
#include "public.h"
//element
typedef int Element;
//element head
typedef struct tagSqQueueHead
{
	int maxSize;
	Element* pBase;
	BOOLEAN bFull;
	int front;
	int rear;	//alway point a empty item

} SqQueue;
/**
  @name		CreateSqQueue()
  @brief	create queue
  @param	SqQueue* head [ /O] sqQueue
  @param	int maxSize [I/ ]  queue size
  @return	void
  */
extern void CreateSqQueue(SqQueue* head, int maxSize);

/**
  @name		DeleteSqQueue()
  @brief	delete SqQueue
  @param	SqQueue* head [I/ ] 
  @return	void
  */
extern void DeleteSqQueue(SqQueue* head);

/**
  @name		*
  @brief	
  @param	 [I/O] 
  @return	
  */
extern void InitSqQueue(SqQueue* head, int maxSize, Element* pMem);

/**
  @name		*
  @brief	
  @param	 [I/O] 
  @return	
  */
extern BOOLEAN IsSqQueueEmpty(SqQueue* head);

/**
  @name		*
  @brief	
  @param	 [I/O] 
  @return	
  */
extern int GetSqQueueLen(SqQueue* head);

/**
  @name		*
  @brief	
  @param	 [I/O] 
  @return	
  */
extern void ClearSqQueue(SqQueue* head);

/**
  @name		PushSqQueueElem()
  @brief	push a element to SqQueue
  @param	SqQueue* head [I/ ] 
  @param	Element e [I/ ] 
  @return	BOOLEAN TRUE = Sucess or FALSE 
  */
extern BOOLEAN PushSqQueueElem(SqQueue* head, Element e);

/**
  @name		PopSqQueueElem()
  @brief	pop a element from SqQueue
  @param	SqQueue* head [I/ ] 
  @param	Element* e[I/ ] 
  @return	BOOLEAN TRUE = success or FALSE = fail
  */
extern BOOLEAN PopSqQueueElem(SqQueue* head, Element* e);

/**
  @name		GetSqQueueElem()
  @brief	get a element of SqQueue
  @param	SqQueue* head [I/ ] sqqueue 
  @param	int index [I/ ] position
  @param	Element* e [/O] element 
  @return	BOOLEAN	TRUE = Success or FALSE = FAIL
  */
extern BOOLEAN GetSqQueueElem(SqQueue* head, int index, Element* e);

#endif	//SQ_QUEUE_H
