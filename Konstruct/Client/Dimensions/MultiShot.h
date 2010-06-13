#pragma once
#include "BasicShot.h"
#include "Common/Utility/kpuArrayList.h"

class MultiShot:
	public BasicShot
{
public:
	MultiShot(void);
	~MultiShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int							m_iShotsToFire;
	float						m_fFireArc;
	float						m_fIntMultiple;
	float						m_fNumOfShotsMultiple;
	float						m_fFireArcMultiple;
};

#define SPEED_MULTIPLE 1.5
#define RECOVERY_MULTIPLE 2
#define MIN_SHOTS_FIRED 2