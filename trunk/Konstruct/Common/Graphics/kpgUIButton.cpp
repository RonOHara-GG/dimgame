#include "StdAfx.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgUIButton.h"

kpgUIButton::kpgUIButton(void)
{
	m_eType = eWT_Button;
}

kpgUIButton::~kpgUIButton(void)
{
}

void kpgUIButton::LoadDefaults()
{
	// Load the default frame
	kpgUIWindow::LoadDefaults();

}

void kpgUIButton::Draw(kpgRenderer* pRenderer, const kpRect& rParent)
{
	if( m_bVisible )
	{		
		// Calculate the rectangle
		CalculateRectangle(rParent);

		// Draw the frame
		if( m_bHasFrame )
			DrawFrame(pRenderer, m_rRect);

		// Draw the text
		if( m_pFont && m_szText )
		{
			kpRect rText = GetTextRectangle();            
			pRenderer->DrawText2D(m_szText, rText, m_pFont);
		}
	}
}

void kpgUIButton::CalculateRectangle(const kpRect& rParent)
{
	kpgUIWindow::CalculateRectangle(rParent);
}

kpRect kpgUIButton::GetTextRectangle()
{
	float fHalfTextWidth = m_fTextWidth * 0.5f;
	float fHalfTextHeight = m_fTextHeight * 0.5f;
	float fMidX = m_rRect.m_fLeft + (m_rRect.Width() * 0.5f);
	float fMidY = m_rRect.m_fTop + (m_rRect.Height() * 0.5f);
	return kpRect(fMidX - fHalfTextWidth, fMidX + fHalfTextWidth, fMidY - fHalfTextHeight, fMidY + fHalfTextHeight);
}

