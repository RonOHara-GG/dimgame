// ScriptFrame.cpp : implementation of the CScriptFrame class
//
#include "stdafx.h"
#include "UIEditor.h"

#include "ScriptFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScriptFrame

IMPLEMENT_DYNCREATE(CScriptFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CScriptFrame, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CScriptFrame construction/destruction

CScriptFrame::CScriptFrame()
{
	// TODO: add member initialization code here
}

CScriptFrame::~CScriptFrame()
{
}


BOOL CScriptFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CScriptFrame diagnostics

#ifdef _DEBUG
void CScriptFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CScriptFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CScriptFrame message handlers
void CScriptFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	SendMessage(WM_CLOSE);
}

int CScriptFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

void CScriptFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CScriptFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
