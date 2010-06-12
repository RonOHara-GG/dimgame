#include "StdAfx.h"
#include "AskaranAI.h"
#include "NPC.h"
#include "Level.h"
#include "Grid.h"
#include "PlayerCharacter.h"

AskaranAI::AskaranAI(void)
{
	m_eCurrentState = eST_Wait;
	m_iPreviousTile = -1;
}

AskaranAI::~AskaranAI(void)
{
}

void AskaranAI::Update(NPC *pNpc, float fDeltaTime)
{
	switch( m_eCurrentState )
	{
	case eST_Aggro:
		{
			Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
			//Charge toward target until within attack range
			if( !pNpc->IsInRange(pNpc->GetTarget(), pNpc->GetActionRange()) )
			{
				kpuVector vLocation = pNpc->GetLocation();
				kpuVector vTarget = pNpc->GetTarget()->GetLocation();

				//if greater than aggro range * 5 then mob breaks
				if( !pNpc->IsInRange(pNpc->GetTarget(), pNpc->GetAggroRange() * 5) )
				{
					pNpc->SetTarget(0);
					int iTile = pGrid->GetTileAtLocation(vLocation);
					pNpc->SetMoveTarget(iTile);
					m_eCurrentState = eST_Wait;
					break;
				}

				int iCurrentTile = pGrid->GetTileAtLocation(vLocation);	

				//Only move if at destination
				if( pNpc->AtDestination() )
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
							pNpc->SetMoveTarget(iTile);
							pNpc->BuildPathToDestination();
							break;
						}													

					}

					if( iTile > 0 )
					{						
						pNpc->SetNextMove(iTile);
						m_iPreviousTile = iCurrentTile;
					}	
					else
						pNpc->SetNextMove(iCurrentTile);
					
				}
				else if( pGrid->GetActor(pNpc->GetDestinationTile()) && pGrid->GetActor(pNpc->GetDestinationTile()) != pNpc)
				{
					//if not at destination but it is occupied change destination to current tile
					pNpc->SetNextMove(iCurrentTile);
				}

			}
			else
			{
				int iTile = pGrid->GetTileAtLocation(pNpc->GetLocation());
				pNpc->SetNextMove(iTile);
				m_iPreviousTile = -1;
				m_eCurrentState = eST_Attack;
			}
			
			

			break;
		}
	case eST_Flee:
		{
			if( pNpc->AtDestination() )
			{
				int iDist = g_pGameState->GetLevel()->GetGrid()->Distance( pNpc->GetTarget(), pNpc );
				int iCurrentTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pNpc->GetLocation());

				if(  iDist < pNpc->GetActionRange() + 2 )
				{
					kpuVector vTarget = pNpc->GetLocation() - pNpc->GetTarget()->GetLocation();
					vTarget.Normalize();			

					//run away
					
					int iTile = g_pGameState->GetLevel()->GetGrid()->BestMove(vTarget, iCurrentTile);

					if( iTile > 0 )
					{						
						pNpc->SetNextMove(iTile);					
					}	
					else
						pNpc->SetNextMove(iCurrentTile);

					
				}
				else
				{					
					pNpc->SetSpeed(pNpc->GetSpeed() * 2);
					pNpc->SetNextMove(iCurrentTile);
					m_eCurrentState = eST_None;
				}
			}


			break;
		}
	case eST_Attack:
		{
			if( !pNpc->IsInRange(pNpc->GetTarget(), pNpc->GetActionRange()) )
			{
				m_eCurrentState = eST_Aggro;
				break;
			}

			float fHealth = pNpc->GetCurrentHealth() / (float)(pNpc->GetMaxHealth()) ;
			if( fHealth > 0.10f )
			{
				pNpc->UseDefaultAttack(pNpc->GetTarget(), g_pGameState->GetLevel()->GetGrid() );
			}
			else
			{
				//cut speed in half
				pNpc->SetSpeed(pNpc->GetSpeed() / 2);

				m_eCurrentState = eST_Flee;
			}

			break;
		}
	case eST_Wait:
		{
			//for now just check if player is in range
			pNpc->SetTarget(g_pGameState->GetPlayer());

			if( pNpc->InLineOfSight(pNpc->GetTarget(), pNpc->GetAggroRange()) )
			{	
				m_iPreviousTile = -1;
				m_eCurrentState = eST_Aggro;
			}

			break;
		}
	}

}





