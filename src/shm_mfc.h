// shm_mfc.h : main header file for the SHM_MFC application
//

#if !defined(AFX_SHM_MFC_H__58F76764_2E83_11D5_841D_00600837E90A__INCLUDED_)
#define AFX_SHM_MFC_H__58F76764_2E83_11D5_841D_00600837E90A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CShm_mfcApp:
// See shm_mfc.cpp for the implementation of this class
//

class CShm_mfcApp : public CWinApp
{
public:
	CShm_mfcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShm_mfcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShm_mfcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHM_MFC_H__58F76764_2E83_11D5_841D_00600837E90A__INCLUDED_)
