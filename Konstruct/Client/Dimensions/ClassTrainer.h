#pragma once
#include "npc.h"

class PlayerCharacter;
enum ePlayerClass;

class ClassTrainer :
	public Npc
{
public:
	ClassTrainer(void);
	~ClassTrainer(void);

	bool Update(float fGameTime);
	void Interact(PlayerCharacter* pPlayer);
	u32 HandleEvent(u32 uEvent);

protected:
	void TrainClass(ePlayerClass eClass);
	void AdjustExp(ePlayerClass eClass, int iAdjustment); //adjust exp split by one either up or down determined by iAdjustment
	void SetDataSource();
	

	bool CheckExpSplit();

protected:
	PlayerCharacter*	m_pTrainee;

	char				m_szDataSources[8][2];
	char				m_szTrainData[8][8];	
	char				m_szExpData[8][5];
};
#define LEAVE_TRAINER			0x40ead5c7

#define TRAIN			0x00011
#define ADJUST_EXP		0x00022




