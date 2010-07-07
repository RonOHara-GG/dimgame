#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"

class Weapon;

class Launch :
	public Skill
{
public:
	Launch(void);
	~Launch(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	int	GetRange(Actor* p1);

	enum LauncherType
	{
		eLT_RPG,
		eLT_Seeker,
		eLT_Cruise,
		eLT_Balistic
	};

	kpuVector		m_vSource;
	DamageType		m_eDamageType;

	//--Loaded Data--
	float				m_fDamageMod;
	int					m_iRangeMod;
	LauncherType		m_eLauncherType;
};
