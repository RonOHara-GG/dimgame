#include "StdAfx.h"
#include "kpuBoundingSphere.h"

kpuBoundingSphere::kpuBoundingSphere(float fRadius, kpuVector vLoc)
{
	m_fRadius = fRadius;
	m_vLocation = vLoc;
}

kpuBoundingSphere::~kpuBoundingSphere(void)
{
}

bool kpuBoundingSphere::Intersects(kpuBoundingSphere &sphere)
{
	float fDistSquared = kpuVector::DistanceSquared(m_vLocation, sphere.m_vLocation);
	float fCombinedRadiSqr = ( m_fRadius * m_fRadius ) + ( sphere.m_fRadius * sphere.m_fRadius );

	return fDistSquared < fCombinedRadiSqr;
}