#pragma once

class NPC;

class AIControl
{
public:
	AIControl(void) {}
	AIControl(NPC* pNpc);
	virtual ~AIControl(void);
	virtual void Update( float fDeltaTime) {}

};
