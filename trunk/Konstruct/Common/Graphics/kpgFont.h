#pragma once

#include "External/Tinyxml/tinyxml.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Utility/kpuLinkedList.h"

#include <d3dx9core.h>

class kpgTexture;

class kpgFontData
{
public:
	kpgFontData();
	~kpgFontData();

	void AddRef();
	void DeRef();

	static kpgFontData*		LoadFont(kpgRenderer* pRenderer, const char* szFaceName, int nHeight = 0, int nWidth = 0, int nWeight = 0, bool bItalic = false);
	static void				UnloadAllFonts();

	static kpgFontData*		GetDefaultFont()						{ return sm_pDefaultFont; }
	static void				SetDefaultFont(kpgFontData* pDefault)	{ sm_pDefaultFont = pDefault; }

	ID3DXFont* GetDeviceFont() const		{ return m_pFont; }

	void CalcRectangle(kpRect& rRect, const char* szText);

	const char* GetFace() const				{ return m_szFaceName; }
	int GetHeight() const;
	int GetWidth() const;
	int GetWeight() const;
	bool GetItalic() const;

protected:
	int				m_nRefCount;
	char*			m_szFaceName;

	ID3DXFont*		m_pFont;

	static kpgFontData*		sm_pDefaultFont;
	static kpuLinkedList	sm_lFontList;
};

class kpgFont
{
public:
	kpgFont(void);
	~kpgFont(void);

	void LoadDefaultFont();

	void Save(TiXmlNode* pParent);
	void Load(TiXmlElement* pParent);

	void CalcRectangle(kpRect& rRect, const char* szText)	{ if( m_pData ) m_pData->CalcRectangle(rRect, szText); }

	ID3DXFont* GetDeviceFont() const		{ return m_pData ? m_pData->GetDeviceFont() : 0; }

protected:
	kpgFontData*	m_pData;
};
