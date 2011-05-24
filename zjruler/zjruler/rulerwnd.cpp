// Ruler.cpp : 实现文件
//

#include "stdafx.h"
#include "zjruler.h"
#include "rulerwnd.h"
#include "aboutdlg.h"
#include "convertdlg.h"
#include "setdlg.h"

#define TEXTLENGTH 230
#define TEXTHEIGTH 20
#define COLORAREALENGTH	10
#define COLORAREAHEIGTH 10
#define IDT_TIMER 1
#define MONITOR_TIMER 1
#define MINWIDTH	500
#define MINHEIGHT 60
#define SPACE	5
#define HTEXTWIDTH	400
#define HTEXTHEIGHT	15
#define VTEXTWIDTH  45
#define VTEXTHEIGHT	(20*7)

// CRulerWnd

IMPLEMENT_DYNAMIC(CRulerWnd, CWnd)

CRulerWnd::CRulerWnd()
{
	m_mouse.x = 4;
	m_mouse.y = 0;
	m_style = TOP;
	m_clr = 0xffffff;
	m_nAlpha = 15;
}

CRulerWnd::~CRulerWnd()
{
}


BEGIN_MESSAGE_MAP(CRulerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
//	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_QUIT, &CRulerWnd::OnMenuQuit)
	ON_COMMAND(ID_MENU_GETCOLOR, &CRulerWnd::OnMenuColor)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_MENU_ROTATE, &CRulerWnd::OnMenuRotate)
//	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_MENU_GETCOLOR, &CRulerWnd::OnUpdateMenuColor)
	ON_COMMAND(ID_MENU_ABOUT, &CRulerWnd::OnMenuAbout)
	ON_COMMAND(ID_MENU_CONVERT, &CRulerWnd::OnMenuConvert)
	ON_COMMAND(ID_MENU_SETTING, &CRulerWnd::OnMenuSetting)
END_MESSAGE_MAP()

BOOL CRulerWnd::CreateWnd()
{
	if(!CreateEx(0,
		::AfxRegisterWndClass(0,::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)),GetSysColorBrush(COLOR_ACTIVEBORDER)),
		NULL,
		WS_POPUP,
		0,0,MINWIDTH,MINHEIGHT,
		NULL,
		NULL))
	{
		return FALSE;
	}
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	return TRUE;
}

int CRulerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow(NULL);
	m_menu.LoadMenu(IDR_MENU);
	return 0;
}



void CRulerWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	HDC hmdc = CreateCompatibleDC(dc.GetSafeHdc());
	HBITMAP hBitmap;
	hBitmap = CreateCompatibleBitmap(dc.GetSafeHdc(),rc.Width(),rc.Height());
	HGDIOBJ hOldBitmap = SelectObject(hmdc, (HGDIOBJ)hBitmap);
	BitBlt(hmdc, 0, 0,rc.Width(), rc.Height(), dc.GetSafeHdc(), 0, 0, SRCCOPY);
	//background
	HBRUSH hBK = CreateSolidBrush(RGB(228, 236, 247));
	HGDIOBJ hOldBK= SelectObject(hmdc, hBK);
	//charset font 
	LOGFONT lf = {0};
	GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
	lf.lfWeight = 0;
	lf.lfWidth = 0;
	lf.lfHeight = -12;
	HFONT hFont = CreateFont(lf.lfHeight,
								lf.lfWidth, 
								lf.lfEscapement,
								lf.lfOrientation,
								lf.lfWeight, 
								lf.lfItalic,
								lf.lfUnderline,
								lf.lfStrikeOut,
								lf.lfCharSet, 
								lf.lfOutPrecision,
								lf.lfClipPrecision,
								lf.lfQuality, 
								lf.lfPitchAndFamily,
								_T("MS Shell Dlg")/*lf.lfFaceName*/);
	HGDIOBJ hOldFont = SelectObject(hmdc, hFont);
	
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(39, 65, 62));
	HPEN hFlagPen = CreatePen(PS_SOLID, 1, RGB(250, 192, 144));
	//current selected color
	HBRUSH hSelColor = CreateSolidBrush(m_clr);

	//Draw
	POINT pt;
	//::FillRect(hmdc, &rc, hBK);
	HGDIOBJ hOldPen = SelectObject(hmdc, hPen);
	::Rectangle(hmdc, rc.left, rc.top, rc.right, rc.bottom);
	SelectObject(hmdc, hOldPen);
	hOldPen = SelectObject(hmdc, hFlagPen);
	if ((TOP == m_style) || (BOTTOM == m_style))
	{
		::MoveToEx(hmdc, m_mouse.x, 0, &pt);
		::LineTo(hmdc, m_mouse.x, rc.Width());
	}
	else if ((LEFT == m_style) || (RIGHT == m_style))
	{
		::MoveToEx(hmdc, 0, m_mouse.y, &pt);
		::LineTo(hmdc, rc.Height(), m_mouse.y);
	}
	SelectObject(hmdc, hPen);
	
	CRect rcText(0, 0, 0, 0);
	CString str;
	if (TOP == m_style)
	{	
		for (int i = 0; i < rc.Width(); i+=10)
		{
			::MoveToEx(hmdc, i, 0, &pt);
			if (0 == i % 100)
			{
				::LineTo(hmdc, i, 20);
			}
			else
			{
				if (0 == i % 50)
				{
					::LineTo(hmdc, i, 15);
				}
				else
				{
					::LineTo(hmdc, i, 10);
				}
			}
		}
		//color area
		CRect rcColorArea;
		rcColorArea.top = rc.bottom - HTEXTHEIGHT;
		rcColorArea.bottom = rcColorArea.top + COLORAREAHEIGTH + 1;
		rcColorArea.left = COLORAREALENGTH - SPACE;
		rcColorArea.right = rcColorArea.left + COLORAREALENGTH;
		SelectObject(hmdc, hSelColor);
		::Rectangle(hmdc, rcColorArea.left, rcColorArea.top, rcColorArea.right, rcColorArea.bottom);
		//text
		rcText.top = rc.bottom - HTEXTHEIGHT;
		rcText.bottom = rcText.top + HTEXTWIDTH;
		rcText.left = rc.left + COLORAREALENGTH + SPACE + SPACE;
		rcText.right = rcText.left + HTEXTWIDTH;
		str.Format("W:%04d H:%04d X:%04d Y:%04d R=%03d R=0x%02X G=%03d G=0x%02X B=%03d B=0x%02X",
			rc.Width(), rc.Height(), m_mouse.x, m_mouse.y,
			GetRValue(m_clr), GetRValue(m_clr), 
			GetGValue(m_clr), GetGValue(m_clr),
			GetBValue(m_clr), GetBValue(m_clr)); 
		int nPrvMode = ::SetBkMode(hmdc, TRANSPARENT);
		COLORREF prevClr = SetTextColor(hmdc, RGB(39, 65, 62));
		::DrawText(hmdc, str, str.GetLength(), &rcText, 0);
		SetTextColor(hmdc, prevClr);
		::SetBkMode(hmdc, nPrvMode);
	}
	else if (LEFT == m_style)
	{
		for (int j = 0; j < rc.Height(); j+=10)
		{
			::MoveToEx(hmdc, 0, j, &pt);
			if (0 == j % 100)
			{
				::LineTo(hmdc, 20, j);
			}
			else
			{
				if (0 == j % 50)
				{
					::LineTo(hmdc, 15, j);
				}
				else
				{
					::LineTo(hmdc, 10, j);
				}
			}
		}
		//color area
		CRect rcColorArea(0, 0, 0, 0);
		rcColorArea.top = rc.top + SPACE;
		rcColorArea.bottom = rcColorArea.top + COLORAREAHEIGTH + 1;
		rcColorArea.left = 20;//rc.right - COLORAREALENGTH - SPACE;
		rcColorArea.right = rcColorArea.left + COLORAREALENGTH;
		SelectObject(hmdc, hSelColor);
		::Rectangle(hmdc, rcColorArea.left, rcColorArea.top, rcColorArea.right, rcColorArea.bottom);
		//text
		rcText.top = rc.top + COLORAREAHEIGTH + SPACE;
		rcText.bottom = rcText.top + VTEXTHEIGHT;
		rcText.left = 20;//rc.right - SPACE;
		rcText.right = rcText.left + VTEXTWIDTH;
		str.Format("R=%03d\nR=0x%02X\nG=%03d\nG=0x%02X\nB=%03d\nB=0x%02X\nW:%04d\nH:%04d\nX:%04d\nY:%04d\n",
			GetRValue(m_clr), GetRValue(m_clr), 
			GetGValue(m_clr), GetGValue(m_clr), 
			GetBValue(m_clr), GetBValue(m_clr), 
			rc.Width(), rc.Height(), m_mouse.x, m_mouse.y); 
		int nPrvMode = ::SetBkMode(hmdc, TRANSPARENT);
		COLORREF prevClr = SetTextColor(hmdc, RGB(39, 65, 62));
		::DrawText(hmdc, str, str.GetLength(), &rcText, 0);
		SetTextColor(hmdc, prevClr);
		::SetBkMode(hmdc, nPrvMode);
	}
	else if (BOTTOM == m_style)
	{
		for (int i = 0; i < rc.Width(); i+=10)
		{
			::MoveToEx(hmdc, i, rc.Height(), &pt);
			if (0 == i % 100)
			{
				::LineTo(hmdc, i, rc.Height() - 20);
			}
			else
			{
				if (0 == i % 50)
				{
					::LineTo(hmdc, i, rc.Height() - 15);
				}
				else
				{
					::LineTo(hmdc, i, rc.Height() - 10);
				}
			}
		}
		//color area
		CRect rcColorArea;
		rcColorArea.top = rc.top + HTEXTHEIGHT;
		rcColorArea.bottom = rcColorArea.top + COLORAREAHEIGTH + 1;
		rcColorArea.left = rc.left + SPACE;
		rcColorArea.right = rcColorArea.left + COLORAREALENGTH;
		SelectObject(hmdc, hSelColor);
		::Rectangle(hmdc, rcColorArea.left, rcColorArea.top, rcColorArea.right, rcColorArea.bottom);
		//text
		rcText.top = rc.top + HTEXTHEIGHT;
		rcText.bottom = rcText.top + HTEXTWIDTH;
		rcText.left = rc.left + COLORAREALENGTH + SPACE + SPACE;
		rcText.right = rcText.left + HTEXTWIDTH;
		str.Format("W:%04d H:%04d X:%04d Y:%04d R=%03d R=0x%02X G=%03d G=0x%02X B=%03d B=0x%02X",
			rc.Width(), rc.Height(), m_mouse.x, m_mouse.y,
			GetRValue(m_clr), GetRValue(m_clr), 
			GetGValue(m_clr), GetGValue(m_clr),
			GetBValue(m_clr), GetBValue(m_clr)); 
		int nPrvMode = ::SetBkMode(hmdc, TRANSPARENT);
		COLORREF prevClr = SetTextColor(hmdc, RGB(39, 65, 62));
		::DrawText(hmdc, str, str.GetLength(), &rcText, 0);
		SetTextColor(hmdc, prevClr);
		::SetBkMode(hmdc, nPrvMode);
	}
	else if (RIGHT == m_style)
	{
		for (int j = 0; j < rc.Height(); j+=10)
		{
			::MoveToEx(hmdc, rc.Width(), j, &pt);
			if (0 == j % 100)
			{
				::LineTo(hmdc, rc.Width() - 20, j);
			}
			else
			{
				if (0 == j % 50)
				{
					::LineTo(hmdc, rc.Width() - 15, j);
				}
				else
				{
					::LineTo(hmdc, rc.Width() - 10, j);
				}
			}
		}
		//color area
		CRect rcColorArea(0, 0, 0, 0);
		rcColorArea.top = rc.left + SPACE;
		rcColorArea.bottom = rcColorArea.top + COLORAREAHEIGTH + 1;
		rcColorArea.left = rc.left + SPACE;
		rcColorArea.right = rcColorArea.left + COLORAREALENGTH;
		SelectObject(hmdc, hSelColor);
		::Rectangle(hmdc, rcColorArea.left, rcColorArea.top, rcColorArea.right, rcColorArea.bottom);
		//text
		rcText.top = rc.left + COLORAREAHEIGTH + SPACE;
		rcText.bottom = rcText.top + VTEXTHEIGHT;
		rcText.left = rc.left + SPACE;
		rcText.right = rcText.left + VTEXTWIDTH;
		str.Format("R=%03d\nR=0x%02X\nG=%03d\nG=0x%02X\nB=%03d\nB=0x%02X\nW:%04d\nH:%04d\nX:%04d\nY:%04d\n",
			GetRValue(m_clr), GetRValue(m_clr), 
			GetGValue(m_clr), GetGValue(m_clr), 
			GetBValue(m_clr), GetBValue(m_clr), 
			rc.Width(), rc.Height(), m_mouse.x, m_mouse.y); 
		int nPrvMode = ::SetBkMode(hmdc, TRANSPARENT);
		COLORREF prevClr = SetTextColor(hmdc, RGB(39, 65, 62));
		::DrawText(hmdc, str, str.GetLength(), &rcText, 0);
		SetTextColor(hmdc, prevClr);
		::SetBkMode(hmdc, nPrvMode);
	}

