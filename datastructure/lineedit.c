/**
  @file		lineedit.c
  @brief	a line edit application
  @details	usage - $>lineedit
  @author	zuohaitao
  @date		2010-02-03
  @warning	
  @bug		
  */

#include "public.h"
#include "stack.h"
#define BACKSPACE	'#'			//!< backspace
#define DELLINE		'@'			//!< delete current line
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
