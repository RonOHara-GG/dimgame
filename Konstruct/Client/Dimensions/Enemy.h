#pragma once
#include "npc.h"

class Enemy :
	public NPC
{
public:
	Enemy(void);
	~Enemy(void);

	void Update(float fGameTime);

protected:
	void Wander(float fDeltaTime); //Wander around at varying distances
	

	float m_fElaspedWanderWait;
	int m_iAttackRange;
};

#define MAX_WANDER_DIST 6 //In tiles
#define MAX_WANDER_WAIT 10 //How much time in seconds the enemy will wait to find a new path
