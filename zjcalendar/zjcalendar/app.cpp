#include "app.h"
#define L	0
#define R	1
#define U	2
#define D	3

//void App::MoveForegroundWindow(
//{
//	HWND workWindow = ::GetForegroundWindow();
//	if(!workWindow) return;
//	TCHAR title[MAX_PATH] = {0};
//	::GetWindowText(workWindow, title, MAX_PATH);
//	RECT workarearc = {0};
//	::SystemParametersInfo(SPI_GETWORKAREA, 0u, &workarearc, SPIF_UPDATEINIFILE);
//}

bool App::winEventFilter(MSG *msg, long *result)
{
	if (msg->message == WM_HOTKEY)
	{
		HWND foregroundwin = ::GetForegroundWindow();
		if(NULL == foregroundwin)
		{
			return QApplication::winEventFilter(msg, result);
		}
		RECT workarea = {0};
		SystemParametersInfo(SPI_GETWORKAREA, 0u, &workarea, SPIF_UPDATEINIFILE);
		RECT winrc = {0};
		if (GetWindowRect(foregroundwin, &winrc))
		{
			if (msg->wParam == hKeyWin[L])
			{

				//winrc.top = workarea.top;
				//winrc.bottom = workarea.bottom;
				if (winrc.left != workarea.left)
				{
					winrc.left = workarea.left;
					//winrc.right = winrc.left + (workarea.right - workarea.left) / 2;
				}
				else
				{
					int w = (winrc.right - winrc.left) / 2;
					winrc.right = winrc.left + w;
				}

			}
			else if (msg->wParam == hKeyWin[R])
			{
				//winrc.top = workarea.top;
				//winrc.bottom = workarea.bottom;
				if (winrc.right != workarea.right)
				{
					winrc.right = workarea.right;
					//winrc.left = workarea.right - (workarea.right - workarea.left) / 2;

				}
				else
				{
					int w = (winrc.right - winrc.left) / 2;
					winrc.left = winrc.right - w;
				}
			}
			else if (msg->wParam == hKeyWin[U])
			{
				//winrc.left = workarea.left;
				//winrc.right = workarea.right;
				if (winrc.top != workarea.top)
				{
					winrc.top = workarea.top;
					//winrc.bottom = winrc.bottom + (workarea.bottom - workarea.top) / 2;

				}
				else
				{
					int h = (winrc.bottom - winrc.top) / 2;
					winrc.bottom = winrc.top + h;
				}
			}
			else if (msg->wParam == hKeyWin[D])
			{
				//winrc.left = workarea.left;
				//winrc.right = workarea.right;
				if (winrc.bottom != workarea.bottom)
				{
					winrc.bottom = workarea.bottom;
					//winrc.top = workarea.bottom - (workarea.bottom - workarea.top) / 2;

				}
				else
				{
					int h = (winrc.bottom - winrc.top) / 2;
					winrc.top = workarea.bottom - h;
				}
			}
			else
			{
				return QApplication::winEventFilter(msg, result);
			}
			::SetWindowPos(foregroundwin, HWND_TOP, winrc.left, winrc.top, winrc.right - winrc.left, winrc.bottom - winrc.top, SWP_NOZORDER);
		}
	}
	return QApplication::winEventFilter(msg, result);
}


App::App(int& argc, char** argv) : QApplication(argc, argv)
{
	hKeyWin[L] = GlobalAddAtomA("#VK_[L|R]WIN_VK_LEFT");
	hKeyWin[R] = GlobalAddAtomA("#VK_[L|R]WIN_VK_RIGHT");
	hKeyWin[U] = GlobalAddAtomA("#VK_[L|R]WIN_VK_UP");
	hKeyWin[D] = GlobalAddAtomA("#VK_[L|R]WIN_VK_DOWN");
}

void App::registerHotKey(WId wid)
{
	_wid = wid;
	RegisterHotKey(_wid, hKeyWin[L], MOD_WIN, VK_LEFT);
	RegisterHotKey(_wid, hKeyWin[R], MOD_WIN, VK_RIGHT);
	RegisterHotKey(_wid, hKeyWin[U], MOD_WIN, VK_UP);
	RegisterHotKey(_wid, hKeyWin[D], MOD_WIN, VK_DOWN);
}

App::~App()
{
	for (int i = 0; i < sizeof(hKeyWin)/sizeof(hKeyWin[0]); i++)
	{
		UnregisterHotKey(_wid, hKeyWin[i]);
		GlobalDeleteAtom(hKeyWin[i]);
	}
}