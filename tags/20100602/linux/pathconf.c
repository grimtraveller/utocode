#include <unistd.h>
#include <stdio.h>
int main(int argv, char* argc[])
{
	char path[] = "/usr/include/unistd.h";
	int name;
	name = _PC_PATH_MAX;
	printf("%d", pathconf(path ,name));
	return 0;
}

