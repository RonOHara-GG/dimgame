#pragma once
#include "kpuVector.h"

class kpuPhysicalObject;

class kpuCollisionData
{
public:

	kpuCollisionData(void)
	{m_fDist = 0.0f; m_bCollided = false; m_pObject = 0; }

	~kpuCollisionData(void)
	{	}

	float m_fDist;
	float m_fVelLength;
	kpuVector m_vVelocity;
	bool m_bCollided;
	kpuPhysicalObject* m_pObject;
};
