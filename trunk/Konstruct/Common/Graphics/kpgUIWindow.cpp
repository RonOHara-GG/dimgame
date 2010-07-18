#include "StdAfx.h"
#include "Common/Graphics/kpgUIWindow.h"
#include "Common/Graphics/kpgUIText.h"
#include "Common/Graphics/kpgUITextInput.h"
#include "Common/Graphics/kpgUIButton.h"
#include "Common/Graphics/kpgUIList.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgTexture.h"
#include "Common/Graphics/kpgUIManager.h"
#include "common/input/kpiInputManager.h"
#include "Common/Utility/kpuPhysicalObject.h"

const float kMinimumWindowWidth = 0.05f;
const float kMinimumWindowHeight = 0.05f;

kpgUIWindow::kpgUIWindow(kpgUIManager* pManager)
{
	m_bHasFrame = false;
	m_pBackground = 0;
	m_pBorder = 0;
	m_pCorner = 0;
	m_pParent = 0;
	m_szName = 0;
	m_pDataSource = 0;
	m_pData = 0;
	m_pContextObj = 0;

	m_eType = eWT_GenericWindow;
	m_pUIManager = pManager;

}

kpgUIWindow::~kpgUIWindow(void)
{
	Destroy();
}

void kpgUIWindow::Destroy()
{
	if( m_pBackground )
		delete m_pBackground;
	if( m_pBorder )
		delete m_pBorder;
	if( m_pCorner )
		delete m_pCorner;

	while( m_lChildren.Next() )
	{
		kpuLinkedList* pIter = m_lChildren.Next();
		kpgUIWindow* pChild = (kpgUIWindow*)pIter->GetPointer();
		delete pChild;
		delete pIter;
	}

	m_pParent = 0;
}

kpgUIWindow* kpgUIWindow::Load(TiXmlNode* pNode, kpgUIManager* pManager)
{
	kpgUIWindow* pWindow = 0;
	TiXmlElement* pElement = static_cast<TiXmlElement*>(pNode);
	if( pElement )
	{
		const char* pType = pElement->Attribute("Type");
		if( pType )
		{
			int nType = atoi(pType);
			switch( nType )
			{
				case eWT_GenericWindow:
					pWindow = new kpgUIWindow(pManager);
					break;
				case eWT_Text:
					pWindow = new kpgUIText(pManager);
					break;
				case eWT_TextInput:
					pWindow = new kpgUITextInput(pManager);
					break;
				case eWT_Button:
					pWindow = new kpgUIButton(pManager);
					break;
				case eWT_List:
					pWindow = new kpgUIList(pManager);
					break;
				default:
					assert(0);
					break;
			}
			if( pWindow )
			{
				pWindow->Load(pElement);
			}
		}
	}

	return pWindow;
}

TiXmlElement* kpgUIWindow::Save(TiXmlNode* pParent)
{
	// Create the xml element
	TiXmlElement* pElement = new TiXmlElement("kpgUIWindow");
	pParent->LinkEndChild(pElement);

	// Save our members
	pElement->SetAttribute("Type", m_eType);
	if( m_szName )
		pElement->SetAttribute("Name", m_szName);
	if( m_pBackground )
		pElement->SetAttribute("Background", m_pBackground->GetFileName());
	if( m_pBorder )
		pElement->SetAttribute("Border", m_pBorder->GetFileName());
	if( m_pCorner )
		pElement->SetAttribute("Corner", m_pBorder->GetFileName());
	pElement->SetAttribute("Orientation", m_eOrientation);
	pElement->SetDoubleAttribute("PositionX", m_fPosition[0]);
	pElement->SetDoubleAttribute("PositionY", m_fPosition[1]);
	pElement->SetDoubleAttribute("Width", m_fWidth);
	pElement->SetDoubleAttribute("Height", m_fHeight);
	pElement->SetAttribute("Visible", m_bVisible ? 1 : 0);
	pElement->SetAttribute("HasFrame", m_bHasFrame ? 1 : 0);

	TiXmlElement* pRect = new TiXmlElement("Rect");
	pRect->SetDoubleAttribute("Left", m_rRect.m_fLeft);
	pRect->SetDoubleAttribute("Right", m_rRect.m_fRight);
	pRect->SetDoubleAttribute("Top", m_rRect.m_fTop);
	pRect->SetDoubleAttribute("Bottom", m_rRect.m_fBottom);
	pElement->LinkEndChild(pRect);

	// Save any children
	kpuLinkedList* pIter = m_lChildren.Next();
	while( pIter )
	{
		kpgUIWindow* pChild = static_cast<kpgUIWindow*>(pIter->GetPointer());
		if( pChild )
		{
			pChild->Save(pElement);
		}

		pIter = pIter->Next();
	}

	return pElement;
}

