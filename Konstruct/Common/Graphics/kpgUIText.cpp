#include "StdAfx.h"
#include "Common/Graphics/kpgUIText.h"
#include "Common/Graphics/kpgFont.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgTexture.h"

kpgUIText::kpgUIText(void)
{
	m_szText = 0;
	m_pFont = new kpgFont();
	m_pFont->LoadDefaultFont();

	m_fLeftMargin = 2.0f;
	m_fTopMargin = 1.0f;

	m_eOrientation = eWO_TopLeft;

	m_eType = eWT_Text;
}

kpgUIText::~kpgUIText(void)
{
	if( m_pFont )
		delete m_pFont;

	if( m_szText )
		free(m_szText);
}

TiXmlElement* kpgUIText::Save(TiXmlNode* pParent)
{
	// Write the base class
	TiXmlElement* pElement = kpgUIWindow::Save(pParent);

	// Write our members out
	pElement->SetAttribute("Text", m_szText ? m_szText : "");
	if( m_pFont )
		m_pFont->Save(pElement);
	pElement->SetDoubleAttribute("LeftMargin", m_fLeftMargin);
	pElement->SetDoubleAttribute("TopMargin", m_fTopMargin);
	pElement->SetDoubleAttribute("TextWidth", m_fTextWidth);
	pElement->SetDoubleAttribute("TextHeight", m_fTextHeight);

	return pElement;
}

void kpgUIText::Load(TiXmlElement* pNode)
{
	// Load the base class
	kpgUIWindow::Load(pNode);

	const char* pText = pNode->Attribute("Text");
	if( pText )
		m_szText = _strdup(pText);

	m_pFont = new kpgFont();
	m_pFont->Load(pNode);
	
	const char* pLeftMargin = pNode->Attribute("LeftMargin");
	if( pLeftMargin )
		m_fLeftMargin = (float)atof(pLeftMargin);
	
	const char* pTopMargin = pNode->Attribute("TopMargin");
	if( pTopMargin )
		m_fTopMargin = (float)atof(pTopMargin);

	const char* pWidth = pNode->Attribute("TextWidth");
	if( pWidth )
		m_fTextWidth = (float)atof(pWidth);

	const char* pHeight = pNode->Attribute("TextHeight");
	if( pHeight )
		m_fTextHeight = (float)atof(pHeight);

	CalculateTextSize();
}

void kpgUIText::SetText(const char* szText)
{
	if( m_szText )
		free(m_szText);

	m_szText = _strdup(szText);

	CalculateTextSize();
}

void kpgUIText::SetFont(kpgFont* pFont)
{
	if( m_pFont )
		delete m_pFont;
	m_pFont = pFont;

	CalculateTextSize();
}

void kpgUIText::Draw(kpgRenderer* pRenderer, const kpRect& rParent)
{
	// Draw the text
	if( m_bVisible )
	{
		CalculateRectangle(rParent);

		// Draw the frame
		if( m_bHasFrame )
			DrawFrame(pRenderer, m_rRect);

		if( m_pFont && m_szText )
		{
			kpRect rText = GetTextRectangle();    
			
			pRenderer->DrawText2D(m_szText, rText, m_pFont);
		}
	}
}

kpRect kpgUIText::GetTextRectangle()
{
	float fLeft = m_rRect.m_fLeft;
	if( m_bHasFrame && m_pBorder )
		fLeft += m_fLeftMargin + m_pBorder->GetWidth();		// Offset by the left border
	float fRight = fLeft + m_fTextWidth;
	float fTop = m_rRect.m_fTop;
	if( m_bHasFrame && m_pBorder )
		fTop += m_fTopMargin + m_pBorder->GetHeight();		// Offset by the top border
	float fBottom = fTop + m_fTextHeight;
	return kpRect(fLeft, fRight, fTop, fBottom);
	
}

void kpgUIText::CalculateTextSize()
{
	if( m_pFont )
	{
		kpRect rRect(0, 2000, 0, 2000);
		m_pFont->CalcRectangle(rRect, m_szText);
		m_fTextWidth = rRect.m_fRight - rRect.m_fLeft;
		m_fTextHeight = rRect.m_fBottom - rRect.m_fTop;
	}
}

void kpgUIText::CalculateRectangle(const kpRect& rParent)
{
	m_fWidth = rParent.m_fLeft + m_fTextWidth; 
	m_fHeight = rParent.m_fTop + m_fTextHeight;
	rParent.TransformPoint(m_fWidth, m_fHeight);

	kpgUIWindow::CalculateRectangle(rParent);
}