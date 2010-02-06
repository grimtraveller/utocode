/*
 * Name:	conversion.c
 * Desc:	deciaml convert num
 * Usage:	$>conversion 2 	
 * Author:	zuohaitao
 * Date:	2010-01-29
 */
#include "stack.h"
int main(int argc, char* argv[])
{
	int b,c;
	int i;
	STACK stack;
	if (argc != 2)
	{
		printf("Usage: conversion num\ne.g. conversion 2 (decimal convert binary)\n");
		return 1;
	}
	c = atoi(argv[1]);
	scanf("%d", &b);
	printf("->\n");
	CreateStack(&stack);
	do
	{
		Push(&stack, (SElement)(b%c));
		b = b/c;
	}
	while (0 != b);

	while(!IsStackEmpty(&stack))
	{
		if (OK != Pop(&stack, (SElement*)&i))
		{
			perror("Pop");
			DestroyStack(&stack);
			return 1;
		}
		printf("%d ", i);
	}
	printf("\n");
	DestroyStack(&stack);
	return 0;
}


