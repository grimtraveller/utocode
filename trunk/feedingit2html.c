#include <stdio.h>
#define BUF_LEN 1024*2
#ifndef NULL
#define NULL 0
#endif //NULL
int main(int argc, char* argv[])
{
	FILE* infile = NULL;
	FILE* out = NULL;
	char buf[BUF_LEN] = {0};
	char title[BUF_LEN] = {0};
	char line[BUF_LEN] = {0};
	char* s = NULL;
	infile = fopen(argv[1], "r");
	if (NULL == infile)
	{
		perror(argv[0]);
		goto ERR;
	}

	out = fopen(argv[2], "w");
	if (NULL == out)
	{
		perror(argv[1]);
		goto ERR;
	}
	fputs("<html>", out);
	fputs("<head>", out);
	fputs("\t<meta http-equiv=\"content-type\" content=\"text/html\";charset=\"UTF-8\">", out);
	fputs("</head>", out);
	fputs("<body>", out);
	fflush(out);
	while (1)
	{
		memset(buf, 0, BUF_LEN);
		fgets(buf, BUF_LEN, infile);
		if (feof(infile))
		{
			break;
		}

		s = (char*)strtok(buf, "|");
		memset(title, 0, BUF_LEN);
		strcpy(title, s);
		s = (char*)strtok(NULL, "|");

		memset(line, 0, BUF_LEN);
		sprintf(line, "<a href=\"%s\" target=\"_blank\">%s</a><br>", s, title);
		printf("%s\n",line);
		fputs(line, out);
		fflush(out);

	}
	fputs("</body>", out);
	fputs("</html>", out);
	fflush(out);
ERR:
	if (NULL != infile)
	{
		fclose(infile);
	}
	if (NULL != out)
	{
		fclose(out);
	}
	return 0;
}
