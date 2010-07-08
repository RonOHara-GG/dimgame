#pragma once

class Npc;

class AIControl
{
public:
	AIControl(void) {}
	AIControl(Npc* pNpc);
	virtual ~AIControl(void);
	virtual void Update( float fDeltaTime) {}

};
