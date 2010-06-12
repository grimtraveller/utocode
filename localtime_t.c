#include <time.h>
int main()
{
	time_t t;
	struct tm* tm;
	tm = localtime(&t);
	printf("%d %d %d\n", tm->tm_year+1, tm->tm_mon+1, tm->tm_mday+1);
	return 0;
}
