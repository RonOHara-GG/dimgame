#include "StdAfx.h"
#include "Common\Graphics\kpgShader.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgTexture.h"
#include "Common\Graphics\kpgLight.h"

// Include the shader data here
#include "Common\Graphics\Shaders\DefaultShader.sh"
#include "Common\Graphics\Shaders\ImmediateMode.sh"

const u32 g_DefaultShaderShaderDataSize = sizeof(g_DefaultShaderShaderData);
const u32 g_ImmediateModeShaderDataSize = sizeof(g_ImmediateModeShaderData);

kpgShader::kpgShader(void)
{
	m_pEffect = 0;
	m_hDefaultTechnique = 0;
	m_hWorldViewProj = 0;
	m_hWorld = 0;
	m_hLightCount = 0;
	m_hLightType = 0;
	m_hLightVector = 0;
	m_hLightColor = 0;
	m_hAmbientColor = 0;
	m_hDefaultTexture = 0;

	m_hCurrentTechnique = 0;

	m_bBound = false;
}

kpgShader::~kpgShader(void)
{
	Unbind();

	if( m_pEffect )
	{
		m_pEffect->Release();
	}
}

void kpgShader::LoadFromMemory(kpgRenderer* pRenderer, const ul32* pShaderData, u32 nShaderDataSize )
{
	if( D3DXCreateEffect(pRenderer->GetDevice(), pShaderData, nShaderDataSize, 0, 0, 0, 0, &m_pEffect, 0) != D3D_OK )
	{
	}
	else
	{
		// Get the default technique
		m_hDefaultTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");

		// Get Global Variables
		m_hWorldViewProj =	m_pEffect->GetParameterBySemantic(NULL, "WORLDVIEWPROJ");
		m_hWorld =			m_pEffect->GetParameterBySemantic(NULL, "WORLD");
		
		// Get Lighting Variables
		m_hLightCount =		m_pEffect->GetParameterBySemantic(NULL, "LIGHTCOUNT");
		m_hLightType =		m_pEffect->GetParameterBySemantic(NULL, "LIGHTTYPE");
		m_hLightVector =	m_pEffect->GetParameterBySemantic(NULL, "LIGHTVECTOR");
		m_hLightColor =		m_pEffect->GetParameterBySemantic(NULL, "LIGHTCOLOR");
		m_hAmbientColor =	m_pEffect->GetParameterBySemantic(NULL, "AMBIENTCOLOR");

		// Get Default Texture Variable
		m_hDefaultTexture =	m_pEffect->GetParameterBySemantic(NULL, "DEFAULTTEXTURE");

		m_hCurrentTechnique = m_hDefaultTechnique;
	}
}

void kpgShader::SetDefaultTexture(const kpgTexture* pTexture)
{
	if( m_hDefaultTexture )
	{
		m_pEffect->SetTexture(m_hDefaultTexture, pTexture->GetDeviceTexture());
	}
}

void kpgShader::SetMatrices(const kpuMatrix& mTransform, const kpuMatrix& mWorld)
{
	if( m_hWorldViewProj )
		m_pEffect->SetMatrix(m_hWorldViewProj, (const D3DXMATRIX*)&mTransform);
	if( m_hWorld )
		m_pEffect->SetMatrix(m_hWorld, (const D3DXMATRIX*)&mWorld);
}

void kpgShader::SetMatrixParam(const char* szSemantic, const kpuMatrix& mMatrix)
{
	D3DXHANDLE hMatrix = m_pEffect->GetParameterBySemantic(NULL, szSemantic);
	if( hMatrix )
		m_pEffect->SetMatrix(hMatrix, (const D3DXMATRIX*)&mMatrix);
}

void kpgShader::SetAmbientColor(const kpuVector& vAmbientColor)
{
	if( m_hAmbientColor )
	{
		m_pEffect->SetFloatArray(m_hAmbientColor, (const float*)&vAmbientColor, 4);
	}
}

void kpgShader::SetLights(const kpgLight** pLightArray)
{
	int nLightCount = 0;
	for( nLightCount; nLightCount < MAX_LIGHTS; nLightCount++ )
	{
		if( !pLightArray[nLightCount] )
			break;
	}

	if( m_hLightCount )
		m_pEffect->SetInt(m_hLightCount, nLightCount);

	int nLightTypes[MAX_LIGHTS];
	kpuVector vVectors[MAX_LIGHTS];
	kpuVector vColors[MAX_LIGHTS];
	for( int i = 0; i < nLightCount; i++ )
	{
		nLightTypes[i] = (int)pLightArray[i]->GetType();
		vVectors[i] = pLightArray[i]->GetPositionDirection();
		vColors[i] = pLightArray[i]->GetColor();
	}
	if( m_hLightType )
		m_pEffect->SetIntArray(m_hLightType, &nLightTypes[0], MAX_LIGHTS);
	if( m_hLightVector )
		m_pEffect->SetFloatArray(m_hLightVector, (const float*)&vVectors[0], 4 * MAX_LIGHTS);
	if( m_hLightColor)
		m_pEffect->SetFloatArray(m_hLightColor, (const float*)&vColors[0], 4 * MAX_LIGHTS);
}

void kpgShader::Bind()
{
	// Set the current technique
	m_pEffect->SetTechnique(m_hCurrentTechnique);

	HRESULT hRes = m_pEffect->Begin(&m_unPasses, 0);

	m_bBound = true;
}

void kpgShader::Unbind()
{
	if( m_bBound )
		m_pEffect->End();
	m_bBound = false;
}

void kpgShader::BeginPass(u32 unPass)
{
	// Start the pass
	m_pEffect->BeginPass(unPass);

	// Update shader variables
	m_pEffect->CommitChanges();
}

void kpgShader::EndPass()
{
	m_pEffect->EndPass();
}