// ScriptFrame.h : interface of the CScriptFrame class
//


#pragma once

#include "ScriptWnd.h"

class CScriptFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CScriptFrame)
public:
	CScriptFrame();

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	// view for the client area of the frame.
	CScriptWnd m_wndView;
	virtual ~CScriptFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
