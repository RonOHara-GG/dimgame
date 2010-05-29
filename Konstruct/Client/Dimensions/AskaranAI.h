#pragma once
#include "aicontrol.h"

class AskaranAI :
	public AIControl
{
public:
	AskaranAI(void);
	~AskaranAI(void);

	void Update(NPC* pNpc, float fDeltaTime);

protected:
	enum eState
	{
		eST_Aggro,
		eST_Attack,
		eST_Flee,
		eST_Wait,
		eST_None
	};


	eState m_eCurrentState;
	int m_iPreviousTile;

};
