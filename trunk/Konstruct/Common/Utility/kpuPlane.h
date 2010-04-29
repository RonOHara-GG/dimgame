#pragma once
#include "Common/utility/kpuVector.h"

class kpuPlane
{
public:
	kpuPlane(void);
	~kpuPlane(void);

	kpuVector GetNorm()	{ return m_vNormal; }

protected:
	kpuVector	m_vNormal;
};
