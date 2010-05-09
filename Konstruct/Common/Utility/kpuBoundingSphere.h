#pragma once
#include "kpuBoundingVolume.h"

class kpuBoundingSphere: public kpuBoundingVolume
{
public:
	kpuBoundingSphere(float fRadius = 0.0f, kpuVector vLoc = kpuv_Zero);
	kpuBoundingSphere(const kpuBoundingSphere& bSphere);
	~kpuBoundingSphere(void);

	void operator =(const kpuBoundingSphere& bSphere);

	kpuCollisionData Intersects(kpuBoundingVolume &bOther);

	void Transform(const kpuMatrix &matrix);

	void Move(kpuVector vVel)		    { m_vLocation +=  vVel; }
	kpuVector GetLocation()				{ return m_vLocation; }
	float	GetRadius()					{ return m_fRadius; }

protected:
	float		m_fRadius;
	kpuVector	m_vLocation;
};
