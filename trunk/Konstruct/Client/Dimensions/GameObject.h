#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuBoundingBox.h"
#include "Common/Utility/kpuBoundingSphere.h"

class GameObject
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	kpuBoundingBox	GetBoundingBox()	{ return m_bBox; }

protected:
	kpuBoundingBox		m_bBox;
	kpuBoundingSphere	m_bSphere;

};
