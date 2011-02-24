#include "jzerror.h"

SHOWMSG g_showfn = NULL;
void jzstrerror(char** errmsg)
{
#ifdef WIN32
	char* buf = NULL;
	FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			jzerrno,
			0,
			(LPSTR)&buf,
			0,
			NULL);
	*(errmsg) = _strdup(buf);
#else
	*(errmsg) = strdup(strerror(jzerrno));
#endif //WIN32
}

void jzperror()
{
	char* errmsg = NULL;
	jzstrerror(&errmsg);
	if (NULL != g_showfn)
	{
		g_showfn(errmsg);
	}
	free(errmsg);
	errmsg = NULL;
}
