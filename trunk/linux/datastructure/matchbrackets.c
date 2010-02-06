/*
 * Name:	matchbrackets.c
 * Descr:	match brackets
 * Usage:	$>matchbrackets
 *			$>2+[3+(7+8)*7
 * Author:	zuohaitao
 * Date:	2010-01-29
 */
#include "public.h"
#include "stack.h"
#define Bshe	'{'
#define Bhe		'}'
#define Mshe	'['
#define Mhe		']'
#define Sshe	'('
#define She		')'
BOOLEAN match(char he, char she)
{
	if (((Bhe == he) AND (Bshe == she)) 
		||((Mhe == he) AND (Mshe == she))
		||((She == he) AND (Sshe == she)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
int main()
{
	char buf[1000] = {0};
	char c, e;
	STACK s;
	scanf("%s", buf);
	int i = 0;
	CreateStack(&s);
	do
	{
		c = buf[i];
		if ((Bshe  == c) OR (Mshe == c) OR (Sshe == c))
		{
			Push(&s, c);
			i++;
			continue;
		}
		else if((Bhe == c) OR (Mhe == c) OR (She == c))
		{
			if (OK != Pop(&s, (int*)&e))
			{
				perror("Pop");
				DestroyStack(&s);
				return 1;
			}
			if (!match(c, e))
			{
				printf("not match\n");
				DestroyStack(&s);
				return 0;
			}
		}
		i++;
	}
	while('\0' != c);
	if (IsStackEmpty(&s))
	{
		printf("match\n");
	}
	else
	{
		printf("not match\n");
	}
	DestroyStack(&s);
	return 0;
}

