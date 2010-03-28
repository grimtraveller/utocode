/**
  @file		sq_queue.h
  @brief	declaration sq_squeue structure and functions
  @details	more see Makefile
  @author	zuohaitao
  @date		2009-03-28
  @warning	
  @bug		
  */

#ifndef SQ_QUEUE_H
#define SQ_QUEUE_H
#include "public.h"
//! element
typedef int Element;
//! element head
typedef struct tagSqQueueHead
{
	int maxSize;	//!< SqQueue max size
	Element* pBase;	//!< SqQueue base address
	BOOLEAN bFull;	//!< SqQueue is full or not
	int front;		//!< SqQueue's top
	int rear;	//!< SqQueue's bottom alway point a empty item

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
  @name		InitSqQueue()
  @brief	Initial SqQueue
  @param	SqQueue* head [I/ ] SqQueue 
  @param	int maxSize [I/ ] SqQueue length
  @param	Element* pMem [I/ ] memory address
  @return	void
  */
extern void InitSqQueue(SqQueue* head, int maxSize, Element* pMem);

/**
  @name		IsSqQueueEmpty()
  @brief	Is SqQueue empty or not
  @param	SqQueue* head [I/ ] 
  @return	BOOLEAN TRUE = empty or FALSE 
  */
extern BOOLEAN IsSqQueueEmpty(SqQueue* head);

/**
  @name		GetSqQueueLen()
  @brief	Get SqQueue Length
  @param	SqQueue* head [I/ ] 
  @return	int - length
  */
extern int GetSqQueueLen(SqQueue* head);

/**
  @name		ClearSqQueue
  @brief	clean SqQueue
  @param	SqQueue* head [I/ ] 
  @return	void
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

#endif	//SQ_QUEUE_H*/
