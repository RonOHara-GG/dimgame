#pragma once

#include "Common/Utility/kpuVector.h"

class kpuBoundingSphere
{
public:
	kpuBoundingSphere(float fRadius. kpuVector vLoc);
	~kpuBoundingSphere(void);

	bool Intersect(kpuBoundingSphere& sphere);
protected:
	float		fRadius;
	kpuVector	m_vLocation;
};
