#pragma once

#include "kpuVector.h"

class kpuBoundingSphere;

class kpuBoundingBox
{
public:
	kpuBoundingBox(kpuVector vMin = kpuv_Zero ,kpuVector vMax= kpuv_Zero );
	~kpuBoundingBox(void);

	bool Intersects(kpuBoundingBox& bBox);
	bool Intersects(kpuBoundingSphere& bSphere);

	bool Contains(kpuBoundingBox& bBox);

	kpuVector GetMax()	{ return m_vMax; }
	kpuVector GetMin()	{ return m_vMin; }

private:
	kpuVector m_vMax;
	kpuVector m_vMin;

};