void kpgUIWindow::Load(TiXmlElement* pElement)
{
	Destroy();

	if( pElement )
	{
		const char* pName = pElement->Attribute("Name");
		if( pName )
		{
			m_szName = 
			_strdup(pName);

			m_uHash = StringHash(m_szName);
		}

		const char* pBackground = pElement->Attribute("Background");
		if( pBackground )
		{
			m_pBackground = new kpgTexture();
			m_pBackground->Load(pBackground);
		}

		const char* pBorder = pElement->Attribute("Border");
		if( pBorder )
		{
			m_pBorder = new kpgTexture();
			m_pBorder->Load(pBorder);
		}
		else
			m_pBorder = 0;

		const char* pCorner = pElement->Attribute("Corner");
		if( pCorner )
		{
			m_pCorner = new kpgTexture();
			m_pCorner->Load(pCorner);
		}
		else
			m_pCorner = 0;

		const char* pOrientation = pElement->Attribute("Orientation");
		if( pOrientation )
			m_eOrientation = (eWindowOrientation)atoi(pOrientation);

		const char* pPositionX = pElement->Attribute("PositionX");
		if( pPositionX )
			m_fPosition[0] = (float)atof(pPositionX);

		const char* pPositionY = pElement->Attribute("PositionY");
		if( pPositionY )
			m_fPosition[1] = (float)atof(pPositionY);

		const char* pWidth = pElement->Attribute("Width");
		if( pWidth )
			m_fWidth = (float)atof(pWidth);

		const char* pHeight = pElement->Attribute("Height");
		if( pHeight )
			m_fHeight = (float)atof(pHeight);

		const char* pVisible = pElement->Attribute("Visible");
		if( pVisible )
		{
			int nVisible = atoi(pVisible);
			m_bVisible = (nVisible != 0);
		}

		const char* pHasFrame = pElement->Attribute("HasFrame");
		if( pHasFrame )
		{
			int nHasFrame = atoi(pHasFrame);
			m_bHasFrame = (nHasFrame != 0);
		}

		//Get window input events
		const char* pClickEvent = pElement->Attribute("ClickEvent");
		if( pClickEvent )
			m_uClickEvent = StringHash(pClickEvent);

		const char* pTargetWindow = pElement->Attribute("TargetWindow");
		if( pTargetWindow )
			m_uTargetHash = StringHash(pTargetWindow);

		const char* pEnterEvent = pElement->Attribute("MouseEnterEvent");
		if( pEnterEvent )
			m_uEnterEvent = StringHash(pEnterEvent);

		const char* pShowWindow = pElement->Attribute("Open");
		if( pShowWindow )
			m_uShowTarget = StringHash(pShowWindow);

		const char* pExitEvent = pElement->Attribute("MouseExitEvent");
		if( pExitEvent )
			m_uExitEvent = StringHash(pExitEvent);

		const char* pCloseWindow = pElement->Attribute("Close");
		if( pCloseWindow )
			m_uCloseTarget = StringHash(pCloseWindow);

		const char* pDataSource = pElement->Attribute("DataSource");
		if( pDataSource )		
			m_pDataSource = _strdup(pDataSource);		

		// Get the rectangle
		for( TiXmlElement* pChild = pElement->FirstChildElement(); pChild; pChild = pChild->NextSiblingElement())
		{
			if( !strcmp(pChild->Value(), "Rect") )
			{
				m_rRect.m_fLeft = (float)atof(pChild->Attribute("Left"));
				m_rRect.m_fRight = (float)atof(pChild->Attribute("Right"));
				m_rRect.m_fTop = (float)atof(pChild->Attribute("Top"));
				m_rRect.m_fBottom = (float)atof(pChild->Attribute("Bottom"));
			}
			else
			{	// Child Window
				kpgUIWindow* pWindow = kpgUIWindow::Load((TiXmlNode*)pChild, m_pUIManager);
				if( pWindow )
				{
					AddChildWindow(pWindow, kpgRenderer::GetInstance());
				}
			}
		}

			
	}
}

