#pragma once
#include "afxwin.h"


// CConvertDlg �Ի���

class CConvertDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertDlg)

public:
	CConvertDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConvertDlg();

// �Ի�������
	enum { IDD = IDD_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
