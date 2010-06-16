#include "StdAfx.h"
#include "AskaranAI.h"
#include "NPC.h"
#include "Level.h"
#include "Grid.h"
#include "PlayerCharacter.h"

AskaranAI::AskaranAI(NPC* pNpc)
{
	m_eCurrentState = eST_Wait;
	m_iPreviousTile = -1;

	m_pTheMindless = pNpc;

}

AskaranAI::~AskaranAI(void)
{
}

void AskaranAI::Update(float fDeltaTime)
{
	//reset last attacked until it has happened this frame
	m_pTheMindless->SetLastAttacked(0);

	switch( m_eCurrentState )
	{
	case eST_Aggro:
		{
			Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
			//Charge toward target until within attack range
			if( !m_pTheMindless->IsInRange(m_pTheMindless->GetTarget(), m_pTheMindless->GetActionRange()) )
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
			
			

			break;
		}
	case eST_Flee:
		{
			if( m_pTheMindless->AtDestination() )
			{
				int iDist = g_pGameState->GetLevel()->GetGrid()->Distance( m_pTheMindless->GetTarget(), m_pTheMindless );
				int iCurrentTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(m_pTheMindless->GetLocation());

				if(  iDist < m_pTheMindless->GetActionRange() + 2 )
				{
					kpuVector vTarget = m_pTheMindless->GetLocation() - m_pTheMindless->GetTarget()->GetLocation();
					vTarget.Normalize();			

					//run away
					
					int iTile = g_pGameState->GetLevel()->GetGrid()->BestMove(vTarget, iCurrentTile);

					if( iTile > 0 )
					{						
						m_pTheMindless->SetNextMove(iTile);					
					}	
					else
						m_pTheMindless->SetNextMove(iCurrentTile);

					
				}
				else
				{					
					m_pTheMindless->SetSpeed(m_pTheMindless->GetSpeed() * 2);
					m_pTheMindless->SetNextMove(iCurrentTile);
					m_eCurrentState = eST_None;
				}
			}


			break;
		}
	case eST_Attack:
		{
			if( !m_pTheMindless->IsInRange(m_pTheMindless->GetTarget(), m_pTheMindless->GetActionRange()) )
			{
				m_eCurrentState = eST_Aggro;
				break;
			}

			float fHealth = m_pTheMindless->GetCurrentHealth() / (float)(m_pTheMindless->GetMaxHealth()) ;
			if( fHealth > 0.10f )
			{
				m_pTheMindless->UseDefaultAttack(m_pTheMindless->GetTarget(), g_pGameState->GetLevel()->GetGrid() );
				m_pTheMindless->SetLastAttacked(m_pTheMindless->GetTarget());
			}
			else
			{
				//cut speed in half
				m_pTheMindless->SetSpeed(m_pTheMindless->GetSpeed() / 2);

				m_eCurrentState = eST_Flee;
			}

			break;
		}
	case eST_Wait:
		{
			//for now just check if player is in range
			m_pTheMindless->SetTarget(g_pGameState->GetPlayer());

			if( m_pTheMindless->InLineOfSight(m_pTheMindless->GetTarget(), m_pTheMindless->GetAggroRange()) )
			{	
				m_iPreviousTile = -1;
				m_eCurrentState = eST_Aggro;
			}

			break;
		}
	}

}





