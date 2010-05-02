#pragma once

#include "kpuVector.h"

class kpuBoundingSphere;

class kpuBoundingBox
{
public:
	kpuBoundingBox(kpuVector vMin = kpuv_Zero ,kpuVector vMax= kpuv_Zero );
	kpuBoundingBox(const kpuBoundingBox& bBox);
	~kpuBoundingBox(void);

	void operator=(const kpuBoundingBox& bBox);

	bool Intersects(kpuBoundingBox& bBox);
	bool Intersects(kpuBoundingSphere& bSphere);

	bool Contains2D(kpuBoundingBox& bBox);

	kpuVector GetMax()	{ return m_vMax; }
	kpuVector GetMin()	{ return m_vMin; }

	void Move(kpuVector vVel)	{ m_vMin +=  vVel; m_vMax += vVel; }

private:
	kpuVector m_vMax;
	kpuVector m_vMin;

};
