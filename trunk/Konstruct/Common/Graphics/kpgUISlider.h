#pragma once
#include "kpguiwindow.h"

class kpgUISlider :
	public kpgUIWindow
{
public:
	kpgUISlider(kpgUIManager* pManager);
	virtual ~kpgUISlider(void);

	virtual void Load(TiXmlElement* pNode);
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	virtual u32 MouseDrag(const kpuVector &vDelta, const kpuVector &vPos);

	void SetSliderPos(float fPos) { m_fSliderPos = fPos; m_fSliderPrevPos = m_fSliderPos; }
	void SetSliderHeight(float fHeight) { m_fSliderHeight = fHeight; }
	void SetSliderWidth(float fWidth) { m_fSliderWidth = fWidth; }

	float GetSliderPos() { return m_fSliderPos; }
	float GetSliderDelta() { return m_fSliderPos - m_fSliderPrevPos; }

	void MoveSlider(float fDelta);

protected:	
	virtual void CalculateRectangle(const kpRect& rParent);

protected:
	enum eSlideDirection
	{
		eSD_Vertical,
		eSD_Horizontal
	};

	kpgTexture*			m_pSliderTexture;
	kpRect				m_rSliderRect;
	float				m_fSliderPos;
	float				m_fSliderPrevPos;
	float				m_fSliderWidth;
	float				m_fSliderHeight;
	eWindowOrientation	m_eSliderOrientation;

	eSlideDirection		m_eSlideDirection;
	
};
