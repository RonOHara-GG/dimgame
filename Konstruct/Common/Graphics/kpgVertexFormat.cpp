#include "StdAfx.h"
#include "Common\Graphics\kpgVertexFormat.h"
#include "Common\Graphics\kpgRenderer.h"

kpgVertexFormat::kpgVertexFormat(u32 unFormatFlags, kpgRenderer* pRenderer)
{
	int nOffset = 0;
	D3DVERTEXELEMENT9 elements[33];
	int nCurElement = 0;

	// Calc position flags
	for( int i = 0; i < 4; i++ )
	{
		if( unFormatFlags & (eVF_Position0 << i) )
		{
			m_nOffsets[eVCT_Position][i] = nOffset;

			elements[nCurElement].Stream = 0;		// Only supporting single stream for now
			elements[nCurElement].Offset = nOffset;
			elements[nCurElement].Type = D3DDECLTYPE_FLOAT3;
			elements[nCurElement].Method = D3DDECLMETHOD_DEFAULT;
			elements[nCurElement].Usage = D3DDECLUSAGE_POSITION;
			elements[nCurElement].UsageIndex = i;

			nOffset += 12;
			nCurElement++;
		}
	}

	// Calc normal flags
	for( int i = 0; i < 4; i++ )
	{
		if( unFormatFlags & (eVF_Normal0 << i) )
		{
			m_nOffsets[eVCT_Normal][i] = nOffset;

			elements[nCurElement].Stream = 0;		// Only supporting single stream for now
			elements[nCurElement].Offset = nOffset;
			elements[nCurElement].Type = D3DDECLTYPE_FLOAT3;
			elements[nCurElement].Method = D3DDECLMETHOD_DEFAULT;
			elements[nCurElement].Usage = D3DDECLUSAGE_NORMAL;
			elements[nCurElement].UsageIndex = i;

			nOffset += 12;
			nCurElement++;
		}
	}

	// Calc color flags
	for( int i = 0; i < 4; i++ )
	{
		if( unFormatFlags & (eVF_Color0 << i) )
		{			
			m_nOffsets[eVCT_Color][i] = nOffset;

			elements[nCurElement].Stream = 0;		// Only supporting single stream for now
			elements[nCurElement].Offset = nOffset;
			elements[nCurElement].Type = D3DDECLTYPE_FLOAT4;
			elements[nCurElement].Method = D3DDECLMETHOD_DEFAULT;
			elements[nCurElement].Usage = D3DDECLUSAGE_COLOR;
			elements[nCurElement].UsageIndex = i;

			nOffset += 16;
			nCurElement++;
		}
	}

	// Calc tex coord flags
	for( int i = 0; i < 4; i++ )
	{
		if( unFormatFlags & (eVF_TexCoord0 << i) )
		{
			m_nOffsets[eVCT_TexCoord][i] = nOffset;

			elements[nCurElement].Stream = 0;		// Only supporting single stream for now
			elements[nCurElement].Offset = nOffset;
			elements[nCurElement].Type = D3DDECLTYPE_FLOAT2;
			elements[nCurElement].Method = D3DDECLMETHOD_DEFAULT;
			elements[nCurElement].Usage = D3DDECLUSAGE_TEXCOORD;
			elements[nCurElement].UsageIndex = i;

			nOffset += 8;
			nCurElement++;
		}
	}

	elements[nCurElement].Stream = 0xFF;
	elements[nCurElement].Offset = 0;
	elements[nCurElement].Type = D3DDECLTYPE_UNUSED;
	elements[nCurElement].Method = 0;
	elements[nCurElement].Usage = 0;
	elements[nCurElement].UsageIndex = 0;

	m_nStride = nOffset;

	pRenderer->GetDevice()->CreateVertexDeclaration(elements, &m_pDeclaration);
}

kpgVertexFormat::~kpgVertexFormat(void)
{
}
