// convertdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "convertdlg.h"


// CConvertDlg 对话框

IMPLEMENT_DYNAMIC(CConvertDlg, CDialog)

CConvertDlg::CConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertDlg::IDD, pParent)
	, m_dpi(0)
	, m_src(0)
	, m_des(0)
{

}

CConvertDlg::~CConvertDlg()
{
}

void CConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOURCETYPE, m_srcType);
	DDX_Control(pDX, IDC_DESTTYPE, m_desType);
	DDX_Text(pDX, IDC_DPI, m_dpi);
	DDX_Text(pDX, IDC_SOURCE, m_src);
	DDX_Text(pDX, IDC_DEST, m_des);
}


BEGIN_MESSAGE_MAP(CConvertDlg, CDialog)
	ON_BN_CLICKED(IDC_CONVERT, &CConvertDlg::OnBnClickedConvert)
END_MESSAGE_MAP()


// CConvertDlg 消息处理程序

BOOL CConvertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_srcType.SetCurSel(0);
	m_desType.SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConvertDlg::OnBnClickedConvert()
{
	UpdateData();
	int src = m_srcType.GetCurSel();
	int des = m_desType.GetCurSel();
	if (0 == src)
	{
		
		if (0 == des)
		{
			//pix - pix
			m_des = m_src;
		}
		else if (1 == des)
		{
			//pix - inch
			if (0 == m_dpi)
			{
				AfxMessageBox("dpi=0");
				return;
			}
			m_des = m_src/m_dpi;
		}
		else if (2 == des)
		{
			//pix - mm
			m_des = (m_src/m_dpi)*2.54;
		}
	}
	else if (1 == src)
	{
		if (0 == des)
		{
			//inch - pix
			m_des = m_src*m_dpi;
			
		}
		else if (1 == des)
		{
			//inch - inch
			m_des = m_src;
		}
		else if (2 == des)
		{
			//inch - mm
			m_des = m_src*2.54;
		}
	}
	else if (2 == src)
	{
		if (0 == des)
		{
			//mm - pix
			m_des = (m_src/2.54)*m_dpi;
			
		}
		else if (1 == des)
		{
			//mm - inch
			m_des = m_src/2.54;
			
		}
		else if (2 == des)
		{
			//mm - mm
			m_des = m_src;
		}
	}
	UpdateData(FALSE);
			
}
