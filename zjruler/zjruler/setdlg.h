#pragma once
#include "afxcmn.h"


// CSetDlg �Ի���

class CSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDlg();

// �Ի�������
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_alpha;
	int m_nAlpha;
	UINT GetAlpha();
};
