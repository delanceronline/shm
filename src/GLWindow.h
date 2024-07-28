#include <gl\gl.h>
#include <gl\glu.h>

#if !defined(AFX_GLWINDOW_H__048D92C6_6DF2_11D3_8A92_00A0CC2492EC__INCLUDED_)
#define AFX_GLWINDOW_H__048D92C6_6DF2_11D3_8A92_00A0CC2492EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGLWindow window

class CGLWindow : public CWnd
{
// Construction
public:
	CGLWindow();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nPixelFormat;
	HGLRC m_hRC;
	HDC m_hDC;
	void DrawScene(void);
	void ViewPattern(void);
	virtual ~CGLWindow();

	// Generated message map functions
public:
	LARGE_INTEGER timerFrequency;
	LARGE_INTEGER globeTime;	// Start time of the animation
	LARGE_INTEGER currentTime;	// The current time
	double frame_time;
	double fSeconds;				// Elapsed time in seconds
	double prevfSeconds;				// Previous time in seconds
	double prevfctrlSeconds;				// Previous controller time in seconds

	GLuint m_Sides[10];
	float mass, k_spring_x, omega_x, displacement_x, amp_x, rotx, roty, posz;
	float k_spring_z, omega_z, displacement_z, amp_z;
	float time, spring_rot, spring_rot_start, spring_rot_end, no_of_rot, ppr;
	float linearvelocity_z, linearvelocity_y, linearvelocity_x;
	float angularspeedx, angularspeedy;
	float phase_lag;
	float unit_pp_x, unit_pp_z;
	GLfloat ambientLight[4];
	GLfloat diffuseLight[4];
	GLfloat specular[4];
	GLfloat specref[4];
	GLfloat lightpos[4];
	GLfloat spotDir[4];
	bool pause;
	bool reset;
	bool play;
	bool view_pattern;
	GLubyte locus_pattern[65535 * 3 + 1];
	
	//{{AFX_MSG(CGLWindow)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLWINDOW_H__048D92C6_6DF2_11D3_8A92_00A0CC2492EC__INCLUDED_)
