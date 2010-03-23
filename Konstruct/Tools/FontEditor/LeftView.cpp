// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "FontEditor.h"

#include "FontEditorDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CFontEditorDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontEditorDoc)));
	return (CFontEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers

void CLeftView::SetFont(CFont* pFont, BOOL bRedraw)
{
	CTreeView::SetFont(pFont, bRedraw);
	GetTreeCtrl().SetFont(pFont, bRedraw);

	// Strip the tree
	GetTreeCtrl().DeleteAllItems();

	// Add all the characters to the tree
	char itemName[2];
	itemName[1] = 0;
	for( char c = 33; c <= 126; c++ )
	{		
		itemName[0] = c;
		GetTreeCtrl().InsertItem(itemName);
	}
}