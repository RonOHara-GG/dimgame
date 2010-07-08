#include "StdAfx.h"
#include "Actor.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Level.h"
#include "Grid.h"
#include "Common/Utility/kpuBoundingCapsule.h"
#include "Common/Utility/kpuCollisionDetection.h"
#include "Common/Utility/kpuCollisionData.h"
#include "Common/Utility/kpuQuadTree.h"
#include "PersistentSkill.h"

Actor::Actor()
{
	m_pModel = 0;
	m_pTarget = 0;

	m_fRotation = 0.0f;
	m_iDestinationTile = -1;
	m_iCurrentPathNode = -1;

	m_vVelocity = kpuv_Zero;

	m_fBaseSpeed = 1.0f;

	//Initalize stats to 0
	m_iStr = 0;				
	m_iAgi = 0;			
	m_iInt = 0;			
	m_iConst = 0;			

	m_iMaxHealth = 0;		
	m_iCurrentHealth = 0;	
	m_iMaxMental = 0;		
	m_iCurrentMental = 0;

	//Initalize Resisits to 0
	m_iCrushRes = 0;
	m_iSlashRes = 0;
	m_iPierceRes = 0;
	m_iMentalRes = 0;
	m_iHeatRes = 0;
	m_iColdRes = 0;
	m_iElectRes = 0;
	m_iWaterRes = 0;
	m_iAcidRes = 0;
	m_iViralRes = 0;
	m_iHolyRes = 0;
	m_iDeathRes = 0;	

	m_vHeading = kpuv_OneZ;

	m_paPersistentSkills = new kpuLinkedList();
	m_iDamageBuff = 0;
	m_iDamageReduction = 0;
}

Actor::~Actor()
{
	if( m_paPersistentSkills )
		delete m_paPersistentSkills;
}

void Actor::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}


void Actor::SetHeading(kpuVector &vHeading)
{
	m_fRotation = atan2(vHeading.GetX(),vHeading.GetZ());
	m_vHeading = vHeading;	
}


//int Actor::GetLevel() { return m_iLevel; }

void Actor::SetNextMove(int iTile)
{
	m_aPathNodes[0] = iTile;
	m_aPathNodes[1] = -1;

	m_iDestinationTile = iTile;
	m_iCurrentPathNode = 0;	
}

void Actor::UpdateMovement(float fDeltaTime)
{
	m_pModel->Rotate(0.0f, m_fRotation, 0.0f);	

	// Check to see if we even should move
	if( m_iDestinationTile >= 0 )
	{
		// Make sure we have a path to our destination
		if( m_iCurrentPathNode < 0 )//|| g_pGameState->GetLevel()->GetGrid()->TileWalkable(m_iDestinationTile) )
		{
			//m_iCurrentPathNode = -1;

			// Need to build a path to the destination
			if( !BuildPathToDestination() )
			{
				// No acceptable path to the destination, just clear it
				m_iDestinationTile = -1;
				return;
			}
		}
		

		// Move down the path
		float fMoveDelta = GetSpeed() * fDeltaTime;

		while( fMoveDelta > 0 )
		{
			int iTargetTile = m_aPathNodes[m_iCurrentPathNode];			

			//Actor* pTileActor = g_pGameState->GetLevel()->GetGrid()->GetActor(iTargetTile);
			////make sure the next tile is still walkable
			//if( pTileActor && pTileActor != this )
			//{				
			//	m_iCurrentPathNode = -1;
			//	return;
			//}

			kpuVector vTargetLocation;
			//g_pGameState->GetLevel()->GetGrid()->GetTileLocation(iTargetTile, vTargetLocation);

			kpuVector vMyLocation = GetLocation();

			//Find the closest tile to the destination
			//kpuVector vTargetToThisActor = vMyLocation - vTargetLocation;
			//vTargetToThisActor.Normalize();
			//int iCurrentDest = g_pGameState->GetLevel()->GetGrid()->NearestTile(vTargetToThisActor, m_iDestinationTile, this);

			g_pGameState->GetLevel()->GetGrid()->GetTileLocation(iTargetTile, vTargetLocation);
			kpuVector vToTarget = vTargetLocation - vMyLocation;
			vToTarget.SetW(0);

			vToTarget.SetY(0);
			float fDistToTarget = vToTarget.Length();			

			if( fDistToTarget == 0.0f )
			{
				m_iDestinationTile = -1;
				m_iCurrentPathNode = -1; 
				break;
			}			

			if( fDistToTarget < fMoveDelta )
			{
				fMoveDelta -= fDistToTarget;
				Move(vToTarget);
				//vMyLocation += vToTarget;
				//SetLocation(vMyLocation);				

				m_iCurrentPathNode++;			

				if(m_iCurrentPathNode >= MAX_PATH_NODES || m_aPathNodes[m_iCurrentPathNode] < 0 )
				{
					// Ran out of path
					if( iTargetTile == m_iDestinationTile )
					{
						// Reached the destination
						m_iDestinationTile = -1;
						m_iCurrentPathNode = -1;
						return;
					}

					// Need to get some more path
					if( !BuildPathToDestination() )
					{
						// No acceptable path to the destination, just clear it
						m_iDestinationTile = -1;
						return;
					}
				}				

			}
			else
			{
				// We have a ways to go before we hit the target
				vToTarget *= 1.0f / fDistToTarget;
				//SetLocation(vMyLocation);
				Move(vToTarget * fMoveDelta);
				break;
			}
		}
	}
}

