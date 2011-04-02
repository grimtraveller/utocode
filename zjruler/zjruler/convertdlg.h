#pragma once
#include "afxwin.h"


// CConvertDlg 对话框

class CConvertDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertDlg)

public:
	CConvertDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConvertDlg();

// 对话框数据
	enum { IDD = IDD_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_srcType;
	CComboBox m_desType;
	int m_dpi;
	afx_msg void OnBnClickedConvert();
	double m_src;
	double m_des;
};
