#pragma once
#include "DamageTypes.h"

class Actor;
class PlayerCharacter;
class TiXmlElement;

#define SKILL_NAME_LENGTH 32

class Skill
{
public:
	Skill(void);
	~Skill(void);

	bool			IsReady() { return m_bReady; }
	bool			MeetRequiredLevel(int iCurrentLevel) { return iCurrentLevel >= m_iRequiredLevel; }
	virtual bool	Activate(PlayerCharacter* pSkillOwner) { return false; } //Activates the skill if all checks pass
	virtual bool	Update(PlayerCharacter *pSkillOwner, float fDeltaTime) { return false; }
	void			UpdateTimers(float fGameTime);
	//bool			FinishedCasting(float fGameTime); //Updates casting timer of skill and tests wether it is finsihesed or not
	bool			Load(TiXmlElement* pStart) { return false; }

	char*			GetName() { return m_szName; }
	int				GetIndex() { return m_iIndex; }
	

protected:
	char		m_szName[SKILL_NAME_LENGTH];
	int			m_iRequiredLevel;
	int			m_iIndex;
	int			m_iSkillRank;
	float		m_fElaspedSinceCast;
	float		m_fRecovery;
	float		m_fElaspedRecovery;
	float		m_fSpeed;
	bool		m_bReady;
};
