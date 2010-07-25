#include "StdAfx.h"
#include "ClassTrainer.h"
#include "PlayerClass.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgUIManager.h"

//sliders for exp
static const u32 s_uHash_Brawler_ExpBar  =		0x647b38b6;
static const u32 s_uHash_Archer_ExpBar  =		0x337043fc;
static const u32 s_uHash_Marksman_ExpBar  =		0x768781c9;
static const u32 s_uHash_Rocketeer_ExpBar  =	0x250371d3;
static const u32 s_uHash_Swordsman_ExpBar  =	0x27ba53e1;
static const u32 s_uHash_Occultist_ExpBar  =	0x4ce3f12b;
static const u32 s_uHash_Medic_ExpBar  =		0x520aee3e;
static const u32 s_uHash_Priest_ExpBar  =		0x829570c1;

static const u32 s_uHash_ClassTraining =		0x3d445257;


ClassTrainer::ClassTrainer(void)
{
	m_pTrainee = 0;
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
			g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_ClassTraining);
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

		//set the data source to display class data
		//exp %s, level, trainer or untrained

		//set train / untrain data sources
		SetDataSource();

		//display class window
		kpgUIManager* pUIManager = g_pGameState->GetUIManager();
		pUIManager->OpenUIWindow(s_uHash_ClassTraining, this);

	}
}

void ClassTrainer::SetDataSource()
{
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();
	

	for(int i = 0; i < NUM_OF_CLASSES; i++)
	{
		//set each class's current exp value and 

	}

}

u32 ClassTrainer::HandleEvent(u32 uEvent)
{
	if( uEvent == 0 )
		return 0;

	switch( uEvent )
	{
	case TRAIN:
		TrainClass(eCL_Archer);
		return 0;
	case LEAVE_TRAINER:
		if( CheckExpSplit() )
		{
			//close window and leave
			m_pTrainee = 0;
			g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_ClassTraining);	
		}
		return 0;		
	}

	return IE_NOT_HANDLED;
}

void ClassTrainer::TrainClass(ePlayerClass eClass)
{
	if( m_pTrainee->HasClass(eClass) )	
	{	//remove the class
		float fFree = m_pTrainee->RemoveClass(eClass);

		//split up free exp among remaining classes
		for(int i = 0; i < NUM_OF_CLASSES; i++)
		{
			if( m_pTrainee->HasClass((ePlayerClass)i) )			
				m_pTrainee->AdjustExpSplit((ePlayerClass)i, fFree / m_pTrainee->ClassCount());			
		}
	}
	else
	{
		float fExp = 0.05f;

		//remove 5% split by each class
		for(int i = 0; i < NUM_OF_CLASSES; i++)
		{
			if( m_pTrainee->HasClass((ePlayerClass)i) )			
				m_pTrainee->AdjustExpSplit((ePlayerClass)i, -fExp / m_pTrainee->ClassCount());			
		}

		m_pTrainee->AddNewClass(eClass, 0.05f);
	}
		
}
		
bool ClassTrainer::CheckExpSplit()
{
	//make sure there is no class is below 5%

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
	m_pTrainee->AdjustExpSplit(eClass, 0.01f * iAdjustment);
}