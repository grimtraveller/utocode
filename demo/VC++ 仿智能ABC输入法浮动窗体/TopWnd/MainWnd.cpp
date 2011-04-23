// MainWnd.cpp : 实现文件
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "TopWnd.h"
#include "MainWnd.h"
#include ".\mainwnd.h"


// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

#define FRAMEWIDTH	4

CMainWnd::CMainWnd()
{
	m_bDrag = FALSE;
	for(int i=0;i<5;i++)m_bBTState[i]=true;
}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_QUIT, OnQuitTopWnd)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_NCLBUTTONUP()
END_MESSAGE_MAP()



// CMainWnd 消息处理程序


bool CMainWnd::CreateMainWnd(void)
{
	if(!CreateEx(0,
		::AfxRegisterWndClass(0,::LoadCursor(NULL,MAKEINTRESOURCE(IDC_CROSS)),GetSysColorBrush(COLOR_ACTIVEBORDER)),
		NULL,
		WS_POPUP,
		0,0,300,50,
		NULL,
		NULL))
		return false;

	return true;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//加载光标资源
	//m_hCrossCursor=AfxGetApp()->LoadCursor(IDC_CROSSCUR);
	ASSERT(m_hCrossCursor);
/*	//加载位图资源
	m_BT1_1.LoadBitmap(IDB_BITMAP1_1);
	m_BT2_1.LoadBitmap(IDB_BITMAP2_1);
	m_BT3_1.LoadBitmap(IDB_BITMAP3_1);
	m_BT4_1.LoadBitmap(IDB_BITMAP4_1);
	m_BT5_1.LoadBitmap(IDB_BITMAP5_1);
	m_BT1_2.LoadBitmap(IDB_BITMAP1_2);
	m_BT2_2.LoadBitmap(IDB_BITMAP2_2);
	m_BT3_2.LoadBitmap(IDB_BITMAP3_2);
	m_BT4_2.LoadBitmap(IDB_BITMAP4_2);
	m_BT5_2.LoadBitmap(IDB_BITMAP5_2);	
	//设置每个位图矩形：五个位图紧挨着排列在客户区中间，四周为宽度为FRAMEWIDTH的边框
	m_rcBT1.left=FRAMEWIDTH+0;  m_rcBT1.right=m_rcBT1.left+20; m_rcBT1.top=FRAMEWIDTH+0;m_rcBT1.bottom=m_rcBT1.top+20;
	m_rcBT2.left=m_rcBT1.right; m_rcBT2.right=m_rcBT2.left+40; m_rcBT2.top=FRAMEWIDTH+0;m_rcBT2.bottom=m_rcBT2.top+20;
	m_rcBT3.left=m_rcBT2.right; m_rcBT3.right=m_rcBT3.left+20; m_rcBT3.top=FRAMEWIDTH+0;m_rcBT3.bottom=m_rcBT3.top+20;
	m_rcBT4.left=m_rcBT3.right; m_rcBT4.right=m_rcBT4.left+20; m_rcBT4.top=FRAMEWIDTH+0;m_rcBT4.bottom=m_rcBT4.top+20;
	m_rcBT5.left=m_rcBT4.right; m_rcBT5.right=m_rcBT5.left+20; m_rcBT5.top=FRAMEWIDTH+0;m_rcBT5.bottom=m_rcBT5.top+20;


	//根据客户区尺寸计算并设置窗口尺寸
	CRect rcClient(0,0,120+2*FRAMEWIDTH-1,20+2*FRAMEWIDTH-1);
	SetWindowPos(&wndTopMost,0,0,rcClient.Width(),rcClient.Height(),SWP_NOMOVE|SWP_NOACTIVATE);
*/
	CenterWindow(NULL);

	//加载加速键资源
	m_hAcc=::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR));

	return 0;
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	CString str;
	str.Format(_T("x=%d y=%d"), rc.Width(), rc.Height());
	dc.DrawText(str, str.GetLength(), CRect(0,0, 100, 100), 0);
	for (int i = 0; i < rc.Width(); i+=10)
	{
		dc.MoveTo(i, 0);
		if (0 == i % 100)
		{
			dc.LineTo(i, 20);
		}
		else
		{
			dc.LineTo(i, 10);
		}
	}
