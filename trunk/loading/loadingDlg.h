// loadingDlg.h : header file
//

#if !defined(AFX_LOADINGDLG_H__5E0CC6CB_0434_4385_9C40_9B950BED48CD__INCLUDED_)
#define AFX_LOADINGDLG_H__5E0CC6CB_0434_4385_9C40_9B950BED48CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg dialog
#define BMPSIZE		8
class CLoadingDlg : public CDialog
{
// Construction
public:
	CLoadingDlg(CWnd* pParent = NULL);	// standard constructor
	int m_bmpres[BMPSIZE];

	CRect m_BmpRect;
	BOOL m_bDrawing;
	HANDLE m_hEventKill;
	int m_nThreadStatus;
	static DWORD WINAPI ShowBmp(LPVOID lpParameter);
	int  DrawBmp(int bmp);
	void SetText(CString Message);
	void PeekMsg();

// Dialog Data
	//{{AFX_DATA(CLoadingDlg)
	enum { IDD = IDD_LOADING_DIALOG };
	CStatic	m_ctrlMsg;
	CStatic	m_ctrlBmp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoadingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADINGDLG_H__5E0CC6CB_0434_4385_9C40_9B950BED48CD__INCLUDED_)
