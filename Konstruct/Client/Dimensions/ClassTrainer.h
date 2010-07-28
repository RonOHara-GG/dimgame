#pragma once
#include "npc.h"

class PlayerCharacter;
class kpgUISlider;
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
	kpgUISlider*	GetClassSlider(ePlayerClass eClass);
	void UpdateClassExpSliders();
	

protected:
	PlayerCharacter*	m_pTrainee;

};
#define LEAVE_TRAINER			0x40ead5c7

#define TRAIN			0xe2f3843
#define ADJUST_EXP		0xc856f09d




