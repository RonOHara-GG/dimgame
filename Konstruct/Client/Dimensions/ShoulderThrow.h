#pragma once
#include "strike.h"

class ShoulderThrow :
	public Strike
{
public:
	ShoulderThrow(void);
	~ShoulderThrow(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int m_iThrowDist;
	int	m_iStrFactor;

};

#define BASE_THROW_DIST 3