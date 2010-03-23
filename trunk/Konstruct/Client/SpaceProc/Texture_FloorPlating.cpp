#include "StdAfx.h"
#include "Texture_FloorPlating.h"

#include "Shaders\MetalFloor.sh"


Texture_FloorPlating::Texture_FloorPlating(u32 unWidth, u32 unHeight) : kppTexture(unWidth, unHeight)
{
	m_pShaderData = g_MetalFloorShaderData;
}

Texture_FloorPlating::~Texture_FloorPlating(void)
{
}
