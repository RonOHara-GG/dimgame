#pragma once
#include "Common/utility/kpuVector.h"

class kpuPlane
{
public:
	kpuPlane(kpuVector v1, kpuVector v2, kpuVector v3);
	~kpuPlane(void);

	void	  ReCreate(kpuVector v1, kpuVector v2, kpuVector v3);
	float	  DistanceToPoint(kpuVector vOther); //Get the distance from the plane to a given point
	kpuVector GetNorm()	{ return m_vNormal; }
	kpuVector GetPoint() { return m_vPoint; }

protected:
	kpuVector	m_vNormal;
	kpuVector	m_vPoint;
};
