/*
 * Name:	alarm_t.c
 * Desc:	study SIGXXX(SIGARLM, SIGINT)
 * Usage:	
			$>gcc alarm_t.c -o ~/bin/alarm_t
			$>~/bin/alarm_t
			
			ctrl+c to do or undo SIGARLM
			ctrl+z to kill

			win32
			>cl alarm_t.c User32.lib -D "WIN32"
			>alarm_t.exe

			ctrl+c to do or undo timer
			ctrl+break to kill
 * Author:	zuohaitao
 * Date:	2008/12/28
 * Todo:	test linux
 */
#ifdef WIN32
#include <Windows.h>
#else
#include <signal.h>
#endif	//WIN32
#include <stdio.h>
#define TIMER_ELAPSE	5000
void do_alarm();
void undo_alarm();
#ifdef WIN32
void CALLBACK alarmproc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
BOOL CtrlHandler(DWORD fdwCtrlType);
UINT_PTR timeid = 1;
#else
typedef void (*Sigfunc)(int);
void do_sigint(int signalno);
Sigfunc sigfunc;
void alarmproc(int);
#endif //WIN32
typedef enum _operator_e
{
	DOALARM = 0,
	ALARM = 1,
	UNDOALARM = 2,
	NOALARM = 3
} operator_e;

//void do_alarm(int signalno);
//void undo_alarm();
operator_e how = DOALARM;
int main()
{
	unsigned int i = 0;
#ifdef WIN32
	MSG msg;
#endif //WIN32
#ifdef WIN32
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE))
	{
		printf("SetConsoleCtrlHandler Error %d\n", GetLastError());
		return -1;
	}
#else
	signal(SIGINT, do_sigint);
#endif //WIN32
	while(1)
	{
#ifdef WIN32
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_TIMER == msg.message)
			{
				DispatchMessage(&msg);
			}
		}
		Sleep(1000);
#else
		sleep(1);
#endif //WIN32
		//[NOTE]printf is not safe!!!
		printf("Sleep...%d\n", i++);
		if (DOALARM == how)
		{
			do_alarm();
			how = ALARM;
		}
		else if (UNDOALARM == how)
		{
			undo_alarm();
			how = NOALARM;
		}
	}
	return 0;
}

#ifdef WIN32
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch(fdwCtrlType)
	{
		case CTRL_C_EVENT:
			if (ALARM == how)
			{
				how = UNDOALARM;
			}
			else if (NOALARM == how)
			{
				how = DOALARM;
			}
			return TRUE;
		default:
			return FALSE;
	}
} 
#else
void do_sigint(int signo)
{
	if (ALARM == how)
	{
		how = UNDOALARM;
	else if (NOALARM == how)
	{
		how = DOALARM;
	}
}
#endif //WIN32
void do_alarm(int nsec)
{
	printf("%s\n", __FUNCTION__); 
#ifdef WIN32
	timeid = SetTimer(NULL, timeid, TIMER_ELAPSE, alarmproc);
	if (0 == timeid)
	{
		printf("Errno no:%d\n", GetLastError());
		return;
	}
#else
	if ((sigfunc =(Sigfunc)signal(SIGALRM, alarmproc) )< 0)
	{
		perror("signal()");
		return;
	}
	if (0 != alarm(TIMER_ELAPSE))
	{
		perror("alarm was already set!");
		return;
	}
#endif
	return;
}
void undo_alarm()
{
	printf("%s\n", __FUNCTION__);
#ifdef WIN32
	KillTimer(NULL, timeid);
	timeid = 1;
#else
	alarm(0);
	signal(SIGALRM, sigfunc);
#endif //WIN32
}
#ifdef WIN32
void CALLBACK alarmproc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	printf("alarm\n");
	return;
}
#else
void alarmproc(int nSec)
{
	printf("alarm\n");
}
#endif //WIN32

