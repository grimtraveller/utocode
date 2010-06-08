/*
 * Name:	matrix.c
 * Descr:	make a matrix (N*N)
			like:N=4
			 1  2  3  4
			12 13 14  5
			11 16 15  6
			10  9  8  7
 *Usage:	$>gcc matrix.c -o matrix
			$>cat in.txt|./matrix > out.txt
 *Author:	zuohaitao
 *Date:		2008/02/21
 *Modify:	del chinese char for window 
 *Whoer:	zuohaitao
 *Date:		2008/02/22
 */

#include <stdio.h>
#include <stdlib.h>
int main
(int argc, char* argv[])
{
	int n;
	//-,|
	int row, col;
	int* p;
	int i,j;
	//t means direction;to=0 right, to=1 down to=2 left, to=3 up;
	int to;
	//matrix border
	int top, right, bottom, left;

	//get N
    fscanf(stdin,"%d",&n);
	if (n < 0 || n > 10000)
	{
		printf("N too Big! what will you do?\n");
		exit(1);
	}

	p = (int*)malloc(sizeof(int)*n*n);
	if (0 == p)
	{
		printf("malloc fail\n");
		exit(1);
	}
	to = 0;
	top = right = bottom = left = 0;
	row = col = 0;

	for (i = 0; i < n*n; i++)
	{
		switch(to)
		{
		case 0:
			if (col < n-right)
			{
				*(p+row*n+col) = i+1;
				col++;
			}
			else
			{
				//top border add 1 
				top++;
				//change direction 
				to = 1;
				//reinit col and row
				col--;
				row = top;
				//go back
				i--;
			}
			break;
		case 1:
			if (row < n-bottom)
			{
				*(p+row*n+col) = i+1;
				row++;
			}
			else
			{
				right++;
				to = 2;
				row--;
				col = n - 1 - right;
				i--;
			}
			break;
		case 2:
			if (col >= left)
			{
				*(p+row*n+col) = i+1;
				col--;
			}
			else
			{
				bottom++;
				to = 3;
				col++;
				row = n - 1 - bottom;
				i--;
			}
			break;
		case 3:
			if (row >= top)
			{
				*(p+row*n+col) = i+1;
				row--;
			}
			else
			{
				left++;
				to = 0;
				row++;
				col = left;
				i--;
			}
			break;
		default:
			break;
		}

	}

	//print matrix
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%6d ", *(p+i*n+j));
		}
		printf("\n");
	}
	free(p);
	return 0;
}
