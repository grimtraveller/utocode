// aboutdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "aboutdlg.h"


// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	m_strVerInfo = _T(
					"version 1.5 2012-10-08 \r\n"
					"\t fix color menu status bug \r\n"
					"version 1.5 2011-08-21 \r\n"
					"\t press shift and move mouse can show the position in ruler border. \r\n"
					"\t shift + keyboard(left,up,right,down) to change ruler size. \r\n"
					"version 1.4 2011-08-10 \r\n"
					"\t add input keyboard(left,up,right,down) to move ruler position. \r\n"
					"version 1.3 2011-05-24 \r\n"
					"\t change ruler information area size. \r\n"
					"\t improve transparent function\r\n"
					"version 1.2 2011-04-19 \r\n"
					"\t add version info edit. \r\n"
					"\t fix x,y value. \r\n"
					"\t change ruler information position. \r\n"
					);
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSIONINFO, m_strVerInfo);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序
