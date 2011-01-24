// Ruler.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "Ruler.h"
#define TEXTLENGTH 90
#define TEXTHEIGTH 20

// CRuler

IMPLEMENT_DYNAMIC(CRuler, CWnd)

CRuler::CRuler()
{

}

CRuler::~CRuler()
{
}


BEGIN_MESSAGE_MAP(CRuler, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
ON_WM_SETCURSOR()
ON_WM_NCLBUTTONDOWN()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CRuler::CreateWnd()
{
	if(!CreateEx(0,
		::AfxRegisterWndClass(0,::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)),GetSysColorBrush(COLOR_ACTIVEBORDER)),
		NULL,
		WS_POPUP,
		0,0,300,50,
		NULL,
		NULL))
	{
		return FALSE;
	}
	return TRUE;
}

int CRuler::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow(NULL);

	return 0;
}



void CRuler::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	CString str;
	str.Format(_T("x=%d y=%d"), rc.Width(), rc.Height());
	dc.SetBkMode(TRANSPARENT);
	CRect rcText;
	if (rc.Width() - TEXTLENGTH < 0)
	{
		rcText.left = 0;
	}
	else
	{
		rcText.left = rc.Width() - TEXTLENGTH;
	}
	if (rc.Height() - TEXTHEIGTH < 0)
	{
		rcText.top = 0;
	}
	else
	{
		rcText.top = rc.Height() - TEXTHEIGTH;
	}
	rcText.right = rcText.left + TEXTLENGTH;
	rcText.bottom = rcText.top + TEXTHEIGTH;
	dc.DrawText(str, str.GetLength(), rcText, 0);
	
	for (int i = 0; i < rc.Width(); i+=10)
	{
		dc.MoveTo(i, 0);
		if (0 == i % 100)
		{
			dc.LineTo(i, 20);
		}
		else
		{
			if (0 == i % 50)
			{
				dc.LineTo(i, 15);
			}
			else
			{
				dc.LineTo(i, 10);
			}
		}
	}

	for (int j = 0; j < rc.Height(); j+=10)
	{
		dc.MoveTo(0, j);
		if (0 == j % 100)
		{
			dc.LineTo(20, j);
		}
		else
		{
			if (0 == j % 50)
			{
				dc.LineTo(15, j);
			}
			else
			{
				dc.LineTo(10, j);
			}
		}
	}


}


LRESULT CRuler::OnNcHitTest(CPoint point)
{
	CRect rect;
	GetWindowRect(&rect);
	if(point.x <= rect.left+3)
	{
		return HTLEFT;
	}
	else if(point.x >= rect.right-3)
	{
		return HTRIGHT;
	}
	else if(point.y <= rect.top+3)
	{
		return HTTOP;
	}
	else if(point.y >= rect.bottom-3)
	{
		return HTBOTTOM;
	}
	else if(point.x <= rect.left+10 && point.y <= rect.top+10)
	{
		return HTTOPLEFT;
	}
	else if(point.x >= rect.right-10 && point.y <= rect.top+10)
	{
		return HTTOPRIGHT;
	}
	else if(point.x <= rect.left+10 && point.y >= rect.bottom-10)
	{
		return HTBOTTOMLEFT;
	}
	else if(point.x >= rect.right-10 && point.y >= rect.bottom-10)
	{
		return HTBOTTOMRIGHT;
	}
	else
	{
		return CWnd::OnNcHitTest(point);
	}
}

BOOL CRuler::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU ||
		nHitTest == HTMENU || nHitTest == HTCLIENT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}
	else if(nHitTest == HTTOP || nHitTest == HTBOTTOM)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
	}
	else if(nHitTest == HTLEFT || nHitTest == HTRIGHT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
	}
	else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT)
	{
	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
	}
	else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
	{
	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
	}
	else
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}
	return TRUE;

	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CRuler::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if(nHitTest == HTTOP)
	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOM)
	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTLEFT)
	{
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	}
	else if(nHitTest == HTRIGHT)
	{
		SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	}
	else if(nHitTest == HTTOPLEFT)
	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTTOPRIGHT)
	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOMLEFT)
	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	else if(nHitTest == HTBOTTOMRIGHT)
	SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
	else
	{
		CWnd::OnNcLButtonDown(nHitTest, point);
	}

	//CWnd::OnNcLButtonDown(nHitTest, point);
}

void CRuler::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	::InvalidateRect(GetSafeHwnd(), &rc, TRUE);
}

void CRuler::OnMouseMove(UINT nFlags, CPoint point)
{
	SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));
	if(nFlags&MK_LBUTTON)
	{
		POINT pt;
		pt.x=point.x;pt.y=point.y;
		ClientToScreen(&pt);
		SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CRuler::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonDown(nFlags, point);
}