void Actor::Move(kpuVector vVel)
{	
	//Remove from current tile
	g_pGameState->GetLevel()->GetGrid()->RemoveActor(this);
	
	m_pCurrentNode->Remove(this);			

	SetLocation(GetLocation() + vVel);
	
	vVel.Normalize();
	m_fRotation = atan2(vVel.GetX(),vVel.GetZ());
	m_vHeading = vVel;

	m_pCurrentNode->ReAdd(this);	

	//Add actor to current tile
	g_pGameState->GetLevel()->GetGrid()->AddActor(this);		
}

void Actor::Move(float fDeltaTime, kpuVector vDir)
{
	kpuPhysicalObject::Move(vDir * m_fBaseSpeed);
	m_fRotation = atan2(vDir.GetX(),vDir.GetZ());
	m_vHeading = vDir;

}

bool Actor::BuildPathToDestination()
{
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();

	// Get my current tile
	int iCurrentTile = pGrid->GetTileAtLocation(GetLocation());
	kpuVector vStart, vEnd, vEndToStart;

	pGrid->GetTileLocation(iCurrentTile, vStart);
	pGrid->GetTileLocation(m_iDestinationTile, vEnd);
	vEndToStart = vStart - vEnd;
	vEndToStart.Normalize();

	/*if( pGrid->GetActor(iBestDestination) )
		iBestDestination = pGrid->BestMove(vEndToStart, m_iDestinationTile);*/

	// Build the path
	if(pGrid->BuildPath(iCurrentTile, m_iDestinationTile, m_aPathNodes, MAX_PATH_NODES, this) )
	{
		m_iCurrentPathNode = 0;		
		return true;
	}	

	m_iCurrentPathNode = -1;
	return false;
}

void Actor::BuildStraightPath(float fDist, kpuVector vDirection)
{
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();

	m_iDestinationTile = pGrid->GetTileAtLocation(GetLocation() + (vDirection * fDist));

	// Build the path
	pGrid->BuildStraightPath(pGrid->GetTileAtLocation(GetLocation()), m_iDestinationTile, m_aPathNodes, this, vDirection);	
	m_iCurrentPathNode = 0;		
}

