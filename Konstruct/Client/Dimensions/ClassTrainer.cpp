#include "StdAfx.h"
#include "ClassTrainer.h"
#include "PlayerClass.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgUIManager.h"

ClassTrainer::ClassTrainer(void)
{
}

ClassTrainer::~ClassTrainer(void)
{
}

bool ClassTrainer::Update(float fGameTime)
{
	//check if there is a player in transaction with merchant.  If so make sure
	//that the player is still in range, if he isn't close any open dialog

	if( m_pTrainee )
	{
		m_pTrainee->SetTarget(this);

		if( !IsInRange(m_pTrainee, m_fActionRange) )
		{
			m_pTrainee = 0;

			//close open dialog/ windows
			//g_pGameState->GetUIManager()->CloseUIWindow();
			//g_pGameState->GetUIManager()->CloseUIWindow();		
		}
	}

	return true;
}

void ClassTrainer::Interact(PlayerCharacter *pPlayer)
{
	if( IsInRange(pPlayer, m_fActionRange) )
	{
		m_pTrainee = pPlayer;
		m_fUnusedExp = 0.0;

		//set the data source to display class data
		//exp %s, level, trainer or untrained

		//display class window

	}


}

u32 ClassTrainer::HandleEvent(u32 uEvent)
{
	if( uEvent == 0 )
		return 0;

	switch( uEvent )
	{
	case TRAIN_ARCHER:
		TrainClass(eCL_Archer);
		return 0;
	case TRAIN_BRAWLER:
		TrainClass(eCL_Brawler);
		return 0;
	case TRAIN_PRIEST:
		TrainClass(eCL_Priest);
		return 0;
	case TRAIN_MEDIC:
		TrainClass(eCL_Medic);
		return 0;
	case TRAIN_OCCULTIST:
		TrainClass(eCL_Occultist);
		return 0;
	case TRAIN_ROCKETEER:
		TrainClass(eCL_Rocketeer);
		return 0;
	case TRAIN_MARKSMAN:
		TrainClass(eCL_Marksman);
		return 0;
	case TRAIN_SWORDSMAN:
		TrainClass(eCL_Swordsman);
		return 0;
	case LEAVE_TRAINER:
		if( CheckExpSplit() )
		{
			//close window and leave
			m_pTrainee = 0;
			//g_pGameState->GetUIManager()->CloseUIWindow();	
		}
		return 0;		
	}

	return IE_NOT_HANDLED;
}

void ClassTrainer::TrainClass(ePlayerClass eClass)
{
	if( m_pTrainee->HasClass(eClass) )
	{
		//remove the class
		m_fUnusedExp += m_pTrainee->RemoveClass(eCL_Brawler);
		return;
	}

	m_pTrainee->AddNewClass(eClass, 0.0f);
}
		
bool ClassTrainer::CheckExpSplit()
{
	//make sure there in no left over experince and no class is below 5%
	if( m_fUnusedExp > 0.0f )
		return false;

	for(int i = 0; i < NUM_OF_CLASSES; i++)
	{
		if( m_pTrainee->HasClass((ePlayerClass)i) )
		{
			//check to make sure it has over 5% exp
			float fExp = m_pTrainee->GetExpSplit((ePlayerClass)i);

			if( fExp < 0.05f )
				return false;
		}
	}

	return true;
}

void ClassTrainer::AdjustExp(ePlayerClass eClass, int iAdjustment)
{
	//adjust exp by 1%
	if( m_fUnusedExp > 0.0f )
	{
		m_pTrainee->AdjustExpSplit(eClass, 0.01f * iAdjustment);
		m_fUnusedExp -= 0.01f;
	}
}