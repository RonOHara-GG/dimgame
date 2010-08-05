#pragma once

#include "Common\Utility\kpuMatrix.h"

#include <d3d9.h>

#define MAX_LIGHTS	4
#define MAX_BONES	32

typedef struct _kpgQuadUVs
{
	float	m_fTopLeftU, m_fTopLeftV;
	float	m_fTopRightU, m_fTopRightV;
	float	m_fBottomLeftU, m_fBottomLeftV;
	float	m_fBottomRightU, m_fBottomRightV;

	_kpgQuadUVs()	{}
	_kpgQuadUVs(float fTLU, float fTLV, float fTRU, float fTRV, float fBLU, float fBLV, float fBRU, float fBRV)
	{
		m_fTopLeftU = fTLU;			m_fTopLeftV = fTLV;
		m_fTopRightU = fTRU;		m_fTopRightV = fTRV;
		m_fBottomLeftU = fBLU;		m_fBottomLeftV = fBLV;
		m_fBottomRightU = fBRU;		m_fBottomRightV = fBRV;
	}
} kpgQuadUVs;

class kpgShader;
class kpgTexture;
class kpgVertexBuffer;
class kpgFont;
class kpgGeometryInstance;
class kpgLight;

class kpgRenderer
{
public:

	enum ePrimitiveType
	{
		ePT_PointList = 1,
		ePT_LineList,
		ePT_LineStrip,
		ePT_TriList,
		ePT_TriStrip,
		ePT_TriFan,
	};

	enum eDirtyFlags
	{
		eDF_None =			0,
		eDF_Matrices =		1,
		eDF_Lights =		2,
		eDF_AmbientLight =	4,
		
		eDF_AllDirty =		0xFFFFFFFF
	};

	enum eCullModes
	{
		eCM_None =				1,
		eCM_Clockwise =			2,
		eCM_CounterClockwise =	3,
	};

	kpgRenderer();
	~kpgRenderer(void);
	bool Create(HWND hWnd);

	// Singleton
	static kpgRenderer* kpgRenderer::GetInstance();

	// Frame operations
	void BeginFrame();
	void EndFrame();

	// Shader stuff
	kpgShader* SetShader(kpgShader* pShader);

	// Lighting
	void SetAmbientLightColor(const kpuVector& vAmbientColor);
	void SetLight(int nLightIndex, const kpgLight* pLight);

	// Immediate mode
	void DrawText2D(const char* szText, const kpRect& rRect, const kpgFont* pFont);
	void DrawQuad2D(const kpRect& rRect, const kpgTexture* pTexture = 0, kpgQuadUVs* pUVs = 0);

	void BeginImmediate(ePrimitiveType eType, int nVertexCount, bool bBindShader = true);
	void EndImmediate();

	void AddImmediateVertex(const kpuVector& vPos, const kpuVector& vUV);
	void AddImmediateVertex(const kpuVector& vPos, const kpuVector& vNormal, const kpuVector& vUV);

	// Geometry Drawing
	void DrawInstancedGeometry(kpgGeometryInstance* pInstance, kpuMatrix* pOffsetMatrix = 0);

	// Accessors
	IDirect3DDevice9*	GetDevice() const				{ return m_pDevice; }
	float				GetScreenWidth() const			{ return m_fScreenWidth; }
	float				GetScreenHeight() const			{ return m_fScreenHeight; }
	kpgShader*			GetDefaultShader() const		{ return m_pDefaultShader; }
	kpgShader*			GetImmediateShader() const		{ return m_pImmediateShader; }

	void				SetProjectionMatrix(const kpuMatrix& mProj);
	void				SetViewMatrix(const kpuMatrix& mView);
	void				SetWorldMatrix(const kpuMatrix& mWorld);

	const kpuMatrix&	GetProjectionMatrix() const		{ return m_mProjection; }
	const kpuMatrix&	GetViewMatrix() const			{ return m_mView; }
	const kpuMatrix&	GetWorldMatrix() const			{ return m_mWorld; }
	const kpuMatrix&	GetWorldViewProjMatrix() const	{ return m_mWorldViewProjection; }

	// Render State
	void				SetCullMode(eCullModes mode);

protected:
	IDirect3D9*			m_pD3D;
	IDirect3DDevice9*	m_pDevice;

	kpgTexture*			m_pMissingTexture;
	kpgShader*			m_pCurrentShader;
	kpgShader*			m_pDefaultShader;
	kpgShader*			m_pImmediateShader;
	kpgVertexBuffer*	m_pImmediateBuffer;
	int					m_nImmediateVertexCount;
	float				m_fScreenWidth;
	float				m_fScreenHeight;
	kpuMatrix			m_mProjection;
	kpuMatrix			m_mView;
	kpuMatrix			m_mWorld;
	kpuMatrix			m_mWorldViewProjection;
	u32					m_eDirtyFlags;

	kpuVector			m_vAmbientLightColor;
	const kpgLight*		m_pLights[MAX_LIGHTS];

	static kpgRenderer*	sm_pRenderer;
};