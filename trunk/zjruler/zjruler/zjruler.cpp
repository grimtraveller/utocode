// zjruler.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "zjruler.h"
#include "rulerwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CzjrulerApp

BEGIN_MESSAGE_MAP(CzjrulerApp, CWinApp)
END_MESSAGE_MAP()


// CzjrulerApp construction

CzjrulerApp::CzjrulerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CzjrulerApp object

CzjrulerApp theApp;


// CzjrulerApp initialization

BOOL CzjrulerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	CRulerWnd* pRulerWnd = new CRulerWnd;

	if(!pRulerWnd->CreateWnd())
	{
		return FALSE;
	}
	pRulerWnd->ShowWindow(SW_SHOW);
	pRulerWnd->UpdateWindow();
	m_pMainWnd = pRulerWnd;
	MSG Msg;
	while(0 != ::GetMessage(&Msg, m_pMainWnd->GetSafeHwnd(), 0, 0))
	{
		::TranslateMessage(&Msg);
		::DispatchMessage(&Msg);
	}
	delete pRulerWnd;
	return FALSE;
}
