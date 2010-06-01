#include "StdAfx.h"
#include "Enemy.h"
#include "grid.h"
#include "level.h"
#include "LoadStructures.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "AskaranAI.h"


Enemy::Enemy(EnemyLoadStructure& loadStruct):NPC()
{
	m_pModel = new kpgModel();
	m_pModel->SetGeometryInstance(loadStruct.pModel->GetInstance(0));
	
	CalculateBoundingVolumes(loadStruct.pCollision);

	m_pAIBehavior = new AskaranAI();

	m_fElaspedWanderWait = 0.0f;
	m_fElaspedAttack = 0.0f;
	m_bAttackable = true;
	
	m_iLevel = loadStruct.iLevel;
	m_fMaxHealth = loadStruct.fHealth;
	m_fCurrentHealth = m_fMaxHealth;
	m_fBaseSpeed = loadStruct.fSpeed;

	m_iCrushRes = loadStruct.iCrushRes;
	m_iSlashRes = loadStruct.iSlashRes;
	m_iPierceRes = loadStruct.iPierceRes;
    m_iMentalRes = loadStruct.iMentalRes;
    m_iHeatRes = loadStruct.iHeatRes;
    m_iColdRes = loadStruct.iColdRes;
    m_iElectRes = loadStruct.iElectRes;
    m_iWaterRes = loadStruct.iWaterRes;
    m_iAcidRes = loadStruct.iAcidRes;
    m_iViralRes = loadStruct.iViralRes;
    m_iHolyRes = loadStruct.iHolyRes;
    m_iDeathRes = loadStruct.iDeathRes;

    m_fDamage = loadStruct.fDamage;
    m_fActionRange = loadStruct.fAttackRange;
	m_fAggroRange = loadStruct.fAggroRange;
    m_fAttackSpeed = loadStruct.fAttackSpeed;
    m_eDamageType = (DamageType)loadStruct.iDamageType;

}

Enemy::~Enemy(void)
{
}

bool Enemy::Update(float fGameTime)
{
	if( m_fCurrentHealth > 0 )
	{
		UpdateMovement(fGameTime);	

		m_pAIBehavior->Update(this, fGameTime);

		//update timers
		if( m_fElaspedAttack < m_fAttackSpeed )
			m_fElaspedAttack += fGameTime;

		return true;
	}

	return false;
}

void Enemy::UpdateMovement(float fDeltaTime)
{
	//if not at destination check to see if target has an actor in it
	if( m_iDestinationTile > -1 && m_iCurrentPathNode > -1)
	{
 		Actor* pTileActor = g_pGameState->GetLevel()->GetGrid()->GetActor(m_aPathNodes[m_iCurrentPathNode]);

		//make sure the next tile is still walkable
		if( pTileActor && pTileActor != this )
		{		
			//Set target tile has current tile
			SetNextMove(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(GetLocation()));			
		}
		
	}

	Actor::UpdateMovement(fDeltaTime);
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

			if( pGrid->BuildPath(iCurrentTile, m_iDestinationTile, m_aPathNodes, MAX_PATH_NODES, this) )
			{
				m_iCurrentPathNode = 0;
				return;
			}

			m_iCurrentPathNode = -1;
		}
	}
}

bool Enemy::UseDefaultAttack(Actor *pTarget, Grid *pGrid)
{
	if(pTarget->Attackable())
	{		
		if( m_fElaspedAttack >= m_fAttackSpeed )
		{			
			m_fElaspedAttack = 0.0f;
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

			return true;
			
		}		
	}

	return false;
}
