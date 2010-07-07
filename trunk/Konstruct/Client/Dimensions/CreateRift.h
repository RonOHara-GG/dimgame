#pragma once
#include "basicshot.h"

class CreateRift :
	public BasicShot
{
public:
	CreateRift(void);
	~CreateRift(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return m_iMinRange + m_iSkillRank /  m_iRangeMod; }
	
	float		m_fMinDuration;

	float		m_fMinRadius;
	float		m_fRadiusMod;	
	
	int			m_iRangeMod;
	int			m_iMinRange;
	
	int			m_iResistMod;
	int			m_iMinResist;

};
