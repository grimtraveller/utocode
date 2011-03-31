#include <time.h>
int main()
{
	time_t t = {0};
	struct tm* tm;
	time(&t);
	tm = localtime(&t);
	if (NULL == tm)
	{
		perror("localtime");
		return -1;
	}
	printf("%4d-%02d-%02d %02d:%02d:%02d (weekday:%d yearday:%d)\n",
			1900+tm->tm_year+1, 
			tm->tm_mon+1, 
			tm->tm_mday+1,
			tm->tm_hour+1,
			tm->tm_min+1,
			tm->tm_sec+1,
			tm->tm_wday+1,
			tm->tm_yday+1
			);
	return 0;
}
