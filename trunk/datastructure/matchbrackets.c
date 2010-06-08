/**
  @file		matchbrackets.c
  @brief	match brackets
  @details	usage 
			$>matchbrackets
			$>2+[3+(7+8)*7
  @author	zuohaitao
  @date		2010-01-29
  @warning	
  @bug		
  */

#include "public.h"
#include "stack.h"
#define Bshe	'{'
#define Bhe		'}'
#define Mshe	'['
#define Mhe		']'
#define Sshe	'('
#define She		')'

/**
  @name		match
  @brief	match function
  @param	char he [I/ ]  left bracket
  @param	char she [I/ ]  right bracket
  @return	BOOLEAN
  */
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
	int i = 0;
	scanf("%s", buf);
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

