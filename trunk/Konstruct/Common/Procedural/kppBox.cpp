#include "StdAfx.h"
#include "Common\Procedural\kppBox.h"
#include "Common\Graphics\kpgVertexBuffer.h"
#include "Common\Graphics\kpgIndexBuffer.h"

kppBox::kppBox(void)
{
}

kppBox::kppBox(float fWidth, float fHeight, float fDepth)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
}

kppBox::~kppBox(void)
{
}

void kppBox::Build()
{
	m_pVertexBuffer = new kpgVertexBuffer();
	m_pVertexBuffer->Create(14, kpgRenderer::ePT_TriList, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0, kpgRenderer::GetInstance());
	m_pVertexBuffer->Lock();
   
	//				+y
	//			0-------1
	//			| \	  /	|
	//			|	2	|
	//			| /	  \	|
	//  0-------3-------4-------1
	//	| \	  /	| \	  /	| \	  /	|
	//-x|	13	|	5 -z|	12	|+x
	//	| /	  \	| /	  \	| /	  \	|
	//  9-------6-------7-------10
	//			| \	  /	|
	//			|	8	|-y
	//			| /	  \	|
	//			9-------10
	//			| \	  /	|
	//			|   11	|+z
	//			| /	  \	|
	//		    0-------1


	float fHalfWidth = m_fWidth * 0.5f;
	float fHalfHeight = m_fHeight * 0.5f;
	float fHalfDepth = m_fHeight * 0.5f;

	m_pVertexBuffer->SetPosition(0, kpuVector(-fHalfWidth,  fHalfHeight,  fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(1, kpuVector( fHalfWidth,  fHalfHeight,  fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(2, kpuVector(		 0.0f,  fHalfHeight,        0.0f, 0.0f));
	m_pVertexBuffer->SetPosition(3, kpuVector(-fHalfWidth,  fHalfHeight, -fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(4, kpuVector( fHalfWidth,  fHalfHeight, -fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(5, kpuVector(       0.0f,         0.0f, -fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(6, kpuVector(-fHalfWidth, -fHalfHeight, -fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(7, kpuVector( fHalfWidth, -fHalfHeight, -fHalfDepth, 0.0f));	
	m_pVertexBuffer->SetPosition(8, kpuVector(       0.0f, -fHalfHeight,        0.0f, 0.0f));
	m_pVertexBuffer->SetPosition(9, kpuVector(-fHalfWidth, -fHalfHeight,  fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(10,kpuVector( fHalfWidth, -fHalfHeight,  fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(11,kpuVector(       0.0f,         0.0f,  fHalfDepth, 0.0f));
	m_pVertexBuffer->SetPosition(12,kpuVector( fHalfWidth,         0.0f,        0.0f, 0.0f));
	m_pVertexBuffer->SetPosition(13,kpuVector(-fHalfWidth,         0.0f,        0.0f, 0.0f));

	kpuVector vMidUV(0.5f, 0.5f, 0.0f, 0.0f);
	kpuVector vEndUV(1.0f, 1.0f, 0.0f, 0.0f);
	m_pVertexBuffer->SetUV(0, kpuv_OneY	);
	m_pVertexBuffer->SetUV(1, vEndUV	);
	m_pVertexBuffer->SetUV(2, vMidUV	);
	m_pVertexBuffer->SetUV(3, kpuv_Zero	);
	m_pVertexBuffer->SetUV(4, kpuv_OneX	);
	m_pVertexBuffer->SetUV(5, vMidUV	);
	m_pVertexBuffer->SetUV(6, kpuv_OneY	);
	m_pVertexBuffer->SetUV(7, vEndUV	);	
	m_pVertexBuffer->SetUV(8, vMidUV	);
	m_pVertexBuffer->SetUV(9, kpuv_Zero	);
	m_pVertexBuffer->SetUV(10,kpuv_OneX	);
	m_pVertexBuffer->SetUV(11,vMidUV	);
	m_pVertexBuffer->SetUV(12,vMidUV	);
	m_pVertexBuffer->SetUV(13,vMidUV	);
	m_pVertexBuffer->Unlock();

	m_pIndexBuffer = new kpgIndexBuffer();
	m_pIndexBuffer->Create(72);
	m_pIndexBuffer->Lock();

	// +y Top Face
	m_pIndexBuffer->SetIndex(0, 0);
	m_pIndexBuffer->SetIndex(1, 1);
	m_pIndexBuffer->SetIndex(2, 2);

	m_pIndexBuffer->SetIndex(3, 1);
	m_pIndexBuffer->SetIndex(4, 4);
	m_pIndexBuffer->SetIndex(5, 2);

	m_pIndexBuffer->SetIndex(6, 4);
	m_pIndexBuffer->SetIndex(7, 3);
	m_pIndexBuffer->SetIndex(8, 2);

	m_pIndexBuffer->SetIndex(9, 3);
	m_pIndexBuffer->SetIndex(10, 0);
	m_pIndexBuffer->SetIndex(11, 2);

	// -z Front Face
	m_pIndexBuffer->SetIndex(12, 3);
	m_pIndexBuffer->SetIndex(13, 4);
	m_pIndexBuffer->SetIndex(14, 5);

	m_pIndexBuffer->SetIndex(15, 4);
	m_pIndexBuffer->SetIndex(16, 7);
	m_pIndexBuffer->SetIndex(17, 5);

	m_pIndexBuffer->SetIndex(18, 7);
	m_pIndexBuffer->SetIndex(19, 6);
	m_pIndexBuffer->SetIndex(20, 5);

	m_pIndexBuffer->SetIndex(21, 6);
	m_pIndexBuffer->SetIndex(22, 3);
	m_pIndexBuffer->SetIndex(23, 5);

	// -y Bottom Face
	m_pIndexBuffer->SetIndex(24, 6);
	m_pIndexBuffer->SetIndex(25, 7);
	m_pIndexBuffer->SetIndex(26, 8);

	m_pIndexBuffer->SetIndex(27, 7);
	m_pIndexBuffer->SetIndex(28, 10);
	m_pIndexBuffer->SetIndex(29, 8);

	m_pIndexBuffer->SetIndex(30, 10);
	m_pIndexBuffer->SetIndex(31, 9);
	m_pIndexBuffer->SetIndex(32, 8);

	m_pIndexBuffer->SetIndex(33, 9);
	m_pIndexBuffer->SetIndex(34, 6);
	m_pIndexBuffer->SetIndex(35, 8);

	// +z Back Face
	m_pIndexBuffer->SetIndex(36, 9);
	m_pIndexBuffer->SetIndex(37, 10);
	m_pIndexBuffer->SetIndex(38, 11);

	m_pIndexBuffer->SetIndex(39, 10);
	m_pIndexBuffer->SetIndex(40, 1);
	m_pIndexBuffer->SetIndex(41, 11);

	m_pIndexBuffer->SetIndex(42, 1);
	m_pIndexBuffer->SetIndex(43, 0);
	m_pIndexBuffer->SetIndex(44, 11);

	m_pIndexBuffer->SetIndex(45, 0);
	m_pIndexBuffer->SetIndex(46, 9);
	m_pIndexBuffer->SetIndex(47, 11);

	// +x Right Face
	m_pIndexBuffer->SetIndex(48, 4);
	m_pIndexBuffer->SetIndex(49, 1);
	m_pIndexBuffer->SetIndex(50, 12);

	m_pIndexBuffer->SetIndex(51, 1);
	m_pIndexBuffer->SetIndex(52, 10);
	m_pIndexBuffer->SetIndex(53, 12);

	m_pIndexBuffer->SetIndex(54, 10);
	m_pIndexBuffer->SetIndex(55, 7);
	m_pIndexBuffer->SetIndex(56, 12);

	m_pIndexBuffer->SetIndex(57, 7);
	m_pIndexBuffer->SetIndex(58, 4);
	m_pIndexBuffer->SetIndex(59, 12);

	// -x Left Face
	m_pIndexBuffer->SetIndex(60, 0);
	m_pIndexBuffer->SetIndex(61, 3);
	m_pIndexBuffer->SetIndex(62, 13);

	m_pIndexBuffer->SetIndex(63, 3);
	m_pIndexBuffer->SetIndex(64, 6);
	m_pIndexBuffer->SetIndex(65, 13);

	m_pIndexBuffer->SetIndex(66, 6);
	m_pIndexBuffer->SetIndex(67, 9);
	m_pIndexBuffer->SetIndex(68, 13);

	m_pIndexBuffer->SetIndex(69, 9);
	m_pIndexBuffer->SetIndex(70, 0);
	m_pIndexBuffer->SetIndex(71, 13);
	m_pIndexBuffer->Unlock();

	kppGeometry::Build();
}
