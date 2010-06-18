#pragma once
#include "Common/Graphics/kpgUIText.h"

class kpgUIButton :	public kpgUIText
{
public:
	kpgUIButton(void);
	virtual ~kpgUIButton(void);

	void LoadDefaults();
	void Load(TiXmlElement *pNode);
	
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

protected:
	kpRect GetTextRectangle();
	virtual void CalculateRectangle(const kpRect& rParent);

protected:
	

};
