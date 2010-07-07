#pragma once
#include "strike.h"

class ShoulderThrow :
	public Strike
{
public:
	ShoulderThrow(void);
	~ShoulderThrow(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float m_fMinThrow;
	int	m_iStrFactor;

};

