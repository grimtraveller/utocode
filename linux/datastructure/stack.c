#include "public.h"
#include "stack.h"

void CreateStack(STACK* s)
{
	s->base = (SElement*)DSmalloc(sizeof(SElement) * STACK_INIT_SIZE);
	s->top = s->base;
	s->nSize = STACK_INIT_SIZE;
}

void DestroyStack(STACK* s)
{
	free(s->base);
	s->nSize = 0;
}

BOOLEAN IsStackEmpty(STACK* s)
{
	if (s->top == s->base)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void ClearStack(STACK* s)
{
	s->top = s->base;
}

unsigned int GetStackLength(STACK* s)
{
	return (s->top - s->base);
}

Status GetStackTop(STACK* s, SElement* e)
{
	if (IsStackEmpty(s))
	{
		//Stack is Empty!;
		return ERROR;
	}
	else
	{
		*e = *(s->top);
		return OK;
	}
}

void Push(STACK* s, SElement e)
{
	if (s->nSize == (s->top - s->base))
	{
		s->nSize += STACKINCREMENT;
		s->base = (SElement*)DSrealloc(s->base, sizeof(SElement) * s->nSize);
	}
	s->top++;
	*(s->top) = e;
}

int Pop(STACK* s, SElement* e)
{
	if (IsStackEmpty(s))
	{
		//"Stack is Empty!";
		return ERROR;
	}
	*e = *(s->top);
	s->top--;
	return OK;
}
