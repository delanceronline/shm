// shm_mfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "shm_mfc.h"
#include "shm_mfcDlg.h"
#include <math.h>
#include <afxdlgs.h>
#include <strstrea.h>
#include <stdlib.h>
#include <afx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShm_mfcDlg dialog

CShm_mfcDlg::CShm_mfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShm_mfcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShm_mfcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShm_mfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShm_mfcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShm_mfcDlg, CDialog)
	//{{AFX_MSG_MAP(CShm_mfcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShm_mfcDlg message handlers

BOOL CShm_mfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		16,							// Want 32 bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		16,								// Size of depth buffer
		1,								// Not used to select mode
		0,								// Not used to select mode
		0,	            				// Not used to select mode
		0,								// Not used to select mode
		0,0,0 };
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	
	view_pattern_mode = false;
	
	WNDCLASS wndClass;

	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = AfxWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = AfxGetInstanceHandle();
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "SHMClass";

	// Register the window class
	AfxRegisterClass(&wndClass);
	
	//Sliders for camera.
	y_rot_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_BOTTOM, CRect(CPoint(160, 0), CSize(200, 35)), this, IDC_ROT_Y_SLIDER);
	y_rot_slider.SetRange(0, 360);
	y_rot_slider.SetPos(180);
	y_rot_slider.SetTicFreq(36);
	y_rot_slider.SetLineSize(1);

	zoom_slider.Create(WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_LEFT, CRect(CPoint(500, 70), CSize(35, 200)), this, IDC_ZOOM_SLIDER);
	zoom_slider.SetRange(0, 100);
	zoom_slider.SetPos(20);
	zoom_slider.SetTicFreq(5);
	zoom_slider.SetLineSize(1);

	x_rot_slider.Create(WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_AUTOTICKS | TBS_RIGHT, CRect(CPoint(0, 70), CSize(35, 200)), this, IDC_ROT_X_SLIDER);
	x_rot_slider.SetRange(0, 90);
	x_rot_slider.SetPos(27);
	x_rot_slider.SetTicFreq(9);
	x_rot_slider.SetLineSize(1);

	//Sliders for parameters.
	init_omega_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_TOP, CRect(CPoint(550, 80), CSize(200, 35)), this, IDC_INIT_OMEGA_SLIDER);
	init_omega_slider.SetRange(1, 100);
	init_omega_slider.SetPos(20);
	init_omega_slider.SetTicFreq(5);
	init_omega_slider.SetLineSize(1);

	omega_ratio_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_TOP, CRect(CPoint(550, 145), CSize(200, 35)), this, IDC_OMEGA_RATIO_SLIDER);
	omega_ratio_slider.SetRange(1, 100);
	omega_ratio_slider.SetPos(10);
	omega_ratio_slider.SetTicFreq(4);
	omega_ratio_slider.SetLineSize(1);

	phase_lag_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_TOP, CRect(CPoint(550, 210), CSize(200, 35)), this, IDC_PHASE_LAG_SLIDER);
	phase_lag_slider.SetRange(0, 100);
	phase_lag_slider.SetPos(0);
	phase_lag_slider.SetTicFreq(4);
	phase_lag_slider.SetLineSize(1);

	amp_x_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_TOP, CRect(CPoint(550, 275), CSize(200, 35)), this, IDC_AMP_X_SLIDER);
	amp_x_slider.SetRange(0, 40);
	amp_x_slider.SetPos(40);
	amp_x_slider.SetTicFreq(2);
	amp_x_slider.SetLineSize(1);

	amp_z_slider.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS | TBS_TOP, CRect(CPoint(550, 340), CSize(200, 35)), this, IDC_AMP_Z_SLIDER);
	amp_z_slider.SetRange(0, 40);
	amp_z_slider.SetPos(40);
	amp_z_slider.SetTicFreq(2);
	amp_z_slider.SetLineSize(1);

	//Value displays for parameters.
	init_omega_static.Create("", WS_CHILD | WS_VISIBLE | SS_SUNKEN , CRect(CPoint(700, 55), CSize(38, 20)), this, IDC_INIT_OMEGA_STATIC);
	init_omega_static.SetWindowText("2.000");

	omega_ratio_static.Create("", WS_CHILD | WS_VISIBLE | SS_SUNKEN , CRect(CPoint(700, 120), CSize(38, 20)), this, IDC_OMEGA_RATIO_STATIC);
	omega_ratio_static.SetWindowText("1.000");

	phase_lag_static.Create("", WS_CHILD | WS_VISIBLE | SS_SUNKEN , CRect(CPoint(700, 185), CSize(38, 20)), this, IDC_PHASE_LAG_STATIC);
	phase_lag_static.SetWindowText("0.000");

	amp_x_static.Create("", WS_CHILD | WS_VISIBLE | SS_SUNKEN , CRect(CPoint(700, 250), CSize(38, 20)), this, IDC_AMP_X_STATIC);
	amp_x_static.SetWindowText("4.000");

	amp_z_static.Create("", WS_CHILD | WS_VISIBLE | SS_SUNKEN , CRect(CPoint(700, 315), CSize(38, 20)), this, IDC_AMP_Z_STATIC);
	amp_z_static.SetWindowText("4.000");

	button = (CButton*)GetDlgItem(IDC_BUTTON4);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON5);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(FALSE);

	CDC *pDC;
	pDC = GetDC();
	memdc_graphpaper.CreateCompatibleDC(pDC);
	graphpaper.LoadBitmap(IDB_GRAPHPAPER);
	memdc_graphpaper.SelectObject(&graphpaper);

	CRect glRect;
	// Create the storm direction window control
	GetDlgItem(IDC_TEST_FRAME)->GetWindowRect(&glRect);

	newRect.top = glRect.top;
	newRect.bottom = glRect.top + 420;
	newRect.left = glRect.left + 30;
	newRect.right = glRect.left + 480;

	// Destroy the window and reset the pixel format
	m_glWnd.DestroyWindow();
	
	HDC hDC = ::GetDC(this->m_hWnd);
	m_glWnd.m_nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	::ReleaseDC(this->m_hWnd, hDC);

	m_glWnd.Create("SHMClass", NULL, WS_CHILD | WS_VISIBLE, newRect, this, 901, NULL);
	m_glWnd.Invalidate(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShm_mfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShm_mfcDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		int x = 100, y = 100;

		dc.BitBlt(700, 400, 64, 64, &memdc_graphpaper, 0, 0, SRCCOPY);
				
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShm_mfcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CShm_mfcDlg::OnHScroll(UINT nSBCode, UINT nPos, CWnd* pCtrl)
{
	CSliderCtrl *pSld = (CSliderCtrl*)pCtrl;

	if(pSld == &y_rot_slider)
		m_glWnd.roty = (float)pSld->GetPos();
	
	if(pSld == &init_omega_slider)
	{
		CString s;
		float init_omega;
		
		init_omega = (float)pSld->GetPos() / 10.0f;
		m_glWnd.omega_x = init_omega;
		m_glWnd.omega_z = init_omega * ((float)omega_ratio_slider.GetPos() / 10.0f);
		s.Format("%f", init_omega);
		init_omega_static.SetWindowText(s);
	}

	if(pSld == &omega_ratio_slider)
	{
		CString s;
		float init_omega;

		init_omega = (float)pSld->GetPos() / 10.0f;
		m_glWnd.omega_z = m_glWnd.omega_x * init_omega;
		s.Format("%f", init_omega);
		omega_ratio_static.SetWindowText(s);
	}

	if(pSld == &phase_lag_slider)
	{
		CString s;
		
		m_glWnd.phase_lag = (float)pSld->GetPos() / 10.0f;
		s.Format("%f", m_glWnd.phase_lag);
		phase_lag_static.SetWindowText(s);
	}

	if(pSld == &amp_x_slider)
	{
		CString s;
		
		m_glWnd.amp_x = (float)pSld->GetPos() / 10.0f;
		s.Format("%f", m_glWnd.amp_x);
		amp_x_static.SetWindowText(s);
	}

	if(pSld == &amp_z_slider)
	{
		CString s;
		
		m_glWnd.amp_z = (float)pSld->GetPos() / 10.0f;
		s.Format("%f", m_glWnd.amp_z);
		amp_z_static.SetWindowText(s);
	}
}

void CShm_mfcDlg::OnVScroll(UINT nSBCode, UINT nPos, CWnd* pCtrl)
{
	CSliderCtrl *pSld = (CSliderCtrl*)pCtrl;

	if(pSld == &zoom_slider)
		m_glWnd.posz = (float)pSld->GetPos() * 0.5f;

	if(pSld == &x_rot_slider)
		m_glWnd.rotx = (float)pSld->GetPos();

}

//Pause button.
void CShm_mfcDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_glWnd.pause = !m_glWnd.pause;
	m_glWnd.play = false;
	m_glWnd.reset = false;

	button = (CButton*)GetDlgItem(IDC_BUTTON4);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON5);
	button->EnableWindow(TRUE);
	
	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON3);
	button->EnableWindow(TRUE);

	init_omega_slider.EnableWindow(FALSE);
	omega_ratio_slider.EnableWindow(FALSE);
	phase_lag_slider.EnableWindow(FALSE);
	amp_x_slider.EnableWindow(FALSE);
	amp_z_slider.EnableWindow(FALSE);
}

