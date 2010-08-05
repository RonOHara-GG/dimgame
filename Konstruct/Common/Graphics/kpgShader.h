#pragma once

#include <d3d9.h>
#include <d3dx9effect.h>
#include "Common/Utility/kpuFixedArray.h"

class kpgTexture;
class kpgRenderer;
class kpuMatrix;
class kpuVector;
class kpgLight;

class kpgShader
{
public:
	kpgShader(void);
	~kpgShader(void);

	void LoadFromFile(kpgRenderer* pRenderer, const char* pszFilename);
	void LoadFromMemory(kpgRenderer* pRenderer, const BYTE* pShaderData, u32 nShaderDataSize);

	
	void SetDefaultTexture(const kpgTexture* pTexture);
	void SetTexture(const kpgTexture* pTexture);
	void SetMatrices(const kpuMatrix& mTransform, const kpuMatrix& mWorld);
	void SetMatrixParam(const char* szSemantic, const kpuMatrix& mMatrix);
	void SetAmbientColor(const kpuVector& vAmbientColor);
	void SetLights(const kpgLight** pLightArray);
	void SetSkinningMatricies(kpuFixedArray<kpuMatrix>* paMatricies);

	void Bind();
	void Unbind();

	u32 GetPassCount() const				{ return m_unPasses; }
	void BeginPass(u32 unPass);
	void EndPass();

protected:
	ID3DXEffect*	m_pEffect;
	D3DXHANDLE		m_hCurrentTechnique;
	u32				m_unPasses;
	bool			m_bBound;

	D3DXHANDLE		m_hDefaultTechnique;
	D3DXHANDLE		m_hWorldViewProj;
	D3DXHANDLE		m_hWorld;
	D3DXHANDLE		m_hLightCount;
	D3DXHANDLE		m_hLightType;
	D3DXHANDLE		m_hLightVector;
	D3DXHANDLE		m_hLightColor;
	//D3DXHANDLE		m_hLightRange;
	//D3DXHANDLE		m_hLightAttenuation
	D3DXHANDLE		m_hAmbientColor;	
	D3DXHANDLE		m_hDefaultTexture;
	D3DXHANDLE		m_hSkinningMatricies;
	kpgTexture*		m_pDefaultTexture;

};

// Shader Data Declarations
extern const BYTE g_DefaultShaderShaderData[];
extern const BYTE g_ImmediateModeShaderData[];

extern const u32 g_DefaultShaderShaderDataSize;
extern const u32 g_ImmediateModeShaderDataSize;