#pragma once
#include "spreadablebuff.h"

class SanityAnchor :
	public SpreadableBuff
{
public:
	SanityAnchor(PlayerCharacter* pSource);
	~SanityAnchor(void);

	void Tick(PlayerCharacter* pTarget);
	SpreadableBuff* CopyBuff();
};
