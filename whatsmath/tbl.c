#include <stdio.h>
//#define ADD 0
#define N 13
char bit[N] = {0};
int main(int argc, char* argv[])
{
	int i,j,t,k,l;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
#ifdef ADD
			t = i+j;
#else
			t = i*j;
#endif 
			k = 0;
			memset(bit, 0x0, N*sizeof(char));
			do
			{
				bit[k] = t%N;
				switch (bit[k])
				{
				case 0:

					bit[k] = '0';
					break;	
				case 1:

					bit[k] = '1';
					break;	
				case 2:

					bit[k] = '2';
					break;					
				case 3:

					bit[k] = '3';
					break;					
				case 4:

					bit[k] = '4';
					break;					
				case 5:

					bit[k] = '5';
					break;					
				case 6:

					bit[k] = '6';
					break;					
				case 7:

					bit[k] = '7';
					break;					
				case 8:

					bit[k] = '8';
					break;					
				case 9:

					bit[k] = '9';
					break;					

				case 10:

					bit[k] = 'a';
					break;

				case 11:

					bit[k] = 'b';
					break;

				case 12:

					bit[k] = 'c';
					break;

				case 13:

					bit[k] = 'd';
					break;

				case 14:

					bit[k] = 'e';
					break;
				case 15:

					bit[k] = 'f';
					break;
				case 16:

					bit[k] = 'g';
					break;
				case 17:

					bit[k] = 'h';
					break;
				case 18:

					bit[k] = 'i';
					break;
				case 19:

					bit[k] = 'j';
					break;
				}
				t = t/N;
				k++;
			}
			while (0 != t);
			for (l = N;l >= 0;l--)
			{
				if (bit[l] != 0)
				{
					printf("%c",bit[l]);
				}
			}
			printf(" ");
		}
		printf("\n");
	}
	return 0;
}