/*	
	CBitmap* pOldBmp;
	CDC  CompatibleDC;
	CompatibleDC.CreateCompatibleDC(&dc);
	
	//绘制第一个按钮
	if(m_bBTState[0])
		pOldBmp=CompatibleDC.SelectObject(&m_BT1_1);
	else
		pOldBmp=CompatibleDC.SelectObject(&m_BT1_2);
	dc.BitBlt(m_rcBT1.left,m_rcBT1.top,m_rcBT1.Width(),m_rcBT1.Height(),&CompatibleDC,0,0,SRCCOPY);

	//绘制第二个按钮
	if(m_bBTState[1])
		CompatibleDC.SelectObject(&m_BT2_1);
	else
		CompatibleDC.SelectObject(&m_BT2_2);
	dc.BitBlt(m_rcBT2.left,m_rcBT2.top,m_rcBT2.Width(),m_rcBT2.Height(),&CompatibleDC,0,0,SRCCOPY);

	//绘制第三个按钮
	if(m_bBTState[2])
		CompatibleDC.SelectObject(&m_BT3_1);
	else
		CompatibleDC.SelectObject(&m_BT3_2);
	dc.BitBlt(m_rcBT3.left,m_rcBT3.top,m_rcBT3.Width(),m_rcBT3.Height(),&CompatibleDC,0,0,SRCCOPY);

	//绘制第四个按钮
	if(m_bBTState[3])
		CompatibleDC.SelectObject(&m_BT4_1);
	else
		CompatibleDC.SelectObject(&m_BT4_2);
	dc.BitBlt(m_rcBT4.left,m_rcBT4.top,m_rcBT4.Width(),m_rcBT4.Height(),&CompatibleDC,0,0,SRCCOPY);

	//绘制第五个按钮
	if(m_bBTState[4])
		CompatibleDC.SelectObject(&m_BT5_1);
	else
		CompatibleDC.SelectObject(&m_BT5_2);
	dc.BitBlt(m_rcBT5.left,m_rcBT5.top,m_rcBT5.Width(),m_rcBT5.Height(),&CompatibleDC,0,0,SRCCOPY);

	//绘制边框
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.Draw3dRect(&rcClient,RGB(192,192,192),RGB(0,0,0));
	rcClient.DeflateRect(1,1,1,1);
	dc.Draw3dRect(&rcClient,RGB(255,255,255),RGB(144,144,144));


	CompatibleDC.SelectObject(pOldBmp);
*/
	
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	bool bInRegionNow=IsInRegion(point);
	TRACE("Mouse move:(%d,%d) InRegion=%d\n",point.x,point.y,bInRegionNow);	
	bInRegionNow = true;
	if(bInRegionNow)
	{
		//SetCursor(m_hCrossCursor);
		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));
		if(nFlags&MK_LBUTTON)
		{
			POINT pt;
			pt.x=point.x;pt.y=point.y;
			ClientToScreen(&pt);
			SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,(LPARAM)(pt.x|(pt.y>>16)));
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

