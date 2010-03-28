/**
  @file		stack.h
  @brief	declaration stack functions
  @details	
  @author	zuohaitao
  @date		2009-03-28
  @warning	
  @bug		
  */
#ifndef STACK_H
#define STACK_H
#include "public.h"
#define STACK_INIT_SIZE 100
#define	STACKINCREMENT	10
#define SElement int
//typedef int SElement;
struct tagStack
{
	SElement* base;
	SElement* top;
	int nSize;
};
//!@brief tagStack
typedef struct tagStack STACK;
/**
  @name		CreateStack()
  @brief	create stack
  @param	STACK* s [I/ ] stack 
  @return	void
  */
extern void CreateStack(STACK* s);

/**
  @name		DestroyStack()
  @brief	destroy stack
  @param	STACK* s [I/ ] 
  @return	void
  */
extern void DestroyStack(STACK* s);

/**
  @name		*
  @brief	
  @param	 [I/O] 
  @return	
  */
extern BOOLEAN IsStackEmpty(STACK* s);

/**
  @name		ClearStack()
  @brief	clean stack
  @param	STACK* s [I/ ] 
  @return	void
  */
extern void ClearStack(STACK* s);

/**
  @name		GetStackLength()
  @brief	get length of stack
  @param	STACK* s [I/ ] 
  @return	unsigned int
  */
extern unsigned int GetStackLength(STACK* s);

/**
  @name		GetStackTop
  @brief	get top of stack
  @param	STACK* s [I/ ] stack
  @param	SElemet e [/O] element
  @return	
  */
extern Status GetStackTop(STACK* s, SElement* e);

/**
  @name		Push
  @brief	push element to stack
  @param	STACK* s [I/ ] stack
  @param	SElement e [I/ ] element 
  @return	void
  */
extern void Push(STACK* s, SElement e);

/**
  @name		Pop
  @brief	pop element from stack
  @param	STACK* s [I/ ] stack 
  @param	SElement* e [ /O] SElement* e
  @return	Status OK success or ERROR
  */
extern Status Pop(STACK* s, SElement* e);

//int TraverseStack(SElement* e int(* visit)());
#endif	//STACK_H
