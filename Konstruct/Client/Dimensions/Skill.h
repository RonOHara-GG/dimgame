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

	bool	IsReady() { return m_bReady; }
	bool	MeetRequiredLevel(int iCurrentLevel) { return iCurrentLevel >= m_iRequiredLevel; }
	bool	Use(Actor* pTarget, PlayerCharacter* pSkillOwner, Grid* pGrid); //Activates the skill if all checks pass
	void	UpdateTimers(float fGameTime);
	bool	FinishedCasting(float fGameTime); //Updates casting timer of skill and tests wether it is finsihesed or not
	void	ApplyEffect(Actor* pTarget); //Applys skill to target

protected:

	int			m_iRequiredLevel;
	int			m_iRange; //In tiles
	int			m_iRadius;  //In tiles
	int			m_iDamage;
	DamageType  m_eDamageType;
	float		m_fSpeed;
	float		m_fElaspedSinceCast;
	float		m_fRecovery;
	float		m_fElaspedRecovery;
	bool		m_bReady;
};
