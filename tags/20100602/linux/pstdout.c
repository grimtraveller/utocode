#include <stdio.h>
int main()
{
	fwrite("hello stdout\n", (size_t)sizeof("hello stdout\n"), (size_t)sizeof(char), stdout);

	printf("stdout=0x%x\n",stdout);
	return 0;
}
