/*
 * Name:	getcodepos_t.c
 * Desc:	test the MARCO __FILE__ __FUNCTION__ __LINE__
 * Usage:	$>getcodepos
 * Author:	zuohaitao
 * Date:
 */
#include <stdio.h>
void func();
int main()
{
	func();
	return 0;
}
void func()
{
	printf("file=%s,function=%s,line=%d",__FILE__,__FUNCTION__,__LINE__);
}
