// FontEditorDoc.h : interface of the CFontEditorDoc class
//


#pragma once

class CLeftView;
class CFontEditorView;

class CFontEditorDoc : public CDocument
{
protected: // create from serialization only
	CFontEditorDoc();
	DECLARE_DYNCREATE(CFontEditorDoc)

// Attributes
public:
	CFont*	m_pFont;
	CLeftView*	m_pLeftView;
	CFontEditorView*	m_pRightView;

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CFontEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNew();
};


