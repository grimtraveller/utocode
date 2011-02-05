int x = 12;
int power(int x, int n);
int main(int argc, char* argv[])
{
	int p,t;
	int i = 0;
	t = 0;
	for (i = 0; i < 5; i++)
	{
		t += power(x, i);
	}
	printf("%d\n");
	t = 0;
	for (i = 0; i < 5; i++)
	{
		p = power(x, i);
		if (0 == i%2)
		{
			p *= 2;
		}
		t += p;
	}
	printf("%d\n");
	return 0;
}

int power(int x, int n)
{
	int p = 1;
	int i;
	for (i = 0; i < n; i++)
	{
		p *= x;
	}
	return p;
}