#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
int
main(int argc, char* argv[])
{
	int i;
	int e = -1;
	STACK s;
	CreateStack(&s);
   for (i = 0; i < 129; i++)
	{
		Push(&s, i);
		printf("stack length = %d\n", GetStackLength(&s));
	}

	ClearStack(&s);
	if (0 == GetStackLength(&s))
	{
		printf("ClearStack Success!\n");
	}
	else
	{

		printf("ClearStack Fail!\n");
	}
	for (i = 0; i < 129; i++)
	{
		Push(&s, i);
	}

	for (i = 0; i < 129; i++)
	{
		GetStackTop(&s, &e);
		printf("GetStackTop:%d ", e);
		e = -1;
		if (ERROR == Pop(&s, &e))
		{
			return 1;
		}
		printf("Pop:%d \n", e);
	}
	DestroyStack(&s);
	return 0;
}