kpgUIWindow* kpgUIWindow::GetChild(u32 uHash)
{
	//Check this window
	if( m_uHash == uHash )
		return this;

	//check the kids
	kpuLinkedList* pIter = m_lChildren.Next();
	while( pIter )
	{
		kpgUIWindow* pChild = (kpgUIWindow*)pIter->GetPointer();		

		kpgUIWindow* pNextChild = pChild->GetChild(uHash);

		if( pNextChild )
			return pNextChild;

		pIter = pIter->Next();
	}

	return 0;
}

void kpgUIWindow::GetDataSource()
{
	if( m_pDataSource) 
		m_pData = *m_pUIManager->GetDataSource(m_pDataSource);
}


void kpgUIWindow::LoadDefaults()
{
	Destroy();

	m_pBackground = new kpgTexture();
	m_pBackground->Load("ScreenLayouts\\System\\Images\\DefaultBackground.dds");
	
	m_pBorder = new kpgTexture();
	m_pBorder->Load("ScreenLayouts\\System\\Images\\DefaultBorder.dds");

	m_pCorner = new kpgTexture();
	m_pCorner->Load("ScreenLayouts\\System\\Images\\DefaultCorner.dds");

	m_eOrientation = eWO_Center;
	m_fPosition[0] = 0.25f;
	m_fPosition[1] = 0.25f;
	m_fWidth = 0.5f;
	m_fHeight = 0.5f;
	m_bVisible = true;
	m_bHasFrame = true;
}

void kpgUIWindow::AddChildWindow(kpgUIWindow* pChild, const kpgRenderer* pRenderer)
{
	// Add the child to the children list
	m_lChildren.AddTail(pChild);

	// Set the parent pointer in the child to this window
	pChild->m_pParent = this;

}

void kpgUIWindow::Update()
{
	// Update the children
	kpuLinkedList* pIter = m_lChildren.Next();
	while( pIter )
	{
		kpgUIWindow* pChild = (kpgUIWindow*)pIter->GetPointer();
		pChild->Update();
		pIter = pIter->Next();
	}
}

void kpgUIWindow::Draw(kpgRenderer* pRenderer, const kpRect& rParent)
{
	if( m_bVisible )
	{		
		// Calculate the rectangle
		CalculateRectangle(rParent);

		// Draw the frame
		if( m_bHasFrame )
			DrawFrame(pRenderer, m_rRect);

		// Draw the children
		kpuLinkedList* pIter = m_lChildren.Next();
		while( pIter )
		{
			kpgUIWindow* pChild = (kpgUIWindow*)pIter->GetPointer();
			pChild->Draw(pRenderer, m_rRect);
			pIter = pIter->Next();
		}
	}
}

