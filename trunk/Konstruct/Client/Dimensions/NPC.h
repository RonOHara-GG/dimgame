#pragma once
#include "actor.h"

class NPC :
	public Actor
{
public:
	NPC(void);
	virtual ~NPC(void);

	virtual void Update(float fGameTime) {};


protected:
	enum NPCState
	{
		eNS_Waiting,
		eNS_Wandering,
		eNS_Aggroed,
		eNS_Attacking,
		eNS_Dead
	};

	NPCState			m_eCurrentState;

};
