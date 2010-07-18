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
	

	bool CheckExpSplit();

protected:
	PlayerCharacter*	m_pTrainee;
	float				m_fUnusedExp;
};
#define LEAVE_TRAINER		0xA0

#define TRAIN_BRAWLER		0x01
#define BRAWLER_EXP_UP		0x09
#define BRAWLER_EXP_DOWN	0x0A

#define TRAIN_ARCHER		0x02
#define ARCHER_EXP_UP		0x0B
#define ARCHER_EXP_DOWN		0x0C

#define TRAIN_MEDIC			0x03
#define MEDIC_EXP_UP		0x0D
#define MEDIC_EXP_DOWN		0x0E

#define TRAIN_SWORDSMAN		0x04
#define SWORDSMAN_EXP_UP	0x10
#define SWORDSMAN_EXP_DOWN	0x11

#define TRAIN_MARKSMAN		0x05
#define MARKSMAN_EXP_UP		0x12
#define MARKSMAN_EXP_DOWN	0x13

#define TRAIN_ROCKETEER		0x06
#define ROCKETEER_EXP_UP	0x14
#define ROCKETEER_EXP_DOWN	0x15

#define TRAIN_PRIEST		0x07
#define PRIEST_EXP_UP		0x16
#define PRIEST_EXP_DOWN		0x17

#define TRAIN_OCCULTIST		0x08
#define OCCULTIST_EXP_UP	0x18
#define OCCULTIST_EXP_DOWN	0x19
