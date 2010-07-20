#include "StdAfx.h"
#include "kpgUISlider.h"
#include "kpgTexture.h"
#include "kpgRenderer.h"
#include "kpgUIManager.h"

kpgUISlider::kpgUISlider(kpgUIManager* pManager) : kpgUIWindow(pManager)
{
	m_pSliderTexture = 0;
	m_fSliderPos = 0.0f;
	m_fSliderHeight = 0.0f;
	m_fSliderWidth = 0.0f;
}

kpgUISlider::~kpgUISlider(void)
{
	delete m_pSliderTexture;
}

void kpgUISlider::Load(TiXmlElement *pNode)
{
	kpgUIWindow::Load(pNode);

	//load slider texture
	const char* szSlider = pNode->Attribute("Slider");
	if( szSlider )
	{
		m_pSliderTexture = new kpgTexture();
		m_pSliderTexture->Load(szSlider);
		szSlider = 0;
	}

	//get its orientation
	szSlider = pNode->Attribute("SliderOrientation");
	if( szSlider )
	{
		m_eSliderOrientation = (eWindowOrientation)atoi(szSlider);
		szSlider = 0;
	}

	szSlider = pNode->Attribute("SlideDirection");
	{
		m_eSlideDirection = (eSlideDirection)atoi(szSlider);
		szSlider = 0;
	}

	szSlider = pNode->Attribute("SliderHeight");
	if( szSlider )
	{
		m_fSliderHeight = (float)atof(szSlider);
		szSlider = 0;
	}

	szSlider = pNode->Attribute("SliderWidth");
	if( szSlider )
	{
		m_fSliderWidth = (float)atof(szSlider);
		szSlider = 0;
	}



}

void kpgUISlider::Draw(kpgRenderer *pRenderer, const kpRect &rParent)
{
	kpgUIWindow::Draw(pRenderer, rParent);

	if( m_bVisible )
	{
		if( m_pSliderTexture )
			pRenderer->DrawQuad2D(m_rSliderRect, m_pSliderTexture);
	}
}

void kpgUISlider::CalculateRectangle(const kpRect& rParent)
{
	kpgUIWindow::CalculateRectangle(rParent);

	float fWidth = m_fSliderWidth * m_rRect.Width();
	float fHeight = m_fSliderHeight * m_rRect.Height();


	//calculate the slide button's rect
	switch(m_eSliderOrientation )
	{
	case eWO_Center:
		switch(m_eSlideDirection)
		{
		case eSD_Horizontal:
			m_rSliderRect.m_fLeft = m_rRect.m_fLeft + ( m_fSliderPos * m_rRect.Width() ) - (fWidth * 0.5f);
			m_rSliderRect.m_fRight = m_rSliderRect.m_fLeft + fWidth;
			m_rSliderRect.m_fTop = m_rRect.m_fTop + ( ( m_rRect.Height() - fHeight) * 0.5f) ;
			m_rSliderRect.m_fBottom = m_rSliderRect.m_fTop + fHeight;
			break;
		case eSD_Vertical:
			m_rSliderRect.m_fLeft = m_rRect.m_fLeft + (( m_rRect.Width() - fWidth) * 0.5f);
			m_rSliderRect.m_fRight = m_rSliderRect.m_fLeft + fWidth;
			m_rSliderRect.m_fTop = m_rRect.m_fTop + ( m_fSliderPos * m_rRect.Height() ) - (fHeight * 0.5f) ;
			m_rSliderRect.m_fBottom = m_rSliderRect.m_fTop + fHeight;
			break;
		}
		break;
	case eWO_TopLeft:
		switch(m_eSlideDirection)
		{
		case eSD_Horizontal:
			m_rSliderRect.m_fLeft = m_rRect.m_fLeft + m_fSliderPos * m_rRect.Width();
			m_rSliderRect.m_fRight = m_rSliderRect.m_fLeft + fWidth;
			m_rSliderRect.m_fTop = m_rRect.m_fTop + ( ( m_rRect.Height() - fHeight) * 0.5f);
			m_rSliderRect.m_fBottom = m_rSliderRect.m_fTop + fHeight;
			break;
		case eSD_Vertical:
			m_rSliderRect.m_fLeft = m_rRect.m_fLeft + (( m_rRect.Width() - fWidth) * 0.5f);
			m_rSliderRect.m_fRight = m_rSliderRect.m_fLeft + fWidth;
			m_rSliderRect.m_fTop = m_rRect.m_fTop + m_fSliderPos * m_rRect.Height();
			m_rSliderRect.m_fBottom = m_rSliderRect.m_fTop + fHeight;
			break;
		}
		break;
	}



}

u32 kpgUISlider::MouseDrag(const kpuVector &vDelta, const kpuVector &vPos)
{
	float fX = vPos.GetX();
	float fY = vPos.GetY();

	//make sure the click was within the slider
	if( fY > m_rSliderRect.m_fBottom || fY < m_rSliderRect.m_fTop || fX < m_rSliderRect.m_fLeft || fX > m_rSliderRect.m_fRight)
		return IE_NOT_HANDLED;

	//figure out the % move of the slider
	float fMove = 0.0f;

	switch(m_eSlideDirection)
	{
	case eSD_Horizontal:
		fMove = vDelta.GetX() / m_rRect.Width();
		break;
	case eSD_Vertical:
		fMove = vDelta.GetY() / m_rRect.Height();
		break;
	}

	MoveSlider(fMove);

	return kpgUIWindow::MouseDrag(vDelta, vPos);
}

void kpgUISlider::MoveSlider(float fDelta)
{	
	//make sure you stay within the bounds of the slide bar
	if( m_fSliderPos + fDelta < 0.0f )
	{
		m_fSliderPos = 0.0f;
		return;
	}
	switch(m_eSliderOrientation)
	{
	case eWO_Center:
		if( m_fSliderPos + fDelta > 1.0f )
		{
			m_fSliderPos = 1.0f;
			return;
		}
		break;
	case eWO_TopLeft:		
		switch(m_eSlideDirection)
		{
		case eSD_Horizontal:
			if( m_fSliderPos + fDelta > 1.0f - m_fSliderWidth )
			{
				m_fSliderPos = 1.0f - m_fSliderWidth;
				return;
			}
			break;
		case eSD_Vertical:
			if( m_fSliderPos + fDelta > 1.0f - m_fSliderHeight )
			{
				m_fSliderPos = 1.0f - m_fSliderHeight;
				return;
			}
			break;
		}
		break;
	}
			
	m_fSliderPrevPos = m_fSliderPos;
	m_fSliderPos += fDelta;
}