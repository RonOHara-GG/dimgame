#pragma once
#include "spreadablebuff.h"

class TargetAnalysis :
	public SpreadableBuff
{
public:
	TargetAnalysis(PlayerCharacter* pSource);
	~TargetAnalysis(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	void Deactivate();

	SpreadableBuff* CopyBuff();
protected:
	PlayerCharacter*	m_pTarget;
	int					m_iDamageBuff;
};
