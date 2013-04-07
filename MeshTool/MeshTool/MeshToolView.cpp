
// MeshToolView.cpp : implementation of the CMeshToolView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MeshTool.h"
#endif

#include "MeshToolDoc.h"
#include "MeshToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeshToolView

IMPLEMENT_DYNCREATE(CMeshToolView, CView)

BEGIN_MESSAGE_MAP(CMeshToolView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN_MODEL_FILE, &CMeshToolView::OnOpenModelFile)
END_MESSAGE_MAP()

// CMeshToolView construction/destruction

CMeshToolView::CMeshToolView()
	: /*m_nXRotation(10)
	, m_nYRotation(0)
	, m_bRotateScene(false)
	, */m_iOldMousePoint(0)
{
	// TODO: add construction code here
	G_pScene = new CGeoScene;
	G_vLight= Point3D(0.0,0.0,5.0);

	G_pArcBall = new CArcBall;
	G_pArcBall->InitBall();
}

CMeshToolView::~CMeshToolView()
{
	if (G_pScene)
		delete G_pScene;
	G_pScene = NULL;

	if (G_pArcBall)
		delete G_pArcBall;
	G_pArcBall = NULL;
}

BOOL CMeshToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//  改变窗口属性，使得窗口在重绘时不绘制被子窗口或其他
	//  窗口覆盖的部分；
	// OpenGL只对WS_CLIPCHILDREN|WS_CLIPSIBLINGS类型窗口有效
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 
	// or:  cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;

	return CView::PreCreateWindow(cs);
}

// CMeshToolView drawing

void CMeshToolView::OnDraw(CDC* /*pDC*/)
{
	CMeshToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	// 用户自定义函数，用于绘制三维场景
	DrawScene(); 
}


// CMeshToolView diagnostics

#ifdef _DEBUG
void CMeshToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMeshToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMeshToolDoc* CMeshToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshToolDoc)));
	return (CMeshToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMeshToolView message handlers


void CMeshToolView::DrawScene(void)
{
	glPushMatrix();
	initRenderingWindow(NULL,this);

	/*glRotatef(m_nXRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(m_nYRotation, 0.0f, 0.0f, 1.0f);*/
	glMultMatrixf(G_pArcBall->GetBallMatrix());

	G_pScene->DrawScene ();	

	glPopMatrix();

	SwapBuffers(m_hDC);

}


int CMeshToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	DWORD dwFlags = (PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Structure size,
		1,				// Structure version number
		dwFlags,			// Property flags
		PFD_TYPE_RGBA,			// RGBA mode
		24,				// 24-bit color
		0, 0, 0, 0, 0, 0,		// 8-bit each color
		0, 0, 0, 0, 0, 0, 0,		// No alpha or accum. buffer,
		32,				// 32-bit z-buffer
		0, 0,				// No stencil or aux buffer
		PFD_MAIN_PLANE,			// Mainn layer type
		0,				// Reserved
		0, 0, 0,			// Unsupported.
	};

	HWND  hwnd = GetSafeHwnd();
	m_hDC = ::GetDC(hwnd);

	int PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	BOOL success = SetPixelFormat(m_hDC, PixelFormat,&pfd);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	G_pArcBall->InitBall();
	G_pView=this;


	initRenderingWindow(NULL,this);
	return 0;
}


void CMeshToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hRC);	
	::ReleaseDC(GetSafeHwnd(),m_hDC);	

}


BOOL CMeshToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void CMeshToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	initRenderingWindow(NULL,this);

	DrawScene();

}


void CMeshToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	G_pArcBall->MouseDown(point,G_nWinWidth, G_nWinHeight);
	//m_bRotateScene = TRUE;
	G_theMouseButton = MOUSE_LEFT_BUTTON;
	m_iOldMousePoint = point;

	CView::OnLButtonDown(nFlags, point);
}


void CMeshToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//m_bRotateScene = FALSE;
	G_pArcBall->MouseUp(point, G_nWinWidth, G_nWinHeight);
	G_theMouseButton = NO_BUTTON;
	DrawScene();
	m_iOldMousePoint=point;

	CView::OnLButtonUp(nFlags, point);
}


void CMeshToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*if (m_bRotateScene) {
		m_nXRotation -= (float) (m_iOldMousePoint.x - point.x) / 3.0f;
		m_nYRotation -= (float) (m_iOldMousePoint.y - point.y) / 3.0f;
		m_iOldMousePoint = point;
		DrawScene();
	}*/


	if (G_theMouseButton == MOUSE_LEFT_BUTTON)
	{
		G_pArcBall->MouseMove(point, G_nWinWidth, G_nWinHeight);
		DrawScene();
	}
	else if (G_theMouseButton == MOUSE_RIGHT_BUTTON)
	{
		float scale = 2*(point.y - G_iOldPos.y)/(float)G_nWinWidth;
		G_fEyeDistScale = G_fEyeDistScale + scale;
		if( G_fEyeDistScale < .2f ) G_fEyeDistScale = 0.2f;
		if( G_fEyeDistScale > 5.f ) G_fEyeDistScale = 5.f;

		G_iOldPos = point;
		DrawScene();			
	}


	m_iOldMousePoint=point;

	CView::OnMouseMove(nFlags, point);
}


void CMeshToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	G_iOldPos = point;
	G_theMouseButton = MOUSE_RIGHT_BUTTON;

	m_iOldMousePoint=point;

	CView::OnRButtonDown(nFlags, point);
}


void CMeshToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	G_theMouseButton = NO_BUTTON;

	m_iOldMousePoint=point;

	CView::OnRButtonUp(nFlags, point);
}


void CMeshToolView::ResetCamera(void)
{
	G_fEyeDistScale = 1.0f;

	G_theMouseButton = NO_BUTTON;
	G_pArcBall->InitBall();
	initRenderingWindow(NULL,this);
	DrawScene();
}


void CMeshToolView::OnOpenModelFile()
{
	// TODO: Add your command handler code here
	TCHAR defExt[] = L".vm";
	TCHAR szFilter[] = L"VM file(*.vm)|*.vm|All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, defExt, NULL , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT
		
		, szFilter, NULL);

	if (dlg.DoModal()!=IDOK) return;

	CString sFileExt=dlg.GetFileExt ();
	sFileExt.MakeLower();

	if (sFileExt=="vm")
	{
		CString pathName_ = dlg.GetPathName();
		// Convert a TCHAR string to a LPCSTR
		CT2CA pszConvertedAnsiString (pathName_);

		// construct a std::string using the LPCSTR input
		//std::string pathName (pszConvertedAnsiString);
		G_sModelPath = pszConvertedAnsiString;
		G_pScene->LoadVMModel(G_sModelPath);
	}

	CDocument* pDoc=GetDocument();
	pDoc->UpdateAllViews (NULL);
}
