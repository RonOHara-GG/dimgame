#include "StdAfx.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgShader.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"
#include "Common\Graphics\kpgFont.h"
#include "Common\Graphics\kpgGeometry.h"
#include "Common\Graphics\kpgGeometryInstance.h"
#include "Common\Graphics\kpgTexture.h"
#include "Common\Utility\kpuVector.h"

static kpgShader* s_pDefaultShader = 0;
static kpgShader* s_pImmediateModeShader = 0;
kpgRenderer*	kpgRenderer::sm_pRenderer = 0;

kpgRenderer::kpgRenderer()
{
	m_pD3D = 0;	
	m_pCurrentShader = 0;
	m_pImmediateBuffer = 0;
	m_pMissingTexture = 0;

	m_vAmbientLightColor = kpuv_Zero;

	memset(m_pLights, 0, sizeof(m_pLights));

	m_eDirtyFlags |= eDF_None;
}

kpgRenderer::~kpgRenderer(void)
{
	if( s_pDefaultShader )
	{
		delete s_pDefaultShader;
		s_pDefaultShader = 0;
	}

	if( s_pImmediateModeShader )
	{
		delete s_pImmediateModeShader;
		s_pImmediateModeShader= 0;
	}

	if( m_pImmediateBuffer )
	{
		delete m_pImmediateBuffer;
	}

	if( m_pMissingTexture )
	{
		delete m_pMissingTexture;
	}
}

bool kpgRenderer::Create(HWND hWnd)
{
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	HRESULT hRes = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice );
	assert(SUCCEEDED(hRes));

	// Load the MissingTexture texture
	m_pMissingTexture = new kpgTexture();
	m_pMissingTexture->Load("MissingTexture.dds");

	if( !s_pDefaultShader )
	{
		s_pDefaultShader = new kpgShader();
		s_pDefaultShader->LoadFromMemory(this, (BYTE*)g_DefaultShaderShaderData, g_DefaultShaderShaderDataSize);
		s_pDefaultShader->SetDefaultTexture(m_pMissingTexture);
	}
	m_pDefaultShader = s_pDefaultShader;

	// Load the default immediate mode shader
	if( !s_pImmediateModeShader )
	{
		s_pImmediateModeShader = new kpgShader();
		s_pImmediateModeShader->LoadFromMemory(this, (BYTE*)g_ImmediateModeShaderData, g_ImmediateModeShaderDataSize);	
		s_pImmediateModeShader->SetDefaultTexture(m_pMissingTexture);
	}
	m_pImmediateShader = s_pImmediateModeShader;

	// Load the default font
	kpgFontData* pDefaultFont = kpgFontData::LoadFont(this, "Arial", 14);
	kpgFontData::SetDefaultFont(pDefaultFont);

	RECT rRect;
	GetClientRect(hWnd, &rRect);
	m_fScreenWidth = (float)(rRect.right - rRect.left);
	m_fScreenHeight = (float)(rRect.bottom - rRect.top);

	m_pCurrentShader = 0;
	m_pImmediateBuffer = 0;

	m_eDirtyFlags |= eDF_None;

	return true;
}

kpgRenderer* kpgRenderer::GetInstance()
{
	if( !sm_pRenderer )
		sm_pRenderer = new kpgRenderer();
	return sm_pRenderer;
}

void kpgRenderer::SetProjectionMatrix(const kpuMatrix& mProj)
{
	m_mProjection = mProj;
}

void kpgRenderer::SetViewMatrix(const kpuMatrix& mView)
{
	m_mView = mView;
}

void kpgRenderer::SetWorldMatrix(const kpuMatrix& mWorld)
{
	m_mWorld = mWorld;

	//m_mWorldViewProjection = (m_mView * m_mProjection) * m_mWorld;
	//m_mWorldViewProjection = (m_mProjection * m_mView) * m_mWorld;
	m_mWorldViewProjection = m_mWorld * m_mView * m_mProjection;

	m_eDirtyFlags |= eDF_Matrices;
}

void kpgRenderer::BeginFrame()
{
	HRESULT res = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xFF004080, 1.0f, 0);
	assert(SUCCEEDED(res));
	m_pDevice->BeginScene();

	if( m_pCurrentShader )
		m_pCurrentShader->Bind();
}

