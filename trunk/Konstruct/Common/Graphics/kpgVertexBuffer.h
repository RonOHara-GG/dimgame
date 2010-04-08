#pragma once

#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgVertexFormat.h"
#include "Common\Utility\kpuVector.h"

class kpgVertexBuffer
{
public:
	kpgVertexBuffer(void);
	~kpgVertexBuffer(void);

	bool Create(int nVertexCount, kpgRenderer::ePrimitiveType eType, u32 unFormatFlags, kpgRenderer* pRenderer);

	void Bind(kpgRenderer* pRenderer, int nStream = 0);
	void Unbind(kpgRenderer* pRenderer, int nStream = 0);
	bool Lock();
	bool Unlock();


	int GetVertexCount() const							{ return m_nVertexCount; }
	kpgRenderer::ePrimitiveType GetPrimType() const		{ return m_ePrimType; }
	int GetPrimCount(int nVertCount) const;

	void SetPosition(int nVertIndex, const kpuVector& vPos, int nSetIndex = 0);
	const kpuVector& GetPosition(int nVertIndex, int nSetIndex = 0) const;
	void SetNormal(int nVertIndex, const kpuVector& vNormal, int nSetIndex = 0);
	const kpuVector& GetNormal(int nVertIndex, int nSetIndex = 0) const;
	void SetUV(int nVertIndex, const kpuVector& vUV, int nSetIndex = 0);
	const kpuVector& GetUV(int nVertIndex, int nSetIndex = 0) const;

protected:
	kpgVertexFormat*				m_pVertexFormat;
	int								m_nVertexCount;
	int								m_nVertexStride;
	bool							m_bLocked;
	kpgRenderer::ePrimitiveType		m_ePrimType;
	IDirect3DVertexBuffer9*			m_pBuffer;
	u8*								m_pLockPointer;
};
