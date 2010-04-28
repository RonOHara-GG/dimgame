#pragma once

#include "Common/Utility/kpuVector.h"

class kpuBoundingSphere;

class kpuBoundingBox
{
public:
	kpuBoundingBox(float fX = 0.0f, float fZ = 0.0f, float fWidth = 0.0f, float fHeight = 0.0f);
	~kpuBoundingBox(void);

	bool Intersects(kpuBoundingBox& bBox);
	bool Intersects(kpuBoundingSphere& bSphere);

	kpuVector GetMax()	{ return m_vMax; }
	kpuVector GetMin()	{ return m_vMin; }

private:
	kpuVector m_vMax;
	kpuVector m_vMin;

};
