#include "StdAfx.h"
#include "kpuBoundingSphere.h"

kpuBoundingSphere::kpuBoundingSphere(float fRadius, kpuVector vLoc)
{
	m_fRadius = fRadius;
	m_vLocation = vLoc;
}

kpuBoundingSphere::kpuBoundingSphere(const kpuBoundingSphere& bSphere)
{
	m_fRadius = bSphere.m_fRadius;
	m_vLocation = bSphere.m_vLocation;

}
kpuBoundingSphere::~kpuBoundingSphere(void)
{
}

void kpuBoundingSphere::operator =(const kpuBoundingSphere& bSphere)
{
	m_fRadius = bSphere.m_fRadius;
	m_vLocation = bSphere.m_vLocation;
}

float kpuBoundingSphere::Intersects(kpuBoundingSphere &sphere)
{
	float fDistSquared = kpuVector::DistanceSquared(m_vLocation, sphere.m_vLocation);
	float fCombinedRadiSqr = ( m_fRadius * m_fRadius ) + ( sphere.m_fRadius * sphere.m_fRadius );

	return fDistSquared - fCombinedRadiSqr;
}