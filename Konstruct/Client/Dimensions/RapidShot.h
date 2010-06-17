#pragma once
#include "basicshot.h"
#include "Common/utility/kpuArrayList.h"

class RapidShot :
	public BasicShot
{
public:
	RapidShot(void);
	~RapidShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iShotCount;
	int		m_iShotMax;
	int		m_iShotMultiple;
	float	m_fSpeedMultiple;
	float	m_fPlayersSpeed;
};
