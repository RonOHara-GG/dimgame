#include "StdAfx.h"
#include "Common\Graphics\kpgTexture.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Utility\kpuFileManager.h"
#include <d3dx9tex.h>

kpgTexture::kpgTexture(void)
{
	m_pTexture = 0;
	m_szFilename = 0;
}

kpgTexture::~kpgTexture(void)
{
	if( m_szFilename )
		free(m_szFilename);
	if( m_pTexture )
		m_pTexture->Release();
}

bool kpgTexture::Load(const char* szFilename)
{
	m_szFilename = _strdup(szFilename);

	// Grab the full filename
	char szFullPath[512];
	if( !kpuFileManager::GetFullFilePath(szFilename, szFullPath, sizeof(szFullPath)) )
		return false;

	// Create the texture
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	if( D3DXCreateTextureFromFile(pRenderer->GetDevice(), szFullPath, &m_pTexture) != D3D_OK )
		return false;

	IDirect3DSurface9* pSurface;
	if( m_pTexture->GetSurfaceLevel(0, &pSurface) != D3D_OK )
	{
		m_pTexture->Release();
		m_pTexture = 0;
		return false;
	}

	D3DSURFACE_DESC desc;
	if( pSurface->GetDesc(&desc) != D3D_OK )
	{
		pSurface->Release();
		m_pTexture->Release();
		m_pTexture = 0;
		return false;
	}
	pSurface->Release();

	m_unWidth = desc.Width;
	m_unHeight = desc.Height;

	return true;
}