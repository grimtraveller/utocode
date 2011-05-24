// setdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "setdlg.h"


// CSetDlg 对话框

IMPLEMENT_DYNAMIC(CSetDlg, CDialog)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{

}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALPHA, m_alpha);
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALPHA, &CSetDlg::OnNMCustomdrawAlpha)
	ON_BN_CLICKED(IDOK, &CSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CSetDlg::OnNMCustomdrawAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CSetDlg::OnBnClickedOk()
{
	m_nAlpha = m_alpha.GetPos();
	OnOK();
}

BOOL CSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_alpha.SetRange(0, 15);
	m_alpha.SetPos(m_nAlpha);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

UINT CSetDlg::GetAlpha()
{
	return m_nAlpha;
}