//Reset button.
void CShm_mfcDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
	m_glWnd.reset = !m_glWnd.reset;

	button = (CButton*)GetDlgItem(IDC_BUTTON4);
	button->EnableWindow(FALSE);	
	button = (CButton*)GetDlgItem(IDC_BUTTON5);
	button->EnableWindow(FALSE);

	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON3);
	button->EnableWindow(TRUE);

	init_omega_slider.EnableWindow(TRUE);
	omega_ratio_slider.EnableWindow(TRUE);
	phase_lag_slider.EnableWindow(TRUE);
	amp_x_slider.EnableWindow(TRUE);
	amp_z_slider.EnableWindow(TRUE);
}

//Play button.
void CShm_mfcDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON3);
	button->EnableWindow(FALSE);

	button = (CButton*)GetDlgItem(IDC_BUTTON4);
	button->EnableWindow(FALSE);	
	button = (CButton*)GetDlgItem(IDC_BUTTON5);
	button->EnableWindow(FALSE);

	m_glWnd.play = true;
	m_glWnd.pause = false;
	m_glWnd.reset = false;

	init_omega_slider.EnableWindow(FALSE);
	omega_ratio_slider.EnableWindow(FALSE);
	phase_lag_slider.EnableWindow(FALSE);
	amp_x_slider.EnableWindow(FALSE);
	amp_z_slider.EnableWindow(FALSE);
}

