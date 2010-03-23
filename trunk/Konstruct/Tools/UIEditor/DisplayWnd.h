// DisplayWnd.h : interface of the CDisplayWnd class
//
#pragma once

#include "External/Tinyxml/tinyxml.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Graphics/kpgUIWindow.h"

class kpgRenderer;

// CDisplayWnd window

class CDisplayWnd : public CWnd
{
// Construction
public:
	CDisplayWnd();

// Attributes
public:
	void SaveLayout(TiXmlDocument* pDoc);
	void LoadLayout(TiXmlDocument* pDoc);

protected:
	kpgRenderer*				m_pRenderer;
	kpuLinkedList				m_lWindowList;

	CPoint						m_ptPreviousCursor;
	bool						m_bDrag;
	kpgUIWindow::eHitLocation	m_eHitLocation;
	kpgUIWindow::eHitLocation	m_eDragLocation;
	kpgUIWindow*				m_pHitWindow;
	kpgUIWindow*				m_pDragWindow;

	CPoint						m_ptContextPoint;

// Operations
private:
	void UpdateCursor(const kpgUIWindow* pWindow, const kpgUIWindow::eHitLocation eHitLocation);
	kpgUIWindow* HitTest(CPoint ptPoint, kpgUIWindow::eHitLocation& eHitLocation);
	void AddWindow(kpgUIWindow* pWindow);
	void Clear();

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CDisplayWnd();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnNewWindow();
	afx_msg void OnNewStaticText();
	afx_msg void OnNewTextInput();
	afx_msg void OnNewButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove( UINT nFlags, CPoint ptPoint );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint ptPoint );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint ptPoint );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint ptPoint );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

