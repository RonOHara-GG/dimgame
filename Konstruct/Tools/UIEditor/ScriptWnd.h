// ScriptWnd.h : interface of the CScriptWnd class
//


#pragma once


// CScriptWnd window

class CScriptWnd : public CWnd
{
// Construction
public:
	CScriptWnd();

// Attributes
public:

protected:
	CString	m_szText;

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CScriptWnd();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

