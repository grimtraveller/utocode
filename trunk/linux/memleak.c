/*
 *	Name:	memleak.c
 *	Usage:	study mtrace
 *			0. $gcc memleak.c -Wall -ggdb -o memleak
 *			1. $MALLOC_TRACE=mtrace.log gdb memleak
 *			2. (gdb)beak main
 *			3. (gdb)command 1
 *			4. >call mtrace()
 *			5. >continue
 *			6. >end
 *			7. (gdb)run
 *			8. quit
 *			9. mtrace memleak mtrace.log
 * Author:	zuohaitao
 * Date:	2008/09/38
 */
#include <stdlib.h>
void memleak();
int main()
{
	memleak();
	return 0;
}
void memleak()
{
	char* p;
	p = (char*)malloc(10*sizeof(char));
}
