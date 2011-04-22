// setdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "setdlg.h"


// CSetDlg 对话框

IMPLEMENT_DYNAMIC(CSetDlg, CDialog)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
	, m_nAlpha(255)
{

}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_ALPHA, m_nAlpha);
	DDV_MinMaxInt(pDX, m_nAlpha, 0, 155);
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALPHA, &CSetDlg::OnNMCustomdrawAlpha)
END_MESSAGE_MAP()


void CSetDlg::OnNMCustomdrawAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
