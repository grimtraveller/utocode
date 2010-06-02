// mmf_tDlg.h : header file
//

#if !defined(AFX_MMF_TDLG_H__E4058D71_333F_4018_BF32_597CEA1FE9F2__INCLUDED_)
#define AFX_MMF_TDLG_H__E4058D71_333F_4018_BF32_597CEA1FE9F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMmf_tDlg dialog

class CMmf_tDlg : public CDialog
{
// Construction
public:
	CMmf_tDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMmf_tDlg)
	enum { IDD = IDD_MMF_T_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMmf_tDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMmf_tDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMF_TDLG_H__E4058D71_333F_4018_BF32_597CEA1FE9F2__INCLUDED_)
