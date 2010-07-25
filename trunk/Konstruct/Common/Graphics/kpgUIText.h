#pragma once

#include "Common/Graphics/kpgUIWindow.h"

class kpgFont;

class kpgUIText : public kpgUIWindow
{
public:
	kpgUIText(kpgUIManager* pManager);
	virtual ~kpgUIText(void);

	void SetText(const char* szText);
	void SetFont(kpgFont* pFont);

	virtual TiXmlElement* Save(TiXmlNode* pParent);
	virtual void Load(TiXmlElement* pNode);

	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

protected:
	virtual void	GetDataSource();
	virtual kpRect GetTextRectangle();
	virtual void CalculateRectangle(const kpRect& rParent);

//private:
	virtual void CalculateTextSize();

protected:
	char*		m_szText;
	kpgFont*	m_pFont;
	float		m_fLeftMargin;
	float		m_fTopMargin;
	float		m_fTextWidth;
	float		m_fTextHeight;
};
