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

#define SPEED_MULTIPLE 1.5f
#define RECOVERY_MULTIPLE 2
#define MIN_SHOTS_FIRED 2