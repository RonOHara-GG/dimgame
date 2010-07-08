#include "StdAfx.h"
#include "PlayerPetAI.h"
#include "PlayerPet.h"
#include "Level.h"
#include "Grid.h"
#include "PlayerCharacter.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuQuadTree.h"

PlayerPetAI::PlayerPetAI(PlayerPet* pPet)	
{
	m_pTheMindless = pPet;
}

PlayerPetAI::~PlayerPetAI(void)
{
}

void PlayerPetAI::Update(float fDeltaTime)
{
	
	switch( m_eCurrentState )
	{
	case eST_Aggro:
		{
			if( !m_pTheMindless->GetTarget() )
			{
				//find target in range			
				float fAggroRange = m_pTheMindless->GetAggroRange();
				kpuBoundingSphere sphere(fAggroRange, m_pTheMindless->GetLocation());
				kpuArrayList<kpuCollisionData> collidedObjects;

				g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

				Actor* pClosest = 0;
				float m_fClosest = fAggroRange * fAggroRange;
				for(int i = 0; i < collidedObjects.Count(); i++)
				{
					kpuCollisionData* pNext = &collidedObjects[i];

					if( pNext->m_pObject->HasFlag(ENEMY) )
					{
						//get distance to the object
						float fDist = kpuVector::DistanceSquared(pNext->m_pObject->GetLocation(), m_pTheMindless->GetLocation());

						if( fDist < m_fClosest )
						{
							m_fClosest = fDist;
							pClosest = (Actor*)pNext->m_pObject;
						}
					}				
				}

				m_pTheMindless->SetTarget(pClosest);
			}
			else
			{

				Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
				//Charge toward target until within attack range
				if( !m_pTheMindless->IsInRange(m_pTheMindless->GetTarget(), m_pTheMindless->GetRange()) )
				{
					kpuVector vLocation = m_pTheMindless->GetLocation();
					kpuVector vTarget = m_pTheMindless->GetTarget()->GetLocation();

					//if greater than aggro range * 5 then mob breaks
					if( !m_pTheMindless->IsInRange(m_pTheMindless->GetTarget(), m_pTheMindless->GetAggroRange() * 5) )
					{
						m_pTheMindless->SetTarget(0);
						int iTile = pGrid->GetTileAtLocation(vLocation);
						m_pTheMindless->SetMoveTarget(iTile);
						m_eCurrentState = eST_Wait;
						break;
					}

					int iCurrentTile = pGrid->GetTileAtLocation(vLocation);	

					//Only move if at destination
					if( m_pTheMindless->AtDestination() )
					{
						kpuVector vDirection, vCurrentToTarget;
						vDirection =  vTarget - vLocation;
						vCurrentToTarget = vDirection;
						vDirection.Normalize();					
						
						int iTile = pGrid->BestMove(vDirection, iCurrentTile);
						
						if( iTile == m_iPreviousTile )
						{
							//see if it is possible to get next to target
							//if not just get close and wait

							iTile = pGrid->BestMove(vDirection * -1 , pGrid->GetTileAtLocation(vTarget));

							if( iTile > 0 )
							{
								m_pTheMindless->SetMoveTarget(iTile);
								m_pTheMindless->BuildPathToDestination();
								break;
							}													

						}

						if( iTile > 0 )
						{						
							m_pTheMindless->SetNextMove(iTile);
							m_iPreviousTile = iCurrentTile;
						}	
						else
							m_pTheMindless->SetNextMove(iCurrentTile);
						
					}
					else if( pGrid->GetActor(m_pTheMindless->GetDestinationTile()) && pGrid->GetActor(m_pTheMindless->GetDestinationTile()) != m_pTheMindless)
					{
						//if not at destination but it is occupied change destination to current tile
						m_pTheMindless->SetNextMove(iCurrentTile);
					}

				}
				else
				{
					int iTile = pGrid->GetTileAtLocation(m_pTheMindless->GetLocation());
					m_pTheMindless->SetNextMove(iTile);
					m_iPreviousTile = -1;
					m_eCurrentState = eST_Attack;
				}
			}

			break;
		}	
	case eST_Attack:
		{
			if( !m_pTheMindless->IsInRange(m_pTheMindless->GetTarget(), m_pTheMindless->GetRange()) )
			{
				m_eCurrentState = eST_Aggro;
				break;
			}

			float fHealth = m_pTheMindless->GetCurrentHealth() / (float)(m_pTheMindless->GetMaxHealth()) ;
			if( fHealth > 0.10f )
			{
				m_pTheMindless->UseDefaultAttack(m_pTheMindless->GetTarget(), g_pGameState->GetLevel()->GetGrid() );
			}			

			break;
		}
	case eST_Defend:
		{
			//if the player gets attack then attack his attacker
			break;
		}
	case eST_Wait:
		{
			
			break;
		}
	}


}