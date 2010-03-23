// UIEditor.h : main header file for the UIEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CUIEditorApp:
// See UIEditor.cpp for the implementation of this class
//

#define UI_MAJOR_VERSION		"1"
#define UI_MINOR_VERSION		"0"

class CScriptFrame;
class CDisplayFrame;

class CUIEditorApp : public CWinApp
{
public:
	CUIEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
protected:
	HMENU m_hDisplayMenu;
	HMENU m_hScriptMenu;
	HACCEL m_hDisplayAccel;
	HACCEL m_hScriptAccel;
	CScriptFrame*	m_pScriptFrame;
	CDisplayFrame*	m_pDisplayFrame;

public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
};

extern CUIEditorApp theApp;