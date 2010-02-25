// mmf_t.h : main header file for the MMF_T application
//

#if !defined(AFX_MMF_T_H__9CADDA2D_8357_49BF_9136_677DF16A588C__INCLUDED_)
#define AFX_MMF_T_H__9CADDA2D_8357_49BF_9136_677DF16A588C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMmf_tApp:
// See mmf_t.cpp for the implementation of this class
//

class CMmf_tApp : public CWinApp
{
public:
	CMmf_tApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMmf_tApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMmf_tApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMF_T_H__9CADDA2D_8357_49BF_9136_677DF16A588C__INCLUDED_)