void kpgUIWindow::DrawFrame(kpgRenderer* pRenderer, const kpRect& rRect)
{
	// Draw the background
	kpRect rTemp = rRect;
	rTemp.Shrink(2);
	pRenderer->DrawQuad2D(rTemp, m_pBackground);

	kpgQuadUVs sUVs;
	if( m_pBorder )
	{
		// Draw the left border		
		rTemp.m_fLeft = rRect.m_fLeft;
		rTemp.m_fTop = rRect.m_fTop + m_pCorner->GetHeight();
		rTemp.m_fBottom = rRect.m_fBottom - m_pCorner->GetHeight();
		rTemp.m_fRight = rRect.m_fLeft + m_pBorder->GetHeight();
		sUVs = kpgQuadUVs(1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		pRenderer->DrawQuad2D(rTemp, m_pBorder, &sUVs);

		// Draw the right border
		rTemp.m_fRight = rRect.m_fRight;
		rTemp.m_fLeft = rRect.m_fRight - m_pBorder->GetHeight();
		sUVs = kpgQuadUVs(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);
		pRenderer->DrawQuad2D(rTemp, m_pBorder, &sUVs);

		// Draw the top border
		rTemp.m_fLeft = (rRect.m_fLeft + m_pCorner->GetWidth());
		rTemp.m_fRight = (rRect.m_fRight - m_pCorner->GetWidth());
		rTemp.m_fTop = rRect.m_fTop;
		rTemp.m_fBottom = rRect.m_fTop + m_pBorder->GetHeight();
		pRenderer->DrawQuad2D(rTemp, m_pBorder);

		// Draw the bottom border
		rTemp.m_fBottom = rRect.m_fBottom;
		rTemp.m_fTop = rRect.m_fBottom - m_pBorder->GetHeight();
		sUVs = kpgQuadUVs(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		pRenderer->DrawQuad2D(rTemp, m_pBorder, &sUVs);
	}

	if( m_pCorner )
	{
		// Draw the top left corner
		rTemp.m_fTop = (rRect.m_fTop);
		rTemp.m_fLeft = (rRect.m_fLeft);
		rTemp.m_fRight = (rRect.m_fLeft + m_pCorner->GetWidth());
		rTemp.m_fBottom = (rRect.m_fTop + m_pCorner->GetHeight());
		pRenderer->DrawQuad2D(rTemp, m_pCorner);

		// Draw the top right corner
		rTemp.m_fRight = (rRect.m_fRight);
		rTemp.m_fLeft = (rRect.m_fRight - m_pCorner->GetHeight());
		rTemp.m_fBottom = rRect.m_fTop + m_pCorner->GetWidth();
		sUVs = kpgQuadUVs(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);
		pRenderer->DrawQuad2D(rTemp, m_pCorner, &sUVs);

		// Draw the bottom left corner
		rTemp.m_fLeft = (rRect.m_fLeft);
		rTemp.m_fRight = (rRect.m_fLeft + m_pCorner->GetHeight());
		rTemp.m_fBottom = (rRect.m_fBottom);
		rTemp.m_fTop = (rTemp.m_fBottom - m_pCorner->GetWidth());
		sUVs = kpgQuadUVs(1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		pRenderer->DrawQuad2D(rTemp, m_pCorner, &sUVs);

		// Draw the bottom right corner
		rTemp.m_fRight = rRect.m_fRight;
		rTemp.m_fLeft = rRect.m_fRight - m_pCorner->GetWidth();
		rTemp.m_fTop = rRect.m_fBottom - m_pCorner->GetHeight();
		sUVs = kpgQuadUVs(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		pRenderer->DrawQuad2D(rTemp, m_pCorner, &sUVs);
	}
}

void kpgUIWindow::CalculateRectangle(const kpRect& rParent)
{
	float fParentWidth = rParent.m_fRight - rParent.m_fLeft;
	float fParentHeight = rParent.m_fBottom - rParent.m_fTop;

	switch( m_eOrientation )
	{
		case eWO_TopLeft:
			m_rRect.m_fLeft = rParent.m_fLeft + fParentWidth * m_fPosition[0];
			m_rRect.m_fTop = rParent.m_fTop + fParentHeight * m_fPosition[1];
			m_rRect.m_fRight = m_rRect.m_fLeft + (fParentWidth * m_fWidth);
			m_rRect.m_fBottom = m_rRect.m_fTop + (fParentHeight * m_fHeight);
			break;
		case eWO_Center:
			{
				float fOffsetW = fParentWidth * m_fWidth * 0.5f;
				float fOffsetH = fParentHeight * m_fHeight * 0.5f;
				m_rRect.m_fLeft = rParent.m_fLeft + ((fParentWidth * m_fPosition[0]) - fOffsetW);
				m_rRect.m_fRight = rParent.m_fLeft + ((fParentWidth * m_fPosition[0]) + fOffsetW);
				m_rRect.m_fTop = rParent.m_fTop + ((fParentHeight * m_fPosition[1]) - fOffsetH);
				m_rRect.m_fBottom = rParent.m_fTop + ((fParentHeight * m_fPosition[1]) + fOffsetH);
			}
			break;
		default:
			break;
	}
	m_rRect.m_fLeft = floorf(m_rRect.m_fLeft);
	m_rRect.m_fRight = floorf(m_rRect.m_fRight);
	m_rRect.m_fTop = floorf(m_rRect.m_fTop);
	m_rRect.m_fBottom = floorf(m_rRect.m_fBottom);
}

kpgUIWindow* kpgUIWindow::HitTest(float fX, float fY, const kpRect& rParent, eHitLocation* pOutLocation)
{
	if( !m_bVisible )
		return 0;

	kpgUIWindow* pHitWindow = 0;
	eHitLocation eHl = eHL_Outside;

	// Make sure we have an accurate rectangle
	CalculateRectangle(rParent);

	// Check to see if the coordinates are inside this window
	if( m_rRect.PointInside(fX, fY) )
	{
		// Point is inside

		// Check Corners
		kpRect rTemp;
		if( m_pCorner )
		{
			rTemp = m_rRect;
			rTemp.m_fRight = rTemp.m_fLeft + m_pCorner->GetWidth();
			rTemp.m_fBottom = rTemp.m_fTop + m_pCorner->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Top Left
				eHl = eHL_TopLeftCorner;
				pHitWindow = this;
			}
			rTemp.m_fRight = m_rRect.m_fRight;
			rTemp.m_fLeft = rTemp.m_fRight - m_pCorner->GetHeight();
			rTemp.m_fBottom = rTemp.m_fTop + m_pCorner->GetWidth();
			if( rTemp.PointInside(fX, fY) )
			{
				// Top Right
				eHl = eHL_TopRightCorner;
				pHitWindow = this;
			}
			rTemp = m_rRect;
			rTemp.m_fRight = rTemp.m_fLeft + m_pBorder->GetHeight();
			rTemp.m_fTop = rTemp.m_fBottom - m_pBorder->GetWidth();
			if( rTemp.PointInside(fX, fY) )
			{
				// Bottom Left
				eHl = eHL_BottomLeftCorner;
				pHitWindow = this;
			}
			rTemp = m_rRect;
			rTemp.m_fLeft = rTemp.m_fRight - m_pBorder->GetWidth();
			rTemp.m_fTop = rTemp.m_fBottom - m_pBorder->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Bottom Right
				eHl = eHL_BottomRightCorner;
				pHitWindow = this;
			}
		}
		
		// Check the borders
		if( eHl == eHL_Outside && m_pBorder )
		{
			rTemp = m_rRect;
			rTemp.m_fRight = m_rRect.m_fLeft + m_pBorder->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Left Border
				eHl = eHL_LeftBorder;
				pHitWindow = this;
			}
			rTemp.m_fRight = m_rRect.m_fRight;
			rTemp.m_fLeft = m_rRect.m_fRight - m_pBorder->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Right Border
				eHl = eHL_RightBorder;
				pHitWindow = this;
			}
			rTemp = m_rRect;
			rTemp.m_fBottom = rTemp.m_fTop + m_pBorder->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Top Window
				eHl = eHL_TopBorder;
				pHitWindow = this;
			}
			rTemp.m_fBottom = m_rRect.m_fBottom;
			rTemp.m_fTop = m_rRect.m_fBottom - m_pBorder->GetHeight();
			if( rTemp.PointInside(fX, fY) )
			{
				// Bottom Border
				eHl = eHL_BottomBorder;
				pHitWindow = this;
			}
		}

		// Check the Children
		if( eHl == eHL_Outside )
		{
			kpuLinkedList* pIter = m_lChildren.Next();
			while( pIter )
			{
				kpgUIWindow* pChild = (kpgUIWindow*)pIter->GetPointer();
				pHitWindow = pChild->HitTest(fX, fY, m_rRect, &eHl);
				if( eHl != eHL_Outside )
					break;

				pIter = pIter->Next();
			}
		}

		if( eHl == eHL_Outside )
		{
			// Must have hit the background
			eHl = eHL_Background;
			pHitWindow = this;
		}
	}

	if( pOutLocation )
		*pOutLocation = eHl;

	return pHitWindow;
}

