// FontEditor.h : main header file for the FontEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CFontEditorApp:
// See FontEditor.cpp for the implementation of this class
//

class CFontEditorApp : public CWinApp
{
public:
	CFontEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
};

extern CFontEditorApp theApp;