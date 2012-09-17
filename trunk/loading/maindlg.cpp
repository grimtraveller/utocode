// maindlg.cpp : implementation file
//

#include "stdafx.h"
#include "loading.h"
#include "maindlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pShowMsgDlg = NULL;
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

void CMainDlg::OnOK() 
{
	//CLoadingDlg dlg;
	//dlg.DoModal();
	ShowMsg("正在处理，请稍候......", TRUE);
	//Sleep(10000);
	for (int i = 1; i <= 1000; i++)
	{
		CString strMsg;
		strMsg.Format("%d", i);
		TRACE("%s\n", strMsg);
		SetWindowText(strMsg);
		Sleep(10);
	}
	ShowMsg("", FALSE);
	//CDialog::OnOK();
}


int CMainDlg::ShowMsg(LPCTSTR MsgTxt, BOOL bStatus)
{
 	if(bStatus)
	{

		if(m_pShowMsgDlg==NULL)
		{		
			m_pShowMsgDlg= new CLoadingDlg();
			if(m_pShowMsgDlg==NULL)
			{
				//申请内存失败
 				return 0;
			}
			
			m_pShowMsgDlg->Create(CLoadingDlg::IDD,AfxGetMainWnd());
			m_pShowMsgDlg->CenterWindow();
   			((CLoadingDlg*)m_pShowMsgDlg)->SetText(MsgTxt);
 			::SetWindowPos(m_pShowMsgDlg->m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOMOVE);
			m_pShowMsgDlg->RedrawWindow();
		}
		else
		{
			if(!::IsWindow(m_pShowMsgDlg->m_hWnd))
				return 0;
			
			((CLoadingDlg*)m_pShowMsgDlg)->ShowWindow(SW_SHOW);
  			((CLoadingDlg*)m_pShowMsgDlg)->SetText(MsgTxt);
 			m_pShowMsgDlg->UpdateWindow();
			
		}
	}
	else
	{
		if(m_pShowMsgDlg!=NULL)
		{
			
			if(!::IsWindow(m_pShowMsgDlg->m_hWnd))
				return 0;
			
			((CLoadingDlg*)m_pShowMsgDlg)->SetText("");
			
			CWnd* ParentWnd=m_pShowMsgDlg->GetParent();

			if(ParentWnd!=NULL)
				::BringWindowToTop(ParentWnd->m_hWnd);

			m_pShowMsgDlg->ShowWindow(SW_HIDE);

		}
	}
	
	return 0;
}

