#pragma once
#include "strike.h"

class ReverseRoundHouse :
	public Strike
{
public:
	ReverseRoundHouse(void);
	~ReverseRoundHouse(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	enum AttackDirection
	{
		e_drFRONT,
		e_drRIGHT,
		e_drUPPER_RIGHT,
		e_drUPPER_LEFT,
		e_drLEFT,
		e_drNONE
	};

	AttackDirection m_eCurrentTarget;
};
