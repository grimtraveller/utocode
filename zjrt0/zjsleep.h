#ifndef ZJSLEEP_H
#define ZJSLEEP_H
#if defined(WIN32)	//OS
#	include <windows.h>
#	define zjsleep(t)	Sleep(t*1000)
#elif defined(LINUX)	//OS
#	define zjsleep(t)	sleep(t)
#else
#	error("unknow OS!")
#endif //OS
#endif //ZJSLEEP_H
