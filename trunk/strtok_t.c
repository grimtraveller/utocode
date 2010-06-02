#include <string.h>
int main()
{
	char* sub;
	char s[] = "a:::::::a:b:c:d:e";
	sub = strtok(s, ":");
	while(0 != sub)
	{
		printf("%s\n", sub);
		sub = strtok(0, ":");
	}
	return 0;
}
