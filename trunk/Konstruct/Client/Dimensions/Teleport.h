#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"

class Teleport :
	public Skill
{
public:
	Teleport(void);
	~Teleport(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int	GetRange() { return m_iMinRange + m_iSkillRank;	}

	int					m_iDestinationTile;
	int					m_iMinRange;
};
