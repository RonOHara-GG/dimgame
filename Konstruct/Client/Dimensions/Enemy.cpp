#include "StdAfx.h"
#include "Enemy.h"
#include "grid.h"
#include "level.h"
#include "LoadStructures.h"
#include "Common/Graphics/kpgGeometryInstance.h"


Enemy::Enemy(EnemyLoadStructure& loadStruct):NPC()
{
	m_pModel = new kpgModel();
	m_pModel->SetGeometryInstance(loadStruct.pModel->GetInstance(0));
	
	CalculateBoundingVolumes(loadStruct.pCollision);

	m_eCurrentState = eNS_Waiting;

	m_fElaspedWanderWait = 0.0f;
	m_bAttackable = true;
	
	m_iLevel = loadStruct.iLevel;
	m_iMaxHealth = loadStruct.iHealth;
	m_iCurrentHealth = m_iMaxHealth;
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

    m_iDamage = loadStruct.iDamage;
    m_fAttackRange = loadStruct.fAttackRange;
	m_fAggroRange = loadStruct.fAggroRange;
    m_fAttackSpeed = loadStruct.fAttackSpeed;
    m_eDamageType = (DamageType)loadStruct.iDamageType;

}

Enemy::~Enemy(void)
{
}

bool Enemy::Update(float fGameTime)
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


	return m_iCurrentHealth > 0;


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
