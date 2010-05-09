#pragma once
#include "Common\Utility\kpuBoundingVolume.h"

class kpuBoundingCapsule :
	public kpuBoundingVolume
{
public:
	kpuBoundingCapsule(kpuVector v1, kpuVector v2, float fRadius);
	~kpuBoundingCapsule(void);

	kpuCollisionData Intersects(kpuBoundingVolume &bOther);
	void Transform(const kpuMatrix& matrix);

	kpuVector GetStart() { return m_vStart; }
	kpuVector GetEnd()   { return m_vEnd; }
	float	  GetRadius() { return m_fRadius; }

protected:
	kpuVector m_vStart;
	kpuVector m_vEnd;
	float	  m_fRadius;

};
