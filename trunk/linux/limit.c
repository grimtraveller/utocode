#include <unistd.h>
#include <stdio.h>
typedef struct 
{
	char mean[100];
	int name;
} MEANNAME;

int main()
{
	int i;
	MEANNAME sys[] = {
		{"ARG_MAX", _SC_ARG_MAX},
		{"CHILD_MAX", _SC_CHILD_MAX},
		{"HOST_NAME_MAX", _SC_HOST_NAME_MAX},
		{"LOGIN_NAME_MAX", _SC_LOGIN_NAME_MAX},
		{"clock ticks", _SC_CLK_TCK},
		{"OPEN_MAX", _SC_OPEN_MAX},
		{"PAGESIZE", _SC_PAGESIZE},
		{"RE_DUP_MAX", _SC_RE_DUP_MAX},
		{"STREAM_MAX", _SC_STREAM_MAX},
		//{"SYMLOOP_MAX", SYMLOOP_MAX},
		{"TTY_NAME_MAX", _SC_TTY_NAME_MAX},
		{"TZNAME_MAX", _SC_TZNAME_MAX},
		{"_POSIX_VERSION", _SC_VERSION},
	};
	char pathname[] = "/";
	MEANNAME path[] = {
		{"_PC_LINK_MAX", _PC_LINK_MAX},
		{"_PC_MAX_CANON", _SC_CHILD_MAX},
		{"_PC_MAX_INPUT", _PC_MAX_INPUT},
		{"_PC_NAME_MAX", _PC_NAME_MAX},
		{"_PC_PATH_MAX", _PC_PATH_MAX},
		{"_PC_PIPE_BUF", _PC_PIPE_BUF},
		{"_PC_CHOWN_RESTRICTED", _PC_CHOWN_RESTRICTED},
		{"_PC_NO_TRUNC", _PC_NO_TRUNC},
		{"_PC_VDISABLE", _PC_VDISABLE},
	};
	for (i = 0; i < sizeof(sys)/sizeof(MEANNAME); i++)
	{
		printf("%s=%d\n", sys[i].mean, sysconf(sys[i].name));
	}
	printf("\npathname=%s\n", pathname);
	for (i = 0; i < sizeof(path)/sizeof(MEANNAME); i++)
	{
		printf("%s=%d\n", path[i].mean, pathconf(pathname, path[i].name));
	}
	return 0;
}
		
