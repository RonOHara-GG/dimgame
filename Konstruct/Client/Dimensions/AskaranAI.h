#pragma once
#include "aicontrol.h"

class AskaranAI :
	public AIControl
{
public:
	AskaranAI(void) {}
	AskaranAI(Npc* pNpc);
	~AskaranAI(void);

	void Update(float fDeltaTime);

private:
	enum eState
	{
		eST_Aggro,
		eST_Attack,
		eST_Flee,
		eST_Wait,
		eST_None
	};


	Npc*		m_pTheMindless;
	eState		m_eCurrentState;
	int			m_iPreviousTile;

};
