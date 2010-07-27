#pragma once
#include "kpguitext.h"
#include "Common/Utility/kpuFixedArray.h"

class kpgUIText;
class kpgFont;
class kpgUISlider;

class kpgUIList :
	public kpgUIText
{
public:
	kpgUIList(kpgUIManager* pManager);
	virtual ~kpgUIList(void);

	virtual void Load(kpuXmlParser* pParser);
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	void ScrollUp();
	void ScrollDown();
	void Scroll();

	void SelectCell(float fX, float fY);
	
	//Events and targets
	virtual u32  ClickEvent(float fX, float fY);		

	int GetSelectedRow() { return m_iCellClicked[1]; }
	int GetSelectedColumn() { return m_iCellClicked[0]; }

protected:
	virtual kpRect GetTextRectangle(kpRect& rRect);
	kpRect GetIconRectangle(kpRect& rRect);
	void CalculateRectangle(const kpRect& rParent);
	void CalculateScrollHeight();
	void GetDataSource();

protected:
	int							m_iRows;
	int							m_iColumns;
	kpuFixedArray<float>*		m_paRowHeights;
	kpuFixedArray<float>*		m_paColumnWidths;	
	kpuFixedArray<kpgTexture*>* m_paIcons;
	float						m_fIconSize[2];
	float						m_fListDimensions[2];

	/** Scroll bar stuff **/
	kpgUISlider*				m_pScrollBar;
	float						m_fViewOffset[2];
	float						m_fScrollDelta;
	float						m_fContentHeight;

	int							m_iCellClicked[2];
	
};
