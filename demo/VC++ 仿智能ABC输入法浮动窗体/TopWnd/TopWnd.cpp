// TopWnd.cpp : ����Ӧ�ó��������Ϊ��
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


// CTopWndApp ����

CTopWndApp::CTopWndApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTopWndApp ����

CTopWndApp theApp;

// CTopWndApp ��ʼ��

BOOL CTopWndApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
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


// CTopWndApp ��Ϣ�������



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CTopWndApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CTopWndApp ��Ϣ�������

