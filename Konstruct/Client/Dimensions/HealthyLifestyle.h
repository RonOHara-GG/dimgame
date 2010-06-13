#pragma once
#include "SpreadableBuff.h"

class HealthyLifestyle :
	public SpreadableBuff
{
public:
	HealthyLifestyle(PlayerCharacter* pSource);
	~HealthyLifestyle(void);

	void Tick(PlayerCharacter* pTarget);
	SpreadableBuff* CopyBuff();


};
