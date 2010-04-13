#pragma once
#include "Common/Utility/kpuArrayList.h"

class Skill;
//template<typename T> class kpuArrayList;

class PlayerCharacter;


class SkillCombo
{
public:
	SkillCombo(void);
	~SkillCombo(void);

	void Swap(int iOldIndex, int iNewIndex); //Swap 2 skills in the combo
	void AddEnd(Skill* pSkill); //Add skill to end of combo
	void RemoveSkillAt(int iIndex); //Remove skill by index
	void Use(float fGameTime, PlayerCharacter* pPlayer); //Goes and uses each skill in the list
	bool IsRunning() { return m_bRunning; } //return true if there are still skills to go off

private:
	kpuArrayList<Skill*>*			m_pSkillList;
	int								m_iNextSkill;
	bool							m_bRunning;
};
