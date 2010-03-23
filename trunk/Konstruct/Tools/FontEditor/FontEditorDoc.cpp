// FontEditorDoc.cpp : implementation of the CFontEditorDoc class
//

#include "stdafx.h"
#include "FontEditor.h"

#include "FontEditorDoc.h"
#include ".\fonteditordoc.h"

#include "LeftView.h"
#include "FontEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontEditorDoc

IMPLEMENT_DYNCREATE(CFontEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CFontEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// CFontEditorDoc construction/destruction

CFontEditorDoc::CFontEditorDoc()
{
	m_pFont = 0;
}

CFontEditorDoc::~CFontEditorDoc()
{
	if( m_pFont )
		delete m_pFont;
}

BOOL CFontEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	POSITION pos = GetFirstViewPosition();
	m_pLeftView = (CLeftView*)GetNextView(pos);
	m_pRightView = (CFontEditorView*)GetNextView(pos);

	m_pLeftView->SetFont(m_pFont);

	return TRUE;
}




// CFontEditorDoc serialization

void CFontEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CFontEditorDoc diagnostics

#ifdef _DEBUG
void CFontEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFontEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFontEditorDoc commands

void CFontEditorDoc::OnFileNew()
{	
	CFontDialog dlg;
	if( dlg.DoModal() != IDCANCEL )
	{
		if( m_pFont )
			delete m_pFont;

		m_pFont = new CFont();
		LOGFONT logFont;
		dlg.GetCurrentFont(&logFont);
		m_pFont->CreateFontIndirect(&logFont);

		// Convert from true type to kpgFont
	}
}
