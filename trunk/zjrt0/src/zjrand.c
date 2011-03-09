#include "zjrand.h"
int
zjrand_scope(int min, int max)
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

ZJBOOL zjrand_boolean()
{
	int n;
	n = rand();
	return (ZJBOOL)(n%2);
}
