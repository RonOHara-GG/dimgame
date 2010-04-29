#include "StdAfx.h"
#include "kpuRay.h"

kpuRay::kpuRay(kpuVector vOrigin, kpuVector vDir)
{
	m_vOrigin = vOrigin;
	m_vDirection = vDir;
}

kpuRay::~kpuRay(void)
{
}
