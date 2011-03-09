#include "zjrt0.h"
#include "zjerror.h"

SHOWMSG_FUNC g_showfn = NULL;
void zjstrerror(char** errmsg)
{
#ifdef WIN32
	char* buf = NULL;
	FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			zjerrno,
			0,
			(LPSTR)&buf,
			0,
			NULL);
	*(errmsg) = _strdup(buf);
#else
	*(errmsg) = strdup(strerror(zjerrno));
#endif //WIN32
}

void zjperror()
{
	char* errmsg = NULL;
	zjstrerror(&errmsg);
	if (NULL != g_showfn)
	{
		g_showfn(errmsg);
	}
	ZJFREE(errmsg);
	errmsg = NULL;
}
