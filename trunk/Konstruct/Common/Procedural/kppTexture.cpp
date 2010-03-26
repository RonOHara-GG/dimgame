#include "StdAfx.h"
#include "kppTexture.h"
#include "Common\Graphics\kpgRenderer.h"

#include <D3DX9.h>

kppTexture::kppTexture(u32 unWidth, u32 unHeight)
{
	m_unWidth = unWidth;
	m_unHeight = unHeight;
}

kppTexture::~kppTexture(void)
{
}

void kppTexture::Build()
{
	// Create the texture shader
	LPD3DXTEXTURESHADER pTextureShader = NULL;
	D3DXCreateTextureShader((DWORD*)m_pShaderData, &pTextureShader);
	pTextureShader->SetDefaults();

	// Create the texture
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	D3DXCreateTexture(pRenderer->GetDevice(), m_unWidth, m_unHeight, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture);

	// Execute the texture shader to fill the texture
	D3DXFillTextureTX(m_pTexture, pTextureShader);

	pTextureShader->Release();
}
