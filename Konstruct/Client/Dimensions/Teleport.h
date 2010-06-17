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
	PlayerCharacter*	m_pTarget;
	kpuVector			m_vTarget;
	bool				m_bTargetSelected;
	float				m_fRange;
	float				m_fMinRange;
};
