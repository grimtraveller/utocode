#pragma once
#include "afxcmn.h"


// CSetDlg 对话框

class CSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDlg();

// 对话框数据
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_alpha;
	int m_nAlpha;
	UINT GetAlpha();
};
