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
typedef struct tagStack STACK;
extern void CreateStack(STACK* s);
extern void DestroyStack(STACK* s);
extern BOOLEAN IsStackEmpty(STACK* s);
extern void ClearStack(STACK* s);
extern unsigned int GetStackLength(STACK* s);
extern Status GetStackTop(STACK* s, SElement* e);
extern void Push(STACK* s, SElement e);
extern Status Pop(STACK* s, SElement* e);
//int TraverseStack(SElement* e int(* visit)());
#endif	//STACK_H
