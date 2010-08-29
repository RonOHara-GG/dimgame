#include "StdAfx.h"
#include "Common\Graphics\kpgGeometry.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgShader.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"

kpgGeometry::kpgGeometry(void)
{
	m_pShader = kpgRenderer::GetInstance()->GetDefaultShader();
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_uNameHash = 0;
}

kpgGeometry::~kpgGeometry(void)
{
	//if( m_pShader != kpgRenderer::GetInstance()->GetDefaultShader() )
	//{
	//	delete m_pShader;
	//}

	if( m_pVertexBuffer )
		delete m_pVertexBuffer;
	if( m_pIndexBuffer )
		delete m_pIndexBuffer;
}

bool kpgGeometry::CreateVertexBuffer(int nVertexCount, kpgRenderer::ePrimitiveType eType, u32 unFormatFlags, kpgRenderer *pRenderer)
{
	if( m_pVertexBuffer )
		delete m_pVertexBuffer;

	m_pVertexBuffer = new kpgVertexBuffer();
	if( !m_pVertexBuffer->Create(nVertexCount, eType, unFormatFlags, pRenderer) )
	{
		delete m_pVertexBuffer;
		m_pVertexBuffer = 0;
	}
	return (m_pVertexBuffer != 0);
}

bool kpgGeometry::CreateIndexBuffer(int nIndexCount)
{
	if( m_pIndexBuffer )
		delete m_pIndexBuffer;

	m_pIndexBuffer = new kpgIndexBuffer();
	if( !m_pIndexBuffer->Create(nIndexCount) )
	{
		delete m_pIndexBuffer;
		m_pIndexBuffer = 0;
	}
	return (m_pIndexBuffer != 0);
}