void kpgUIWindow::TransformPoint(float& fX, float& fY, const kpgRenderer* pRenderer)
{
	if( m_pParent )
		m_pParent->TransformPoint(fX, fY, pRenderer);
	else
	{
		// Top level window, need to get the rectangle from the renderer
		
		// Transform into homogeneous space
		fX = fX / pRenderer->GetScreenWidth();
		fY = fY / pRenderer->GetScreenHeight();
	}

	// Offset by the current window
	float fLeft = GetLeft();
	float fTop = GetTop();
	
	fX -= fLeft;
	fY -= fTop;
}

const kpRect& kpgUIWindow::GetRect(const kpRect& rParent)
{
	CalculateRectangle(rParent);
	return m_rRect;
}

float kpgUIWindow::GetLeft() const
{
    switch( m_eOrientation )
	{
		case eWO_TopLeft:
			return m_fPosition[0];
		case eWO_Center:
			return m_fPosition[0] - (m_fWidth * 0.5f);
	}

	return -1.0f;
}

float kpgUIWindow::GetRight() const
{
    switch( m_eOrientation )
	{
		case eWO_TopLeft:
			return m_fPosition[0] + m_fWidth;
		case eWO_Center:
			return m_fPosition[0] + (m_fWidth * 0.5f);
	}

	return -1.0f;
}

