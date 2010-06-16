#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"

class Launch :
	public Skill
{
public:
	Launch(void);
	~Launch(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	enum LauncherType
	{
		eLT_RPG,
		eLT_Seeker,
		eLT_Cruise,
		eLT_Balistic
	};

	kpuVector		m_vSource;
	Actor*			m_pTarget;
	float			m_fDamage;
	DamageType		m_eDamageType;
	int				m_iRange;

	//--Loaded Data--
	float				m_fDamageMod;
	int					m_iRangeMod;
	LauncherType		m_eLauncherType;
};
