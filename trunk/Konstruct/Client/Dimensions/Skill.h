#pragma once
#include "DamageTypes.h"

class Actor;
class PlayerCharacter;
class Grid;

class Skill
{
public:
	Skill(void);
	~Skill(void);

	bool	MeetRequiredLevel(int iCurrentLevel) { return iCurrentLevel >= m_iRequiredLevel; }
	bool	Use(Actor* pTarget, PlayerCharacter* pSkillOwner, Grid* pGrid);
	void	UpdateTimers(float fGameTime);

protected:

	int			m_iRequiredLevel;
	int			m_iRange; //In tiles
	int			m_iRadius;  //In tiles
	int			m_iDamage;
	DamageType  m_eDamageType;
	float		m_fSpeed;
	float		m_fRecovery;
	float		m_fElaspedRecovery;
	bool		m_bReadyToUse;
};