float kpgUIWindow::GetTop() const
{
    switch( m_eOrientation )
	{
		case eWO_TopLeft:
			return m_fPosition[1];
		case eWO_Center:
			return m_fPosition[1] - (m_fHeight * 0.5f);
	}

	return -1.0f;
}

float kpgUIWindow::GetBottom() const
{
    switch( m_eOrientation )
	{
		case eWO_TopLeft:
			return m_fPosition[1] - m_fHeight;
		case eWO_Center:
			return m_fPosition[1] + (m_fHeight * 0.5f);
	}

	return -1.0f;
}

void kpgUIWindow::GrowLeft(float fDelta)
{
	switch( m_eOrientation )
	{
		case eWO_TopLeft:
			// Clamp to the edge if it will go over
			if( m_fPosition[0] + fDelta < 0.0f )
				fDelta = -m_fPosition[0];

			if( m_fWidth - fDelta < kMinimumWindowWidth )
			{
				float fRight = m_fPosition[0] + m_fWidth;
				m_fWidth = kMinimumWindowWidth;
				m_fPosition[0] = fRight - m_fWidth;
			}
			else
			{
				m_fPosition[0] += fDelta;
				m_fWidth -= fDelta;
			}
			
			m_fWidth = max(m_fWidth, kMinimumWindowWidth);
			break;
		case eWO_Center:
			{
				float fLeft = m_fPosition[0] - (m_fWidth / 2);
				float fRight = fLeft + m_fWidth;
				if( fLeft + fDelta < 0.0f )
					fDelta = -fLeft;
				m_fWidth -= fDelta;
				m_fWidth = max(m_fWidth, kMinimumWindowWidth);
				m_fPosition[0] = fRight - (m_fWidth / 2);
			}
			break;
	}
}