bool Actor::TakeDamage(int iDamage, DamageType eDmgType, int iResistStr)
{
	/*if( iResistStr < GetResist(eDamageType) )
		return false;*/

	switch(eDmgType)
	{
	case eDT_Acid:
		{
			iDamage -= m_iAcidRes - iResistStr;
			break;
		}
	case eDT_Cold:
		{
			iDamage -= m_iColdRes - iResistStr;
		}
	case eDT_Crushing:
		{
			iDamage -= m_iCrushRes - iResistStr;
			break;
		}
	case eDT_Death:
		{
			iDamage -= m_iDeathRes - iResistStr;
			break;
		}
	case eDT_Electrical:
		{
			iDamage -= m_iElectRes - iResistStr;
			break;
		}
	case eDT_Heat:
		{
			iDamage -= m_iHeatRes - iResistStr;
			break;
		}
	case eDT_Holy:
		{
			iDamage -= m_iHolyRes - iResistStr;
			break;
		}
	case eDT_Mental:
		{
			iDamage -= m_iMentalRes - iResistStr;
			break;
		}
	case eDT_Piercing:
		{
			iDamage -= m_iPierceRes - iResistStr;
			break;
		}
	case eDT_Slashing:
		{
			iDamage -= m_iSlashRes - iResistStr;
			break;
		}
	case eDT_Viral:
		{
			iDamage -= m_iViralRes - iResistStr;
			break;
		}
	case eDT_Water:
		{
			iDamage -= m_iWaterRes - iResistStr;
			break;
		}
	}

	if( iDamage <= 0 )
		return false;

	m_iCurrentHealth -= iDamage;

	return true;
}

bool Actor::IsInRange(Actor *pTarget, float fRange)
{
	////Check if target is in range
	//kpuVector vPlayer,  vTarget;
	//pGrid->GetTileLocation(pGrid->GetTileAtLocation(GetLocation()), vPlayer);
	//pGrid->GetTileLocation(pGrid->GetTileAtLocation(pTarget->GetLocation()), vTarget);

	//kpuVector vPlayerToTarget = vTarget - vPlayer;
	//kpuVector vDirection = kpuVector::Normalize(vPlayerToTarget);

	//vPlayerToTarget *= vDirection;

	//float fDist = vPlayerToTarget.Dot(vPlayerToTarget);

	//if(fDist <= iRange)
	//	return true;
	return g_pGameState->GetLevel()->GetGrid()->DistanceSquared(this, pTarget) <= fRange * fRange;
}

bool Actor::UseDefaultAttack(Actor *pTarget, Grid *pGrid)
{
	return false;
}
bool Actor::InLineOfSight(kpuVector vPos, float fRange, Actor* pTarget)
{
	float fDist = kpuVector::DistanceSquared(GetLocation(), vPos);

	if(fDist <= fRange * fRange)
	{		
		kpuBoundingCapsule capsule(GetLocation(), vPos, 0.0f);

		kpuArrayList<kpuCollisionData> aCollisions;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(capsule, &aCollisions);

		//if there is a collision between here and the target then no line of sight
		for(int i = 0; i < aCollisions.Count(); i++)
		{
			if( aCollisions[i].m_pObject != pTarget && aCollisions[i].m_pObject != this )
			{
				//check bounding box vs sphere
				kpuBoundingBox box = aCollisions[i].m_pObject->GetBoundingBox();			
				kpuCollisionData data;
				capsule.Intersects(&box, aCollisions[i].m_pObject->GetMatrix(), data);

				if( data.m_bCollided )
				{
					return false;	
				}
			}
		}

		return true;
	}

	return false;
}

bool Actor::InLineOfSight(Actor *pTarget, float fRange)
{
	//This player will be the start of the capsule and the parameter will be the end
	//We will get the collisions from the quad tree and check a line segment from the center of this actor to the target and see if there are any collisions
	return InLineOfSight(pTarget->GetLocation() + pTarget->GetSphere().GetLocation(), fRange, pTarget);
}

int Actor::GetResist(DamageType eType)
{
	switch(eType)
	{
	case eDT_Acid:
		{
			return m_iAcidRes;			
		}
	case eDT_Cold:
		{
			return m_iColdRes;
		}
	case eDT_Crushing:
		{
			return m_iCrushRes;
		}
	case eDT_Death:
		{
			return m_iDeathRes;
		}
	case eDT_Electrical:
		{
			return m_iElectRes;
		}
	case eDT_Heat:
		{
			return m_iHeatRes;
		}
	case eDT_Holy:
		{
			return m_iHolyRes;
		}
	case eDT_Mental:
		{
			return m_iMentalRes;
		}
	case eDT_Piercing:
		{
			return m_iPierceRes;
		}
	case eDT_Slashing:
		{
			return m_iSlashRes;
		}
	case eDT_Viral:
		{
			return m_iViralRes;
		}
	case eDT_Water:
		{
			return m_iWaterRes;
		}
	}

	return 0;
}