bool CMainWnd::IsInRegion(CPoint pt)
{
	//获取窗口坐标和客户区屏幕坐标
	CRect rcClient,rcButtons;
	GetClientRect(&rcClient);
	rcButtons.SetRect(m_rcBT1.left,m_rcBT1.top,m_rcBT5.right,m_rcBT5.bottom);

	//当在客户区之外，窗口之内时返回true
	if((!rcButtons.PtInRect(pt))&&rcClient.PtInRect(pt))
		return true;

	return false;
}
void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
/*	if(m_rcBT1.PtInRect(point))
	{m_bBTState[0]=!m_bBTState[0];InvalidateRect(&m_rcBT1);}
	else if(m_rcBT2.PtInRect(point))
	{m_bBTState[1]=!m_bBTState[1];InvalidateRect(&m_rcBT2);}
	else if(m_rcBT3.PtInRect(point))
	{m_bBTState[2]=!m_bBTState[2];InvalidateRect(&m_rcBT3);}
	else if(m_rcBT4.PtInRect(point))
	{m_bBTState[3]=!m_bBTState[3];InvalidateRect(&m_rcBT4);}
	else if(m_rcBT5.PtInRect(point))
	{m_bBTState[4]=!m_bBTState[4];InvalidateRect(&m_rcBT5);}
*/
	//if(IsInRegion(point))SetCursor(m_hCrossCursor);

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CMainWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//TRACE("PreTranslateMessage: %d\n",TranslateAccelerator(m_hWnd,m_hAcc,pMsg));
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CMainWnd::OnQuitTopWnd()
{
	// TODO: 在此添加命令处理程序代码
	TRACE("Quit\n");
	PostQuitMessage(0);
}

LRESULT CMainWnd::OnNcHitTest(CPoint point)
{
	TRACE("OnNcHitTest ");
 CRect rect;
 GetWindowRect(&rect);
 if(point.x <= rect.left+3)
 {
	 TRACE("HTLEFT\n");
  return HTLEFT;
 }
 else if(point.x >= rect.right-3)
 {
	 TRACE("HTRIGHT\n");
	 return HTRIGHT;
 }
 else if(point.y <= rect.top+3)
 {
	 TRACE("HTTOP\n");
	 return HTTOP;
 }
 else if(point.y >= rect.bottom-3)
 {
	 TRACE("HTBOTTOM\n");
	 return HTBOTTOM;
 }
 else if(point.x <= rect.left+10 && point.y <= rect.top+10)
 {
	 TRACE("HTTOPLEFT\n");
	 return HTTOPLEFT;
 }
 else if(point.x >= rect.right-10 && point.y <= rect.top+10)
 {
	 TRACE("HTTOPRIGHT\n");
	 return HTTOPRIGHT;
 }
 else if(point.x <= rect.left+10 && point.y >= rect.bottom-10)
 {
	 TRACE("HTBOTTOMLEFT\n");
	 return HTBOTTOMLEFT;
 }
 else if(point.x >= rect.right-10 && point.y >= rect.bottom-10)
 {
	 TRACE("HTBOTTOMRIGHT\n");
	 return HTBOTTOMRIGHT;
 }
 

LRESULT ret = CWnd::OnNcHitTest(point);
TRACE("%x\n", ret);
return ret;
}

BOOL CMainWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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
  //else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT)
  //{
  //SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
  //}
  //else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
  //{
  //SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
  //}
  else
  {
  SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
  }
  return TRUE;
  
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	TRACE0("OnNcLButtonDown\n");
  //if(nHitTest == HTTOP)
  //SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
  //else if(nHitTest == HTBOTTOM)
  //SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
  /*else*/ if(nHitTest == HTLEFT)
  {
  SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
  m_bDrag = TRUE;
  }
  else if(nHitTest == HTRIGHT)
  {
  SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
  m_bDrag = TRUE;
  }
  //else if(nHitTest == HTTOPLEFT)
  //SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
  //else if(nHitTest == HTTOPRIGHT)
  //SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
  //else if(nHitTest == HTBOTTOMLEFT)
  //SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
  //else if(nHitTest == HTBOTTOMRIGHT)
  //SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
  else
  {
	  m_bDrag = FALSE;
	  CWnd::OnNcLButtonDown(nHitTest, point);
  }

	//CWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	::InvalidateRect(GetSafeHwnd(), &rc, TRUE);

	// TODO: 在此处添加消息处理程序代码
}

void CMainWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDrag)
	{
		SendMessage( WM_SYSCOMMAND, SC_SIZE| WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	}

	CWnd::OnNcLButtonUp(nHitTest, point);
}