void kpgUIWindow::GrowRight(float fDelta)
{
	switch( m_eOrientation )
	{
		case eWO_TopLeft:
			if( m_fPosition[0] + m_fWidth + fDelta > 1.0f )
				m_fWidth = 1.0f - m_fPosition[0];
			else
				m_fWidth += fDelta;

			m_fWidth = max(m_fWidth, kMinimumWindowWidth);
			break;
		case eWO_Center:
			{
				float fLeft = m_fPosition[0] - (m_fWidth / 2);

				if( fLeft + m_fWidth + fDelta > 1.0f )
					m_fWidth = 1.0f - fLeft;
				else
					m_fWidth += fDelta;

                m_fWidth = max(m_fWidth, kMinimumWindowWidth);
				m_fPosition[0] = fLeft + (m_fWidth / 2);
			}
			break;
	}
}

void kpgUIWindow::GrowTop(float fDelta)
{
	switch( m_eOrientation )
	{
		case eWO_TopLeft:
			if( m_fPosition[1] + fDelta < 0.0f )
				fDelta = -m_fPosition[1];

			if( m_fHeight - fDelta < kMinimumWindowHeight )
			{
				float fBottom = m_fPosition[1] + m_fHeight;
				m_fPosition[1] = fBottom - kMinimumWindowHeight;
				m_fHeight = kMinimumWindowHeight;
			}
			else
			{
				m_fPosition[1] += fDelta;
				m_fHeight -= fDelta;
			}
			break;
		case eWO_Center:
			{
				float fBottom = m_fPosition[1] + (m_fHeight / 2);
				float fTop = fBottom - m_fHeight;
				
				if( fTop + fDelta < 0.0f )
					fDelta = -fTop;

				if( m_fHeight - fDelta < kMinimumWindowHeight )
				{
					fTop = fBottom - kMinimumWindowHeight;
					m_fHeight = kMinimumWindowHeight;
				}
				else
				{
					fTop += fDelta;
					m_fHeight -= fDelta;
				}
				m_fPosition[1] = fTop + (m_fHeight / 2);
			}
			break;
	}
}

void kpgUIWindow::GrowBottom(float fDelta)
{
	switch( m_eOrientation )
	{
		case eWO_TopLeft:
			if( m_fPosition[1] + m_fHeight + fDelta > 1.0f )
				m_fHeight = 1.0f - m_fPosition[1];
			else
				m_fHeight += fDelta;

			m_fHeight = max(m_fHeight, kMinimumWindowHeight);
			break;
		case eWO_Center:
			{
				float fTop = m_fPosition[1] - (m_fHeight / 2);
				float fBottom = fTop + m_fHeight;

				if( fBottom + fDelta > 1.0f )
					fDelta = 1.0f - fBottom;

				m_fHeight += fDelta;
				m_fHeight = max(m_fHeight, kMinimumWindowHeight);
				m_fPosition[1] = fTop + (m_fHeight / 2);
			}
			break;
	}
}

void kpgUIWindow::GetPosition(float& fX, float& fY)
{
	switch( m_eOrientation )
	{
		case eWO_TopLeft:
            fX = m_fPosition[0];
			fY = m_fPosition[1];
			break;
		case eWO_Center:
			fX = m_fPosition[0] - (m_fWidth / 2);
			fY = m_fPosition[1] - (m_fHeight / 2);
			break;
	}
}

void kpgUIWindow::SetPosition(float fX, float fY)
{
	m_fPosition[0] = fX;
	m_fPosition[1] = fY;
}

