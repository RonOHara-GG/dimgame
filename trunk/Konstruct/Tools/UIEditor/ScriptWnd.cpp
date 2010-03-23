// ScriptWnd.cpp : implementation of the CScriptWnd class
//

#include "stdafx.h"
#include "UIEditor.h"
#include "ScriptWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScriptWnd

CScriptWnd::CScriptWnd() :
	m_szText("Text Window")
{
}

CScriptWnd::~CScriptWnd()
{
}


BEGIN_MESSAGE_MAP(CScriptWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CScriptWnd message handlers

BOOL CScriptWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CScriptWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r(10, 10, 400, 50);
	dc.DrawText(m_szText, r, DT_BOTTOM);
}

