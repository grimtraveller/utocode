/*
 * Name:	lineedit.c
 * Descr:	a line edit applicaion
 * Usage:	$>lineedit
 * Author:	zuohaitao
 * Date:	2010 02 03
 */
#include "public.h"
#include "stack.h"
#define BACKSPACE	'#'
#define DELLINE		'@'
#define BUFLEN		1000
int
main()
{
	char buf[BUFLEN] = {0};
	char c = 0;
	STACK s;
	int i=0;
	char* pout = NULL;
	int n = 0;
	scanf("%s", buf);
	CreateStack(&s);
	while(0 != buf[i])
	{
		c = buf[i];
		if (BACKSPACE == c)
		{
			Pop(&s, (int*)&c);
		}
		else if (DELLINE == c)
		{
			ClearStack(&s);
		}
		else
		{
			Push(&s,c);
		}
		i++;
	}
	n = GetStackLength(&s);
	pout = DSmalloc(sizeof(char) * n);
	while (!IsStackEmpty(&s))
	{

		Pop(&s, (int*)&c);
		pout[n-1] = c;
		n--;
	}
	printf("%s\n", pout);
	free(pout);
	DestroyStack(&s);
	return 0;
}
