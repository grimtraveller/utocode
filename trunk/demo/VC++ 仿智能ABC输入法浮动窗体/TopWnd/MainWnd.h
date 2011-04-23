#pragma once


// CMainWnd

class CMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd();
	virtual ~CMainWnd();
private:
	CBitmap m_BT1_1,m_BT2_1,m_BT3_1,m_BT4_1,m_BT5_1;/*按钮状态true时的位图*/
	CBitmap m_BT1_2,m_BT2_2,m_BT3_2,m_BT4_2,m_BT5_2;/*按钮状态false时的位图*/
	CRect	m_rcBT1,m_rcBT2,m_rcBT3,m_rcBT4,m_rcBT5;/*按钮位图位置矩形*/
	BOOL	m_bBTState[5];							/*
													  按钮当前状态：
													  ＝true 则使用第一组位图，
													  ＝false 使用第二组位图
													*/

	HICON	m_hCrossCursor;

	HACCEL	m_hAcc;
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool CreateMainWnd(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	bool IsInRegion(CPoint pt);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnQuitTopWnd();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);

	BOOL m_bDrag;
};


