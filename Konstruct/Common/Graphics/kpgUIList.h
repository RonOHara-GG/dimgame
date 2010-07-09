#pragma once
#include "kpguiwindow.h"

class kpgUIText;

class kpgUIList :
	public kpgUIWindow
{
public:
	kpgUIList(void);
	virtual ~kpgUIList(void);

	virtual void Load(TiXmlElement* pNode);
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	virtual void SetDataSource(const char* pszData);
	
	//Events and targets
	virtual u32  ClickEvent()			{ return m_uClickEvent; }
	virtual u32  ClickEffectedWindow()	{ return m_uTargetHash; }

protected:
	int				m_iRows;
	int				m_iColumns;
	float			m_fRowHeight;
	float			m_fColumnWidth;
	kpgUIText***	m_paListObjects;
};
