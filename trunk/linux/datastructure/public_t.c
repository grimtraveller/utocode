#include "public.h"
int main(int argc, char* argv[])
{
	printf("NULL=%d TRUE=%d FALSE=%d OK=%d ERROR=%d INFEASIBLE=%d OVERFLOW=%d\n",
	(int)NULL,TRUE, FALSE, OK, ERROR, INFEASIBLE, OVERFLOW);
	DSperror("Test Error!");
	char* p;
	p = DSmalloc(sizeof(char)*100);
	memset(p, 0x20, 100);
	*(p+100) = 0;
	printf("%s\n", p);
	p = DSrealloc(p, sizeof(char)*200);

	memset(p+100, 0x20, 100);
	*(p+200) = 0;
	printf("%s\n", p);
	free(p);
	
	return 0;
}
