#pragma once
#include "BasicShot.h"
#include "Common/Utility/kpuArrayList.h"

class MultiShot:
	public BasicShot
{
public:
	MultiShot(void);
	~MultiShot(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float						m_fIntMultiple;
	int							m_iNumOfShotsMultiple;
	float						m_fFireArcMultiple;
};
#define MIN_SHOTS_FIRED 2