// TopWnd.h : TopWnd Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CTopWndApp:
// �йش����ʵ�֣������ TopWnd.cpp
//

class CTopWndApp : public CWinApp
{
public:
	CTopWndApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTopWndApp theApp;
