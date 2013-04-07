#pragma once


class CMeshToolView;
// CCmdForm form view

class CCmdForm : public CFormView
{
	DECLARE_DYNCREATE(CCmdForm)

protected:
	CCmdForm();           // protected constructor used by dynamic creation
	virtual ~CCmdForm();

public:
	enum { IDD = IDD_CMDFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

protected:
	int m_nRenderingStyles;
private:
	CMeshToolView* GetViewWindow(void);
public:
	afx_msg void OnDrawPointRadio();
	afx_msg void OnDrawLineRadio();
	afx_msg void OnDrawFaceRadio();
	afx_msg void OnSubdivision();
	afx_msg void OnSubdivisionReset();
	afx_msg void OnCheckLightOn();
	afx_msg void OnCheckShowCoordinateAxis();
	afx_msg void OnCheckShowBoundingBox();
	afx_msg void OnButtonInterpolatesub();
	afx_msg void OnCheckShowWireframe();
};


