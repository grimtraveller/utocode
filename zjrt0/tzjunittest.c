#include "zjtype.h"
#include "zjfuncunittest.h"

int add(int a, int b)
{
	return a+b;
}
int
main(int argc, char* argv[])
{
	FUNC_UNIT_TEST((1+2), add, 1,2);
	FUNC_UNIT_TEST((2+2), add, 1,2);
	return 0;
}
