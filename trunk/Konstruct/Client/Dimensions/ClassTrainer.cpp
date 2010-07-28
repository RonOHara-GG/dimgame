#include "StdAfx.h"
#include "ClassTrainer.h"
#include "PlayerClass.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgUISlider.h"

//sliders for exp
static const u32 s_uHash_Brawler_ExpBar  =		0x647b38b6;
static const u32 s_uHash_Archer_ExpBar  =		0x337043fc;
static const u32 s_uHash_Marksman_ExpBar  =		0x27ba53e1;
static const u32 s_uHash_Rocketeer_ExpBar  =	0x4ce3f12b;
static const u32 s_uHash_Swordsman_ExpBar  =	0xed526c5;
static const u32 s_uHash_Occultist_ExpBar  =	0x829570c1;
static const u32 s_uHash_Medic_ExpBar  =		0x768781c9;
static const u32 s_uHash_Priest_ExpBar  =		0x520aee3e;

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

		UpdateClassExpSliders();		

	}
}

void ClassTrainer::SetDataSource()
{
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();


}

u32 ClassTrainer::HandleEvent(u32 uEvent, u32 uParam)
{
	if( uEvent == 0 )
		return 0;

	switch( uEvent )
	{
	case TRAIN:
		TrainClass((ePlayerClass)uParam);
		return 0;
	case ADJUST_EXP:
		AdjustExp((ePlayerClass)uParam);
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
		m_pTrainee->RemoveClass(eClass);
	else	
		m_pTrainee->AddNewClass(eClass);	

	UpdateClassExpSliders();
}

void ClassTrainer::AdjustExp(ePlayerClass eClass)
{
	//Get exp delta
	float fCurrent = m_pTrainee->GetExpSplit(eClass);

	kpgUIManager* pUIManager = g_pGameState->GetUIManager();
	kpgUISlider* pSlider = GetClassSlider(eClass);
	float fExpDelta = pSlider->GetSliderPos() - fCurrent;


	m_pTrainee->AdjustExpSplit(eClass, fExpDelta);

	//subtract the delta from the remainging classes
	fExpDelta /= -m_pTrainee->ClassCount();

	for(int i = 0; i < NUM_OF_CLASSES; i++)
	{
		if( m_pTrainee->HasClass((ePlayerClass)i) && i != eClass )
		{
			m_pTrainee->AdjustExpSplit((ePlayerClass)i, fExpDelta);
			kpgUISlider* pSlider = GetClassSlider((ePlayerClass)i);
			pSlider->MoveSlider(fExpDelta);
		}
	}
}

kpgUISlider* ClassTrainer::GetClassSlider(ePlayerClass eClass)
{
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();

	switch( eClass )
	{
	case eCL_Brawler:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Brawler_ExpBar);
	case eCL_Archer:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Archer_ExpBar);		
	case eCL_Swordsman:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Swordsman_ExpBar);		
	case eCL_Marksman:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Marksman_ExpBar);		
	case eCL_Priest:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Priest_ExpBar);		
	case eCL_Medic:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Medic_ExpBar);		
	case eCL_Rocketeer:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Rocketeer_ExpBar);		
	case eCL_Occultist:
		return (kpgUISlider*)pUIManager->GetUIWindow(s_uHash_Occultist_ExpBar);		
	}
	return 0;
}

void ClassTrainer::UpdateClassExpSliders()
{
	//Set the sliders based on experince split
	for(int i = 0; i < NUM_OF_CLASSES; i++)
	{
		kpgUISlider* pSlider = GetClassSlider((ePlayerClass)i);

		if( m_pTrainee->HasClass((ePlayerClass)i)  )
		{
			float fExpDelta = m_pTrainee->GetExpSplit((ePlayerClass)i);
			pSlider->SetSliderPos(m_pTrainee->GetExpSplit((ePlayerClass)i));
			pSlider->SetVisible(true);
		}
		else
			pSlider->SetVisible(false);

	}
}