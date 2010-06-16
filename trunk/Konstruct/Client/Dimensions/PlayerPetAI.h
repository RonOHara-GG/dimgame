#pragma once
#include "aicontrol.h"

class PlayerPet;

class PlayerPetAI :
	public AIControl
{
public:
	PlayerPetAI(PlayerPet* pPet);
	~PlayerPetAI(void);

	void Update( float fDeltaTime);

private:

	enum eState
	{
		eST_Defend,
		eST_Wait,
		eST_Aggro,
		eST_Attack,
	};

	PlayerPet*	m_pTheMindless;
	eState		m_eCurrentState;
	int			m_iPreviousTile;
};