/*
	if (rc.Width() - TEXTLENGTH < 0)
	{
		rcText.left = COLORAREALENGTH + SPACE;
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
	
	if ((LEFT == m_style)||(RIGHT == m_style))
	{
		rcText.top -= TEXTHEIGTH * 4;
		rcText.bottom = rcText.top + TEXTHEIGTH * 4;
		str.Format("R=%d G=%d B=%d W:%4d H:%4d X:%4d Y:%4d",
			GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr), rc.Width(), rc.Height(), m_mouse.x, m_mouse.y);
	}
	else 
	{
		rcText.bottom = rcText.top + TEXTHEIGTH;
		str.Format("R=%d G=%d B=%d W:%4d H:%4d X:%4d Y:%4d ",
			GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr), rc.Width(), rc.Height(), m_mouse.x, m_mouse.y);

	}

	//text 
	int nPrvMode = ::SetBkMode(hmdc, TRANSPARENT);
	COLORREF prevClr = SetTextColor(hmdc, RGB(39, 65, 62));
	::DrawText(hmdc, str, str.GetLength(), &rcText, 0);
	SetTextColor(hmdc, prevClr);
	::SetBkMode(hmdc, nPrvMode);
	//color area
	CRect rcColorArea;
	rcColorArea.top = rcText.top + 1;
	rcColorArea.bottom = rcText.top + COLORAREAHEIGTH + 1;
	rcColorArea.left = rcText.left - COLORAREALENGTH - SPACE;
	rcColorArea.right = rcColorArea.left + COLORAREALENGTH;
	SelectObject(hmdc, hSelColor);
	::Rectangle(hmdc, rcColorArea.left, rcColorArea.top, rcColorArea.right, rcColorArea.bottom);
*/
	SelectObject(hmdc, hBK);
	BitBlt(dc.GetSafeHdc(), 0, 0,rc.Width(), rc.Height(), hmdc, 0, 0, SRCCOPY);

	//
	SelectObject(hmdc, hOldPen);
	DeleteObject(hPen);
	DeleteObject(hSelColor);

	SelectObject(hmdc, hOldBK);
	DeleteObject(hBK);

	SelectObject(hmdc, hOldFont);
	DeleteObject(hFont);

	SelectObject(hmdc, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hmdc);

