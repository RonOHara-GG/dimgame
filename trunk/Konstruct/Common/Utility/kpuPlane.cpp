#include "StdAfx.h"
#include "kpuPlane.h"

kpuPlane::kpuPlane(kpuVector v1, kpuVector v2, kpuVector v3)
{
	m_vNormal = kpuVector::Normalize(kpuVector::Cross(v2 - v1, v3 - v2));
	m_vPoint = v1;
}

kpuPlane::~kpuPlane(void)
{
}

void kpuPlane::ReCreate(kpuVector v1, kpuVector v2, kpuVector v3)
{
	m_vNormal = kpuVector::Normalize(kpuVector::Cross(v2 - v1, v3 - v2));
	m_vPoint = v1;
}

float kpuPlane::DistanceToPoint(kpuVector vOther)
{
	return ( vOther - m_vPoint ).Dot( m_vNormal );

}