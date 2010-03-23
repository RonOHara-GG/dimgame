#include "StdAfx.h"
#include "Common\Procedural\kppGeometry.h"
#include "Common\Utility\kpuVector.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"

kppGeometry::kppGeometry(void)
{
}

kppGeometry::~kppGeometry(void)
{
}

void kppGeometry::Build()
{
	if( m_pIndexBuffer && m_pVertexBuffer )
	{
		// Compute normals
		// This assumes that all procedural geometry is traingle lists for now
		// it may be nice in the future to check the prim type and build accordingly

		m_pIndexBuffer->Lock();
		m_pVertexBuffer->Lock();

		int nTriangleCount = m_pIndexBuffer->GetIndexCount() / 3;
		int nVertexCount = m_pVertexBuffer->GetVertexCount();
		kpuVector* pNormals = (kpuVector*)malloc(sizeof(kpuVector) * nVertexCount);
		memset(pNormals, 0, sizeof(kpuVector) * nVertexCount);

		for( int i = 0; i < nTriangleCount; i++ )
		{
			int nTri = i * 3;
			int nIndex0 = m_pIndexBuffer->GetIndex(nTri + 0);
			int nIndex1 = m_pIndexBuffer->GetIndex(nTri + 1);
			int nIndex2 = m_pIndexBuffer->GetIndex(nTri + 2);

			const kpuVector& v0 = m_pVertexBuffer->GetPosition(nIndex0);
			const kpuVector& v1 = m_pVertexBuffer->GetPosition(nIndex1);
			const kpuVector& v2 = m_pVertexBuffer->GetPosition(nIndex2);

			kpuVector vEdge01 = v1 - v0;
			kpuVector vEdge02 = v2 - v0;
			vEdge01.Normalize();
			vEdge02.Normalize();

			kpuVector vNormal = vEdge01 % vEdge02;
			vNormal.Normalize();

			pNormals[nIndex0] += vNormal;
			pNormals[nIndex1] += vNormal;
			pNormals[nIndex2] += vNormal;
		}

		for( int i = 0; i < nVertexCount; i++ )
		{
			pNormals[i].Normalize();
			m_pVertexBuffer->SetNormal(i, pNormals[i]);
		}

		free(pNormals);

		m_pIndexBuffer->Unlock();
		m_pVertexBuffer->Unlock();
	}
}
