#pragma once
#include "Common\Graphics\kpgTexture.h"

class kppTexture : public kpgTexture
{
public:
	kppTexture(u32 unWidth, u32 unHeight);
	virtual ~kppTexture(void);

	virtual void Build();

protected:
	const BYTE*	m_pShaderData;
};
