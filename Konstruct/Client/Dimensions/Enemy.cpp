#include "StdAfx.h"
#include "Enemy.h"
#include "grid.h"
#include "level.h"

Enemy::Enemy(kpgModel* model):NPC()
{
	m_pModel = model;
	m_eCurrentState = eNS_Waiting;

	m_fElaspedWanderWait = 0.0f;
}

Enemy::~Enemy(void)
{
}

void Enemy::Update(float fGameTime)
{
	UpdateMovement(fGameTime);

	//Simple finte state machine to control the enemy AI for now
	switch(m_eCurrentState)
	{
		case eNS_Waiting:
		{
			//This will be for things that just stand and wait until they are given some kind of
			//stimuli to do something

			break;
		}
		case eNS_Wandering:
		{
			Wander(fGameTime);

			break;
		}
		case eNS_Aggroed:
		{
			break;
		}
		case eNS_Attacking:
		{
			break;
		}
		case eNS_Dead:
		{
			break;
		}
	}





}

void Enemy::Wander(float fDeltaTime)
{
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
	int iCurrentTile = pGrid->GetTileAtLocation(GetLocation());

	if(m_iDestinationTile < 0)
	{
		m_fElaspedWanderWait += fDeltaTime;

		if(m_fElaspedWanderWait >= MAX_WANDER_WAIT)
		{
			m_fElaspedWanderWait = 0.0f;

			//Build new path to new path			

			//Get target node
			kpuVector vDirection = s_vDirections[rand() % 5];

			m_iDestinationTile = pGrid->GetTileAtLocation(GetLocation() + (vDirection * (rand() / ((float)RAND_MAX + 1)) * MAX_WANDER_DIST));

			if( pGrid->BuildPath(iCurrentTile, m_iDestinationTile, m_aPathNodes, MAX_PATH_NODES) )
			{
				m_iCurrentPathNode = 0;
				return;
			}

			m_iCurrentPathNode = -1;
		}
	}
}
