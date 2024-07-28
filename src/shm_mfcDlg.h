// shm_mfcDlg.h : header file
//

#include "GLWindow.h"
#include "GraphWindow.h"

#if !defined(AFX_SHM_MFCDLG_H__58F76766_2E83_11D5_841D_00600837E90A__INCLUDED_)
#define AFX_SHM_MFCDLG_H__58F76766_2E83_11D5_841D_00600837E90A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShm_mfcDlg dialog

class CShm_mfcDlg : public CDialog
{
// Construction
public:
	CShm_mfcDlg(CWnd* pParent = NULL);	// standard constructor
	CGLWindow m_glWnd;

	int SaveDIBitmap(const char *filename, BITMAPINFO *info, GLubyte *bits);
	// Dialog Data
	//{{AFX_DATA(CShm_mfcDlg)
	enum { IDD = IDD_SHM_MFC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShm_mfcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	public:
	virtual ~CShm_mfcDlg();
	//}}AFX_VIRTUAL

// Implementation
protected:

	float time;
	bool view_pattern_mode;
	HICON m_hIcon;
	CRect newRect;
	CSliderCtrl x_rot_slider;
	CSliderCtrl y_rot_slider;
	CSliderCtrl zoom_slider;
	CSliderCtrl init_omega_slider;
	CSliderCtrl omega_ratio_slider;
	CSliderCtrl phase_lag_slider;
	CSliderCtrl amp_x_slider;
	CSliderCtrl amp_z_slider;
	CStatic init_omega_static;
	CStatic omega_ratio_static;
	CStatic phase_lag_static;
	CStatic amp_x_static;
	CStatic amp_z_static;
	CButton *button;
	CBitmap graphpaper;
	CDC memdc_graphpaper;
	GLubyte locus_pattern_swap[65535 * 3 + 1];
	//GLubyte testing[65536 * 3];

	// Generated message map functions
	//{{AFX_MSG(CShm_mfcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CWnd* pSlider);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CWnd* pSlider);
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHM_MFCDLG_H__58F76766_2E83_11D5_841D_00600837E90A__INCLUDED_)
