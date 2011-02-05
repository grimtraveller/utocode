/* make prime number */
/*p31*/
#define N 5
int 
main(int argv, char* argc)
{
	int i = 0;
	unsigned int num = 2 * 3 + 1;
	for (i = 0; i < N; i++)
	{
		printf("%u ", num);
		num = num * (num - 1);
		num += 1;

	}
	return 0;
}
