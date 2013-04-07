// CmdForm.cpp : implementation file
//

#include "stdafx.h"
#include "MeshTool.h"
#include "CmdForm.h"
#include "MeshToolView.h"
#include "MainFrm.h"


// CCmdForm

IMPLEMENT_DYNCREATE(CCmdForm, CFormView)

CCmdForm::CCmdForm()
	: CFormView(CCmdForm::IDD)
	, m_nRenderingStyles(DRAW_POINT_SCENE)
{
	
}

CCmdForm::~CCmdForm()
{
}

void CCmdForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCmdForm, CFormView)
	ON_BN_CLICKED(IDC_DRAW_POINT_RADIO, &CCmdForm::OnDrawPointRadio)
	ON_BN_CLICKED(IDC_DRAW_LINE_RADIO, &CCmdForm::OnDrawLineRadio)
	ON_BN_CLICKED(IDC_DRAW_FACE_RADIO, &CCmdForm::OnDrawFaceRadio)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIVISION, &CCmdForm::OnSubdivision)
	ON_BN_CLICKED(IDC_BUTTON_SUBDIV_RESET, &CCmdForm::OnSubdivisionReset)
	ON_BN_CLICKED(IDC_CHECK_LIGHT_ON, &CCmdForm::OnCheckLightOn)
	ON_BN_CLICKED(IDC_CHECK_SHOW_COORDINATE_AXIS, &CCmdForm::OnCheckShowCoordinateAxis)
	ON_BN_CLICKED(IDC_CHECK_SHOW_BOUNDING_BOX, &CCmdForm::OnCheckShowBoundingBox)
	ON_BN_CLICKED(IDC_BUTTON_INTERPOLATESUB, &CCmdForm::OnButtonInterpolatesub)
	ON_BN_CLICKED(IDC_CHECK_SHOW_WIREFRAME, &CCmdForm::OnCheckShowWireframe)
END_MESSAGE_MAP()


// CCmdForm diagnostics

#ifdef _DEBUG
void CCmdForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCmdForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCmdForm message handlers






CMeshToolView* CCmdForm::GetViewWindow(void)
{
	CMeshToolApp *pApp = (CMeshToolApp *)AfxGetApp();
	CMainFrame *pMainFrame = (CMainFrame *)pApp->m_pMainWnd;
	CMeshToolView *pView = (CMeshToolView *)pMainFrame->m_wndSplitter.GetPane(0,1);
	return pView;
}



void CCmdForm::OnDrawPointRadio()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles |= DRAW_POINT_SCENE;

	if (m_nRenderingStyles & DRAW_LINE_SCENE)
		m_nRenderingStyles -= DRAW_LINE_SCENE;

	if (m_nRenderingStyles & DRAW_FACE_SCENE)
		m_nRenderingStyles -= DRAW_FACE_SCENE;

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnDrawLineRadio()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles |= DRAW_LINE_SCENE;

	if (m_nRenderingStyles & DRAW_POINT_SCENE)
		m_nRenderingStyles -= DRAW_POINT_SCENE;

	if (m_nRenderingStyles & DRAW_FACE_SCENE)
		m_nRenderingStyles -= DRAW_FACE_SCENE;

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnDrawFaceRadio()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles |= DRAW_FACE_SCENE;

	if (m_nRenderingStyles & DRAW_LINE_SCENE)
		m_nRenderingStyles -= DRAW_LINE_SCENE;

	if (m_nRenderingStyles & DRAW_POINT_SCENE)
		m_nRenderingStyles -= DRAW_POINT_SCENE;

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnSubdivision()
{
	// TODO: Add your control notification handler code here
	G_pScene->m_pHexahedron->ApproximateSubdivision();
	GetViewWindow()->DrawScene();

}


void CCmdForm::OnSubdivisionReset()
{
	// TODO: Add your control notification handler code here
	G_pScene->LoadVMModel(G_sModelPath);
	GetViewWindow()->ResetCamera();
}



void CCmdForm::OnCheckLightOn()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles ^= LIGHT_ON_WHEN_RENDERING;

	if (m_nRenderingStyles & LIGHT_ON_WHEN_RENDERING)
		((CButton*)GetDlgItem(IDC_CHECK_LIGHT_ON))->SetCheck(1);
	else	((CButton*)GetDlgItem(IDC_CHECK_LIGHT_ON))->SetCheck(0);

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnCheckShowCoordinateAxis()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles ^= SHOW_COORDINATE;

	if (m_nRenderingStyles & SHOW_COORDINATE)
		((CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATE_AXIS))->SetCheck(1);
	else	((CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATE_AXIS))->SetCheck(0);

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnCheckShowBoundingBox()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles ^= SHOW_BOUNDING_BOX;

	if (m_nRenderingStyles & SHOW_BOUNDING_BOX)
		((CButton*)GetDlgItem(IDC_CHECK_SHOW_BOUNDING_BOX))->SetCheck(1);
	else	((CButton*)GetDlgItem(IDC_CHECK_SHOW_BOUNDING_BOX))->SetCheck(0);

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnButtonInterpolatesub()
{
	// TODO: Add your control notification handler code here
	G_pScene->m_pHexahedron->InterpolateSubdivision();
	GetViewWindow()->DrawScene();
}


void CCmdForm::OnCheckShowWireframe()
{
	// TODO: Add your control notification handler code here
	m_nRenderingStyles ^= SHOW_WIREFRAME;

	if (m_nRenderingStyles & SHOW_WIREFRAME)
		((CButton*)GetDlgItem(IDC_CHECK_SHOW_WIREFRAME))->SetCheck(1);
	else	((CButton*)GetDlgItem(IDC_CHECK_SHOW_WIREFRAME))->SetCheck(0);

	G_pScene->SetRenderingStyles(m_nRenderingStyles);
	GetViewWindow()->DrawScene();
}
