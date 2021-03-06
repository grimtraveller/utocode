/**
  @file		conversion.c
  @brief	deciaml convert number
  @details	usage
			$>conversion 2
  @author	zuohaitao
  @date		2010-03-28
  @warning	none
  @bug		none
  */
#include "stack.h"
int main(int argc, char* argv[])
{
	int b,c;
	int i;
	STACK stack;
	//! check argments
	if (argc != 2)
	{
		printf("Usage: conversion num\ne.g. conversion 2 (decimal convert binary)\n");
		return 1;
	}
	c = atoi(argv[1]);
	scanf("%d", &b);
	printf("->\n");
	//! save every bit in stack
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