////	CRect rc;
//	GetClientRect(&rc);
//	CString str;
//	if (m_bHex)
//	{
//		str.Format(_T("RGB(%03d,%03d,%03d) x=%d y=%d"), 
//					GetRValue(m_clr), 
//					GetGValue(m_clr), 
//					GetBValue(m_clr), 
//					rc.Width(), rc.Height());
//	}
//	else
//	{
//		str.Format(_T("RGB(%03d,%03d,%03d) x=%d y=%d"), 
//					GetRValue(m_clr), 
//					GetGValue(m_clr), 
//					GetBValue(m_clr), 
//					rc.Width(), rc.Height());
//	}
//	//::SetBkMode(hMemDC, TRANSPARENT);
//	CRect rcText;
//	if (rc.Width() - TEXTLENGTH < 0)
//	{
//		rcText.left = 0;
//	}
//	else
//	{
//		rcText.left = rc.Width() - TEXTLENGTH;
//	}
//	if (rc.Height() - TEXTHEIGTH < 0)
//	{
//		rcText.top = 0;
//	}
//	else
//	{
//		rcText.top = rc.Height() - TEXTHEIGTH;
//	}
//	rcText.right = rcText.left + TEXTLENGTH;
//	rcText.bottom = rcText.top + TEXTHEIGTH;
//	CBrush* pTempBrush = NULL;
//	CBrush OrigBrush;
//
//	CBrush br;
//	br.CreateSolidBrush(m_clr);
//	CPen pen;
//	
//	pen.CreatePen(PS_SOLID, 1, m_clr/*RGB(255-GetRValue(m_clr), 255-GetGValue(m_clr), 255-GetBValue(m_clr))*/);
//	pTempBrush= (CBrush*)dc.SelectObject(br);
//	dc.SelectObject(pen);
//	OrigBrush.FromHandle((HBRUSH)pTempBrush);
//
//	//dc.Rectangle(rcText);
//	dc.SelectObject(&OrigBrush);
//	//dc.SetTextColor(RGB(255-GetRValue(m_clr), 255-GetGValue(m_clr), 255-GetBValue(m_clr)));
//	dc.DrawText(str, str.GetLength(), rcText, 0);
//	
//	for (int i = 0; i < rc.Width(); i+=10)
//	{
//		dc.MoveTo(i, 0);
//		if (0 == i % 100)
//		{
//			dc.LineTo(i, 20);
//		}
//		else
//		{
//			if (0 == i % 50)
//			{
//				dc.LineTo(i, 15);
//			}
//			else
//			{
//				dc.LineTo(i, 10);
//			}
//		}
//	}
//
//	for (int j = 0; j < rc.Height(); j+=10)
//	{
//		dc.MoveTo(0, j);
//		if (0 == j % 100)
//		{
//			dc.LineTo(20, j);
//		}
//		else
//		{
//			if (0 == j % 50)
//			{
//				dc.LineTo(15, j);
//			}
//			else
//			{
//				dc.LineTo(10, j);
//			}
//		}
//	}


}


