#include "StdAfx.h"
#include "Actor.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Level.h"
#include "Grid.h"

Actor::Actor()
{
	m_pModel = 0;

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

	m_bAttackable = false;
}

Actor::~Actor()
{


}

void Actor::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}

kpuVector Actor::GetLocation()
{
	if( m_pModel )
	{
		return m_pModel->GetInstance(0)->GetMatrix().GetD();
	}
	else
	{
		return kpuVector();
	}
}

//int Actor::GetLevel() { return m_iLevel; }

void Actor::SetLocation(const kpuVector& vNewLoc)
{
	//move bounding objects
	m_bBox.Move(vNewLoc - GetLocation());
	m_bSphere.Move(vNewLoc - GetLocation());

	if( m_pModel )
	{
		m_pModel->GetInstance(0)->SetPosition(vNewLoc.GetX(), vNewLoc.GetY(), vNewLoc.GetZ());
	}	
}

void Actor::UpdateMovement(float fDeltaTime)
{
	m_pModel->RotateY(m_fRotation);

	// Check to see if we even should move
	if( m_iDestinationTile >= 0 )
	{
		// Make sure we have a path to our destination
		if( m_iCurrentPathNode < 0 )
		{
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
			//Remove from curren tile
			g_pGameState->GetLevel()->GetGrid()->RemoveActor(this);

			int iTargetTile = m_aPathNodes[m_iCurrentPathNode];
			kpuVector vTargetLocation;
			g_pGameState->GetLevel()->GetGrid()->GetTileLocation(iTargetTile, vTargetLocation);

			kpuVector vMyLocation = GetLocation();
			kpuVector vToTarget = vTargetLocation - vMyLocation;

			vToTarget.SetY(0);
			float fDistToTarget = vToTarget.Length();
			if( fDistToTarget < fMoveDelta )
			{
				fMoveDelta -= fDistToTarget;
				Move(vToTarget);
				//vMyLocation += vToTarget;
				//SetLocation(vMyLocation);
				

				m_iCurrentPathNode++;

				//Add actor to current tile
				g_pGameState->GetLevel()->GetGrid()->AddActor(this);

				if( m_iCurrentPathNode >= MAX_PATH_NODES || m_aPathNodes[m_iCurrentPathNode] < 0 )
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

				m_fRotation = atan2(vToTarget.GetX(),vToTarget.GetZ());

				/*vToTarget.Normalize();
				m_fRotation = acos(vToTarget.GetZ());

				if(vToTarget.GetX() < 0)
					m_fRotation += PI;*/

			}
			else
			{
				// We have a ways to go before we hit the target
				vToTarget *= 1.0f / fDistToTarget;
				//vMyLocation += vToTarget * fMoveDelta;
				//SetLocation(vMyLocation);
				Move(vToTarget * fMoveDelta);
				
				m_fRotation = atan2(vToTarget.GetX(),vToTarget.GetZ());
				/*if(vToTarget.GetX() < 0)
					m_fRotation += PI;*/

				//Add actor to current tile
				g_pGameState->GetLevel()->GetGrid()->AddActor(this);

				break;
			}
		}
	}
}

void Actor::Move(kpuVector& vVel)
{
	kpuPhysicalObject::Move(vVel);

	SetLocation(GetLocation() + vVel);

	//if velocity comes back zero clear path
	if ( vVel == kpuv_Zero )
	{
		m_iDestinationTile = -1;
		m_iCurrentPathNode = -1; 
	}
}

bool Actor::BuildPathToDestination()
{
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();

	// Get my current tile
	int iCurrentTile = pGrid->GetTileAtLocation(GetLocation());

	// Build the path
	if( pGrid->BuildPath(iCurrentTile, m_iDestinationTile, m_aPathNodes, MAX_PATH_NODES) )
	{
		m_iCurrentPathNode = 0;
		return true;
	}
	m_iCurrentPathNode = -1;
	return false;
}

void Actor::TakeDamage(float fDamage, DamageType eDmgType)
{
	switch(eDmgType)
	{
	case eDT_Acid:
		{
			fDamage -= fDamage * (m_iAcidRes / 100);
			break;
		}
	case eDT_Cold:
		{
			fDamage -= fDamage * (m_iColdRes / 100);
			break;
		}
	case eDT_Crushing:
		{
			fDamage -= fDamage * (m_iCrushRes / 100);
			break;
		}
	case eDT_Death:
		{
			fDamage -= fDamage * (m_iDeathRes / 100);
			break;
		}
	case eDT_Electrical:
		{
			fDamage -= fDamage * (m_iElectRes / 100);
			break;
		}
	case eDT_Heat:
		{
			fDamage -= fDamage * (m_iHeatRes / 100);
			break;
		}
	case eDT_Holy:
		{
			fDamage -= fDamage * (m_iHolyRes / 100);
			break;
		}
	case eDT_Mental:
		{
			fDamage -= fDamage * (m_iMentalRes / 100);
			break;
		}
	case eDT_Piercing:
		{
			fDamage -= fDamage * (m_iPierceRes / 100);
			break;
		}
	case eDT_Slashing:
		{
			fDamage -= fDamage * (m_iSlashRes / 100);
			break;
		}
	case eDT_Viral:
		{
			fDamage -= fDamage * (m_iViralRes / 100);
			break;
		}
	case eDT_Water:
		{
			fDamage -= fDamage * (m_iWaterRes / 100);
			break;
		}
	}

	m_iCurrentHealth -= fDamage;

}

bool Actor::IsInRange(Actor *pTarget, int iRange, Grid* pGrid)
{
	//Check if target is in range
	kpuVector vPlayer,  vTarget;
	pGrid->GetTileLocation(pGrid->GetTileAtLocation(GetLocation()), vPlayer);
	pGrid->GetTileLocation(pGrid->GetTileAtLocation(pTarget->GetLocation()), vTarget);

	kpuVector vPlayerToTarget = vTarget - vPlayer;
	kpuVector vDirection = kpuVector::Normalize(vPlayerToTarget);

	vPlayerToTarget *= vDirection;

	float fDist = vPlayerToTarget.Dot(vPlayerToTarget);

	if(fDist <= iRange)
		return true;

	return false;
}

bool Actor::UseDefaultAttack(Actor *pTarget, Grid *pGrid)
{
	return false;
}