void kpgUIWindow::Move(float fDeltaX, float fDeltaY)
{
	float fLeft = GetLeft();
	float fRight = GetRight();
	float fTop = GetTop();
	float fBottom = GetBottom();

	if( fLeft + fDeltaX < 0.0f )
		fDeltaX = -fLeft;
	else if( fRight + fDeltaX > 1.0f )
		fDeltaX = 1.0f - fRight;

	if( fTop + fDeltaY < 0.0f )
		fDeltaY = -fTop;
	else if( fBottom + fDeltaY > 1.0f )
		fDeltaY = 1.0f - fBottom;

	m_fPosition[0] += fDeltaX;
	m_fPosition[1] += fDeltaY;
}
u32 kpgUIWindow::HandleInputEvent(eInputEventType type, u32 button)
{	
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	kpPoint ptMouse = g_pInputManager->GetMouseLoc();
	kpgUIWindow::eHitLocation eHit;

	//Get window
	kpgUIWindow* pWindow = HitTest((float)ptMouse.m_iX, (float)ptMouse.m_iY, kpRect(0.0f, pRenderer->GetScreenWidth(), 0.0f, pRenderer->GetScreenHeight()), &eHit);

	// TODO: Handle this event
	switch(type)
	{
	case eIET_ButtonClick:
		{		
			if( button == KPIM_BUTTON_0 )
			{
				if( pWindow )
				{
					//Get the click event
					switch( pWindow->ClickEvent() )
					{
					case CE_NEW_WINDOW:
						{
							//Change to a new window
							m_pUIManager->OpenUIWindow(pWindow->ClickEffectedWindow());
							m_bVisible = false;
							return 0;
						}	
					case CE_SCROLL_UP:
						{
							kpgUIList* pList = (kpgUIList*)m_pUIManager->GetUIWindow(pWindow->ClickEffectedWindow());

							if( pList )
								pList->ScrollUp();
							return 0;
						}
					case CE_SCROLL_DOWN:
						{
							kpgUIList* pList = (kpgUIList*)m_pUIManager->GetUIWindow(pWindow->ClickEffectedWindow());

							if( pList )
								pList->ScrollDown();
							return 0;
						}
					case CE_CLOSE:
						{
							m_pUIManager->CloseUIWindow(pWindow->CloseTarget());
							return 0;
						}
					case CE_SELECT_CELL:
						{
							kpgUIList* pList = (kpgUIList*)pWindow;

							if( pList )
								pList->SelectCell((float)ptMouse.m_iX, (float)ptMouse.m_iY);
							return 0;
						}
					default:
						{
							//see if context can handle it
							if( m_pContextObj )	
								return m_pContextObj->HandleEvent(pWindow->ClickEvent());

							return pWindow->ClickEvent();
						}
					}	
				}
				
			}
			break;
		}	
	case eIET_MouseMove:
		{
			kpgUIWindow* pWinMouseOver = m_pUIManager->WinMouseOver();

			if( pWinMouseOver )
			{
				if( pWinMouseOver != pWindow )
				{
					//Get mouse exit event
					switch( pWinMouseOver->MouseExitEvent() )
					{
					case CE_CLOSE:
						{
							m_pUIManager->CloseUIWindow(pWinMouseOver->CloseTarget());
							break;
						}						
					}	
				}
			}

			pWinMouseOver = pWindow;
			m_pUIManager->SetWinMouseOver(pWinMouseOver);

			if( pWinMouseOver )
			{
				//Get mouse enter event
				switch( pWinMouseOver->MouseEnterEvent() )
				{
				case CE_OPEN:
					{
						m_pUIManager->OpenUIWindow(pWinMouseOver->ShowTarget());
						break;
					}						
				}	
			}			
			
		}
	case eIET_ButtonUp:
		{
			if( pWindow  )
				return 0;
			break;
		}
	}	
	
	return IE_NOT_HANDLED;
}

void kpgUIWindow::Open(kpuPhysicalObject* pContext)
{
	m_bVisible = true;
	m_pContextObj = pContext;
	GetDataSource();

	//Set children data source
	kpuLinkedList* pNext = m_lChildren.Next();

	while( pNext )
	{
		((kpgUIWindow*)pNext->GetPointer())->GetDataSource();
		pNext = pNext->Next();
	}
}