void kpgRenderer::EndFrame()
{
	if( m_pCurrentShader )
		m_pCurrentShader->Unbind();

	m_pDevice->EndScene();
	m_pDevice->Present(0, 0, 0, 0);
}

void kpgRenderer::DrawText2D(const char* szText, const kpRect& rRect, const kpgFont* pFont)
{
	// Draw the text
	RECT rect = {(long)rRect.m_fLeft, (long)rRect.m_fTop, (long)rRect.m_fRight, (long)rRect.m_fBottom};
	pFont->GetDeviceFont()->DrawText(0, szText, -1, &rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
}

void kpgRenderer::DrawQuad2D(const kpRect& rRect, const kpgTexture* pTexture, kpgQuadUVs* pUVs)
{
	// Generate verticies
	kpuVector vPosTL = kpuVector(rRect.m_fLeft, rRect.m_fTop, 0.0f, 0.0f);
	kpuVector vPosTR = kpuVector(rRect.m_fRight, rRect.m_fTop, 0.0f, 0.0f);
	kpuVector vPosBL = kpuVector(rRect.m_fLeft, rRect.m_fBottom, 0.0f, 0.0f);
	kpuVector vPosBR = kpuVector(rRect.m_fRight, rRect.m_fBottom, 0.0f, 0.0f);

	kpuVector vUVTL, vUVTR, vUVBL, vUVBR;
	if( pUVs )
	{
		vUVTL = kpuVector(pUVs->m_fTopLeftU, pUVs->m_fTopLeftV, 0.0f, 0.0f);
		vUVTR = kpuVector(pUVs->m_fTopRightU, pUVs->m_fTopRightV, 0.0f, 0.0f);
		vUVBL = kpuVector(pUVs->m_fBottomLeftU, pUVs->m_fBottomLeftV, 0.0f, 0.0f);
		vUVBR = kpuVector(pUVs->m_fBottomRightU, pUVs->m_fBottomRightV, 0.0f, 0.0f);
	}
	else
	{
		vUVTL = kpuVector(0.0f, 0.0f, 0.0f, 0.0f);
		vUVTR = kpuVector(1.0f, 0.0f, 0.0f, 0.0f);
		vUVBL = kpuVector(0.0f, 1.0f, 0.0f, 0.0f);
		vUVBR = kpuVector(1.0f, 1.0f, 0.0f, 0.0f);
	}

	// Set the texture in the shader
	m_pImmediateShader->SetDefaultTexture(pTexture);
	
	// Draw the quad
	BeginImmediate(ePT_TriStrip, 4);
	AddImmediateVertex(vPosTL, vUVTL);
	AddImmediateVertex(vPosTR, vUVTR);
	AddImmediateVertex(vPosBL, vUVBL);
	AddImmediateVertex(vPosBR, vUVBR);
	EndImmediate();
}

void kpgRenderer::BeginImmediate(ePrimitiveType eType, int nVertexCount, bool bBindShader)
{
	if( m_pImmediateBuffer && m_pImmediateBuffer->GetVertexCount() < nVertexCount )
	{
		// Buffer exists but is to small, delete the current one
		delete m_pImmediateBuffer;
		m_pImmediateBuffer = 0;
	}

	if( !m_pImmediateBuffer )
	{
		// No vertex buffer allocated, create one
		kpgVertexBuffer* pVB = new kpgVertexBuffer();
		if( pVB->Create(nVertexCount, eType, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0, this) )
		{
			m_pImmediateBuffer = pVB;
		}
	}

	m_nImmediateVertexCount = 0;

	// Bind the Immediate mode shader
	if( bBindShader )
	{
		SetShader(m_pImmediateShader);
	}
}

void kpgRenderer::EndImmediate()
{
	m_pImmediateBuffer->Bind(this);
	for( u32 i = 0; i < m_pCurrentShader->GetPassCount(); i++ )
	{
		m_pCurrentShader->BeginPass(i);
		HRESULT hres = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)m_pImmediateBuffer->GetPrimType(), 0, m_pImmediateBuffer->GetPrimCount(m_nImmediateVertexCount));
		assert(SUCCEEDED(hres));
		m_pCurrentShader->EndPass();
	}
	m_pImmediateBuffer->Unbind(this);
}

