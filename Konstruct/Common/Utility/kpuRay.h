#pragma once
#include "Common/Utility/kpuVector.h"

class kpuRay
{
public:
	kpuRay(kpuVector vOrigin, kpuVector vDir);
	~kpuRay(void);

	kpuVector	GetPoint(float fDist)	{ return m_vOrigin + (m_vDirection * fDist); }

protected:
	kpuVector	m_vOrigin;
	kpuVector	m_vDirection;
};
