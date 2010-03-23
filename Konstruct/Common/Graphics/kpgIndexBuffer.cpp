#include "StdAfx.h"
#include "Common\Graphics\kpgIndexBuffer.h"
#include "Common\Graphics\kpgRenderer.h"

kpgIndexBuffer::kpgIndexBuffer(void)
{
	m_pIndexBuffer = 0;
	m_nIndexCount = 0;
	m_bLocked = false;
}

kpgIndexBuffer::~kpgIndexBuffer(void)
{
	if( m_pIndexBuffer )
		m_pIndexBuffer->Release();
}

bool kpgIndexBuffer::Create(int nIndexCount)
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	if( pRenderer->GetDevice()->CreateIndexBuffer(nIndexCount * sizeof(unsigned short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0) != D3D_OK )
	{
		return false;
	}

	m_nIndexCount = nIndexCount;

	return true;
}

bool kpgIndexBuffer::Lock()
{
	if( !m_bLocked )
	{
		m_bLocked = m_pIndexBuffer->Lock(0, 0, (void**)&m_pLockPtr, 0) == D3D_OK;
	}
	return m_bLocked;
}

bool kpgIndexBuffer::Unlock()
{
	if( m_bLocked )
	{
		m_bLocked = !(m_pIndexBuffer->Unlock() == D3D_OK);
	}
	return !m_bLocked;
}

void kpgIndexBuffer::SetIndex(int nIndex, unsigned short nValue)
{
	if( m_bLocked )
	{
		m_pLockPtr[nIndex] = nValue;
	}
}

int kpgIndexBuffer::GetIndex(int nIndex) const
{
	if( m_bLocked && nIndex >= 0 && nIndex < m_nIndexCount )
	{
		return m_pLockPtr[nIndex];
	}

	return -1;
}