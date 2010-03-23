#pragma once

#include "Common\Graphics\kpgRenderer.h"

class kpgShader;
class kpgVertexBuffer;
class kpgIndexBuffer;

class kpgGeometry
{
public:
	kpgGeometry(void);
	virtual ~kpgGeometry(void);

	bool				CreateVertexBuffer(int nVertexCount, kpgRenderer::ePrimitiveType eType, u32 unFormatFlags, kpgRenderer* pRenderer);
	bool				CreateIndexBuffer(int nIndexCount);

	void				SetShader(kpgShader* pShader)	{ m_pShader = pShader; }
	kpgShader*			GetShader()						{ return m_pShader; }
	kpgVertexBuffer*	GetVertexBuffer()				{ return m_pVertexBuffer; }
	kpgIndexBuffer*		GetIndexBuffer()				{ return m_pIndexBuffer; }
	void				SetName(const char* szName)		{ m_uNameHash = StringHash(szName); }
	void				SetName(u32 uNameHash)			{ m_uNameHash = uNameHash; }
	u32					GetName() const					{ return m_uNameHash; }

protected:
	kpgShader*			m_pShader;
	kpgVertexBuffer*	m_pVertexBuffer;
	kpgIndexBuffer*		m_pIndexBuffer;
	u32					m_uNameHash;
};
