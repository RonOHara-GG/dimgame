#include "StdAfx.h"
#include "Common\Procedural\kppTerrain.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"

kppTerrain::kppTerrain(void)
{
}

kppTerrain::kppTerrain(int iPolesWide, float fWidthBetweenPoles, int iPolesHigh, float fHeightBetweenPoles)
{
	SetPoles(iPolesWide, fWidthBetweenPoles, iPolesHigh, fHeightBetweenPoles);
}

kppTerrain::~kppTerrain(void)
{
}

void kppTerrain::Build()
{
	int iVertexCount = m_iPolesWide * m_iPolesHigh;

	m_pVertexBuffer = new kpgVertexBuffer();
	m_pVertexBuffer->Create(iVertexCount, kpgRenderer::ePT_TriList, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0, kpgRenderer::GetInstance());
	m_pVertexBuffer->Lock();

	int iIndex = 0;
	float fZ = 0;
	float fV = 0;
	for( int h = 0; h < m_iPolesHigh; h++ )
	{
		float fX = 0;
		float fU = 0;
		for( int w = 0; w < m_iPolesWide; w++ )
		{
			m_pVertexBuffer->SetPosition(iIndex, kpuVector(fX, 0.0f, fZ, 1.0f));
			m_pVertexBuffer->SetNormal(iIndex, kpuv_OneY);
			m_pVertexBuffer->SetUV(iIndex, kpuVector(fU, fV, 0.0f, 0.0f));
			iIndex++;

			fX += m_fPoleWidth;
			fU = (float)(w + 1) / (float)m_iPolesWide;
		}

		fZ += m_fPoleHeight;
		fV = (float)(h + 1) / (float)m_iPolesHigh;
	}

	m_pVertexBuffer->Unlock();


	int iTriangles = (m_iPolesWide - 1) * (m_iPolesHigh - 1) * 2;

	m_pIndexBuffer = new kpgIndexBuffer();
	m_pIndexBuffer->Create(iTriangles * 3);
	m_pIndexBuffer->Lock();

	unsigned short usVert = 0;
	iIndex = 0;
	for( int h = 0; h < m_iPolesHigh - 1; h++ )
	{
		for( int w = 0; w < m_iPolesWide - 1; w++ )
		{
			m_pIndexBuffer->SetIndex(iIndex++, usVert);
			m_pIndexBuffer->SetIndex(iIndex++, usVert + 1);
			m_pIndexBuffer->SetIndex(iIndex++, usVert + m_iPolesWide);
			
			m_pIndexBuffer->SetIndex(iIndex++, usVert + 1);
			m_pIndexBuffer->SetIndex(iIndex++, usVert + 1 + m_iPolesWide);
			m_pIndexBuffer->SetIndex(iIndex++, usVert + m_iPolesWide);

			usVert++;
		}
		usVert++;
	}
	m_pIndexBuffer->Unlock();

	kppGeometry::Build();
}
