#pragma once
#include "kpguiwindow.h"
#include "Common/Utility/kpuFixedArray.h"

class kpgUIText;
class kpgFont;

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
	int	ContainsEscChar(const char* psz);

protected:
	int							m_iRows;
	int							m_iColumns;
	//kpuFixedArray<float>*		m_paRowHeights;
	float						m_fRowHeight;
	kpuFixedArray<float>*		m_paColumnWidths;	
	kpuFixedArray<kpgTexture*>* m_paIcons;
	kpgFont*					m_pFont;

	char***						m_pDataSource;

	/** Scroll bar stuff **/
	kpgUIWindow*				m_pScrollBar;
	float						m_fViewPosition[2];
};
