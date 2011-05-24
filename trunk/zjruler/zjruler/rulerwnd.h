#pragma once


// CRulerWnd

class CRulerWnd : public CWnd
{
	DECLARE_DYNAMIC(CRulerWnd)

public:
	CRulerWnd();
	virtual ~CRulerWnd();
	BOOL CreateWnd(void);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	typedef enum STYLEE
	{
		TOP = 0,
		RIGHT = 1,
		BOTTOM = 2,
		LEFT = 3,
		
	}StyleE;
	StyleE m_style;
	int m_nAlpha;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	CMenu m_menu;
	COLORREF m_clr;
	BOOL m_bHex;
	CRect m_rc;
	BOOL m_bGetColor;
	CPoint m_mouse;
public:
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuColor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMenuRotate();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateMenuColor(CCmdUI *pCmdUI);
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuConvert();
	afx_msg void OnMenuSetting();
};


