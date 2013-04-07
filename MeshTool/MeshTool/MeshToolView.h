
// MeshToolView.h : interface of the CMeshToolView class
//

#pragma once

#include "MeshToolDoc.h"
#include "atltypes.h"

class CMeshToolView : public CView
{
protected: // create from serialization only
	CMeshToolView();
	DECLARE_DYNCREATE(CMeshToolView)

// Attributes
public:
	CMeshToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMeshToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	HDC m_hDC;
	HGLRC m_hRC;
public:
	void DrawScene(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
//	float m_nXRotation;
//	float m_nYRotation;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
//	bool m_bRotateScene;
	CPoint m_iOldMousePoint;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void ResetCamera(void);
	afx_msg void OnOpenModelFile();
};

#ifndef _DEBUG  // debug version in MeshToolView.cpp
inline CMeshToolDoc* CMeshToolView::GetDocument() const
   { return reinterpret_cast<CMeshToolDoc*>(m_pDocument); }
#endif

