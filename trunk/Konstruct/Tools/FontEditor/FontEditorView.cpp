// FontEditorView.cpp : implementation of the CFontEditorView class
//

#include "stdafx.h"
#include "FontEditor.h"

#include "FontEditorDoc.h"
#include "FontEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontEditorView

IMPLEMENT_DYNCREATE(CFontEditorView, CListView)

BEGIN_MESSAGE_MAP(CFontEditorView, CListView)
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

// CFontEditorView construction/destruction

CFontEditorView::CFontEditorView()
{
	// TODO: add construction code here

}

CFontEditorView::~CFontEditorView()
{
}

BOOL CFontEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CFontEditorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// CFontEditorView diagnostics

#ifdef _DEBUG
void CFontEditorView::AssertValid() const
{
	CListView::AssertValid();
}

void CFontEditorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CFontEditorDoc* CFontEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontEditorDoc)));
	return (CFontEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CFontEditorView message handlers
void CFontEditorView::OnStyleChanged(int /*nStyleType*/, LPSTYLESTRUCT /*lpStyleStruct*/)
{
	//TODO: add code to react to the user changing the view style of your window
	
	Default();
}