//Save button.
void CShm_mfcDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	const char fileDialogFilter[] = "BMP File (*.bmp) | *.bmp ||";
	const char fileDialogExt[] = "bmp";
	CString s;
	BITMAPINFO *bmpinfo;
	int i, k = 0;
	int l = 65535 * 3 + 1;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		16,							// Want 32 bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		16,								// Size of depth buffer
		1,								// Not used to select mode
		0,								// Not used to select mode
		0,	            				// Not used to select mode
		0,								// Not used to select mode
		0,0,0 };

	bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo->bmiHeader.biWidth = 256;
	bmpinfo->bmiHeader.biHeight = 256;
	bmpinfo->bmiHeader.biPlanes = 1;
	bmpinfo->bmiHeader.biBitCount = 24;
	bmpinfo->bmiHeader.biCompression = BI_RGB;
	bmpinfo->bmiHeader.biSizeImage = 0;
	bmpinfo->bmiHeader.biXPelsPerMeter = 2952;
	bmpinfo->bmiHeader.biYPelsPerMeter = 2952;
	bmpinfo->bmiHeader.biClrUsed = 0;
	bmpinfo->bmiHeader.biClrImportant = 0;
		
	//Save pattern and time before destroying GLWindow.
	for(i = 0; i < l; i += 3)
	{
		locus_pattern_swap[i] = m_glWnd.locus_pattern[i + 2];
		locus_pattern_swap[i + 1] = m_glWnd.locus_pattern[i + 1];
		locus_pattern_swap[i + 2] = m_glWnd.locus_pattern[i];
	}
	time = m_glWnd.time;
		
	//Destroy GLWindow before popping up dialog.
	m_glWnd.DestroyWindow();

	CFileDialog fileDialog(FALSE, fileDialogExt, NULL, OFN_OVERWRITEPROMPT, fileDialogFilter);

	//Pop up file dialog.
	if(fileDialog.DoModal() == IDOK)
	{	
		if(SaveDIBitmap(fileDialog.GetPathName(), bmpinfo, locus_pattern_swap) == 0)
			MessageBox("The file was sucessfully saved.", "Saving Status", MB_ICONINFORMATION | MB_SETFOREGROUND);
		else
			MessageBox("Unable to save the file.", "Saving Status", MB_ICONERROR | MB_OK);
		
		view_pattern_mode = false;
	}

	//Recreate GLWindow.
	HDC hDC = ::GetDC(this->m_hWnd);
	m_glWnd.m_nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	::ReleaseDC(this->m_hWnd, hDC);
	m_glWnd.Create("SHMClass", NULL, WS_CHILD | WS_VISIBLE, newRect, this, 901, NULL);
	m_glWnd.Invalidate(FALSE);
		
	//Restore all valuables.
	m_glWnd.roty = (float)y_rot_slider.GetPos();
	m_glWnd.rotx = (float)x_rot_slider.GetPos();
	m_glWnd.posz = (float)zoom_slider.GetPos() * 0.5f;
	m_glWnd.omega_x = (float)init_omega_slider.GetPos() / 10.0f;
	m_glWnd.omega_z = m_glWnd.omega_x * (float)omega_ratio_slider.GetPos() / 10.0f;
	m_glWnd.phase_lag = (float)phase_lag_slider.GetPos() / 10.0f;
	m_glWnd.amp_x = (float)amp_x_slider.GetPos() / 10.0f;
	m_glWnd.amp_z = (float)amp_z_slider.GetPos() / 10.0f;
	m_glWnd.time = time;
	for(i = 0; i < l; i++)
		m_glWnd.locus_pattern[i] = locus_pattern_swap[i];

	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON3);
	button->EnableWindow(TRUE);

	m_glWnd.pause = true;
	m_glWnd.play = false;
	m_glWnd.reset = false;

	button = (CButton*)GetDlgItem(IDC_BUTTON4);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON5);
	button->EnableWindow(TRUE);
	
	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_BUTTON2);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_BUTTON3);
	button->EnableWindow(TRUE);

	init_omega_slider.EnableWindow(FALSE);
	omega_ratio_slider.EnableWindow(FALSE);
	phase_lag_slider.EnableWindow(FALSE);
	amp_x_slider.EnableWindow(FALSE);
	amp_z_slider.EnableWindow(FALSE);
}

