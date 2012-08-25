/**
 * @file	strtok_t.c 
 * @brief	test strtok function
 * @details	
 * @author	zuohaitao
 * @date	2011-04-24 
 * @warning	
 * @bug	
 */
#include <stdio.h>
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
