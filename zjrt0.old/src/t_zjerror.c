#include "zjtype.h"
#include "zjerror.h"
#include <stdio.h>
void showmsg(const char* errmsg)
{
	printf("%s\n", errmsg);
}

int
main(int argc, char** argv)
{
#ifdef WIN32
	ZJBOOL flag = TRUE;
	char* msg = NULL;
	g_showfn = showmsg;
	CreateFile(TEXT("C:\\a.txt"), 0, 0, NULL, 0, 0, NULL);
	printf("0x08x\n", zjerrno);
	zjerror_return(zjerrno);
	zjerror_exit();
	free(msg);
#elif defined(MAC64)
#endif //WIN32
	return 0;
}	
