// GLWindow.cpp : implementation file
//

#include "stdafx.h"
#include "shm_mfc.h"
#include "GLWindow.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEGTORAD(x) ((x) * 0.017453292519943296)
#define RADTODEG(x) ((x) / 0.017453292519943296)
#define PI 3.1415926535897932
#define TWO_PI (2.0 * 3.1415926535897932)

BYTE* gltResourceBMPBits(UINT nResource, int *nWidth, int *nHeight)
{
	HINSTANCE hInstance;
	HANDLE hBitmap;
	BITMAPINFO bmInfo;

	// Find the bitmap resource
	hInstance = GetModuleHandle(NULL);
	hBitmap = LoadBitmap(hInstance,MAKEINTRESOURCE(nResource));

	if(hBitmap == NULL)
		return NULL;

	GetObject(hBitmap,sizeof(BITMAPINFO),&bmInfo);
	DeleteObject(hBitmap);

	hBitmap = ::LoadResource(hInstance,
		 ::FindResource(hInstance,MAKEINTRESOURCE(nResource), RT_BITMAP));

	if(hBitmap == NULL)
		return NULL;

	BYTE *pData = (BYTE *)LockResource(hBitmap);
	pData += sizeof(BITMAPINFO)-1;

	*nWidth = bmInfo.bmiHeader.biWidth; 
	*nHeight = bmInfo.bmiHeader.biHeight;

	return pData;
}

/////////////////////////////////////////////////////////////////////////////
// CGLWindow

CGLWindow::CGLWindow()
{
	m_hRC = NULL;
	m_hDC = NULL;
	m_nPixelFormat;
}

CGLWindow::~CGLWindow()
{
		CGLWindow::OnDestroy();
}

BEGIN_MESSAGE_MAP(CGLWindow, CWnd)
	//{{AFX_MSG_MAP(CGLWindow)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLWindow message handlers

