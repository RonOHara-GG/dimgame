#include "StdAfx.h"
#include "Common\Graphics\kpgVertexBuffer.h"

kpgVertexBuffer::kpgVertexBuffer(void)
{
	m_pVertexFormat = 0;
	m_pBuffer = 0;
	m_bLocked = false;
}

kpgVertexBuffer::~kpgVertexBuffer(void)
{
	Unlock();

	if( m_pBuffer )
	{
		m_pBuffer->Release();
		m_pBuffer = 0;
	}

	if( m_pVertexFormat )
		delete m_pVertexFormat;
}

bool kpgVertexBuffer::Create(int nVertexCount, kpgRenderer::ePrimitiveType eType, u32 unFormatFlags, kpgRenderer* pRenderer)
{
	m_pVertexFormat = new kpgVertexFormat(unFormatFlags, pRenderer);

	if( pRenderer->GetDevice()->CreateVertexBuffer(m_pVertexFormat->GetStride() * nVertexCount, 0, 0, D3DPOOL_MANAGED, &m_pBuffer, 0) != D3D_OK )
	{
		delete m_pVertexFormat;
		m_pVertexFormat = 0;
		return false;
	}

	m_ePrimType = eType;
	m_nVertexCount = nVertexCount;
	m_nVertexStride = m_pVertexFormat->GetStride();

	return true;
}

void kpgVertexBuffer::Bind(kpgRenderer* pRenderer, int nStream)
{
	// Unlock the buffer
	Unlock();

	// Set the vertex format
	pRenderer->GetDevice()->SetVertexDeclaration(m_pVertexFormat->GetDeclaration());

	// Set the stream source
	pRenderer->GetDevice()->SetStreamSource(nStream, m_pBuffer, 0, m_pVertexFormat->GetStride());
}

void kpgVertexBuffer::Unbind(kpgRenderer* pRenderer, int nStream)
{
	// Clear the vertex buffer
	pRenderer->GetDevice()->SetStreamSource(nStream, 0, 0, 0);

	// Clear the vertex format
	//pRenderer->GetDevice()->SetVertexDeclaration(0);
}

bool kpgVertexBuffer::Lock()
{
	if( !m_bLocked )
	{
		m_bLocked = (m_pBuffer->Lock(0, 0, (void**)&m_pLockPointer, 0) == D3D_OK);
	}
	return m_bLocked;
}

bool kpgVertexBuffer::Unlock()
{
	if( m_bLocked )
	{
		m_bLocked = !(m_pBuffer->Unlock() == D3D_OK);
	}
	return !m_bLocked;
}

int kpgVertexBuffer::GetPrimCount(int nVertCount) const
{
	int nPrimCount;
	switch( m_ePrimType )
	{
		case kpgRenderer::ePT_PointList:
			nPrimCount = nVertCount;
			break;
		case kpgRenderer::ePT_LineList:
			nPrimCount = nVertCount >> 1;
			break;
		case kpgRenderer::ePT_LineStrip:
			nPrimCount = nVertCount - 1;
			break;
		case kpgRenderer::ePT_TriList:
			nPrimCount = nVertCount / 3;
			break;
		case kpgRenderer::ePT_TriStrip:
		case kpgRenderer::ePT_TriFan:
			nPrimCount = nVertCount - 2;
			break;
		default:
			nPrimCount = 0;
			break;
	}
	return nPrimCount;
}

void kpgVertexBuffer::SetPosition(int nVertIndex, const kpuVector& vPos, int nSetIndex)
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_Position, nSetIndex));
		pVertexPointer->SetX(vPos.GetX());
		pVertexPointer->SetY(vPos.GetY());
		pVertexPointer->SetZ(vPos.GetZ());
	}
}

const kpuVector& kpgVertexBuffer::GetPosition(int nVertIndex, int nSetIndex) const
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_Position, nSetIndex));
		return *pVertexPointer;
	}
	return kpuv_Zero;
}

void kpgVertexBuffer::SetNormal(int nVertIndex, const kpuVector& vNormal, int nSetIndex)
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_Normal, nSetIndex));
		pVertexPointer->SetX(vNormal.GetX());
		pVertexPointer->SetY(vNormal.GetY());
		pVertexPointer->SetZ(vNormal.GetZ());
	}
}

const kpuVector& kpgVertexBuffer::GetNormal(int nVertIndex, int nSetIndex) const
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_Normal, nSetIndex));
		return *pVertexPointer;
	}
	return kpuv_Zero;
}

void kpgVertexBuffer::SetUV(int nVertIndex, const kpuVector& vUV, int nSetIndex)
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_TexCoord, nSetIndex));
		pVertexPointer->SetX(vUV.GetX());
		pVertexPointer->SetY(vUV.GetY());
	}
}

const kpuVector& kpgVertexBuffer::GetUV(int nVertIndex, int nSetIndex) const
{
	if( m_bLocked )
	{
		kpuVector* pVertexPointer = (kpuVector*)(m_pLockPointer + (nVertIndex * m_pVertexFormat->GetStride()) + m_pVertexFormat->GetOffset(kpgVertexFormat::eVCT_TexCoord, nSetIndex));
		return *pVertexPointer;
	}

	return kpuv_Zero;
}