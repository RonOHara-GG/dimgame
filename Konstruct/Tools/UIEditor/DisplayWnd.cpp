// DisplayWnd.cpp : implementation of the CDisplayWnd class
//

#include "stdafx.h"
#include "UIEditor.h"
#include "DisplayWnd.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgUIWindow.h"
#include "Common/Graphics/kpgUIText.h"
#include "Common/Graphics/kpgUITextInput.h"
#include "Common/Graphics/kpgUIButton.h"
#include "Common/Graphics/kpgTexture.h"
#include ".\displaywnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDisplayWnd

CDisplayWnd::CDisplayWnd()
{
	m_pRenderer = 0;
	m_bDrag = false;

	m_ptContextPoint.SetPoint(-1, -1);
}

CDisplayWnd::~CDisplayWnd()
{
	if( m_pRenderer )
		delete m_pRenderer;

	Clear();
}

void CDisplayWnd::Clear()
{
	kpuLinkedList* pIter = m_lWindowList.Next();
	while( pIter )
	{
		kpgUIWindow* pWindow = static_cast<kpgUIWindow*>(pIter->GetPointer());
		if( pWindow )
		{
			delete pWindow;
		}

		delete pIter;

		pIter = m_lWindowList.Next();
	}
	
}


BEGIN_MESSAGE_MAP(CDisplayWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_DISPLAY_NEWWINDOW, OnNewWindow)
	ON_COMMAND(ID_NEW_STATIC_TEXT, OnNewStaticText)
	ON_COMMAND(ID_NEW_TEXT_INPUT, OnNewTextInput)
	ON_COMMAND(ID_NEW_BUTTON, OnNewButton)
END_MESSAGE_MAP()



// CDisplayWnd message handlers

BOOL CDisplayWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CDisplayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(1, 30, 0);

	return 0;
}

void CDisplayWnd::OnTimer(UINT_PTR nIDEvent)
{
	PostMessage(WM_PAINT);
	CWnd::OnTimer(nIDEvent);
}

void CDisplayWnd::OnPaint() 
{
	KillTimer(1);
	CPaintDC dc(this); // device context for painting

	if( !m_pRenderer )
	{
		m_pRenderer = kpgRenderer::GetInstance();
		m_pRenderer->Create(GetSafeHwnd());
		kpuMatrix mProj, mView;
		mProj.Orthographic(m_pRenderer->GetScreenWidth(), m_pRenderer->GetScreenHeight(), 0.01f, 500.0f);
		mView.LookAt(kpuVector(0.0f, 0.0f, -1.0f, 0.0f), kpuVector(0.0f, 0.0f, 0.0f, 0.0f), kpuVector(0.0f, 1.0f, 0.0f, 0.0f));

		m_pRenderer->SetProjectionMatrix(mProj);
		m_pRenderer->SetViewMatrix(mView);
	}

	kpuMatrix mWorld;
	mWorld.Identity();
	m_pRenderer->SetWorldMatrix(mWorld);

	kpRect rect;
	rect.m_fLeft = 0;
	rect.m_fRight = m_pRenderer->GetScreenWidth();
	rect.m_fTop = 0;
	rect.m_fBottom = m_pRenderer->GetScreenHeight();
	
	m_pRenderer->BeginFrame();

	// Draw al the UI Windows
	kpuLinkedList* pIter = m_lWindowList.Next();
	while( pIter )
	{
		kpgUIWindow* pWindow = (kpgUIWindow*)pIter->GetPointer();
		pWindow->Update();
		pWindow->Draw(m_pRenderer, rect);
		
		pIter = pIter->Next();
	}

	m_pRenderer->EndFrame();
	
	SetTimer(1, 30, 0);
}

