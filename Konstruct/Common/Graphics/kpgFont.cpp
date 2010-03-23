#include "StdAfx.h"
#include "Common/Graphics/kpgFont.h"
#include "Common/Graphics/kpgTexture.h"
#include "Common/Utility/kpuFileManager.h"

kpgFontData* kpgFontData::sm_pDefaultFont;
kpuLinkedList kpgFontData::sm_lFontList;

// kpgFontData implementation
kpgFontData::kpgFontData()
{
	m_nRefCount = 0;
	m_pFont = 0;
	m_szFaceName = 0;
}

kpgFontData::~kpgFontData()
{
	if( m_szFaceName )
		free(m_szFaceName);
	if( m_pFont )
		m_pFont->Release();
}

void kpgFontData::AddRef()
{
	m_nRefCount++;
}

void kpgFontData::DeRef()
{
	if( m_nRefCount > 0 )
		m_nRefCount--;
}

kpgFontData* kpgFontData::LoadFont(kpgRenderer* pRenderer, const char* szFaceName, int nHeight, int nWidth, int nWeight, bool bItalic)
{
	// Check to see if we already loaded this font
	kpuLinkedList* iter = sm_lFontList.Next();
	while( iter )
	{
		kpgFontData* pData = (kpgFontData*)iter->GetPointer();
		if( pData )
		{
			D3DXFONT_DESC desc;
			pData->m_pFont->GetDesc(&desc);
			bool bMatch = true;
			if( nHeight && desc.Height != nHeight )
				bMatch = false;
			if( nWidth && desc.Width != nWidth )
				bMatch = false;
			if( nWeight && desc.Weight != nWeight )
				bMatch = false;
			if( desc.Italic != bItalic )
				bMatch = false;
		}
		iter = iter->Next();
	}

	// Didnt find one, go ahead and load it
	ID3DXFont* pFont;
	HRESULT hRes = D3DXCreateFont(pRenderer->GetDevice(), nHeight, nWidth, nWeight, 0, bItalic, 0, OUT_TT_ONLY_PRECIS, 0, 0, szFaceName, &pFont);
	if( hRes != D3D_OK )
		return 0;

	kpgFontData* pFontData = new kpgFontData();
	pFontData->m_pFont = pFont;
	pFontData->m_szFaceName = strdup(szFaceName);

	sm_lFontList.AddTail(pFontData);
	return pFontData;
}

void kpgFontData::UnloadAllFonts()
{
	// Go through the list of loaded fonts and destroy them all
	while( sm_lFontList.Next() )
	{
		kpgFontData* pData = (kpgFontData*)sm_lFontList.GetPointer();
		delete pData;
		delete sm_lFontList.Next();
	}
}

void kpgFontData::CalcRectangle(kpRect& rRect, const char* szText)
{
	RECT r = {(long)rRect.m_fLeft, (long)rRect.m_fTop, (long)rRect.m_fLeft + 2000, (long)rRect.m_fTop + 2000};
	int nHeight = m_pFont->DrawText(0, szText, -1, &r, DT_CALCRECT | DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
	rRect.m_fLeft = (float)r.left;
	rRect.m_fTop = (float)r.top;
	rRect.m_fRight = (float)r.right;
	rRect.m_fBottom = (float)r.bottom;
}

int kpgFontData::GetHeight() const
{	
	if( !m_pFont )
		return 0;
	
	D3DXFONT_DESC desc;
	m_pFont->GetDesc(&desc);
	return desc.Height;
}

int kpgFontData::GetWidth() const
{	
	if( !m_pFont )
		return 0;
	
	D3DXFONT_DESC desc;
	m_pFont->GetDesc(&desc);
	return desc.Width;
}

int kpgFontData::GetWeight() const
{	
	if( !m_pFont )
		return 0;
	
	D3DXFONT_DESC desc;
	m_pFont->GetDesc(&desc);
	return desc.Weight;
}

bool kpgFontData::GetItalic() const
{	
	if( !m_pFont )
		return 0;
	
	D3DXFONT_DESC desc;
	m_pFont->GetDesc(&desc);
	return desc.Italic ? true : false;
}

// kpgFont implementation
kpgFont::kpgFont(void)
{
	m_pData = 0;
}

kpgFont::~kpgFont(void)
{
	if( m_pData )
	{
		m_pData->DeRef();
		m_pData = 0;
	}
}

void kpgFont::LoadDefaultFont()
{
	m_pData = kpgFontData::GetDefaultFont();
	if( m_pData )
		m_pData->AddRef();
}

void kpgFont::Save(TiXmlNode* pParent)
{
	// Only save font info if this isnt the default font
	if( m_pData != kpgFontData::GetDefaultFont() )
	{
		TiXmlElement* pFontElement = new TiXmlElement("Font");
		pFontElement->SetAttribute("face", m_pData->GetFace());
		pFontElement->SetAttribute("height", m_pData->GetHeight());
		pFontElement->SetAttribute("width", m_pData->GetWidth());
		pFontElement->SetAttribute("weight", m_pData->GetWeight());
		pFontElement->SetAttribute("italic", m_pData->GetItalic() ? 1 : 0);
		pParent->LinkEndChild(pFontElement);
	}
}

void kpgFont::Load(TiXmlElement* pParent)
{
	TiXmlElement* pChild = pParent->FirstChildElement();
	if( pChild && !strcmp(pChild->Value(), "Font") )
	{
		const char* pFace = pChild->Attribute("face");
		const char* pHeight = pChild->Attribute("height");
		const char* pWidth = pChild->Attribute("width");
		const char* pWeight = pChild->Attribute("weight");
		const char* pItalic = pChild->Attribute("italic");
		int nItalic = atoi(pItalic);

		m_pData = kpgFontData::LoadFont(kpgRenderer::GetInstance(), pFace, atoi(pHeight), atoi(pWidth), atoi(pWeight), (nItalic != 0));
	}
	else
	{
		LoadDefaultFont();
	}
}
