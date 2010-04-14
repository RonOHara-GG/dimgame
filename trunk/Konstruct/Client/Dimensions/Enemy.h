#pragma once
#include "npc.h"
#include "LoadStructures.h"

class Enemy :
	public NPC
{
public:
	Enemy(EnemyLoadStructure& loadStruct, kpgModel* pModel);
	~Enemy(void);

	bool Update(float fGameTime);

protected:
	void Wander(float fDeltaTime); //Wander around at varying distances
	

	float		m_fElaspedWanderWait;
	float		m_fAggroRange;
	float		m_fAttackRange;
	float		m_fAttackSpeed;
	int			m_iDamage;
	DamageType	m_eDamageType;
	
};

#define MAX_WANDER_DIST 6 //In tiles
#define MAX_WANDER_WAIT 10 //How much time in seconds the enemy will wait to find a new path
