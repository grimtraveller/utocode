#include "jztype.h"
#include "jzerror.h"
#include <stdio.h>
void showmsg(const char* errmsg)
{
	printf("%s\n", errmsg);
}

int
main(int argc, char** argv)
{
	BOOLEAN flag = TRUE;
	char* msg = NULL;
	g_showfn = showmsg;
	CreateFile(TEXT("C:\\a.txt"), 0, 0, NULL, 0, 0, NULL);
	printf("0x08x\n", jzerrno);
	jzerror_return(jzerrno);
	jzerror_exit();
	free(msg);
	return 0;
}	