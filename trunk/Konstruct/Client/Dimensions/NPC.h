#pragma once
#include "actor.h"

class AIControl;

class NPC :
	public Actor
{
public:
	NPC(void);
	virtual ~NPC(void);

	virtual bool Update(float fGameTime) {return true;};
	float	GetActionRange()	{ return m_fActionRange; }
	float	GetAggroRange()		{ return m_fAggroRange; }

	
	


protected:
	AIControl*			m_pAIBehavior;

	float				m_fActionRange; //range enemy must be to player to interact
	float				m_fAggroRange;	//range at which an NPC has perception of player

};