void Actor::SetResist(int iAmount, DamageType eType)
{
	switch(eType)
	{
	case eDT_Acid:
		{
			m_iAcidRes = iAmount;			
		}
	case eDT_Cold:
		{
			m_iColdRes = iAmount;	
		}
	case eDT_Crushing:
		{
			m_iCrushRes = iAmount;	
		}
	case eDT_Death:
		{
			m_iDeathRes = iAmount;	
		}
	case eDT_Electrical:
		{
			m_iElectRes = iAmount;	
		}
	case eDT_Heat:
		{
			m_iHeatRes = iAmount;	
		}
	case eDT_Holy:
		{
			m_iHolyRes = iAmount;	
		}
	case eDT_Mental:
		{
			m_iMentalRes = iAmount;	
		}
	case eDT_Piercing:
		{
			m_iPierceRes = iAmount;	
		}
	case eDT_Slashing:
		{
			m_iSlashRes = iAmount;	
		}
	case eDT_Viral:
		{
			m_iViralRes = iAmount;	
		}
	case eDT_Water:
		{
			m_iWaterRes = iAmount;	
		}
	}
}
	
void Actor::Heal(int iAmount)
{
	m_iCurrentHealth += iAmount;

	if( m_iCurrentHealth > m_iMaxHealth )
		m_iCurrentHealth = m_iMaxHealth;
}

void Actor::HealMental(int iAmount)
{
	m_iCurrentMental += iAmount;

	if( m_iCurrentMental > m_iMaxMental )
		m_iCurrentMental = m_iMaxMental;
}

void Actor::RemovePersistentSkill(PersistentSkill* pSkill)
{
	kpuLinkedList* pNext = m_paPersistentSkills->Next();

	while( pNext )
	{
		void* pData = pNext->GetPointer();
		if( pData == pSkill )
		{
			delete pData;
			delete pNext;
			break;
		}

		pNext = pNext->Next();
	}
}

void Actor::AddPersistentSkill(PersistentSkill* pSkill)
{	
	m_paPersistentSkills->AddTail(pSkill);
}

bool Actor::HasPersistentSkill(u32 uID)
{
	kpuLinkedList* pNext = m_paPersistentSkills->Next();

	while( pNext )
	{
		Skill* pData = (Skill*)pNext->GetPointer();
		
		if( pData->GetID() == uID )
			return true;

		pNext = pNext->Next();
	}

	return false;
}

void Actor::AreaEffect(kpuVector vCenter, float fRadius, void* pEffect, void* pSource)
{
	//make sure target has line of sight to effect
	kpuBoundingSphere sphere(fRadius, vCenter);	

	kpuBoundingSphere sphere2 = GetSphere();
	sphere2.Transform(GetMatrix());
	kpuBoundingCapsule capsule(sphere.GetLocation(), sphere2.GetLocation(), 0.0f);

	kpuArrayList<kpuCollisionData> aCollisions;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(capsule, &aCollisions);

	//if there is a collision between here and the target then no line of sight
	for(int i = 0; i < aCollisions.Count(); i++)
	{
		if( aCollisions[i].m_pObject != pSource && aCollisions[i].m_pObject != this )
		{
			//check bounding box vs sphere
			kpuBoundingBox box = aCollisions[i].m_pObject->GetBoundingBox();			
			kpuCollisionData data;
			capsule.Intersects(&box, aCollisions[i].m_pObject->GetMatrix(), data);

			if( data.m_bCollided )			
				return;	
			
		}
	}
	//apply effect which for now is just damag
	TakeDamage(*(int*)pEffect, (DamageType)*(int*)((int*)pEffect + 1));
}