#pragma once

#include "kpuVector.h"

class kpuBoundingSphere
{
public:
	kpuBoundingSphere(float fRadius = 0.0f, kpuVector vLoc = kpuv_Zero);
	~kpuBoundingSphere(void);

	bool Intersects(kpuBoundingSphere& sphere);
protected:
	float		m_fRadius;
	kpuVector	m_vLocation;
};
