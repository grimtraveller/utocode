//============================================================================================
// frhed - free hex editor

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#include <string.h>
#include <stdio.h>
#include <commctrl.h>
#include <objbase.h>
#include <shlobj.h>

HINSTANCE hMainInstance;
LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);

#include "resource.h"
#include "simparr.cpp"
#include "diagbox.cpp"
#include "hexwnd.cpp"

//--------------------------------------------------------------------------------------------
// WinMain: the starting point.
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, char* szCmdLine, int iCmdShow)
{
	hMainInstance = hInstance;

	// Register window class and open window.
	HACCEL hAccel;

	static char szAppName[] = "frhed wndclass";
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof (wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON1));
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = MAKEINTRESOURCE (IDR_MAINMENU);
	wndclass.lpszClassName = szAppName;
//Pabs changed        // cast required to compile in mscv++6-stricter compliance with ansi
	wndclass.hIconSm =(HICON) LoadImage (hInstance, MAKEINTRESOURCE (IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
//end

	RegisterClassEx (&wndclass);

	hWnd = CreateWindow (szAppName,
		"frhed window",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		hexwnd.iWindowX,
		hexwnd.iWindowY,
		hexwnd.iWindowWidth,
		hexwnd.iWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow( hWnd, hexwnd.iWindowShowCmd );
	UpdateWindow (hWnd);

	if( szCmdLine != NULL && strlen( szCmdLine ) != 0 )
	{
		// Command line not empty: open a file on startup.

		// BoW Patch: Remove any " by filtering the command line
		char sz[MAX_PATH];
		char* p = szCmdLine;
		strncpy( sz, szCmdLine, sizeof( sz ) );
		for( int i = 0; i < MAX_PATH; ++i )
		{
			char c = sz[i];
			if( c == 0 )
			{
				*p = 0;
				break;
			}
			else if( c != '"' )
			{
				*p++ = c;
			}
		}
		// end of patch

		char lpszPath[MAX_PATH];
		HRESULT hres = ResolveIt( hWnd, szCmdLine, lpszPath );
		if( SUCCEEDED( hres ) )
		{
			// Trying to open a link file: decision by user required.
			int ret = MessageBox( hWnd,
				"You are trying to open a link file.\n"
				"Click on Yes if you want to open the file linked to,\n"
				"or click on No if you want to open the link file itself.\n"
				"Choose Cancel if you want to abort opening.",
				"frhed", MB_YESNOCANCEL | MB_ICONQUESTION );
			switch( ret )
			{
			case IDYES:
				hexwnd.load_file( lpszPath );
				break;
			case IDNO:
				hexwnd.load_file( szCmdLine );
				break;
			case IDCANCEL:
				break;
			}
		}
		else
		{
			hexwnd.load_file( szCmdLine );
		}
	}

	hAccel = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_ACCELERATOR1));

	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator (hWnd, hAccel, &msg))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}
	return msg.wParam;
}

//--------------------------------------------------------------------------------------------
// The main window procedure.
LRESULT CALLBACK MainWndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return hexwnd.OnWndMsg( hwnd, iMsg, wParam, lParam );
}

//============================================================================================
