#pragma once
#include "DamageTypes.h"

class Actor;
class Weapon;
class PlayerCharacter;
class TiXmlElement;
enum eWeaponType;

#define SKILL_NAME_LENGTH 32

class Skill
{
public:
	Skill(void);
	~Skill(void);

	bool			IsReady() { return m_bReady; }
	bool			MeetRequiredLevel(int iCurrentLevel) { return iCurrentLevel >= m_iRequiredLevel; }
	virtual bool	Activate(PlayerCharacter* pSkillOwner);  //Activates the skill if all checks pass
	virtual bool	Update(PlayerCharacter *pSkillOwner, float fDeltaTime) { return false; }
	void			UpdateTimers(float fGameTime);
	//bool			FinishedCasting(float fGameTime); //Updates casting timer of skill and tests wether it is finsihesed or not
	bool			Load(TiXmlElement* pStart) { return false; }

	char*			GetName() { return m_szName; }
	int				GetIndex() { return m_iIndex; }
	
	virtual float	GetSpeed() { return m_fSpeed; }

	bool			InRange(Actor* p1, Actor* p2, int iRange);
	bool			InRange(Actor* p1, int iTile1, int iRange);
	bool			InRange(int iTile1, int iTile2, int iRange);

protected:
	bool		VaildTarget(PlayerCharacter* pSkillOwner);
	int			GetTargetTile(); //return tile at mouse location
	int			GetRange() { return 0; }

	char		m_szName[SKILL_NAME_LENGTH];
	int			m_iRankMultipleMin;	
	int			m_iRankMultipleMax;	
	int			m_iRequiredLevel;
	int			m_iIndex;
	int			m_iSkillRank;
	float		m_fElaspedSinceCast;
	float		m_fRecovery;	
	float		m_fRecoveryMod;
	float		m_fElaspedRecovery;
	float		m_fSpeed;
	bool		m_bReady;
	bool		m_bExecuted;

	//Targeting
	bool		m_bTargetRequired;
	Actor*		m_pTarget; //Actor target
	int			m_iTargetTile; //Location target
	u32			m_uTargetType;

	//Weapon
	bool		m_bWeaponRequired;
	bool		m_bSecondaryRequired;
	eWeaponType	m_eWeaponType;
	Weapon*		m_pEquipped;
};
