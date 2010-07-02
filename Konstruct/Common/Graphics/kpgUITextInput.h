#pragma once
#include "Common/Graphics/kpgUIText.h"
#include "Common/Utility/kpuStopwatch.h"

class kpgTexture;

class kpgUITextInput :	public kpgUIText
{
public:
	kpgUITextInput(void);
	virtual ~kpgUITextInput(void);

	void LoadDefaults();
	void Load(TiXmlElement* pElement);
	void SetPasswordMode(bool bPasswordMode = true);
	void SetText(const char* szText);

	virtual void Update();
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	void LooseFocus() { m_bFocus = false; }

	//Input events
	u32	ClickEvent();
	u32 KeyPressEvent(u32 uKey);

private:
	virtual void CalculateRectangle(const kpRect& rParent);
	void AddText(char* szText, int iStrLen, int iIndex); //Adds text to string at given index

protected:
	bool			m_bFocus;
	bool			m_bShiftPress;

	kpgTexture*		m_pCursor;
	int				m_iCursorPos;
	bool			m_bBlink;
	kpuStopwatch	m_BlinkTimer;
	char*			m_szPasswordText;
};