void CDisplayWnd::OnMouseMove(UINT nFlags, CPoint ptPoint)
{
	m_pHitWindow = HitTest(ptPoint, m_eHitLocation);

	if( m_bDrag )
	{
		CPoint ptDelta = ptPoint - m_ptPreviousCursor;

		kpRect rect;
		rect.m_fLeft = 0;
		rect.m_fRight = m_pRenderer->GetScreenWidth();
		rect.m_fTop = 0;
		rect.m_fBottom = m_pRenderer->GetScreenHeight();

		float fX1 = (float)m_ptPreviousCursor.x;
		float fY1 = (float)m_ptPreviousCursor.y;
		float fX2 = (float)ptPoint.x;
		float fY2 = (float)ptPoint.y;
		m_pDragWindow->TransformPoint(fX1, fY1, m_pRenderer);
		m_pDragWindow->TransformPoint(fX2, fY2, m_pRenderer);

		float fXDelta = fX2 - fX1;
		float fYDelta = fY2 - fY1;

		float fX = (float)ptDelta.x;
		float fY = (float)ptDelta.y;
		m_pDragWindow->TransformPoint(fX, fY, m_pRenderer);
		fX = fXDelta;
		fY = fYDelta;
		switch( m_eDragLocation )
		{
			case kpgUIWindow::eHL_LeftBorder:
				m_pDragWindow->GrowLeft(fX);
				break;
			case kpgUIWindow::eHL_RightBorder:							
				m_pDragWindow->GrowRight(fX);
				break;
			case kpgUIWindow::eHL_TopBorder:
				m_pDragWindow->GrowTop(fY);
				break;
			case kpgUIWindow::eHL_BottomBorder:
				m_pDragWindow->GrowBottom(fY);
				break;
			case kpgUIWindow::eHL_TopLeftCorner:
				m_pDragWindow->GrowLeft(fX);
				m_pDragWindow->GrowTop(fY);
				break;
			case kpgUIWindow::eHL_BottomRightCorner:
				m_pDragWindow->GrowRight(fX);
				m_pDragWindow->GrowBottom(fY);
				break;
			case kpgUIWindow::eHL_TopRightCorner:
				m_pDragWindow->GrowRight(fX);
				m_pDragWindow->GrowTop(fY);
				break;
			case kpgUIWindow::eHL_BottomLeftCorner:
				m_pDragWindow->GrowLeft(fX);
				m_pDragWindow->GrowBottom(fY);
				break;
			case kpgUIWindow::eHL_Background:
				m_pDragWindow->Move(fX, fY);
				break;
			default:
				break;
		}
	}
	else
	{
		UpdateCursor(m_pHitWindow, m_eHitLocation);
	}

	m_ptPreviousCursor = ptPoint;
}

void CDisplayWnd::AddWindow(kpgUIWindow* pWindow)
{
	if( m_ptContextPoint.x >= 0 )
	{
		// New Window from the context menu.
		ScreenToClient(&m_ptContextPoint);
		kpgUIWindow::eHitLocation eHitLocation;
		kpgUIWindow* pParent = HitTest(m_ptContextPoint, eHitLocation);
		if( pParent )
		{
			// We hit a window, create a child
			pParent->AddChildWindow(pWindow, (float)m_ptContextPoint.x, (float)m_ptContextPoint.y, m_pRenderer);
			return;
		}
		else
		{
			// Create a new top level window at the click location
			float fX = (float)m_ptContextPoint.x / m_pRenderer->GetScreenWidth();
			float fY = (float)m_ptContextPoint.y / m_pRenderer->GetScreenHeight();
			pWindow->SetPosition(fX, fY);
		}
	}

	m_lWindowList.AddTail(pWindow);
}

void CDisplayWnd::OnNewWindow()
{
	kpgUIWindow* pNewWindow = new kpgUIWindow();
	pNewWindow->LoadDefaults();

	AddWindow(pNewWindow);
}

void CDisplayWnd::OnNewStaticText()
{
	kpgUIText* pText = new kpgUIText();
	pText->SetText("Static Text");

	AddWindow(pText);
}

void CDisplayWnd::OnNewTextInput()
{
	kpgUITextInput* pInput = new kpgUITextInput();
	pInput->LoadDefaults();
	pInput->SetText("Input");

	AddWindow(pInput);
}

void CDisplayWnd::OnNewButton()
{
	kpgUIButton* pButton = new kpgUIButton();
	pButton->LoadDefaults();
	pButton->SetText("Button");

	AddWindow(pButton);
}

