#if !defined(AFX_MAINDLG_H__F6F5C82E_F09B_4638_B1B8_A122DC2458AE__INCLUDED_)
#define AFX_MAINDLG_H__F6F5C82E_F09B_4638_B1B8_A122DC2458AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// maindlg.h : header file
//
#include "loadingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	int ShowMsg(LPCTSTR MsgTxt, BOOL bStatus);
	CLoadingDlg* m_pShowMsgDlg;
// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__F6F5C82E_F09B_4638_B1B8_A122DC2458AE__INCLUDED_)
