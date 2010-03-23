// LeftView.h : interface of the CLeftView class
//


#pragma once

class CFontEditorDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CFontEditorDoc* GetDocument();

// Operations
public:
	virtual void SetFont(CFont* pFont, BOOL bRedraw = 1);

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CFontEditorDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CFontEditorDoc*>(m_pDocument); }
#endif

