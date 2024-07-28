// GraphWindow.cpp : implementation file
//

#include "stdafx.h"
#include "shm_mfc.h"
#include "GraphWindow.h"
#include "GLWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GraphWindow

GraphWindow::GraphWindow()
{
}

GraphWindow::GraphWindow(CShm_mfcDlg *parentDialog)
{
}

GraphWindow::~GraphWindow()
{
}


BEGIN_MESSAGE_MAP(GraphWindow, CWnd)
	//{{AFX_MSG_MAP(GraphWindow)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// GraphWindow message handlers

int GraphWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(ID_CLOCK_TIMER, 1000, NULL);
	
	return 0;
}

void GraphWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}

void GraphWindow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CString str;
	dc.TextOut(0, 0, "Hello.");	

	CWnd::OnTimer(nIDEvent);
}
