#pragma once

#include <d3d9.h>

class kpgRenderer;

enum eVertexFormatFlag
{
	eVF_Position0	= 0x00000001,
	eVF_Position1	= 0x00000002,
	eVF_Position2	= 0x00000004,
	eVF_Position3	= 0x00000008,
	eVF_Normal0		= 0x00000010,
	eVF_Normal1		= 0x00000020,
	eVF_Normal2		= 0x00000040,
	eVF_Normal3		= 0x00000080,
	eVF_Color0		= 0x00000100,
	eVF_Color1		= 0x00000200,
	eVF_Color2		= 0x00000400,
	eVF_Color3		= 0x00000800,
	eVF_TexCoord0	= 0x00001000,
	eVF_TexCoord1	= 0x00002000,
	eVF_TexCoord2	= 0x00004000,
	eVF_TexCoord3	= 0x00008000,
};

class kpgVertexFormat
{
public:
	enum eVertexComponentType
	{
		eVCT_Position,
		eVCT_Normal,
		eVCT_Color,
		eVCT_TexCoord,

		eVCT_Last,
	};

	kpgVertexFormat(u32 unFormatFlags, kpgRenderer* pRenderer);
	~kpgVertexFormat(void);

	int GetStride() const											{ return m_nStride; }
	int GetOffset(eVertexComponentType eType, int nIndex) const		{ return m_nOffsets[eType][nIndex]; }

	IDirect3DVertexDeclaration9* GetDeclaration() const				{ return m_pDeclaration; }

protected:
	int		m_nStride;
	int		m_nOffsets[eVCT_Last][4];

	IDirect3DVertexDeclaration9*		m_pDeclaration;
};
