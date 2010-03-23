#pragma once

#include <d3d9.h>

class kpgRenderer;

class kpgTexture
{
public:
	kpgTexture(void);
	~kpgTexture(void);

	bool Load(const char* szFilename);

	u32		GetWidth() const						{ return m_unWidth; }
	u32		GetHeight()	const						{ return m_unHeight; }
	IDirect3DTexture9* GetDeviceTexture() const		{ return m_pTexture; }
	const char* GetFileName() const					{ return m_szFilename; }

protected:
	IDirect3DTexture9*	m_pTexture;
	char*				m_szFilename;
	u32					m_unWidth;
	u32					m_unHeight;
};
