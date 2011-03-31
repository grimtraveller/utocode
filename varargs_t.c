#include <stdio.h>
#include <string.h>
#include <stdarg.h>
int hollerA(char* str, char a, char b, char c)
{
	printf(str, a, b, c);
	return 0;
}

int hollerB(str, a, b, c)
	char * str;
	char* a;
	char* b;
	char* c;
{
	printf(str, a, b, c);
	return 0;
}
int hollerC(char* str, ...)
{
	char var[100];
	va_list ap;
	int i = 0;
	va_start(ap, str);
	/*
	we can set var count throuh the first argment
	  like
	  while(str == 0)
	  {
		...
		str++;
		...
	  }
	*/

	for (i = 0; i < 100; i++)// max 100 for simple
	{
		var[i] = (char) va_arg(ap, int);
	}
	printf(str, var[0], var[1], var[2], var[3]);
	va_end(ap);
}

int main()
{
	char str[100];
	char a, b, c;
	a = 'a';
	b = 'b';
	c = 'c';
	printf("hollerA:");
	strcpy(str, "%c %c %c\n");
	hollerA(str, a, b, c);
	
	printf("hollerB:");
	strcpy(str, "\n");
	hollerB(str);

	printf("hollerC:");
	strcpy(str, "%c %c %c %c\n");
	hollerC(str, a,b,c,'d');
	return 0;
}
