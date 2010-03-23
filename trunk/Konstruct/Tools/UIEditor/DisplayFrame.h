// DisplayFrame.h : interface of the CDisplayFrame class
//


#pragma once

#include "DisplayWnd.h"

class CDisplayFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDisplayFrame)
public:
	CDisplayFrame();

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
	CDisplayWnd m_wndView;
	virtual ~CDisplayFrame();
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
