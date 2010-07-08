#pragma once
#include "actor.h"

class AIControl;

class Npc :
	public Actor
{
public:
	Npc(void);
	virtual ~Npc(void);

	virtual bool	Update(float fGameTime);
	virtual void	Interact(PlayerCharacter* pPlayer) { }

	float			GetRange()		{ return m_fActionRange; }
	void			SetActionRange(float fActionRange) { m_fActionRange = fActionRange; }
	float			GetAggroRange()		{ return m_fAggroRange; }
	Actor*			GetLastAttacked() { return m_pLastAttacked; }
	void			SetLastAttacked(Actor* pActor) { m_pLastAttacked = pActor; }

	void			SetAI(AIControl* pAI);

	
	


protected:
	AIControl*			m_pAIBehavior;

	float				m_fActionRange; //range enemy must be to player to interact
	float				m_fAggroRange;	//range at which an Npc has perception of player
	
	Actor*				m_pLastAttacked; //The actor the Npc dealt damage to last update
	bool				m_bStatic;
	char*				m_pszName;

	u32					m_uDialogWindow; //The name of the window used for dialog with this Npc

};
