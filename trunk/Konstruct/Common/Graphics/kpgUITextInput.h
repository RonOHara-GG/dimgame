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
	void SetPasswordMode(bool bPasswordMode = true);
	void SetText(const char* szText);

	virtual void Update();
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

private:
	virtual void CalculateRectangle(const kpRect& rParent);

protected:
	kpgTexture*		m_pCursor;
	bool			m_bBlink;
	kpuStopwatch	m_BlinkTimer;
	char*			m_szPasswordText;
};
