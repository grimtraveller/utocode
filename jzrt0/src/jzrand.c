#include "jzrand.h"
int
jzrand_scope(int min, int max)
{
	int n;
	while(1)
	{
		n = rand();
		n = n%max;
		if ((n >= min))
		{
			break;
		}
	}
	return n;
}

BOOLEAN jzrand_boolean()
{
	int n;
	n = rand();
	return (BOOLEAN)(n%2);
}
