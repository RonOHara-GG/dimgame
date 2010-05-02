#pragma once

#include "kpuVector.h"

class kpuBoundingSphere
{
public:
	kpuBoundingSphere(float fRadius = 0.0f, kpuVector vLoc = kpuv_Zero);
	kpuBoundingSphere(const kpuBoundingSphere& bSphere);
	~kpuBoundingSphere(void);

	void operator =(const kpuBoundingSphere& bSphere);

	float Intersects(kpuBoundingSphere& sphere);

	void Move(kpuVector vVel)		{ m_vLocation +=  vVel; }
	kpuVector GetLocation()				{ return m_vLocation; }
	float	GetRadius()					{ return m_fRadius; }

protected:
	float		m_fRadius;
	kpuVector	m_vLocation;
};
