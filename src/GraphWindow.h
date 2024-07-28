#if !defined(AFX_GRAPHWINDOW_H__64066424_3348_11D5_841D_00600837E90A__INCLUDED_)
#define AFX_GRAPHWINDOW_H__64066424_3348_11D5_841D_00600837E90A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GraphWindow window

class CShm_mfcDlg;

class GraphWindow : public CWnd
{
// Construction
public:
	GraphWindow();
	GraphWindow(CShm_mfcDlg *parentDialog);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GraphWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GraphWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(GraphWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHWINDOW_H__64066424_3348_11D5_841D_00600837E90A__INCLUDED_)
