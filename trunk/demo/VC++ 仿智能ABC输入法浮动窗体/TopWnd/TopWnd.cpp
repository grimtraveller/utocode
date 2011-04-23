// TopWnd.cpp : 定义应用程序的类行为。
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "TopWnd.h"
#include "mainwnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTopWndApp

BEGIN_MESSAGE_MAP(CTopWndApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CTopWndApp 构造

CTopWndApp::CTopWndApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTopWndApp 对象

CTopWndApp theApp;

// CTopWndApp 初始化

BOOL CTopWndApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();
	CMainWnd*pWnd=new CMainWnd();
	if(!pWnd->CreateMainWnd())
		return false;

	pWnd->ShowWindow(SW_SHOW);
	pWnd->UpdateWindow();
	m_pMainWnd=pWnd;

	return TRUE;
}


// CTopWndApp 消息处理程序



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTopWndApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CTopWndApp 消息处理程序