LRESULT CRulerWnd::OnNcHitTest(CPoint point)
{
	CRect rect;
	GetWindowRect(&rect);

	if (LEFT == m_style || RIGHT == m_style)
	{

	}	
	else if (TOP == m_style || BOTTOM == m_style)
	{

	}

	if(point.x <= rect.left+3)
	{
		if (LEFT == m_style || RIGHT == m_style)
		{
			return CWnd::OnNcHitTest(point);
		}
		return HTLEFT;
	}
	else if(point.x >= rect.right-3)
	{
		if (LEFT == m_style || RIGHT == m_style)
		{
			return CWnd::OnNcHitTest(point);
		}
		return HTRIGHT;
	}
	else if(point.y <= rect.top+3)
	{
		if (TOP == m_style || BOTTOM == m_style)
		{
			return CWnd::OnNcHitTest(point);
		}
		return HTTOP;
	}
	else if(point.y >= rect.bottom-3)
	{
		if (TOP == m_style || BOTTOM == m_style)
		{
			return CWnd::OnNcHitTest(point);
		}
		return HTBOTTOM;
	}
	/*
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
	*/
	else
	{
		return CWnd::OnNcHitTest(point);
	}
}

BOOL CRulerWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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

void CRulerWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	GetClientRect(&m_rc);
	ClientToScreen(&m_rc);
	if(nHitTest == HTTOP)
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		rc.top = point.y;
		//if (rc.Height() >= MINHEIGHT)
		{
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
		}

	}
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

