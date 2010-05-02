#pragma once
#include "Common/utility/kpuVector.h"

class kpuPlane
{
public:
	kpuPlane(kpuVector v1, kpuVector v2, kpuVector v3);
	~kpuPlane(void);

	kpuVector GetNorm()	{ return m_vNormal; }
	kpuVector GetPoint() { return m_vPoint; }

protected:
	kpuVector	m_vNormal;
	kpuVector	m_vPoint;
};