void CDisplayWnd::OnLButtonDown(UINT nFlags, CPoint ptPoint)
{
	m_bDrag = true;
	m_pDragWindow = HitTest(ptPoint, m_eDragLocation);

	UpdateCursor(m_pDragWindow, m_eDragLocation);
}

void CDisplayWnd::OnLButtonUp(UINT nFlags, CPoint ptPoint)
{
	m_bDrag = false;
	m_pDragWindow = 0;
}

void CDisplayWnd::OnContextMenu( CWnd* pWnd, CPoint ptPoint )
{
	HMENU hContext = LoadMenu(NULL, MAKEINTRESOURCE(IDR_CONTEXT));
	HMENU hMenu = GetSubMenu(hContext, 0);
	TrackPopupMenu(hMenu, 0, ptPoint.x, ptPoint.y, 0, GetSafeHwnd(), 0);
	DestroyMenu(hContext);

	m_ptContextPoint = ptPoint;
}

void CDisplayWnd::UpdateCursor(const kpgUIWindow* pWindow, const kpgUIWindow::eHitLocation eHitLocation)
{
	if( pWindow )
	{
		// Mouse is over this window, use the appropriate cursor
		switch( eHitLocation )
		{
			case kpgUIWindow::eHL_LeftBorder:
			case kpgUIWindow::eHL_RightBorder:
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));						
				break;
			case kpgUIWindow::eHL_TopBorder:
			case kpgUIWindow::eHL_BottomBorder:
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				break;
			case kpgUIWindow::eHL_TopLeftCorner:
			case kpgUIWindow::eHL_BottomRightCorner:
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
				break;
			case kpgUIWindow::eHL_TopRightCorner:
			case kpgUIWindow::eHL_BottomLeftCorner:
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
				break;
			case kpgUIWindow::eHL_Background:
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				break;
			default:
				//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				break;
		}
	}
}

kpgUIWindow* CDisplayWnd::HitTest(CPoint ptPoint, kpgUIWindow::eHitLocation& eHitLocation)
{
	if( m_pRenderer )
	{
		kpRect rect;
		rect.m_fLeft = 0;
		rect.m_fRight = m_pRenderer->GetScreenWidth();
		rect.m_fTop = 0;
		rect.m_fBottom = m_pRenderer->GetScreenHeight();

		kpuLinkedList* pIter = m_lWindowList.Last();
		while( pIter && pIter != &m_lWindowList )
		{
			kpgUIWindow* pWindow = (kpgUIWindow*)pIter->GetPointer();
			
			kpgUIWindow::eHitLocation hitLocation;
			kpgUIWindow* pHit = pWindow->HitTest((float)ptPoint.x, (float)ptPoint.y, rect, &hitLocation);
			if( pHit )
			{
				eHitLocation = hitLocation;
				return pHit;
			}

			pIter = pIter->Next();
		}
	}
	return 0;
}

void CDisplayWnd::SaveLayout(TiXmlDocument* pDoc)
{
	// Store all the windows, they will save their own children
	kpuLinkedList* pIter = m_lWindowList.Next();
	while( pIter )
	{
		// Save this window
		kpgUIWindow* pWindow = static_cast<kpgUIWindow*>(pIter->GetPointer());
		if( pWindow )
		{
			pWindow->Save(pDoc);
		}

		// Move on to the next window
		pIter = pIter->Next();
	}
}

void CDisplayWnd::LoadLayout(TiXmlDocument* pDoc)
{
	Clear();

	for( TiXmlNode* pChild = pDoc->FirstChild(); pChild; pChild = pChild->NextSibling() )
	{
		switch( pChild->Type() )
		{
			case TiXmlNode::ELEMENT:
				{
					kpgUIWindow* pWindow = kpgUIWindow::Load(pChild);
					if( pWindow )
					{
						AddWindow(pWindow);
					}
				}
				break;
			case TiXmlNode::DECLARATION:
				break;
			default:
				ASSERT(0);
				break;
		}
	}
}