void CRulerWnd::OnSize(UINT nType, int cx, int cy)
{
/*
	if ((cy < MINHEIGHT) || (cx < MINWIDTH))
	{
		CWnd::OnSize(nType, cx, MINHEIGHT);
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		MoveWindow(m_rc.left, m_rc.top, m_rc.Width(), MINHEIGHT);
		return;
	}
*/
	CWnd::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	::InvalidateRect(GetSafeHwnd(), &rc, TRUE);
}

void CRulerWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags&MK_LBUTTON)
	{
		POINT pt;
		pt.x=point.x;pt.y=point.y;
		ClientToScreen(&pt);
		SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
	}
	m_mouse = point;
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

void CRulerWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

//void CRulerWnd::OnDestroy()
//{
//	CWnd::OnDestroy();
//}

void CRulerWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	ClientToScreen(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,point.x,point.y,this);
	if (m_bGetColor)
	{
		pSubMenu->CheckMenuItem(ID_MENU_GETCOLOR, MF_CHECKED);
	}
	else
	{
		pSubMenu->CheckMenuItem(ID_MENU_GETCOLOR, MF_UNCHECKED);
	}
}

void CRulerWnd::OnMenuQuit()
{
	SendMessage(WM_CLOSE);
}

void CRulerWnd::OnMenuColor()
{
	if (m_bGetColor)
	{
		SetTimer(IDT_TIMER, MONITOR_TIMER, NULL);
	}
	else
	{
		KillTimer(IDT_TIMER);
	}
	m_bGetColor = m_bGetColor ? FALSE:TRUE;
}

void CRulerWnd::OnTimer(UINT_PTR nIDEvent)
{
	POINT pt;
	GetCursorPos(&pt);
	HDC hdc = ::GetDC(NULL);
	m_clr = ::GetPixel(hdc, pt.x, pt.y);
	::ReleaseDC(NULL, hdc);
	//OnPaint();
	//OnPaint();
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}

void CRulerWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nID == (SC_SIZE | WMSZ_TOP))
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		if (rc.Height() < MINHEIGHT)
		{
			return;
		}

	}

	CWnd::OnSysCommand(nID, lParam);
}

void CRulerWnd::OnMenuRotate()
{
	m_style = (StyleE)((m_style >= LEFT) ? (TOP) : (m_style + 1));
	//SendMessage(WM_SIZE);
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	MoveWindow(rc.left, rc.top, rc.Height(), rc.Width());
	//SendMessage(WM_SIZE, rc.Height(), rc.Width());
}

BOOL CRulerWnd::OnEraseBkgnd(CDC* pDC)
{
	return NULL;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CRulerWnd::OnUpdateMenuColor(CCmdUI *pCmdUI)
{
}

void CRulerWnd::OnMenuAbout()
{
	CAboutDlg about;
	about.DoModal();
}

void CRulerWnd::OnMenuConvert()
{
	CConvertDlg convert;
	convert.DoModal();
}

void CRulerWnd::OnMenuSetting()
{
	CSetDlg dlg;
	dlg.m_nAlpha = m_nAlpha;
	if (IDOK == dlg.DoModal())
	{
		m_nAlpha = dlg.GetAlpha();
		::SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), m_nAlpha*10+105, LWA_ALPHA);
	}
}