void kpgRenderer::AddImmediateVertex(const kpuVector& vPos, const kpuVector& vUV)
{
	m_pImmediateBuffer->Lock();
	m_pImmediateBuffer->SetPosition(m_nImmediateVertexCount, vPos);
	m_pImmediateBuffer->SetUV(m_nImmediateVertexCount, vUV);
	m_nImmediateVertexCount++;
}

void kpgRenderer::AddImmediateVertex(const kpuVector& vPos, const kpuVector& vNormal, const kpuVector& vUV)
{
	m_pImmediateBuffer->Lock();
	m_pImmediateBuffer->SetPosition(m_nImmediateVertexCount, vPos);
	m_pImmediateBuffer->SetNormal(m_nImmediateVertexCount, vNormal);
	m_pImmediateBuffer->SetUV(m_nImmediateVertexCount, vUV);
	m_nImmediateVertexCount++;
}

kpgShader* kpgRenderer::SetShader(kpgShader* pShader)
{
	kpgShader* pWas = m_pCurrentShader;
	if( m_pCurrentShader != pShader )
	{
		// Unbind the current shader
		if( m_pCurrentShader )
			m_pCurrentShader->Unbind();

		// Set the shader
		m_pCurrentShader = pShader;
		
		// Bind the shader
		if( m_pCurrentShader )
		{
			m_pCurrentShader->Bind();

			// Make sure shader variables get uploaded
			m_eDirtyFlags = eDF_AllDirty;
		}
	}

	if( m_pCurrentShader )
	{
		// Update shader variables
		if( m_eDirtyFlags & eDF_Matrices )
		{
			// Upload the matrices
			m_pCurrentShader->SetMatrices(m_mWorldViewProjection, m_mWorld);
		}
		if( m_eDirtyFlags & eDF_Lights )
		{
			// Upload lighting variables
			m_pCurrentShader->SetLights(m_pLights);
		}
		if( m_eDirtyFlags & eDF_AmbientLight )
		{
			// Upload ambient color
			m_pCurrentShader->SetAmbientColor(m_vAmbientLightColor);
		}
		

		m_eDirtyFlags = 0;
	}

	return pWas;
}

void kpgRenderer::SetAmbientLightColor(const kpuVector& vAmbientColor)
{
	if( m_vAmbientLightColor != vAmbientColor )
	{
		m_eDirtyFlags |= eDF_AmbientLight;
		m_vAmbientLightColor = vAmbientColor;
	}
}

void kpgRenderer::SetLight(int nLightIndex, const kpgLight* pLight)
{
	if( nLightIndex >= 0 && nLightIndex < MAX_LIGHTS )
	{
		m_pLights[nLightIndex] = pLight;
		m_eDirtyFlags |= eDF_Lights;
	}
}

void kpgRenderer::DrawInstancedGeometry(kpgGeometryInstance* pInstance, kpuMatrix* pOffsetMatrix)
{
	kpgGeometry* pGeometry = pInstance->GetGeometry();

	kpgVertexBuffer* pVertexBuffer = pGeometry->GetVertexBuffer();
	kpgIndexBuffer* pIndexBuffer = pGeometry->GetIndexBuffer();	
	kpgTexture*		pTexture = pGeometry->GetTexture();


	if( pOffsetMatrix )
		SetWorldMatrix(*pOffsetMatrix * pInstance->GetMatrix());
	else
		SetWorldMatrix(pInstance->GetMatrix());
	
	kpgShader* pShader = pGeometry->GetShader();
	
	if(pTexture)
	{
		pShader->SetTexture(pTexture);
	}

	SetShader(pShader);		

	m_pDevice->SetIndices(*pIndexBuffer);
	
	pVertexBuffer->Bind(this);
	
	for( u32 i = 0; i < pShader->GetPassCount(); i++ )
	{
		pShader->BeginPass(i);		
		m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)pVertexBuffer->GetPrimType(), 0, 0, pVertexBuffer->GetVertexCount(), 0, pVertexBuffer->GetPrimCount(pIndexBuffer->GetIndexCount()));
		//m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)pVertexBuffer->GetPrimType(), 0, 1);
		pShader->EndPass();
	}

	pVertexBuffer->Unbind(this);

	
}