//View pattern button.
void CShm_mfcDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	view_pattern_mode = !view_pattern_mode;

	if(view_pattern_mode)
	{
		CClientDC dc(this);
	
		SetDlgItemText(IDC_BUTTON5, "View Model");
		button = (CButton*)GetDlgItem(IDC_BUTTON5);
		button->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON4);
		button->EnableWindow(TRUE);

		button = (CButton*)GetDlgItem(IDC_BUTTON1);
		button->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON2);
		button->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON3);
		button->EnableWindow(FALSE);

		m_glWnd.view_pattern = true;
		
		button = (CButton*)GetDlgItem(IDC_BUTTON5);
		button->EnableWindow(TRUE);
	}
	else
	{
		
		SetDlgItemText(IDC_BUTTON5, "View Pattern");
		button = (CButton*)GetDlgItem(IDC_BUTTON5);
		button->EnableWindow(FALSE);
		
		m_glWnd.view_pattern = false;
		
		//Set control states.
		button->EnableWindow(TRUE);
		button = (CButton*)GetDlgItem(IDC_BUTTON4);
		button->EnableWindow(FALSE);

		button = (CButton*)GetDlgItem(IDC_BUTTON1);
		button->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON2);
		button->EnableWindow(TRUE);
		button = (CButton*)GetDlgItem(IDC_BUTTON3);
		button->EnableWindow(TRUE);
	}
}

int CShm_mfcDlg :: SaveDIBitmap(const char *filename, BITMAPINFO *info, GLubyte *bits)
{
    CFile myFile;
    int              size,         //Size of file
                     infosize,     //Size of bitmap info
                     bitsize;      //Size of bitmap pixels
    BITMAPFILEHEADER header;       //File header

	
    if (myFile.Open(filename, CFile::modeCreate | CFile::modeWrite) == 0)
		return (-1);

    //Figure out bitmap size.
    if (info->bmiHeader.biSizeImage == 0)
	bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 *
		  abs(info->bmiHeader.biHeight);
    else
	bitsize = info->bmiHeader.biSizeImage;

    //Figure out header size.
    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression)
	{
	case BI_BITFIELDS :
            infosize += 12;  //Add 3 RGB doubleword masks.
            if (info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RGB :
            if (info->bmiHeader.biBitCount > 8 &&
        	info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RLE8 :
	case BI_RLE4 :
            if (info->bmiHeader.biClrUsed == 0)
              infosize += (1 << info->bmiHeader.biBitCount) * 4;
	    else
              infosize += info->bmiHeader.biClrUsed * 4;
	    break;
	}

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

    // Write file header, bitmap information, and bitmap pixel data.
    header.bfType      = 'MB';
    header.bfSize      = size;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits   = sizeof(BITMAPFILEHEADER) + infosize;

	myFile.Write(&header, sizeof(BITMAPFILEHEADER));
	myFile.Write(info, infosize);
	myFile.Write(bits, bitsize);

    myFile.Close();
    return (0);
}

CShm_mfcDlg :: ~CShm_mfcDlg()
{
	m_glWnd.DestroyWindow();
}