void CGLWindow::OnPaint() 
{
	QueryPerformanceCounter(&currentTime);
	fSeconds = (double)(currentTime.QuadPart - globeTime.QuadPart) / (double)timerFrequency.QuadPart;
	frame_time = (double)(fSeconds - prevfctrlSeconds);
	prevfctrlSeconds = fSeconds;
		
	if(reset)
	{
		//Clean image buffer contents.
		int l = 65535 * 3 + 1;

		time = 0.0f;
		pause = false;
		play = false;
		for(int i = 0; i < l; i++)
			locus_pattern[i] = 0xdd;

		//Redefinite pattern texture.
		glBindTexture(GL_TEXTURE_2D, m_Sides[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, locus_pattern);
	}

	if(pause)
	{
			
		reset = false;
		play = false;
			
		//Redefinite pattern texture.
		glBindTexture(GL_TEXTURE_2D, m_Sides[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, locus_pattern);
			
	}

	if(play)
	{
		//Advacne frame time.
		time += (float)frame_time;
		pause = false;
		reset = false;
	}

	angularspeedx = 0.0f;
	angularspeedy = 0.0f;
	linearvelocity_z = 0.0f;
		
	if(!view_pattern)
		DrawScene();
	else
		ViewPattern();

	SwapBuffers(m_hDC);
}

void CGLWindow::DrawScene()
{
	float var, a, b, c, angle, spring_inc, temp1, temp2, springlen;
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();									// Reset The View

	glTranslatef(0.0f, 0.0f, -posz - 10.0f);
	glRotatef(rotx, 1.0f, 0.0f, 0.0f);
	glRotatef(roty, 0.0f, 1.0f, 0.0f);
	
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-8.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		
		//Draw wall.
		glBindTexture(GL_TEXTURE_2D, m_Sides[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -2.0f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -2.0f, -3.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 5.0f, -3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 5.0f,  3.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, m_Sides[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(11.0f, -2.0f, 8.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -2.0f, 8.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 5.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(11.0f, 5.0f, 8.0f);
		glEnd();
		
		//Draw floor.
		glBindTexture(GL_TEXTURE_2D, m_Sides[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -2.0f, -9.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0f, -2.0f,  9.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -2.0f, 9.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, -2.0f, -9.0f);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		//Calculate displacements
		displacement_x = (float)(amp_x * sin(omega_x * time));
		displacement_z = (float)(amp_z * sin(omega_z * time + phase_lag));
		
		glColor3f(1.0f, 1.0f, 1.0f);
		
		//Draw spring in x-axis.
		c = 0.5f;
		temp1 = displacement_x + 7.0f;
		temp2 = displacement_z;
		springlen = powf(temp1 * temp1 + temp2 * temp2, 0.5f);

		spring_inc = (springlen - 1.0f) / (no_of_rot * ppr);

		glPushMatrix();
			var = temp2 / temp1;
			var = (float)RADTODEG(atan((double)var));
			glRotatef(-var, 0.0f, 1.0f, 0.0f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.5f, 0.0f, 0.0f);
				glVertex3f(0.5f, 0.5f, 0.0f);
				for(angle = spring_rot_start; angle <= spring_rot_end; angle += spring_rot)
				{
					a = (float)(0.5f * sin(angle));
					b = (float)(0.5f * cos(angle));
					glVertex3f(c, b, a);
					c += spring_inc;
				}
				glVertex3f(c , 0.0f, 0.0f);
				glVertex3f(c + 0.5f, 0.0f, 0.0f);
			glEnd();
		glPopMatrix();

	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 8.0f);

		//Draw spring in z-axis.
		c = -0.5f;
		temp1 = displacement_x;
		temp2 = 7.0f - displacement_z;
		springlen = powf(temp1 * temp1 + temp2 * temp2, 0.5f);

		spring_inc = (springlen - 1.0f) / (no_of_rot * ppr);

		glPushMatrix();
			var = temp1 / temp2;
			var = (float)RADTODEG(atan((double)var));
			glRotatef(-var, 0.0f, 1.0f, 0.0f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, -0.5f);
				glVertex3f(0.0f, 0.5f, -0.5f);
				for(angle = spring_rot_start; angle <= spring_rot_end; angle += spring_rot)
				{
					a = (float)(0.5f * sin(angle));
					b = (float)(0.5f * cos(angle));
					glVertex3f(a, b, c);
					c -= spring_inc;
				}
				glVertex3f(0.0f, 0.0f, c);
				glVertex3f(0.0f, 0.0f, c - 0.5f);
			glEnd();
		glPopMatrix();

	glPopMatrix();

	if(play)
	{
		//Record pattern.
		temp1 = (4.0f + displacement_x) / unit_pp_x;
		temp2 = (4.0f + displacement_z) / unit_pp_z;

		int vert, horz;
		vert = (int)temp1;
		horz = (int)temp2;
		
		if(vert == 256)
			vert = 255;
		if(horz == 256)
			horz = 255;

		i = (256 * 3 * vert) + horz * 3 + 1;
		
		locus_pattern[i - 1] = 0x00;
		locus_pattern[i + 1 - 1] = 0x00;
		locus_pattern[i + 2 - 1] = 0xff;

		glBindTexture(GL_TEXTURE_2D, m_Sides[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, locus_pattern);
	}
	
	//Draw pattern.
	glPushMatrix();
		glTranslatef(-4.0f, -1.9f, -4.0f);	
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_Sides[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 8.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 0.0f, 8.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(8.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();

	//Draw box.
	glTranslatef(displacement_x, 0.0f, displacement_z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Sides[0]);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	//Draw pen.
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.2f, 0.8f, 0.6f);
	glBegin(GL_LINE_STRIP);
		glVertex3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -2.0f, 0.0f);
	glEnd();

}

int CGLWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	QueryPerformanceCounter(&globeTime);
	QueryPerformanceFrequency(&timerFrequency);

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//Get device context.
	m_hDC = ::GetDC(this->m_hWnd);
	
	//Set pixel format.
	SetPixelFormat(m_hDC, m_nPixelFormat, NULL);

	//Create rendering context and make it current.
	m_hRC = wglCreateContext(m_hDC);

	if(m_hRC != NULL)
		wglMakeCurrent(m_hDC, m_hRC);
	else
		return -1;

	if(m_hRC != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
		glGenTextures(4, m_Sides);
		BYTE *pBytes;
		int nWidth, nHeight;
			
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//Load textures.
		pBytes = gltResourceBMPBits(IDB_WOOD, &nWidth, &nHeight);
		glBindTexture(GL_TEXTURE_2D, m_Sides[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nWidth, nHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

		pBytes = gltResourceBMPBits(IDB_WALL1, &nWidth, &nHeight);
		glBindTexture(GL_TEXTURE_2D, m_Sides[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nWidth, nHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

		pBytes = gltResourceBMPBits(IDB_FLOOR, &nWidth, &nHeight);
		glBindTexture(GL_TEXTURE_2D, m_Sides[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nWidth, nHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

		pBytes = gltResourceBMPBits(IDB_WALL2, &nWidth, &nHeight);
		glBindTexture(GL_TEXTURE_2D, m_Sides[3]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nWidth, nHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

		int l = 65535 * 3 + 1;
		for(int i = 0; i < l; i++)
			locus_pattern[i] = 0xdd;

		glBindTexture(GL_TEXTURE_2D, m_Sides[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, locus_pattern);

		//Initialize parameters.
		mass = 1.5f;
		k_spring_x = 6.0f;
		k_spring_z = 4.0f;
		amp_x = 4.0f;
		amp_z = 4.0f;
		no_of_rot = 4.0f;
		ppr = 20.0f;
		omega_x = 2.0f;
		omega_z = 2.0f;
		spring_rot = (float)TWO_PI / ppr;
		spring_rot_start = 0.0f;
		spring_rot_end = (float)TWO_PI * no_of_rot;
		phase_lag = 0.0f;

		rotx = 27.0f;
		roty = 180.0f;
		posz = 10.0f;

		linearvelocity_z = 0.0f;
		linearvelocity_y = 0.0f;
		linearvelocity_x = 0.0f;
		angularspeedx = 0.0f;
		angularspeedy = 0.0f;

		prevfSeconds = 0.0;
		prevfctrlSeconds = 0.0;

		time = 0.0f;

		pause = true;
		reset = false;
		play = false;

		view_pattern = false;
							
		unit_pp_x = 8.0f / 256.0f;
		unit_pp_z = 8.0f / 256.0f;
	}
	else
		return -1;
		
	return 0;
}

void CGLWindow::OnDestroy() 
{
	CWnd::OnDestroy();

	if(m_hRC)
	{	
		glDeleteTextures(6, m_Sides);


		wglMakeCurrent(m_hDC, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	::ReleaseDC(this->m_hWnd, m_hDC);
	m_hDC = NULL;
}

void CGLWindow::OnSize(UINT nType, int w, int h) 
{
	CWnd::OnSize(nType, w, h);
	
	GLfloat fAspect;

	//Prevent divide by zero.
	if(h == 0)
		h = 1;

	//Set viewport to window dimensions.
    glViewport(0, 0, w, h);

	//Reset coordinate system.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    fAspect = (float)w/(float)h;
    gluPerspective(45.0, fAspect, 1.0, 1500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CGLWindow::ViewPattern(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

	glTranslatef(-4.0f, -1.9f, -4.0f);	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Sides[4]);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 8.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 0.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(8.0f, 0.0f, 0.0f);
	glEnd();
}
