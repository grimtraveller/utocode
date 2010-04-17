#include "jzmem.h"
#include <stdio.h>
void show_leak(char* file, int line, void* p, int len)
{
	printf("%s %d : 0x%x %d\n", file, line, (int)p, len);
}
int main()
{
	construct_jzmem(NULL);
	//jzmalloc(100, __FILE__, __LINE__);
	char* p = JZMALLOC(100);
	
	jzcheck_leak(show_leak);
	destruct_jzmem();
	return 0;
}
