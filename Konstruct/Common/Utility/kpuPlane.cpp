#include "StdAfx.h"
#include "kpuPlane.h"

kpuPlane::kpuPlane(kpuVector v1, kpuVector v2, kpuVector v3)
{
	m_vNormal = kpuVector::Cross(v1 - v2, v1 - v3);
	m_vPoint = v1;
}

kpuPlane::~kpuPlane(void)
{
}
