// FontEditorView.h : interface of the CFontEditorView class
//


#pragma once


class CFontEditorView : public CListView
{
protected: // create from serialization only
	CFontEditorView();
	DECLARE_DYNCREATE(CFontEditorView)

// Attributes
public:
	CFontEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CFontEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FontEditorView.cpp
inline CFontEditorDoc* CFontEditorView::GetDocument() const
   { return reinterpret_cast<CFontEditorDoc*>(m_pDocument); }
#endif

