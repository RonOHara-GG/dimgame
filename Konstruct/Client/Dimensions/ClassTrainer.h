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
	u32 HandleEvent(u32 uEvent, u32 uParam);

protected:
	void TrainClass(ePlayerClass eClass);
	void AdjustExp(ePlayerClass eClass);
	void SetDataSource();
	

	bool CheckExpSplit();

protected:
	PlayerCharacter*	m_pTrainee;

	char				m_szDataSources[8][2];
	char				m_szTrainData[8][8];	
	char				m_szExpData[8][5];
};
#define LEAVE_TRAINER			0x40ead5c7

#define TRAIN			0xe2f3843
#define ADJUST_EXP		0xc856f09d




