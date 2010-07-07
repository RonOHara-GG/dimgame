#pragma once
#include "BasicShot.h"

class ScatterShot :
	public BasicShot
{
public:
	ScatterShot(void);
	~ScatterShot(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	
	float		m_fRadiusMod;
	float		m_fMinRadius;
	

};
