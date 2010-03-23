#include "StdAfx.h"
#include "Common\Procedural\kppPlane.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"

kppPlane::kppPlane(void)
{
}

kppPlane::kppPlane(float fWidth, float fHeight, const kpuVector& vNormal)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_vNormal = vNormal;
	m_vNormal.Normalize();
}

kppPlane::~kppPlane(void)
{
}

void kppPlane::Build()
{
	m_pVertexBuffer = new kpgVertexBuffer();
	m_pVertexBuffer->Create(4, kpgRenderer::ePT_TriList, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0, kpgRenderer::GetInstance());
	m_pVertexBuffer->Lock();

	//	   0--------1
	//	  /        /
	//	 /		  /
	//	2--------3

	float fHalfWidth = m_fWidth * 0.5f;
	float fHalfHeight = m_fHeight * 0.5f;

	// Find the plane vectors
	kpuVector right = m_vNormal % kpuv_OneY;
	if( right == kpuv_Zero )
		right = m_vNormal % kpuv_OneZ;
	kpuVector up = right % m_vNormal;

	kpuVector vLeftEdge = right * -fHalfWidth;
	kpuVector vRightEdge = right * fHalfWidth;
	kpuVector vTopEdge = up * fHalfHeight;
	kpuVector vBottomEdge = up * -fHalfHeight;
	
	kpuVector vVertex0 = vLeftEdge + vTopEdge;
	kpuVector vVertex1 = vRightEdge + vTopEdge;
	kpuVector vVertex2 = vLeftEdge + vBottomEdge;
	kpuVector vVertex3 = vRightEdge + vBottomEdge;

	m_pVertexBuffer->SetPosition(0, vVertex0);		m_pVertexBuffer->SetUV(0, kpuv_Zero);
	m_pVertexBuffer->SetPosition(1, vVertex1);		m_pVertexBuffer->SetUV(1, kpuv_OneX);
	m_pVertexBuffer->SetPosition(2, vVertex2);		m_pVertexBuffer->SetUV(2, kpuv_OneY);
	m_pVertexBuffer->SetPosition(3, vVertex3);		m_pVertexBuffer->SetUV(3, kpuVector(1.0f, 1.0f, 0.0f, 0.0f));
	m_pVertexBuffer->Unlock();


	m_pIndexBuffer = new kpgIndexBuffer();
	m_pIndexBuffer->Create(6);
	m_pIndexBuffer->Lock();

	m_pIndexBuffer->SetIndex(0, 0);
	m_pIndexBuffer->SetIndex(1, 1);
	m_pIndexBuffer->SetIndex(2, 2);
	m_pIndexBuffer->SetIndex(3, 1);
	m_pIndexBuffer->SetIndex(4, 3);
	m_pIndexBuffer->SetIndex(5, 2);

	m_pIndexBuffer->Unlock();


	kppGeometry::Build();
}
