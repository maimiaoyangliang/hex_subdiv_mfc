
// MeshTool.h : main header file for the MeshTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMeshToolApp:
// See MeshTool.cpp for the implementation of this class
//

class CMeshToolApp : public CWinApp
{
public:
	CMeshToolApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMeshToolApp theApp;
