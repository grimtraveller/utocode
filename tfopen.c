#include <stdio.h>
#include <windows.h>
/* ，fopen是以非独占方式打开文件 */
int
main(int argc, char* argv[])
{
	char c;
	FILE* fp = fopen("f", "rb");
	while(1)
	{
		fseek(fp, 0, SEEK_SET);
		if (1 == fread(&c, sizeof(char)*1, 1, fp))
		{
			printf("%c\n", c);
		}
		Sleep(1);
	}
	fclose(fp);

	return 0;
}
